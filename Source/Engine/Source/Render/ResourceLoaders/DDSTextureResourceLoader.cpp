#include "StdAfx.hpp"
#include <Render/ResourceLoaders/DDSTextureResourceLoader.hpp>
#include <Render/ResourceLoaders/TextureResourceExtraData.hpp>
#include <System/ResourceSystem/ResourceHandle.hpp>
#include <System/ResourceSystem/ResourceManager.hpp>

#include <DXUT11\Core\DXUT.h>
#include <d3dx11tex.h>
#include <tinyxml2/tinyxml2.h>

namespace box
{
	void fillSamplerElement(D3D11_SAMPLER_DESC& out, const char* name, const char* value)
	{
		if (!strcmp(name, "Filter"))
		{
			if (!strcmp(value, "MIN_MAG_MIP_LINEAR"))
			{
				out.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			}
		}
		else if (!strcmp(name, "AddressU"))
		{
			if (!strcmp(value, "WRAP"))
			{
				out.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			}
		}
		else if (!strcmp(name, "AddressV"))
		{
			if (!strcmp(value, "WRAP"))
			{
				out.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			}
		}
		else if (!strcmp(name, "AddressW"))
		{
			if (!strcmp(value, "WRAP"))
			{
				out.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			}
		}
		else if (!strcmp(name, "CmpFunc"))
		{
			if (!strcmp(value, "NEVER"))
			{
				out.ComparisonFunc = D3D11_COMPARISON_NEVER;
			}
		}
	}

	DDSTextureResourceLoader::~DDSTextureResourceLoader()
	{
	}

	bool DDSTextureResourceLoader::loadResource(U8* buffer, size_t size, std::shared_ptr<ResourceHandle> handle)
	{
		std::shared_ptr<TextureResourceExtraData> extra(new TextureResourceExtraData());

		// Load the Texture
		if (FAILED(D3DX11CreateShaderResourceViewFromMemory(DXUTGetD3D11Device(), buffer, size, NULL, NULL, &extra->m_shaderResourceView, NULL)))
		{
			return false;
		}

		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		box::Resource samplerState(handle->getResource().m_name + ".xml");
		auto samplerHandle = box::ResourceManager::Instance().getHandle(samplerState);
		if (samplerHandle)
		{
			const U8* data = samplerHandle->buffer();
			const size_t size = samplerHandle->getSize();
			tinyxml2::XMLDocument xmlDoc;
			tinyxml2::XMLError result = xmlDoc.Parse(reinterpret_cast<const char*>(data), size);
			if (result == tinyxml2::XMLError::XML_SUCCESS)
			{
				if (tinyxml2::XMLNode* root = xmlDoc.RootElement())
				{
					for (tinyxml2::XMLNode* child = root->FirstChildElement(); child; child = child->NextSibling())
					{
						if (auto element = child->ToElement())
						{
							const char* name = element->Name();
							const char* val = element->GetText();
							if (name && val)
							{
								fillSamplerElement(sampDesc, name, val);
							}
						}
					}
				}
			}
		}

		if (FAILED(DXUTGetD3D11Device()->CreateSamplerState(&sampDesc, &extra->m_samplerState)))
		{
			return false;
		}

		handle->setExtra(extra);
		return true;
	}
}