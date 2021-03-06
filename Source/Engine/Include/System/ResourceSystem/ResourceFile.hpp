#pragma once

#include <string>

namespace box
{
	class Resource;
}

namespace box
{
	class ResourceFile
	{
	public:
		ResourceFile() = default;
		virtual ~ResourceFile() = default;
		virtual bool open() = 0;
		virtual bool isOpened() const = 0;
		virtual  size_t getRawResourceSize(const Resource&) = 0;
		virtual size_t getRawResource(const Resource&, U8*) = 0;
		virtual size_t getResourcesCount() const = 0;
		virtual std::string getResourceName(size_t) const = 0;
		virtual std::string getFullResourceName(size_t) const = 0;
		virtual void resyncFolder() = 0;
	};
}