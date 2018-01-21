#pragma once

#include "System/ResourceSystem/ResourceExtraData.hpp"
#include <D3D11.h>

namespace box
{
	class TextureResourceExtraData : public ResourceExtraData
	{
	public:
		~TextureResourceExtraData();

		virtual const std::string& getName() const { return "TextureResourceExtraData"; }
		ID3D11ShaderResourceView* getShaderResourceView() { return m_shaderResourceView; }
		ID3D11SamplerState* getSamplerState() { return m_samplerState; }
		ID3D11Texture2D* getTexture2D() { return m_texture; }

	private:
		friend class DDSTextureResourceLoader;
		ID3D11ShaderResourceView* m_shaderResourceView;
		ID3D11SamplerState* m_samplerState;
		ID3D11Texture2D* m_texture;
	};
}