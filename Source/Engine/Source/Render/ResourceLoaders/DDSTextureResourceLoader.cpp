#include "StdAfx.hpp"
#include <Render/ResourceLoaders/DDSTextureResourceLoader.hpp>
#include <Render/ResourceLoaders/TextureResourceExtraData.hpp>
#include <System/ResourceSystem/ResourceHandle.hpp>

#include <DXUT11\Core\DXUT.h>
#include <d3dx11tex.h>

namespace box
{
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
		if (FAILED(DXUTGetD3D11Device()->CreateSamplerState(&sampDesc, &extra->m_samplerState)))
		{
			return false;
		}

		handle->setExtra(extra);
		return true;
	}
}