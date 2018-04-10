#include "StdAfx.hpp"
#include "Render/Material.hpp"
#include "System/ResourceSystem/ResourceManager.hpp"

namespace box
{
	bool Material::apply(ID3D11DeviceContext* context)
	{
		m_shader->updateEnvironment();
		m_shader->setActiveTechnique(0);
		Technique& technique =  m_shader->getActiveTechnique();
		return technique.apply(context);
	}

	tinyxml2::XMLNode* Material::serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const
	{
		return nullptr;
	}

	bool Material::loadFromXML(tinyxml2::XMLNode* node)
	{
		if (auto element = node->ToElement())
		{
			if (strcmp(element->Name(), "Material") == 0)
			{
				const char* name = element->Attribute("name");
				m_name = name;
				if (auto shaderElement = element->FirstChildElement("Shader"))
				{
					if (const char* desc = shaderElement->Attribute("desc"))
					{
						Resource r(desc);
						auto handle = ResourceManager::Instance().getHandle(r);
						auto shader = handle->getExtraTyped<Shader>();
						if (shader)
						{
							m_shader = shader;
							return true;
						}
					}

				}
			}
		}
		return false;
	}
}
