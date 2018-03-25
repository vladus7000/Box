#pragma once

namespace Exports
{
	namespace Resources
	{
		int ImportModel(const char* fileName);
		int GetResourceCollectionSizeForXml();
		int SerializeResourceCollectionToXml(char* out);
	}
}
