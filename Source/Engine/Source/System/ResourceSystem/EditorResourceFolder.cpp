#include "StdAfx.hpp"
#include "System/ResourceSystem/EditorResourceFolder.hpp"
#include "System/ResourceSystem/Resource.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <windows.h>

namespace
{
	bool isFolderExists(const char* path)
	{
		struct stat info;
		bool result = false;
		if (stat(path, &info) != 0)
		{
			result = false;
		}
		else if (info.st_mode & S_IFDIR)
		{
			result = true;
		}
		else
		{
			result = false;
		}
		return result;
	}

	void listDir(const std::wstring& folderName, std::vector<std::string>& out)
	{
		std::wstring pattern(folderName.begin(), folderName.end());
		pattern.append(L"\\*");

		WIN32_FIND_DATA f;
		HANDLE h = FindFirstFile(pattern.c_str(), &f);

		if (h == INVALID_HANDLE_VALUE) { return; }

		do
		{
			std::wstring name(f.cFileName);
			std::wstring filePath = folderName + L"/" + name;

			if (name == L"." || name == L"..") { continue; }

			if (f.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				listDir(filePath, out);
			}
			else
			{
				out.push_back(std::string(filePath.begin(), filePath.end()));
			}
		}while (FindNextFile(h, &f));
		FindClose(h);
	}

	void getListOfFiles(const std::string& folderName, std::vector<std::string>& out)
	{
		std::wstring folder(folderName.begin(), folderName.end());
		listDir(folder, out);
	}
}

namespace box
{

	EditorResourceFolder::EditorResourceFolder(const std::string& folderName)
		: m_opened(false)
		, m_folderName(folderName)
	{
	}

	bool EditorResourceFolder::open()
	{
		if (isFolderExists(m_folderName.c_str()))
		{
			m_opened = true;
			getListOfFiles(m_folderName, m_files);

			for (std::string& it : m_files)
			{
				it.erase(0, m_folderName.size() + 1);
			}

		}
		return m_opened;
	}

	bool EditorResourceFolder::isOpened() const
	{
		return m_opened;
	}

	size_t EditorResourceFolder::getRawResourceSize(const Resource& r)
	{
		for (const auto& file : m_files)
		{
			if (file == r.m_name)
			{
				return 1;
			}
		}

		return 0;
	}

	size_t EditorResourceFolder::getRawResource(const Resource& r, U8* data)
	{
		return 0;
	}

	size_t EditorResourceFolder::getResourcesCount() const
	{
		return 0;
	}

	std::string EditorResourceFolder::getResourceName(size_t i) const
	{
		return "";
	}

}
