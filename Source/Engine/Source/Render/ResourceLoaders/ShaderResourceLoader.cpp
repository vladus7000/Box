#include "StdAfx.hpp"
#include <Render/ResourceLoaders/ShaderResourceLoader.hpp>
#include <Render/ResourceLoaders/ShaderResourceExtraData.hpp>
#include <System/ResourceSystem/ResourceHandle.hpp>
#include <System/ResourceSystem/ResourceManager.hpp>

#include <DXUT11\Core\DXUT.h>
#include <d3dx11tex.h>
#include <tinyxml2/tinyxml2.h>

namespace box
{
	bool ShaderResourceLoader::loadResource(U8* buffer, size_t size, std::shared_ptr<ResourceHandle> handle)
	{
		std::shared_ptr<ShaderResourceExtraData> extra(new ShaderResourceExtraData());

		tinyxml2::XMLDocument xmlDoc;

		tinyxml2::XMLError result = xmlDoc.Parse(reinterpret_cast<const char*>(buffer), size);
		if (result == tinyxml2::XMLError::XML_SUCCESS)
		{
			tinyxml2::XMLNode* m_root = xmlDoc.RootElement();
			handle->setExtra(extra);

			return true;
		}
		// TODO: implement me
		//{
		//	if (auto root = xmlData->getXMLRoot())
		//	{
		//		for (tinyxml2::XMLNode* child = root->FirstChildElement(); child; child = child->NextSibling())
		//		{
		//			if (auto element = child->ToElement())
		//			{
		//				const char* name = element->Name();
		//				const char* val = element->GetText();
		//				if (name && val)
		//				{
		//					fillSamplerElement(sampDesc, name, val);
		//				}
		//			}
		//		}
		//	}
		//}

		handle->setExtra(extra);
		return true;
	}
}