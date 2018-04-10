#include "StdAfx.hpp"
#include <Render/ResourceLoaders/XmlHelper.hpp>
#include <Render/ResourceLoaders/ShaderResourceLoader.hpp>
#include <Render/Technique.hpp>
#include <Render/Shader.hpp>
#include <System/ResourceSystem/ResourceHandle.hpp>
#include <System/ResourceSystem/ResourceManager.hpp>
#include <Render/ShaderEnvironmentProviders/DefaultShaderEnvironmentProvider.hpp>

#include <DXUT11\Core\DXUT.h>
#include <d3dx11tex.h>
#include <tinyxml2/tinyxml2.h>

namespace box
{
	struct Common
	{
		~Common()
		{
			SAVE_RELEASE(commonBlend);
			SAVE_RELEASE(commonDepthStencil);
			SAVE_RELEASE(commonRasterizer);
		}
		ID3D11BlendState* commonBlend = nullptr;
		ID3D11DepthStencilState* commonDepthStencil = nullptr;
		ID3D11RasterizerState* commonRasterizer = nullptr;
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

		const char* fileName = nullptr;
		U8* shaderText = nullptr;
		size_t shaderSize = 0;
	};

	bool compileShader(const U8* shaderText, size_t shaderSize, const char* fileName, const char* entry, const char* shaderModel, ID3D10Blob*& out)
	{
		DWORD dwShaderFlags = 0;
		ID3DBlob* pErrorBlob = nullptr;

		auto hr = D3DX11CompileFromMemory(reinterpret_cast<const char*>(shaderText), shaderSize, fileName, NULL, NULL, entry, shaderModel, dwShaderFlags, 0, NULL, &out, &pErrorBlob, NULL);
		if (FAILED(hr))
		{
			if (pErrorBlob != NULL)
			{
				OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			}
			if (pErrorBlob)
			{
				pErrorBlob->Release();
			}
			return false;
		}
		if (pErrorBlob)
		{
			pErrorBlob->Release();
		}
		return true;
	}

	bool parseDefines(tinyxml2::XMLElement* definesRoot)
	{
		//TODO implement me
		return true;
	}

	bool parseInputBuffers(tinyxml2::XMLElement* inputBuffersRoot)
	{
		//TODO implement me
		return true;
	}

	bool parseInputLayout(tinyxml2::XMLElement* inputLayoutRoot, std::vector<D3D11_INPUT_ELEMENT_DESC>& out)
	{
		out.clear();
		static const char* semanticNames[] = { "POSITION", "NORMAL", "TEXCOORD", "COLOR", "TANGENT", "BINORMAL" };
		std::map<std::string, U32> nameToIndex;
		nameToIndex["Position"] = 0;
		nameToIndex["Normal"] = 1;
		nameToIndex["Texcoord"] = 2;
		nameToIndex["Color"] = 3;
		nameToIndex["Tangent"] = 4;
		nameToIndex["Binormal"] = 5;

		for (auto item = inputLayoutRoot->FirstChild(); item; item = item->NextSibling())
		{
			auto element = item->ToElement();

			const char* semanticName = element->Attribute("semanticName");
			U32 semanticIndex(0);
			U32 stream(0);
			element->QueryUnsignedAttribute("semanticIndex", &semanticIndex);
			element->QueryUnsignedAttribute("stream", &stream);
			const char* format = element->Attribute("format");

			const bool first = out.size() == 0;
			auto dxFormat = g_dxgiFormats.find(format)->second;
			U32 index = nameToIndex.find(semanticName)->second;

			out.push_back({ semanticNames[index], semanticIndex, dxFormat, stream, first ? 0 : D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}
		return out.size() > 0;
	}

	bool parseRasterizer(tinyxml2::XMLElement* rasterizerRoot, ID3D11RasterizerState*& out)
	{
		D3D11_RASTERIZER_DESC rasterDesc;
		ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));

		START_LOOP(rasterizerRoot)
			ITEM_TEXT(rasterDesc.FillMode, "FillMode", g_fillModes);
			ITEM_TEXT(rasterDesc.CullMode, "CullMode", g_cullModes);

			ITEM_FLOAT(rasterDesc.DepthBiasClamp, "DepthBiasClamp");
			ITEM_FLOAT(rasterDesc.SlopeScaledDepthBias, "SlopeScaledDepthBias");

			ITEM_BOOL(rasterDesc.FrontCounterClockwise, "FrontClockWise");
			ITEM_BOOL(rasterDesc.DepthBias, "DepthBias");
			ITEM_BOOL(rasterDesc.DepthClipEnable, "DepthClip");
			ITEM_BOOL(rasterDesc.ScissorEnable, "Scissor");
			ITEM_BOOL(rasterDesc.MultisampleEnable, "Multisample");
			ITEM_BOOL(rasterDesc.AntialiasedLineEnable, "AntialiasedLine");
		END_LOOP
		auto device = DXUTGetD3D11Device();
		HRESULT res = device->CreateRasterizerState(&rasterDesc, &out);
		return !FAILED(res);
	}

	bool parseDepthStencil(tinyxml2::XMLElement* depthStencilRoot, ID3D11DepthStencilState*& out)
	{
		D3D11_DEPTH_STENCIL_DESC dsDesc;
		ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		START_LOOP(depthStencilRoot)
			ITEM_BOOL(dsDesc.DepthEnable, "DepthEnable");

			ITEM_TEXT(dsDesc.DepthWriteMask, "WriteMask", g_WriteMasks);
			
			ITEM_TEXT(dsDesc.DepthFunc, "DepthFunc", g_cmpFuncs);
			ITEM_INT(dsDesc.StencilReadMask, "StencilReadMask");
			ITEM_INT(dsDesc.StencilWriteMask, "StencilWriteMask");

			ITEM_BOOL(dsDesc.StencilEnable, "StencilEnable");

			// Stencil operations if pixel is front-facing
			ITEM_TEXT(dsDesc.FrontFace.StencilFailOp, "StencilFrontFailOp", g_stencilOps);
			ITEM_TEXT(dsDesc.FrontFace.StencilDepthFailOp, "StencilFrontDepthFailOp", g_stencilOps);
			ITEM_TEXT(dsDesc.FrontFace.StencilPassOp, "StencilFrontPassOp", g_stencilOps);
			ITEM_TEXT(dsDesc.FrontFace.StencilFunc, "StencilFrontFunc", g_cmpFuncs);

			// Stencil operations if pixel is back-facing
			ITEM_TEXT(dsDesc.BackFace.StencilFailOp, "StencilBackFailOp", g_stencilOps);
			ITEM_TEXT(dsDesc.BackFace.StencilDepthFailOp, "StencilBackDepthFailOp", g_stencilOps);
			ITEM_TEXT(dsDesc.BackFace.StencilPassOp, "StencilBackPassOp", g_stencilOps);
			ITEM_TEXT(dsDesc.BackFace.StencilFunc, "StencilBackFunc", g_cmpFuncs);

		END_LOOP

		auto device = DXUTGetD3D11Device();
		HRESULT res = device->CreateDepthStencilState(&dsDesc, &out);
		return !FAILED(res);
	}

	bool parseBlend(tinyxml2::XMLElement* blendRoot, ID3D11BlendState*& out)
	{
		D3D11_BLEND_DESC BlendState;
		ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
		bool alphaToCoverage = false;
		bool independentBlend = false;
		blendRoot->QueryBoolAttribute("AlphaToCoverage", &alphaToCoverage);
		blendRoot->QueryBoolAttribute("IndependentBlend", &independentBlend);
		BlendState.AlphaToCoverageEnable = alphaToCoverage;
		BlendState.IndependentBlendEnable = independentBlend;
		for (auto rt = blendRoot->FirstChild(); rt; rt = rt->NextSibling())
		{
			auto el = rt->ToElement();
			int target(0);
			el->QueryIntAttribute("name", &target);
			START_LOOP(rt)
				ITEM_BOOL(BlendState.RenderTarget[target].BlendEnable, "BlendEnable");
				ITEM_TEXT(BlendState.RenderTarget[target].RenderTargetWriteMask, "WriteMask", g_colorWriteMasks);

				ITEM_TEXT(BlendState.RenderTarget[target].SrcBlend, "ColorSrc", g_Blends);
				ITEM_TEXT(BlendState.RenderTarget[target].DestBlend, "ColorDst", g_Blends);
				ITEM_TEXT(BlendState.RenderTarget[target].BlendOp, "ColorOp", g_BlendOps);

				ITEM_TEXT(BlendState.RenderTarget[target].SrcBlendAlpha, "AlphaSrc", g_Blends);
				ITEM_TEXT(BlendState.RenderTarget[target].DestBlendAlpha, "AlphaDst", g_Blends);
				ITEM_TEXT(BlendState.RenderTarget[target].BlendOpAlpha, "AlphaOp", g_BlendOps);
			END_LOOP
		}

		auto device = DXUTGetD3D11Device();
		HRESULT res = device->CreateBlendState(&BlendState, &out);
		return !FAILED(res);
	}

	bool parseOutput(tinyxml2::XMLElement* outputRoot)
	{
		//TODO implement me
		return true;
	}

	bool parseVS(tinyxml2::XMLElement* vsRoot, Technique& out, Common& common)
	{
		const char* entryPoint = vsRoot->Attribute("entryPoint");
		const char* shaderModel = vsRoot->Attribute("shaderModel");
		ID3D11VertexShader* vertexShader = nullptr;
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
		bool ret = true;

		if (!entryPoint || !shaderModel)
		{
			return false;
		}

		do
		{
			if (auto elem = vsRoot->FirstChildElement("Defines"))
			{
				ret &= parseDefines(elem);
				if (!ret) break;
			}

			if (auto elem = vsRoot->FirstChildElement("InputBuffers"))
			{
				ret &= parseInputBuffers(elem);
				if (!ret) break;
			}

			if (auto elem = vsRoot->FirstChildElement("InputLayout"))
			{
				ret &= parseInputLayout(elem, inputLayoutDesc);
			}
			else
			{
				if (common.inputLayoutDesc.size() > 0)
				{
					inputLayoutDesc = common.inputLayoutDesc;
				}
				else
				{
					ret = false;
				}
			}
			if (!ret) break;
			ID3DBlob* rawShader = nullptr;

			ret &= compileShader(common.shaderText, common.shaderSize, common.fileName, entryPoint, shaderModel, rawShader);
			if (!ret) break;

			auto device = DXUTGetD3D11Device();
			device->CreateVertexShader(rawShader->GetBufferPointer(), rawShader->GetBufferSize(), nullptr, &out.vertexShader);

			if (inputLayoutDesc.size() > 0)
			{
				device->CreateInputLayout(inputLayoutDesc.data(), inputLayoutDesc.size(), rawShader->GetBufferPointer(), rawShader->GetBufferSize(), &out.inputLayout);
			}

			if (out.inputLayout == nullptr || out.vertexShader == nullptr)
			{
				ret = false;
			}
		} while (0);

		return ret;
	}

	bool parsePS(tinyxml2::XMLElement* psRoot, Technique& out, Common& common)
	{
		const char* entryPoint = psRoot->Attribute("entryPoint");
		const char* shaderModel = psRoot->Attribute("shaderModel");
		ID3D11PixelShader* pixelShader = nullptr;
		bool ret = true;

		if (!entryPoint || !shaderModel)
		{
			return false;
		}

		if (auto elem = psRoot->FirstChildElement("Defines"))
		{
			ret &= parseDefines(elem);
		}

		if (auto elem = psRoot->FirstChildElement("Rasterizer"))
		{
			ret &= parseRasterizer(elem, out.rasterizerState);
		}
		else
		{
			if (common.commonRasterizer)
			{
				out.rasterizerState = common.commonRasterizer;
				out.rasterizerState->AddRef();
			}
		}

		if (auto elem = psRoot->FirstChildElement("DepthStencil"))
		{
			ret &= parseDepthStencil(elem, out.depthStencilView);
		}
		else
		{
			if (common.commonDepthStencil)
			{
				out.depthStencilView = common.commonDepthStencil;
				out.depthStencilView->AddRef();
			}
		}

		if (auto elem = psRoot->FirstChildElement("BlendDescriptions"))
		{
			ret &= parseBlend(elem, out.blendState);
		}
		else
		{
			if (common.commonBlend)
			{
				out.blendState = common.commonBlend;
				out.blendState->AddRef();
			}
		}

		if (auto elem = psRoot->FirstChildElement("Outputs"))
		{
			ret &= parseOutput(elem);
		}

		ID3DBlob* rawShader = nullptr;

		ret &= compileShader(common.shaderText, common.shaderSize, common.fileName, entryPoint, shaderModel, rawShader);

		if (ret)
		{
			auto device = DXUTGetD3D11Device();
			device->CreatePixelShader(rawShader->GetBufferPointer(), rawShader->GetBufferSize(), nullptr, &out.pixelShader);
		}

		return out.pixelShader != nullptr;
	}

	bool parseTechnique(tinyxml2::XMLNode* techniqueRoot, Technique& out, Common& common)
	{
		auto element = techniqueRoot->ToElement();
		if (const char* techniqueName = element->Attribute("name"))
		{
			out.name = techniqueName;
		}

		bool vsOK = false;
		bool psOK = false;

		if (auto vs = techniqueRoot->FirstChildElement("VS"))
		{
			vsOK = parseVS(vs, out, common);
		}

		if (auto ps = techniqueRoot->FirstChildElement("PS"))
		{
			psOK = parsePS(ps, out, common);
		}

		return (vsOK && psOK);
	}

	ShaderEnvironmentProvider::ShaderEnvironmentProviderStrong getProvider(const char* name)
	{
		if (strcmp(name, "DefaultShaderEnvironmentProvider") == 0)
		{
			return std::make_shared<DefaultShaderEnvironmentProvider>();
		}

		return ShaderEnvironmentProvider::ShaderEnvironmentProviderStrong();
	}

	bool ShaderResourceLoader::loadResource(U8* buffer, size_t size, std::shared_ptr<ResourceHandle> handle)
	{
		std::shared_ptr<Shader> extra = std::make_shared<Shader>(handle->getResource().m_name);

		tinyxml2::XMLDocument xmlDoc;

		tinyxml2::XMLError result = xmlDoc.Parse(reinterpret_cast<const char*>(buffer), size);
		if (result == tinyxml2::XMLError::XML_SUCCESS)
		{
			const char* shaderEnvironmentProvider = nullptr;

			tinyxml2::XMLNode* root = xmlDoc.RootElement();
			Common ShaderCommon;

			if (auto element = root->ToElement())
			{
				ShaderCommon.fileName = element->Attribute("file");
				shaderEnvironmentProvider = element->Attribute("provider");
			}

			bool ok = false;
			do
			{
				if (!ShaderCommon.fileName
				 || !shaderEnvironmentProvider)
				{
					break;
				}

				extra->setProvider(getProvider(shaderEnvironmentProvider));

				box::Resource samplerStateResource(ShaderCommon.fileName);
				auto shaderCode = box::ResourceManager::Instance().getHandle(samplerStateResource);
				if (shaderCode->getStatus() == ResourceHandle::Status::Ready)
				{
					ShaderCommon.shaderText = shaderCode->buffer();
					ShaderCommon.shaderSize = shaderCode->getSize();
				}
				else
				{
					break;
				}
				{ // common stuff
					if (auto elem = root->FirstChildElement("Defines"))
					{
						parseDefines(elem);
					}

					if (auto elem = root->FirstChildElement("InputBuffers"))
					{
						parseInputBuffers(elem);
					}

					if (auto elem = root->FirstChildElement("InputLayout"))
					{
						parseInputLayout(elem, ShaderCommon.inputLayoutDesc);
					}
					
					if (auto elem = root->FirstChildElement("Rasterizer"))
					{
						parseRasterizer(elem, ShaderCommon.commonRasterizer);
					}

					if (auto elem = root->FirstChildElement("DepthStencil"))
					{
						parseDepthStencil(elem, ShaderCommon.commonDepthStencil);
					}

					if (auto elem = root->FirstChildElement("BlendDescriptions"))
					{
						parseBlend(elem, ShaderCommon.commonBlend);
					}

					if (auto elem = root->FirstChildElement("Outputs"))
					{
						parseOutput(elem);
					}
				}

				bool failedParseTechniques = false;
				if (tinyxml2::XMLNode* techniques = root->FirstChildElement("techniques"))
				{
					for (auto child = techniques->FirstChild(); child; child = child->NextSibling())
					{
						Technique t;
						if (parseTechnique(child, t, ShaderCommon))
						{
							extra->m_techniques.emplace_back(t);
						}
						else
						{
							failedParseTechniques = true;
							break;
						}
					}
				}
				ok = !failedParseTechniques;
			} while (0);

			if (ok)
			{
				handle->setExtra(extra);
			}

			return ok;
		}

		return false;
	}
}