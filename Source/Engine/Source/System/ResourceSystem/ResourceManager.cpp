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
#endif

		bool res = m_cache->init();
		if (res)
		{
			m_cache->registerLoader(std::shared_ptr<ResourceLoader>(new XMLResourceLoader()));

			std::shared_ptr<Event_ResourceCacheStarted> event(new Event_ResourceCacheStarted);
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
			Process::StrongProcessPtr load(new PreloadProcess(wildcard, m_cache));
			ProcessManager::Instance().attachProcess(load);
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
				Process::StrongProcessPtr load(new LoadResourceProcess(handle));
				ProcessManager::Instance().attachProcess(load);
			}
			return handle;
		}
	}

	void ResourceManager::registerLoader(std::shared_ptr<ResourceLoader> loader)
	{
		m_cache->registerLoader(loader);
	}

	void ResourceManager::importModelFromFile(const std::string& fileName)
	{
		Assimp::Importer importer;
		const aiScene* assimpScene = importer.ReadFile(fileName, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Quality | aiProcess_GenUVCoords);
		if (assimpScene)
		{
			auto rootNode = assimpScene->mRootNode;
			if (assimpScene->mNumMeshes > 0)
			{
				ID3D11DeviceContext* context = DXUTGetD3D11DeviceContext();
				ID3D11Device* device = DXUTGetD3D11Device();


				auto assimpMesh = assimpScene->mMeshes[0];
				struct SimpleVertexFormat
				{
					float pos[3];
					float tcoord[2];
					float norm[3];
				};

				ID3D11Buffer* vertexBuffer = nullptr;
				ID3D11Buffer* indexBuffer = nullptr;
				D3D11_BUFFER_DESC vbDesc;
				vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
				vbDesc.ByteWidth = assimpMesh->mNumVertices * sizeof(SimpleVertexFormat);
				vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				vbDesc.CPUAccessFlags = 0;
				vbDesc.MiscFlags = 0;
				vbDesc.StructureByteStride = 0;

				std::vector<SimpleVertexFormat> verts;
				verts.reserve(assimpMesh->mNumVertices);
				for (int i = 0; i < assimpMesh->mNumVertices; i++)
				{
					verts.push_back(SimpleVertexFormat());
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

				device->CreateBuffer(&vbDesc, &initData, &vertexBuffer);

				std::vector<U16> indices;
				verts.reserve(assimpMesh->mNumFaces * 3);
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
				ibDesc.ByteWidth = indices.size() * sizeof(U16);
				ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				ibDesc.CPUAccessFlags = 0;
				ibDesc.MiscFlags = 0;
				ibDesc.StructureByteStride = 0;

				initData.pSysMem = indices.data();

				device->CreateBuffer(&ibDesc, &initData, &indexBuffer);

				Model::ModelStrongPtr model = std::make_shared<Model>(fileName);
				Mesh::MeshStrongPtr mesh = std::make_shared<Mesh>(vertexBuffer, indexBuffer, indices.size());

				{ ///Adding default material
					Shader::ShaderStrongPtr shader = std::make_shared<Shader>("desc/lighting/phong.shader");
					shader->restore();
					Material::MaterialStrongPtr matarial = std::make_shared<Material>("Default");
					matarial->setShader(shader);
					mesh->setMaterial(matarial);
				}

				model->addMesh(mesh);

				{
					ResourceHandle::StrongResourceHandlePtr res = std::make_shared<ResourceHandle>(Resource(fileName), nullptr, 0, nullptr);
					res->setExtra(model);
					std::shared_ptr<Event_ResourceLoaded> event(new Event_ResourceLoaded(Event_ResourceLoaded::ResourceType::Model, res));
					EventSystem::Instance().raiseEvent(event);
				}
			}

		}
	}

	int ResourceManager::getResourceCollectionSizeForXml()
	{
		return m_cache->getResourceCollectionSizeForXml();
	}

	void ResourceManager::serializeResourceCollectionToXml(char* out)
	{
		m_cache->serializeResourceCollectionToXml(out);
	}

}