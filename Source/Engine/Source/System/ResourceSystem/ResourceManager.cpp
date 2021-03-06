#include "StdAfx.hpp"
#include "System\ResourceSystem\ResourceManager.hpp"
#include "System\ResourceSystem\ZipFile.hpp"
#include "System/ResourceSystem/XMLResourceLoader.hpp"
#include "System/Process/ProcessManager.hpp"
#include "System/Process/Processes/PreloadProcess.hpp"
#include "System/Process/Processes/LoadResourceProcess.hpp"
#include "System\EventSystem\EventSystem.hpp"
#include "System\EventSystem\EngineEvents.hpp"
#include "System\ResourceSystem\ResourceEvents.hpp"
#include "System\ResourceSystem\EditorResourceFolder.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Render/Model.hpp"
#include <d3d11.h>
#include <DXUT11\Core\DXUT.h>

namespace
{
	const box::U32 DefaultCacheSizeMb = 100;
}

namespace box
{
	SINGLETON_ACCESSOR(ResourceManager);

	bool ResourceManager::init()
	{
		m_cache = new ResourceCache(DefaultCacheSizeMb);
		ResourceFile* resFile(nullptr);
#ifdef GAME_BUILD
		{
			resFile = new ResourceZipFile("Configs.zip");
			m_cache->addResourceFile(resFile);
		}

		{
			resFile = new ResourceZipFile("Sounds.zip");
			m_cache->addResourceFile(resFile);
		}

		{
			resFile = new ResourceZipFile("Textures.zip");
			m_cache->addResourceFile(resFile);
		}

		{
			resFile = new ResourceZipFile("Models.zip");
			m_cache->addResourceFile(resFile);
		}

		{
			resFile = new ResourceZipFile("Animations.zip");
			m_cache->addResourceFile(resFile);
		}

		{
			resFile = new ResourceZipFile("Shaders.zip");
			m_cache->addResourceFile(resFile);
		}
#else
		{
			resFile = new EditorResourceFolder("../Assets/Configs/");
			m_cache->addResourceFile(resFile);
		}

		{
			resFile = new EditorResourceFolder("../Assets/Sounds/");
			m_cache->addResourceFile(resFile);
		}

		{
			resFile = new EditorResourceFolder("../Assets/Textures/");
			m_cache->addResourceFile(resFile);
		}

		{
			resFile = new EditorResourceFolder("../Assets/Models/");
			m_cache->addResourceFile(resFile);
		}

		{
			resFile = new EditorResourceFolder("../Assets/Animations/");
			m_cache->addResourceFile(resFile);
		}

		{
			resFile = new EditorResourceFolder("../Assets/Shaders/");
			m_cache->addResourceFile(resFile);
		}
		{
			resFile = new EditorResourceFolder("../Assets/Materials/");
			m_cache->addResourceFile(resFile);
		}
#endif

		bool res = m_cache->init();
		if (res)
		{
			m_cache->registerLoader(std::make_shared<XMLResourceLoader>());
			
			std::shared_ptr<Event_ResourceCacheStarted> event = std::make_shared<Event_ResourceCacheStarted>();
			EventSystem::Instance().raiseEvent(event);
		}
		return res;
	}

	void ResourceManager::deinit()
	{
		delete m_cache;
	}

	void ResourceManager::preload(char* wildcard, bool threaded)
	{
		if (!threaded)
		{
			m_cache->preload(wildcard);
		}
		else
		{
			ProcessManager::Instance().attachProcess(std::make_shared<PreloadProcess>(wildcard, m_cache));
		}
	}

	std::shared_ptr<ResourceHandle> ResourceManager::getHandle(Resource& r, bool async)
	{
		if (!async)
		{
			return m_cache->getHandle(r);
		}
		else
		{
			std::shared_ptr<ResourceHandle> handle = m_cache->find(r);
			if (!handle)
			{
				handle = std::make_shared<ResourceHandle>(r, nullptr, 0, m_cache);
				m_cache->insertToLoadQueue(handle);
				ProcessManager::Instance().attachProcess(std::make_shared<LoadResourceProcess>(handle));
			}
			return handle;
		}
	}

	void ResourceManager::registerLoader(std::shared_ptr<ResourceLoader> loader)
	{
		m_cache->registerLoader(loader);
	}

	int ResourceManager::importStaticModel(const std::string& fileName, const std::string& modelName)
	{
		std::vector<box::Mesh::MeshStrongPtr> meshes;
		loadStaticModelFromFile(fileName, meshes);
		if (meshes.size() > 0)
		{
			Model::ModelStrongPtr model = std::make_shared<Model>(modelName, fileName);
			Resource r("desc/default.material");
			auto handle = ResourceManager::Instance().getHandle(r);
			auto material = handle->getExtraTyped<Material>();

			for (auto& mesh : meshes)
			{
				if (!mesh->getMaterial().lock())
				{
					mesh->setMaterial(material);
				}
				model->addMesh(mesh);
			}

			{
				ResourceHandle::StrongResourceHandlePtr res = std::make_shared<ResourceHandle>(Resource(fileName), nullptr, 0, nullptr);
				res->setExtra(model);
				EventSystem::Instance().raiseEvent(std::make_shared<Event_ResourceLoaded>(Event_ResourceLoaded::ResourceType::Model, res));
			}

			return 0;
		}

		return -1;
	}

	int ResourceManager::ImportDynamicModel(const std::string& fileName)
	{
		return -1;
	}

	void ResourceManager::loadModel(const std::string& fileName, Model& model)
	{
		std::vector<box::Mesh::MeshStrongPtr> meshes;
		loadStaticModelFromFile(fileName, meshes, false, false);

		for (auto& mesh : meshes)
		{
			model.addMesh(mesh);
		}
	}

	int ResourceManager::ImportShader(const std::string& fileName)
	{
		return -1;
	}

	int ResourceManager::ImportDDSTexture(const std::string& fileName)
	{
		return -1;
	}

	int ResourceManager::CompileShader(const std::string& fileName)
	{
		return -1;
	}

	int ResourceManager::getResourceCollectionSizeForXml()
	{
		return m_cache->getResourceCollectionSizeForXml();
	}

	void ResourceManager::serializeResourceCollectionToXml(char* out)
	{
		m_cache->serializeResourceCollectionToXml(out);
	}

	void ResourceManager::resyncResourceFolders()
	{
		m_cache->resyncResourceFolders();
	}

	void readMaterial(aiMaterial* material, Material::MaterialStrongPtr out)
	{
		aiString name;
		material->Get(AI_MATKEY_NAME, name);
		out->setName(name.C_Str());
		out->setShaderName("desc/default.shader");

		Resource r("desc/default.shader");
		auto handle = ResourceManager::Instance().getHandle(r);
		auto shader = handle->getExtraTyped<Shader>();
		out->setShader(shader);

		out->setSrcFile(std::string("desc/") + name.C_Str() + ".material");

		if (material->GetTextureCount(aiTextureType_AMBIENT) > 0) // map_Ka
		{
				aiString path;
				material->GetTexture(aiTextureType_AMBIENT, 0, &path);
				out->setTextureName(0, path.C_Str());
		}
		if (material->GetTextureCount(aiTextureType_DIFFUSE)) // map_Kd
		{
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			out->setTextureName(1, path.C_Str());
		}
		if (material->GetTextureCount(aiTextureType_OPACITY)) // map_d
		{
			aiString path;
			material->GetTexture(aiTextureType_OPACITY, 0, &path);
			out->setTextureName(2, path.C_Str());
		}
		if (material->GetTextureCount(aiTextureType_HEIGHT)) // map_bump or bump
		{
			aiString path;
			material->GetTexture(aiTextureType_HEIGHT, 0, &path);
			out->setTextureName(3, path.C_Str());
		}
		if (material->GetTextureCount(aiTextureType_SHININESS)) // map_Ns
		{
			aiString path;
			material->GetTexture(aiTextureType_SHININESS, 0, &path);
			out->setTextureName(4, path.C_Str());
		}
	}

	void ResourceManager::loadStaticModelFromFile(const std::string& fileName, std::vector<box::Mesh::MeshStrongPtr>& outMeshes, bool readMaterials, bool saveNewMaterialDesc)
	{
		outMeshes.clear();
		std::vector<Material::MaterialStrongPtr> outMaterials;

		Assimp::Importer importer;
		const aiScene* assimpScene = importer.ReadFile(fileName, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Quality | aiProcess_GenUVCoords);
		if (assimpScene)
		{
			auto rootNode = assimpScene->mRootNode;

			if (assimpScene->mNumMaterials && readMaterials)
			{
				outMaterials.reserve(assimpScene->mNumMaterials);

				for (U32 i = 0; i < assimpScene->mNumMaterials; i++)
				{
					auto material = assimpScene->mMaterials[i];
					outMaterials.push_back(std::make_shared<Material>());

					readMaterial(material, outMaterials.back());

					if (saveNewMaterialDesc)
					{
						FILE* pFile;
						tinyxml2::XMLDocument xmlDoc2;
						pFile = fopen(outMaterials.back()->getSrcFile().c_str(), "w");
						if (pFile)
						{
							tinyxml2::XMLNode* pRoot = outMaterials.back()->serializeToXML(nullptr, xmlDoc2);
							xmlDoc2.InsertFirstChild(pRoot);
							tinyxml2::XMLPrinter printer2(pFile, false);
							xmlDoc2.Print(&printer2);
							fclose(pFile);
						}
					}
				}
			}

			if (assimpScene->mNumMeshes > 0)
			{
				outMeshes.reserve(assimpScene->mNumMeshes);
				for (U32 meshI = 0; meshI < assimpScene->mNumMeshes; meshI++)
				{
					ID3D11DeviceContext* context = DXUTGetD3D11DeviceContext();
					ID3D11Device* device = DXUTGetD3D11Device();

					auto assimpMesh = assimpScene->mMeshes[meshI];

					Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
					Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
					D3D11_BUFFER_DESC vbDesc;
					vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
					vbDesc.ByteWidth = assimpMesh->mNumVertices * sizeof(VFs::PosTcoordNorm);
					vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
					vbDesc.CPUAccessFlags = 0;
					vbDesc.MiscFlags = 0;
					vbDesc.StructureByteStride = 0;

					std::vector<VFs::PosTcoordNorm> verts;
					verts.reserve(assimpMesh->mNumVertices);
					for (int i = 0; i < assimpMesh->mNumVertices; i++)
					{
						verts.push_back(VFs::PosTcoordNorm());
						verts[i].pos[0] = assimpMesh->mVertices[i].x;
						verts[i].pos[1] = assimpMesh->mVertices[i].y;
						verts[i].pos[2] = assimpMesh->mVertices[i].z;

						if (assimpMesh->mTextureCoords[0])
						{
							verts[i].tcoord[0] = assimpMesh->mTextureCoords[0][i].x;
							verts[i].tcoord[1] = assimpMesh->mTextureCoords[0][i].y;
						}
						else
						{
							verts[i].tcoord[0] = 0.0f;
							verts[i].tcoord[1] = 0.0f;
						}
						verts[i].norm[0] = assimpMesh->mNormals[i].x;
						verts[i].norm[1] = assimpMesh->mNormals[i].y;
						verts[i].norm[2] = assimpMesh->mNormals[i].z;
					}
					D3D11_SUBRESOURCE_DATA initData;
					initData.pSysMem = verts.data();

					device->CreateBuffer(&vbDesc, &initData, vertexBuffer.GetAddressOf());

					std::vector<U32> indices;
					indices.reserve(assimpMesh->mNumFaces * 3);
					for (U32 i = 0; i < assimpMesh->mNumFaces; i++)
					{
						const auto& face = assimpMesh->mFaces[i];
						for (U32 j = 0; j < face.mNumIndices; j++)
						{
							indices.push_back(face.mIndices[j]);
						}
					}

					D3D11_BUFFER_DESC ibDesc;
					ibDesc.Usage = D3D11_USAGE_IMMUTABLE;
					ibDesc.ByteWidth = indices.size() * sizeof(U32);
					ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
					ibDesc.CPUAccessFlags = 0;
					ibDesc.MiscFlags = 0;
					ibDesc.StructureByteStride = 0;

					initData.pSysMem = indices.data();

					device->CreateBuffer(&ibDesc, &initData, indexBuffer.GetAddressOf());

					Mesh::MeshStrongPtr mesh = std::make_shared<Mesh>(vertexBuffer, indexBuffer, indices.size(), sizeof(VFs::PosTcoordNorm), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

					if (assimpMesh->mMaterialIndex < outMaterials.size())
					{
						mesh->setMaterial(outMaterials[assimpMesh->mMaterialIndex]);
					}

					if (assimpMesh->mName.length > 0)
					{
						const char* name = assimpMesh->mName.C_Str();
						mesh->setName(name);
					}
					else
					{
						char buf[50];
						snprintf(buf, 50, "mesh_%d\0", meshI);
						mesh->setName(buf);
					}
					outMeshes.push_back(mesh);
				}
			}
		}
	}

}