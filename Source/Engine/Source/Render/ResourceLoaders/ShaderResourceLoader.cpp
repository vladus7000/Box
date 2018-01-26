#include "StdAfx.hpp"
#include <Render/ResourceLoaders/ShaderResourceLoader.hpp>
#include <Render/ResourceLoaders/ShaderResourceExtraData.hpp>
#include <System/ResourceSystem/ResourceHandle.hpp>
#include <System/ResourceSystem/ResourceManager.hpp>

#include <DXUT11\Core\DXUT.h>
#include <d3dx11tex.h>
#include <tinyxml2/tinyxml2.h>
#include <map>
#include <string>

namespace
{
	std::map<std::string, D3D11_COMPARISON_FUNC> getCmpFuncs()
	{
		std::map<std::string, D3D11_COMPARISON_FUNC> ret;
		ret["Never"] = D3D11_COMPARISON_NEVER;
		ret["Less"] = D3D11_COMPARISON_LESS;
		ret["Equal"] = D3D11_COMPARISON_EQUAL;
		ret["LessEqual"] = D3D11_COMPARISON_LESS_EQUAL;
		ret["Greater"] = D3D11_COMPARISON_GREATER;
		ret["NotEqual"] = D3D11_COMPARISON_NOT_EQUAL;
		ret["GrEqual"] = D3D11_COMPARISON_GREATER_EQUAL;
		ret["Always"] = D3D11_COMPARISON_ALWAYS;
		return ret;
	}

	std::map<std::string, D3D11_FILL_MODE> getFillMode()
	{
		std::map<std::string, D3D11_FILL_MODE> ret;
		ret["Solid"] = D3D11_FILL_SOLID;
		ret["Wireframe"] = D3D11_FILL_WIREFRAME;
		return ret;
	}
	std::map<std::string, D3D11_CULL_MODE> getCullModes()
	{
		std::map<std::string, D3D11_CULL_MODE> ret;
		ret["None"] = D3D11_CULL_NONE;
		ret["Front"] = D3D11_CULL_FRONT;
		ret["Back"] = D3D11_CULL_BACK;
		return ret;
	}
	std::map<std::string, D3D11_DEPTH_WRITE_MASK> getWriteMasks()
	{
		std::map<std::string, D3D11_DEPTH_WRITE_MASK> ret;
		ret["MaskAll"] = D3D11_DEPTH_WRITE_MASK_ALL;
		ret["MaskZero"] = D3D11_DEPTH_WRITE_MASK_ZERO;

		return ret;
	}

	std::map<std::string, D3D11_STENCIL_OP> getStencilOp()
	{
		std::map<std::string, D3D11_STENCIL_OP> ret;
		ret["Keep"] = D3D11_STENCIL_OP_KEEP;
		ret["Zero"] = D3D11_STENCIL_OP_ZERO;
		ret["Replace"] = D3D11_STENCIL_OP_REPLACE;
		ret["IncrSat"] = D3D11_STENCIL_OP_INCR_SAT;
		ret["DecrSat"] = D3D11_STENCIL_OP_DECR_SAT;
		ret["Invert"] = D3D11_STENCIL_OP_INVERT;
		ret["Incr"] = D3D11_STENCIL_OP_INCR;
		ret["Decr"] = D3D11_STENCIL_OP_DECR;

		return ret;
	}


	std::map<std::string, D3D11_BLEND> getBlend()
	{
		std::map<std::string, D3D11_BLEND> ret;

		ret["Zero"] = D3D11_BLEND_ZERO;
		ret["One"] = D3D11_BLEND_ONE;
		ret["SrcColor"] = D3D11_BLEND_SRC_COLOR;
		ret["InvSrcColor"] = D3D11_BLEND_INV_SRC_COLOR;
		ret["SrcAlpha"] = D3D11_BLEND_SRC_ALPHA;
		ret["InvSrcAlpha"] = D3D11_BLEND_INV_SRC_ALPHA;
		ret["DstAlpha"] = D3D11_BLEND_DEST_ALPHA;
		ret["InvDstAlpha"] = D3D11_BLEND_INV_DEST_ALPHA;
		ret["DstColor"] = D3D11_BLEND_DEST_COLOR;
		ret["InvDstColor"] = D3D11_BLEND_INV_DEST_COLOR;
		ret["SrcAlphaSat"] = D3D11_BLEND_SRC_ALPHA_SAT;
		ret["BlendFactor"] = D3D11_BLEND_BLEND_FACTOR;
		ret["InvBlendFactor"] = D3D11_BLEND_INV_BLEND_FACTOR;
		ret["Src1Color"] = D3D11_BLEND_SRC1_COLOR;
		ret["InvSrc1Color"] = D3D11_BLEND_INV_SRC1_COLOR;
		ret["Src1Alpha"] = D3D11_BLEND_SRC1_ALPHA;
		ret["InvSrc1Alpha"] = D3D11_BLEND_INV_SRC1_ALPHA;
		return ret;
	}
	
	std::map<std::string, D3D11_BLEND_OP> getBlendOp()
	{
		std::map<std::string, D3D11_BLEND_OP> ret;
		ret["Add"] = D3D11_BLEND_OP_ADD;
		ret["Substract"] = D3D11_BLEND_OP_SUBTRACT;
		ret["RevSubstract"] = D3D11_BLEND_OP_REV_SUBTRACT;
		ret["Min"] = D3D11_BLEND_OP_MIN;
		ret["Max"] = D3D11_BLEND_OP_MAX;
		return ret;
	}

	std::map<std::string, UINT8> getColorWriteMask()
	{
		std::map<std::string, UINT8> ret;
		ret["Red"] = D3D11_COLOR_WRITE_ENABLE_RED;
		ret["Green"] = D3D11_COLOR_WRITE_ENABLE_GREEN;
		ret["Blue"] = D3D11_COLOR_WRITE_ENABLE_BLUE;
		ret["Alpha"] = D3D11_COLOR_WRITE_ENABLE_ALPHA;
		ret["All"] = D3D11_COLOR_WRITE_ENABLE_ALL;
		return ret;
	}

	std::map<std::string, D3D11_COMPARISON_FUNC> g_cmpFuncs = getCmpFuncs();
	std::map<std::string, D3D11_FILL_MODE> g_fillModes = getFillMode();
	std::map<std::string, D3D11_CULL_MODE> g_cullModes = getCullModes();
	std::map<std::string, D3D11_DEPTH_WRITE_MASK> g_WriteMasks = getWriteMasks();
	std::map<std::string, D3D11_STENCIL_OP> g_stencilOps = getStencilOp();

	std::map<std::string, D3D11_BLEND> g_Blends = getBlend();
	std::map<std::string, D3D11_BLEND_OP> g_BlendOps = getBlendOp();
	std::map<std::string, UINT8> g_colorWriteMasks = getColorWriteMask();
}

#define START_LOOP(root) for (auto item = root->FirstChild(); item; item = item->NextSibling()) { bool found = false;
#define END_LOOP }

#define ITEM_TEXT(var, name, map) if (found)continue;do{ auto el = item->ToElement(); if (!strcmp(el->Attribute("name"), name)) {const char* v = el->Attribute("value"); auto p = map.find(v); if (p != map.end()){var = p->second;found = true;} }   }while(0)
#define ITEM_BOOL(var, name, textValue, dxValue) if (found)continue;do{ auto el = item->ToElement(); if (!strcmp(el->Attribute("name"), name)) { if (!strcmp(el->Attribute("value"), textValue)) {var = dxValue; found = true; } }}while(0)
#define ITEM_BOOL(var, name) if (found)continue;do {auto el = item->ToElement(); if (!strcmp(el->Attribute("name"), name)) { if (!strcmp(el->Attribute("value"), "true")) { var = true; found = true; } else { var = false; found = true; } } } while (0)

#define ITEM_INT(var, name) if (found)continue;do {auto el = item->ToElement(); if (!strcmp(el->Attribute("name"), name)) { int val(0); el->QueryIntAttribute("value", &val); var = val;found = true;} } while (0)
#define ITEM_FLOAT(var, name) if (found)continue;do {auto el = item->ToElement(); if (!strcmp(el->Attribute("name"), name)) { float val(0.0f); el->QueryFloatAttribute("value", &val); var = val;found = true;} } while (0)

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
		// TODO implement reading
		//BOOL AlphaToCoverageEnable;
		//BOOL IndependentBlendEnable;
		D3D11_BLEND_DESC BlendState;
		ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
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