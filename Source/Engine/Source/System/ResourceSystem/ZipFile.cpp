#include "StdAfx.hpp"
#include <stdio.h>
#include "System\ResourceSystem\ZipFile.hpp"
#include "System\ResourceSystem\Resource.hpp"
#include "zlib\zlib.h"

#include <map>
#include <algorithm>
#include <functional>

namespace box
{
	typedef unsigned long dword;
	typedef unsigned short word;
	typedef unsigned char byte;

	class ZipFile
	{
		using ZipContentsMap = std::map<std::string, int>;
	public:
		ZipFile()
			: m_file(nullptr)
			, m_dirData(nullptr)
			, m_numEntries(0)
			, m_papDir(nullptr)
		{
		}

		~ZipFile()
		{

		}

		bool init(const std::string& fileName)
		{
			deinit();
			m_file = fopen(fileName.c_str(), "rb");

			if (!m_file)
			{
				deinit();
				return false;
			}


			// Assuming no extra comment at the end, read the whole end record.
			ZipDirHeader dh;

			fseek(m_file, -(size_t)sizeof(dh), SEEK_END);
			long dhOffset = ftell(m_file);
			memset(&dh, 0, sizeof(dh));
			fread(&dh, sizeof(dh), 1, m_file);

			// Check
			if (dh.sig != ZipDirHeader::SIGNATURE)
				return false;

			// Go to the beginning of the directory.
			fseek(m_file, dhOffset - dh.dirSize, SEEK_SET);

			// Allocate the data buffer, and read the whole thing.
			m_dirData = new U8[dh.dirSize + dh.nDirEntries * sizeof(*m_papDir)];
			if (!m_dirData)
				return false;
			memset(m_dirData, 0, dh.dirSize + dh.nDirEntries * sizeof(*m_papDir));
			fread(m_dirData, dh.dirSize, 1, m_file);

			// Now process each entry.
			U8* pfh = m_dirData;
			m_papDir = (const ZipDirFileHeader **)(m_dirData + dh.dirSize);

			bool success = true;

			for (int i = 0; i < dh.nDirEntries && success; i++)
			{
				ZipDirFileHeader &fh = *(ZipDirFileHeader*)pfh;

				// Store the address of nth file for quicker access.
				m_papDir[i] = &fh;

				// Check the directory entry integrity.
				if (fh.sig != ZipDirFileHeader::SIGNATURE)
					success = false;
				else
				{
					pfh += sizeof(fh);

					// Convert UNIX slashes to DOS backlashes.
					for (int j = 0; j < fh.fnameLen; j++)
						if (pfh[j] == '/')
							pfh[j] = '\\';

					char fileName[_MAX_PATH];
					memcpy(fileName, pfh, fh.fnameLen);
					fileName[fh.fnameLen] = 0;
					_strlwr_s(fileName, _MAX_PATH);
					std::string spath = fileName;
					m_contentsMap[spath] = i;

					// Skip name, extra and comment fields.
					pfh += fh.fnameLen + fh.xtraLen + fh.cmntLen;
				}
			}
			if (!success)
			{
				delete [] m_dirData;
			}
			else
			{
				m_numEntries = dh.nDirEntries;
			}

			return success;
		}

		void deinit()
		{
			m_contentsMap.clear();
			m_numEntries = 0;
			if (m_file)
			{
				fclose(m_file);
				m_file = nullptr;
			}

			if (m_dirData)
			{
				delete[] m_dirData;
				m_dirData = nullptr;
			}
		}

		size_t getNumFiles() { return m_numEntries; }
		std::string getFileName(size_t i)
		{
			std::string fileName = "";
			if (i >= 0 && i < m_numEntries)
			{
				char pszDest[_MAX_PATH];
				memcpy(pszDest, m_papDir[i]->GetName(), m_papDir[i]->fnameLen);
				pszDest[m_papDir[i]->fnameLen] = '\0';
				fileName = pszDest;
			}
			return fileName;
		}

		size_t getFileSize(size_t i)
		{
			if (i < 0 || i >= m_numEntries)
				return 0;
			else
				return m_papDir[i]->ucSize;
		}

		bool readFile(size_t i, U8* buffer)
		{
			if (buffer == nullptr || i < 0 || i >= m_numEntries)
				return false;

			// Quick'n dirty read, the whole file at once.
			// Ungood if the ZIP has huge files inside

			// Go to the actual file and read the local header.
			fseek(m_file, m_papDir[i]->hdrOffset, SEEK_SET);
			ZipLocalHeader h;

			memset(&h, 0, sizeof(h));
			fread(&h, sizeof(h), 1, m_file);
			if (h.sig != ZipLocalHeader::SIGNATURE)
				return false;

			// Skip extra fields
			fseek(m_file, h.fnameLen + h.xtraLen, SEEK_CUR);

			if (h.compression == Z_NO_COMPRESSION)
			{
				// Simply read in raw stored data.
				fread(buffer, h.cSize, 1, m_file);
				return true;
			}
			else if (h.compression != Z_DEFLATED)
				return false;

			// Alloc compressed data buffer and read the whole stream
			char *pcData = new char[h.cSize];
			if (!pcData)
				return false;

			memset(pcData, 0, h.cSize);
			fread(pcData, h.cSize, 1, m_file);

			bool ret = true;

			// Setup the inflate stream.
			z_stream stream;
			int err;

			stream.next_in = (Bytef*)pcData;
			stream.avail_in = (uInt)h.cSize;
			stream.next_out = (Bytef*)buffer;
			stream.avail_out = h.ucSize;
			stream.zalloc = (alloc_func)0;
			stream.zfree = (free_func)0;

			// Perform inflation. wbits < 0 indicates no zlib header inside the data.
			err = inflateInit2(&stream, -MAX_WBITS);
			if (err == Z_OK)
			{
				err = inflate(&stream, Z_FINISH);
				inflateEnd(&stream);
				if (err == Z_STREAM_END)
					err = Z_OK;
				inflateEnd(&stream);
			}
			if (err != Z_OK)
				ret = false;

			delete[] pcData;
			return ret;
		}

		int find(const std::string& path)
		{
			std::string lowerCase = path;
			std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), ::tolower);
			auto it = m_contentsMap.find(lowerCase);
			if (it == m_contentsMap.end())
			{
				return -1;
			}

			return it->second;
		}

	private:
#pragma pack(1)
		struct ZipLocalHeader
		{
			enum
			{
				SIGNATURE = 0x04034b50,
			};
			dword   sig;
			word    version;
			word    flag;
			word    compression;      // Z_NO_COMPRESSION or Z_DEFLATED
			word    modTime;
			word    modDate;
			dword   crc32;
			dword   cSize;
			dword   ucSize;
			word    fnameLen;         // Filename string follows header.
			word    xtraLen;          // Extra field follows filename.
		};

		struct ZipDirHeader
		{
			enum
			{
				SIGNATURE = 0x06054b50
			};
			dword   sig;
			word    nDisk;
			word    nStartDisk;
			word    nDirEntries;
			word    totalDirEntries;
			dword   dirSize;
			dword   dirOffset;
			word    cmntLen;
		};

		struct ZipDirFileHeader
		{
			enum
			{
				SIGNATURE = 0x02014b50
			};
			dword   sig;
			word    verMade;
			word    verNeeded;
			word    flag;
			word    compression;      // COMP_xxxx
			word    modTime;
			word    modDate;
			dword   crc32;
			dword   cSize;            // Compressed size
			dword   ucSize;           // Uncompressed size
			word    fnameLen;         // Filename string follows header.
			word    xtraLen;          // Extra field follows filename.
			word    cmntLen;          // Comment field follows extra field.
			word    diskStart;
			word    intAttr;
			dword   extAttr;
			dword   hdrOffset;

			char *GetName() const { return (char *)(this + 1); }
			char *GetExtra() const { return GetName() + fnameLen; }
			char *GetComment() const { return GetExtra() + xtraLen; }
		};
#pragma pack()
		ZipContentsMap m_contentsMap;
		FILE* m_file;
		U8* m_dirData;
		size_t m_numEntries;
		const ZipDirFileHeader** m_papDir;
	};

	ResourceZipFile::ResourceZipFile(const std::string& path)
		: m_zipFile(nullptr)
		, m_fileName(path)
		, m_isOpened(false)
	{
	}

	ResourceZipFile::~ResourceZipFile()
	{
		if (m_zipFile)
		{
			m_zipFile->deinit();

			delete m_zipFile;
			m_zipFile = nullptr;

			m_isOpened = false;
		}
	}

	bool ResourceZipFile::open()
	{
		if (m_isOpened)
		{
			return true;
		}

		m_zipFile = new ZipFile();
		bool ret = false;
		if (m_zipFile->init(m_fileName))
		{
			ret = true;
		}
		m_isOpened = ret;
		return ret;
	}

	size_t ResourceZipFile::getRawResourceSize(const Resource& r)
	{
		int resourceIdx = m_zipFile->find(r.m_name);
		if (resourceIdx < 0)
		{
			return 0;
		}
		return m_zipFile->getFileSize(resourceIdx);
	}

	size_t ResourceZipFile::getRawResource(const Resource& r, U8* buffer)
	{
		size_t resourceIdx = m_zipFile->find(r.m_name);

		if (resourceIdx < 0)
		{
			return 0;
		}

		size_t size = m_zipFile->getFileSize(resourceIdx);
		m_zipFile->readFile(resourceIdx, buffer);

		return size;
	}

	size_t ResourceZipFile::getResourcesCount() const
	{
		return m_zipFile->getNumFiles();
	}

	std::string ResourceZipFile::getResourceName(size_t i) const
	{
		std::string name("");
		if (i >= 0 && i < m_zipFile->getNumFiles())
		{
			name = m_zipFile->getFileName(i);
		}
		return name;
	}
}