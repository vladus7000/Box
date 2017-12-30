#pragma once

#include <string>
#include <memory>

namespace box
{
	class ResourceHandle;
	class ResourceLoader
	{
	public:
		using StrongResourceLoaderPtr = std::shared_ptr<ResourceLoader>;
		using WeakResourceLoaderPtr = std::weak_ptr<ResourceLoader>;

	public:
		ResourceLoader() = default;
		virtual ~ResourceLoader() = default;
		virtual const std::string getPattern() const = 0;
		virtual bool useRawFile() const = 0;
		virtual size_t getLoadedResourceSize(U8* buffer, size_t size) = 0;
		virtual bool loadResource(U8* buffer, size_t size, std::shared_ptr<ResourceHandle> handle) = 0;
	};
}