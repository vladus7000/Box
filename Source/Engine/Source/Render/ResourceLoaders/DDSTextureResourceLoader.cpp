#include "StdAfx.hpp"
#include <Render/ResourceLoaders/XmlHelper.hpp>
#include <Render/ResourceLoaders/DDSTextureResourceLoader.hpp>
#include <Render/Texture.hpp>
#include "System/ResourceSystem/XMLResourceExtraData.hpp"
#include <System/ResourceSystem/ResourceHandle.hpp>
#include <System/ResourceSystem/ResourceManager.hpp>

#include <DXUT11\Core\DXUT.h>
#include <d3dx11tex.h>

namespace box
{
	bool parseSamplerState(tinyxml2::XMLElement* samplerRoot, Microsoft::WRL::ComPtr<ID3D11SamplerState>& out)
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

		if (FAILED(DXUTGetD3D11Device()->CreateSamplerState(&sampDesc, out.GetAddressOf())))
		{
			return false;
		}
		return true;
	}

	DDSTextureResourceLoader::~DDSTextureResourceLoader()
	{
	}

	DDSTextureResourceLoader::DDSTextureResourceLoader()
	{
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		DXUTGetD3D11Device()->CreateSamplerState(&samplerDesc, m_defaultSamplerState.GetAddressOf());
	}

	bool DDSTextureResourceLoader::loadResource(U8* buffer, size_t size, std::shared_ptr<ResourceHandle> handle)
	{
		std::shared_ptr<Texture> extra = std::make_shared<Texture>();

		// Load the Texture
		if (FAILED(D3DX11CreateShaderResourceViewFromMemory(DXUTGetD3D11Device(), buffer, size, NULL, NULL, extra->m_shaderResourceView.GetAddressOf(), NULL)))
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
				if (!parseSamplerState(root->ToElement(), extra->m_samplerState))
				{
					return false;
				}
			}
		}
		else
		{
			extra->m_samplerState = m_defaultSamplerState;
		}

		handle->setExtra(extra);
		return true;
	}
}