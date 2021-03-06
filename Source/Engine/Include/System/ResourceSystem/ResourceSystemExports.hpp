#pragma once

namespace Exports
{
	namespace Resources
	{
		int ImportStaticModel(const char* fileName, const char* modelName);
		int ImportDynamicModel(const char* fileName);
		int ImportShader(const char* fileName);
		int ImportDDSTexture(const char* fileName);

		int CompileShader(const char* fileName);

		int GetResourceCollectionSizeForXml();
		int SerializeResourceCollectionToXml(char* out);

		int ResyncResourceFolders();
	}
}
