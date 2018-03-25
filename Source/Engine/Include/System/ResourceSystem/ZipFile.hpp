#pragma once
#include "ResourceFile.hpp"
#include <string>

namespace box
{
	class ZipFile;
}

namespace box
{
	class ResourceZipFile : public ResourceFile
	{
	public:
		explicit ResourceZipFile(const std::string& path);
		virtual ~ResourceZipFile();

		virtual bool open() override;
		virtual bool isOpened() const override { return m_isOpened; }
		virtual  size_t getRawResourceSize(const Resource& r) override;
		virtual size_t getRawResource(const Resource&, U8* buffer) override;
		virtual size_t getResourcesCount() const  override;
		virtual const std::string& getResourceName(size_t i) const  override;

	private:
		ZipFile* m_zipFile;
		std::string m_fileName;
		bool m_isOpened;
	};
}