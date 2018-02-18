#include "StdAfx.hpp"
#include <Render/ResourceLoaders/XmlHelper.hpp>
#include <Render/ResourceLoaders/DDSTextureResourceLoader.hpp>
#include <Render/ResourceLoaders/TextureResourceExtraData.hpp>
#include "System/ResourceSystem/XMLResourceExtraData.hpp"
#include <System/ResourceSystem/ResourceHandle.hpp>
#include <System/ResourceSystem/ResourceManager.hpp>

#include <DXUT11\Core\DXUT.h>
#include <d3dx11tex.h>

namespace box
{
	bool parseSamplerState(tinyxml2::XMLElement* samplerRoot, ID3D11SamplerState*& out)
	{
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));

		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		START_LOOP(samplerRoot)
			ITEM_TEXT(sampDesc.Filter, "Filter", g_Filters);
			ITEM_TEXT(sampDesc.AddressU, "AddressU", g_addressModes);
			ITEM_TEXT(sampDesc.AddressV, "AddressV", g_addressModes);
			ITEM_TEXT(sampDesc.AddressW, "AddressW", g_addressModes);
			ITEM_TEXT(sampDesc.ComparisonFunc, "CmpFunc", g_cmpFuncs);
			ITEM_FLOAT(sampDesc.MinLOD, "MinLod");
			ITEM_FLOAT(sampDesc.MaxLOD, "MaxLOD");
		END_LOOP

		if (FAILED(DXUTGetD3D11Device()->CreateSamplerState(&sampDesc, &out)))
		{
			return false;
		}
		return true;
	}

	DDSTextureResourceLoader::~DDSTextureResourceLoader()
	{
	}

	bool DDSTextureResourceLoader::loadResource(U8* buffer, size_t size, std::shared_ptr<ResourceHandle> handle)
	{
		std::shared_ptr<TextureResourceExtraData> extra(new TextureResourceExtraData());

		// Load the Texture
		if (FAILED(D3DX11CreateShaderResourceViewFromMemory(DXUTGetD3D11Device(), buffer, size, NULL, NULL, &extra->getTextureData().shaderResourceView, NULL)))
		{
			return false;
		}

		box::Resource samplerStateResource(handle->getResource().m_name + ".xml");
		auto samplerHandleXML = box::ResourceManager::Instance().getHandle(samplerStateResource);

		auto xmlData = samplerHandleXML->getExtraTyped<box::XMLResourceExtraData>();
		if (xmlData)
		{
			if (auto root = xmlData->getXMLRoot())
			{
				if (parseSamplerState(root->ToElement(), extra->getTextureData().samplerState))
				{
					handle->setExtra(extra);
					return true;
				}
				else
				{
					return false;
				}
			}
		}

		return true;
	}
}