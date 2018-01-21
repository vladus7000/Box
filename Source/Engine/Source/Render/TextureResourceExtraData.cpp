#include "StdAfx.hpp"
#include <Render/TextureResourceExtraData.hpp>

namespace box
{
	box::TextureResourceExtraData::~TextureResourceExtraData()
	{
		if (m_shaderResourceView)
		{
			m_shaderResourceView->Release();
			m_shaderResourceView = nullptr;
		}
		if (m_samplerState)
		{
			m_samplerState->Release();
			m_samplerState = nullptr;
		}
		
		if (m_texture)
		{
			m_texture->Release();
			m_texture = nullptr;
		}
	}
}