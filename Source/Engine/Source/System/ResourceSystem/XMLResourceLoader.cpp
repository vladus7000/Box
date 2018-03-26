#include "StdAfx.hpp"
#include "System/ResourceSystem/XMLResourceLoader.hpp"
#include "System/ResourceSystem/XMLResourceExtraData.hpp"
#include <System/ResourceSystem/ResourceHandle.hpp>

namespace box
{
	bool XMLResourceLoader::loadResource(U8 * buffer, size_t size, std::shared_ptr<ResourceHandle> handle)
	{
		std::shared_ptr<XMLResourceExtraData> extra = std::make_shared<XMLResourceExtraData>();

		tinyxml2::XMLError result = extra->m_xmlDoc.Parse(reinterpret_cast<const char*>(buffer), size);
		if (result == tinyxml2::XMLError::XML_SUCCESS)
		{
			extra->m_root = extra->m_xmlDoc.RootElement();
			handle->setExtra(extra);

			return true;
		}
		return false;
	}
}
