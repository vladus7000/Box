#include "StdAfx.hpp"
#include "Render/ResourceLoaders/ModelResourceLoader.hpp"
#include <tinyxml2/tinyxml2.h>
#include "Render/Model.hpp"

namespace box
{

bool ModelResourceLoader::loadResource(U8* buffer, size_t size, std::shared_ptr<ResourceHandle> handle)
{
	std::shared_ptr<Model> extra = std::make_shared<Model>("","");

	tinyxml2::XMLDocument xmlDoc;

	tinyxml2::XMLError result = xmlDoc.Parse(reinterpret_cast<const char*>(buffer), size);
	if (result == tinyxml2::XMLError::XML_SUCCESS)
	{
		tinyxml2::XMLNode* root = xmlDoc.RootElement();

		if (extra->loadFromXML(root))
		{
			handle->setExtra(extra);
		}

		return true;
	}

	return false;
}

}