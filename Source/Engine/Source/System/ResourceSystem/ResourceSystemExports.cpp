#include "StdAfx.hpp"
#include "System/ResourceSystem/ResourceSystemExports.hpp"
#include "System/Engine.hpp"

#include "System/ResourceSystem/ResourceManager.hpp"

namespace Exports
{
	extern box::Engine* g_engineInstance;

namespace Resources
{
#define CHECK_ENGINE() if (!g_engineInstance) { return -1; }
	int ImportModel(const char* fileName)
	{
		CHECK_ENGINE();

		box::ResourceManager::Instance().importModelFromFile(fileName);

		return 0;
	}

	int GetResourceCollectionSizeForXml()
	{
		CHECK_ENGINE();
		return box::ResourceManager::Instance().getResourceCollectionSizeForXml();
	}

	int SerializeResourceCollectionToXml(char* out)
	{
		CHECK_ENGINE();
		box::ResourceManager::Instance().serializeResourceCollectionToXml(out);
		return 0;
	}

	int LoadLevelFromXMLFile(const char* fileName)
	{
		CHECK_ENGINE();
		return 0;
	}

	int LoadLevelFromXMLBuffer(const char* buffer)
	{
		CHECK_ENGINE();
		return 0;
	}

	int GetLevelSizeForXml()
	{
		CHECK_ENGINE();
		return 0;
	}

	int SaveLevelToXMLFile(const char* fileName)
	{
		CHECK_ENGINE();
		return 0;
	}

	int SerializeLevelToXML(char* out)
	{
		CHECK_ENGINE();
		return 0;
	}

	int GetLevelInfoSizeForXml()
	{
		CHECK_ENGINE();
		return 0;

	}

	int SerializeLevelInfoToXml(char* out)
	{
		CHECK_ENGINE();
		return 0;
	}
}
}