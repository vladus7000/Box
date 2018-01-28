#pragma once

#include <System/ResourceSystem/ResourceLoader.hpp>

namespace box
{
	class SdkmeshResourceLoader : public ResourceLoader
	{
	public:
		virtual ~SdkmeshResourceLoader() = default;
		virtual const std::string getPattern() const { return "*.sdkmesh"; }
		virtual bool useRawFile() const { return false; }
		virtual bool discardRawBufferAfterLoad() const { return false; }
		virtual size_t getLoadedResourceSize(U8* buffer, size_t size) { return size; }
		virtual bool loadResource(U8* buffer, size_t size, std::shared_ptr<ResourceHandle> handle);
	};
}