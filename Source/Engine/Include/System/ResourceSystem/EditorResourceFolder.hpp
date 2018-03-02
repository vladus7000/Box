#pragma once

#include "ResourceFile.hpp"
#include <vector>

namespace box
{
	class EditorResourceFolder : public ResourceFile
	{
	public:
		EditorResourceFolder(const std::string& folderName);
		virtual ~EditorResourceFolder() = default;
		virtual bool open() override;
		virtual bool isOpened() const override;
		virtual  size_t getRawResourceSize(const Resource&) override;
		virtual size_t getRawResource(const Resource&, U8*) override;
		virtual size_t getResourcesCount() const override;
		virtual std::string getResourceName(size_t) const override;
	private:
		bool m_opened;
		std::string m_folderName;
		std::vector<std::string> m_files;
	};
}