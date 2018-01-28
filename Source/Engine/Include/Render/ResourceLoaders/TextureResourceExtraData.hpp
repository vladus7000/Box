#pragma once

#include "System/ResourceSystem/ResourceExtraData.hpp"
#include <D3D11.h>

namespace box
{
	class TextureResourceExtraData : public ResourceExtraData
	{
	public:
		class TextureData;
		~TextureResourceExtraData() = default;

		virtual const std::string& getName() const { return "TextureResourceExtraData"; }
		const TextureData& getTextureData() const { return m_textureData; }
		TextureData& getTextureData() { return m_textureData; }

		class TextureData
		{
			void releaseAll()
			{
				SAVE_RELEASE(shaderResourceView);
				SAVE_RELEASE(samplerState);
				SAVE_RELEASE(texture);
			}
		public:
			TextureData()
				: shaderResourceView(nullptr)
				, samplerState(nullptr)
				, texture(nullptr)
			{}
			~TextureData()
			{
				releaseAll();
			}

			TextureData(const TextureData& rhs)
				: TextureData()
			{
				*this = rhs;
			}
			TextureData(TextureData&& rhs)
				: TextureData()
			{
				*this = rhs;
			}

			TextureData& operator=(const TextureData& rhs)
			{
				releaseAll();

				if (rhs.shaderResourceView)
				{
					shaderResourceView = rhs.shaderResourceView;
					shaderResourceView->AddRef();
				}
				if (rhs.samplerState)
				{
					samplerState = rhs.samplerState;
					samplerState->AddRef();
				}
				if (rhs.texture)
				{
					texture = rhs.texture;
					texture->AddRef();
				}
				return *this;
			}
			TextureData& operator=(TextureData&& rhs)
			{
				releaseAll();
				if (rhs.shaderResourceView)
				{
					shaderResourceView = rhs.shaderResourceView;
					rhs.shaderResourceView = nullptr;
				}
				if (rhs.samplerState)
				{
					samplerState = rhs.samplerState;
					rhs.samplerState = nullptr;
				}
				if (rhs.texture)
				{
					texture = rhs.texture;
					rhs.texture = nullptr;
				}
				return *this;
			}

			ID3D11ShaderResourceView* shaderResourceView;
			ID3D11SamplerState* samplerState;
			ID3D11Texture2D* texture;
		};
		private:
			friend class DDSTextureResourceLoader;
		TextureData m_textureData;
	};
}