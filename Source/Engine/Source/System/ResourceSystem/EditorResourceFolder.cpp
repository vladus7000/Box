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

	EditorResourceFolder::~EditorResourceFolder()
	{
		for (auto& file : m_files)
		{
			fclose(file.handle);
		}
		m_files.clear();
	}

	bool EditorResourceFolder::open()
	{
		if (isFolderExists(m_folderName.c_str()))
		{
			m_opened = true;
			std::vector<std::string> files;
			getListOfFiles(m_folderName, files);
			m_files.reserve(files.size());
			for (std::string& it : files)
			{
				openFile(it);
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
			if (file.name == r)
			{
				return file.fileSize;
			}
		}
		return 0;
	}

	size_t EditorResourceFolder::getRawResource(const Resource& r, U8* data)
	{
		size_t size = 0;
		for (const auto& file : m_files)
		{
			if (file.name == r)
			{
				size = file.fileSize;

				size_t result = fread(data, 1, file.fileSize, file.handle);
				if (result != size)
				{
					size = 0;
				}
			}
		}
		return size;
	}

	size_t EditorResourceFolder::getResourcesCount() const
	{
		return m_files.size();
	}

	std::string EditorResourceFolder::getResourceName(size_t i) const
	{
		return m_files[i].name.m_name;
	}

	std::string EditorResourceFolder::getFullResourceName(size_t i) const
	{
		return m_folderName + m_files[i].name.m_name;
	}

	void EditorResourceFolder::resyncFolder()
	{
		std::vector<std::string> files;
		std::vector<std::string> filesToAdd;
		filesToAdd.reserve(2);
		getListOfFiles(m_folderName, files);
		for (std::string& it : files)
		{
			bool found = false;
			std::string tmp = it;
			tmp.erase(0, m_folderName.size() + 1);
			std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
			for (const auto& file : m_files)
			{
				if (file.name.m_name == tmp)
				{
					found = true;
				}
			}
			if (found)
			{
				continue;
			}
			filesToAdd.push_back(it);
		}
		for (auto& it : filesToAdd)
		{
			openFile(it);
		}
	}

	void EditorResourceFolder::openFile(std::string& file)
	{
		FILE* fileHandle = fopen(file.c_str(), "rb");
		ASSERT(fileHandle, "the file should be opened");
		file.erase(0, m_folderName.size() + 1);

		fseek(fileHandle, 0L, SEEK_END);
		std::size_t size = ftell(fileHandle);
		fseek(fileHandle, 0L, SEEK_SET);
		m_files.emplace_back(file, size, fileHandle);
	}

}
