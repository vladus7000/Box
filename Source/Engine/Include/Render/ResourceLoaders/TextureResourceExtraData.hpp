#pragma once

#include "System/ResourceSystem/ResourceExtraData.hpp"
#include "Render/Texture.hpp"
#include <D3D11.h>
#include <wrl.h>

namespace box
{
	class TextureResourceExtraData : public ResourceExtraData
	{
	public:
		class TextureData;
		~TextureResourceExtraData() = default;

		virtual const std::string& getName() const { return "TextureResourceExtraData"; }
		//const TextureData& getTextureData() const { return m_textureData; }
		//TextureData& getTextureData() { return m_textureData; }

		private:
			friend class DDSTextureResourceLoader;

	};
}