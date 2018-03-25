#include "gtest/gtest.h"
#include "System/Types.hpp"
#include "System/ResourceSystem/ResourceManager.hpp"
#include "System/ResourceSystem/ResourceCache.hpp"
#include "System/ResourceSystem/ResourceFile.hpp"

#include <vector>
#include <string>
#include <memory>

using namespace box;
template< typename T >
struct array_deleter
{
	void operator ()(T const * p)
	{
		delete[] p;
	}
};

class TestResFile : public box::ResourceFile
{
	struct File
	{
		std::string name;
		size_t size;
		std::shared_ptr<char> data;
	};

public:
	TestResFile()
	{
		static const char test1[] = "# Blender MTL File: 'None'\
			# Material Count : 2\
			newmtl Material\
			Ns 96.078431\
			Ka 1.000000 1.000000 1.000000\
			Kd 0.640000 0.640000 0.640000\
			Ks 0.500000 0.500000 0.500000\
			Ke 0.000000 0.000000 0.000000\
			Ni 1.000000\
			d 1.000000\
			illum 2\
			newmtl None\
			Ns 0\
			Ka 0.000000 0.000000 0.000000\
			Kd 0.8 0.8 0.8\
			Ks 0.8 0.8 0.8\
			d 1\
			illum 2";

		size_t size1 = sizeof(test1);

		File f;
		{


			f.name = "scripts/test.txt";
			f.size = size1;
			f.data = std::shared_ptr<char>(new char[size1], array_deleter<char>());
			memcpy(f.data.get(), test1, size1);
			m_resFile.push_back(f);
		}

	}

	~TestResFile()
	{
	}

	virtual bool open() override
	{
		return true;
	}

	virtual bool isOpened() const override
	{
		return true;
	}

	virtual  size_t getRawResourceSize(const Resource& r) override
	{
		for (auto& it : m_resFile)
		{
			if (it.name == r.m_name)
			{
				return it.size;
				break;
			}
		}

		return 0;
	}

	virtual size_t getRawResource(const Resource& r, U8* out) override
	{
		for (auto& it : m_resFile)
		{
			if (it.name == r.m_name)
			{
				memcpy(out, it.data.get(), it.size);
				return it.size;
				break;
			}
		}

		return 0;
	}

	virtual size_t getResourcesCount() const override
	{
		return m_resFile.size();
	}

	virtual std::string getResourceName(size_t i) const override
	{
		return m_resFile[i].name;
	}

	virtual std::string getFullResourceName(size_t i) const override
	{
		return m_resFile[i].name;
	}
private:
	std::vector<File> m_resFile;
};

TEST(ResourceManager_Startup_test, Positive)
{
	
	const bool result = ResourceManager::Instance().init();
	ResourceManager::Instance().deinit();
	EXPECT_TRUE(result);
}

TEST(ResourceCache_test, Positive)
{
	{
		TestResFile* resFile = new TestResFile();
		ResourceCache rCache(1, resFile);
		rCache.init();

		{
			box::Resource r("scripts/test.txt");
			auto handle = rCache.getHandle(r);
			size_t size = handle->getSize();
			U8* d = handle->buffer();
			EXPECT_TRUE(handle != nullptr);
		}

		{
			box::Resource r("scripts/test.txt");
			auto handle = rCache.getHandle(r);
			size_t size = handle->getSize();
			U8* d = handle->buffer();
			EXPECT_TRUE(handle != nullptr);
		}

		{
			box::Resource r("scripts/test1.txt");
			auto handle = rCache.getHandle(r);
			EXPECT_TRUE(handle == nullptr);
		}

	}
}
