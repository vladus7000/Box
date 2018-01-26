#include "StdAfx.hpp"
#include <Render/ResourceLoaders/ShaderResourceLoader.hpp>
#include <Render/ResourceLoaders/ShaderResourceExtraData.hpp>
#include <System/ResourceSystem/ResourceHandle.hpp>
#include <System/ResourceSystem/ResourceManager.hpp>

#include <DXUT11\Core\DXUT.h>
#include <d3dx11tex.h>
#include <tinyxml2/tinyxml2.h>

namespace box
{
	struct Common
	{
		ID3D11BlendState* commonBlend = nullptr;
		ID3D11DepthStencilState* commonDepthStencil = nullptr;
		ID3D11RasterizerState* commonRasterizer = nullptr;
		ID3D11InputLayout* commonInputLayout = nullptr;

		const char* fileName = nullptr;
	};

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

	bool parseInputLayout(tinyxml2::XMLElement* inputLayoutRoot, ID3D11InputLayout*& out)
	{
		//TODO implement me
		return true;
	}

	bool parseRasterizer(tinyxml2::XMLElement* rasterizerRoot, ID3D11RasterizerState*& out)
	{
		// TODO implement reading
		D3D11_RASTERIZER_DESC rasterDesc;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.DepthBias = false;
		rasterDesc.DepthBiasClamp = 0;
		rasterDesc.SlopeScaledDepthBias = 0;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.ScissorEnable = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.AntialiasedLineEnable = false;

		auto device = DXUTGetD3D11Device();
		HRESULT res = device->CreateRasterizerState(&rasterDesc, &out);
		return !FAILED(res);
	}

	bool parseDepthStencil(tinyxml2::XMLElement* depthStencilRoot, ID3D11DepthStencilState*& out)
	{
		// TODO implement reading
		D3D11_DEPTH_STENCIL_DESC dsDesc;
		// Depth test parameters
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		// Stencil test parameters
		dsDesc.StencilEnable = false;
		dsDesc.StencilReadMask = 0xFF;
		dsDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		auto device = DXUTGetD3D11Device();
		HRESULT res = device->CreateDepthStencilState(&dsDesc, &out);
		return !FAILED(res);
	}

	bool parseBlend(tinyxml2::XMLElement* blendRoot, ID3D11BlendState*& out)
	{
		// TODO implement reading

		D3D11_BLEND_DESC BlendState;
		ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
		BlendState.RenderTarget[0].BlendEnable = FALSE;
		BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		auto device = DXUTGetD3D11Device();
		HRESULT res = device->CreateBlendState(&BlendState, &out);
		return !FAILED(res);
	}

	bool parseOutput(tinyxml2::XMLElement* outputRoot)
	{
		//TODO implement me
		return true;
	}

	bool parseVS(tinyxml2::XMLElement* vsRoot, ShaderResourceExtraData::Technique& out, Common& common)
	{
		const char* entryPoint = vsRoot->Attribute("entryPoint");
		ID3D11VertexShader* vertexShader = nullptr;
		bool ret = true;

		if (!entryPoint)
		{
			return false;
		}

		if (auto elem = vsRoot->FirstChildElement("Defines"))
		{
			ret &= parseDefines(elem);
		}

		if (auto elem = vsRoot->FirstChildElement("InputBuffers"))
		{
			ret &= parseInputBuffers(elem);
		}

		if (auto elem = vsRoot->FirstChildElement("InputLayout"))
		{
			ret &= parseInputLayout(elem, out.inputLayout);
		}
		else
		{
			if (common.commonInputLayout)
			{
				out.inputLayout = common.commonInputLayout;
			}
			else
			{
				ret = false;
			}
		}

		return ret;
	}

	bool parsePS(tinyxml2::XMLElement* psRoot, ShaderResourceExtraData::Technique& out, Common& common)
	{
		const char* entryPoint = psRoot->Attribute("entryPoint");
		ID3D11PixelShader* pixelShader = nullptr;
		bool ret = true;

		if (!entryPoint)
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
			}
		}

		if (auto elem = psRoot->FirstChildElement("Outputs"))
		{
			ret &= parseOutput(elem);
		}

		return ret;
	}

	bool parseTechnique(tinyxml2::XMLNode* techniqueRoot, ShaderResourceExtraData::Technique& out, Common& common)
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

	bool ShaderResourceLoader::loadResource(U8* buffer, size_t size, std::shared_ptr<ResourceHandle> handle)
	{
		std::shared_ptr<ShaderResourceExtraData> extra(new ShaderResourceExtraData());

		tinyxml2::XMLDocument xmlDoc;

		tinyxml2::XMLError result = xmlDoc.Parse(reinterpret_cast<const char*>(buffer), size);
		if (result == tinyxml2::XMLError::XML_SUCCESS)
		{
			tinyxml2::XMLNode* root = xmlDoc.RootElement();
			Common ShaderCommon;

			if (auto element = root->ToElement())
			{
				ShaderCommon.fileName = element->Attribute("file");
			}

			bool ok = false;
			do
			{
				if (!ShaderCommon.fileName)
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

					/*if (auto elem = root->FirstChildElement("InputLayout"))
					{
						parseInputLayout(elem);
					}
					*/
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
						ShaderResourceExtraData::Technique t;
						if (parseTechnique(child, t, ShaderCommon))
						{
							extra->m_techniques.push_back(t);
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

			return true;
		}

		handle->setExtra(extra);
		return true;
	}
}