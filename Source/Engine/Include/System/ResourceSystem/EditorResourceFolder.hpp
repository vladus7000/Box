#pragma once

#include "ResourceFile.hpp"
#include "Resource.hpp"

#include <vector>

#include <stdio.h>

namespace box
{
	class EditorResourceFolder : public ResourceFile
	{
	public:
		EditorResourceFolder(const std::string& folderName);
		virtual ~EditorResourceFolder();
		virtual bool open() override;
		virtual bool isOpened() const override;
		virtual  size_t getRawResourceSize(const Resource&) override;
		virtual size_t getRawResource(const Resource&, U8*) override;
		virtual size_t getResourcesCount() const override;
		virtual std::string getResourceName(size_t) const override;
		virtual std::string getFullResourceName(size_t) const override;
	private:
		struct FileWrapper
		{
			FileWrapper(const std::string& fileName, std::size_t size, FILE* file)
				: name(fileName)
				, fileSize(size)
				, handle(file)
				{}

			Resource name;
			std::size_t fileSize = 0;
			FILE* handle = nullptr;
		};

		bool m_opened;
		std::string m_folderName;
		std::vector<FileWrapper> m_files;
	};
}