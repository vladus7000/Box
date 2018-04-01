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
	int ImportStaticModel(const char* fileName, const char* modelName)
	{
		CHECK_ENGINE();

		return box::ResourceManager::Instance().importStaticModel(fileName, modelName);
	}

	int ImportDynamicModel(const char* fileName)
	{
		CHECK_ENGINE();
		return box::ResourceManager::Instance().ImportDynamicModel(fileName);
	}

	int ImportShader(const char* fileName)
	{
		CHECK_ENGINE();
		return box::ResourceManager::Instance().ImportShader(fileName);
	}

	int ImportDDSTexture(const char* fileName)
	{
		CHECK_ENGINE();
		return box::ResourceManager::Instance().ImportDDSTexture(fileName);
	}

	int CompileShader(const char* fileName)
	{
		CHECK_ENGINE();
		return box::ResourceManager::Instance().CompileShader(fileName);
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

	int ResyncResourceFolders()
	{
		CHECK_ENGINE();
		box::ResourceManager::Instance().resyncResourceFolders();
		return 0;

	}

}
}