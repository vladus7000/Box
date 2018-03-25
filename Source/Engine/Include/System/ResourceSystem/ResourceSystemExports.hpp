#pragma once

namespace Exports
{
	namespace Resources
	{
		int ImportModel(const char* fileName);
		int GetResourceCollectionSizeForXml();
		int SerializeResourceCollectionToXml(char* out);

		int LoadLevelFromXMLFile(const char* fileName);
		int LoadLevelFromXMLBuffer(const char* buffer);

		int GetLevelSizeForXml();
		int SaveLevelToXMLFile(const char* fileName);
		int SerializeLevelToXML(char* out);

		int GetLevelInfoSizeForXml();
		int SerializeLevelInfoToXml(char* out);

	}
}
