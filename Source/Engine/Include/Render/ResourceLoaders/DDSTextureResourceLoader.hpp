#pragma once

#include <System/ResourceSystem/ResourceLoader.hpp>

namespace box
{
	class DDSTextureResourceLoader : public ResourceLoader
	{
	public:
		virtual ~DDSTextureResourceLoader();
		virtual const std::string getPattern() const { return "*.dds"; }
		virtual bool useRawFile() const { return false; }
		virtual bool discardRawBufferAfterLoad() const { return true; }
		virtual size_t getLoadedResourceSize(U8* buffer, size_t size) { return 0; }
		virtual bool loadResource(U8* buffer, size_t size, std::shared_ptr<ResourceHandle> handle);
	};
}