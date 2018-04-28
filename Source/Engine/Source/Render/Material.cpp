#include "StdAfx.hpp"
#include "Render/Material.hpp"
#include "System/ResourceSystem/ResourceManager.hpp"
#include "Render\ResourceLoaders\TextureResourceExtraData.hpp"

namespace box
{

	Material::~Material()
	{
	}

	bool Material::apply(ID3D11DeviceContext* context)
	{
		m_shader->setActiveTechnique(0);
		Technique& technique =  m_shader->getActiveTechnique();
		return technique.apply(context);
	}

	tinyxml2::XMLNode* Material::serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const
	{
		auto rootElement = doc.NewElement("Material");
		rootElement->SetAttribute("name", m_name.c_str());

		auto shaderElement = doc.NewElement("Shader");
		shaderElement->SetAttribute("desc", m_shaderName.c_str());
		rootElement->InsertEndChild(shaderElement);

		auto texturesElement = doc.NewElement("Textures");

		for (U32 i = 0; i < MaxTextures; i++)
		{
			char buf[50];
			snprintf(buf, 50, "slot%d\0", i + 1);
			auto element = doc.NewElement(buf);
			element->SetAttribute("desc", m_texturesNames[i].c_str());
			texturesElement->InsertEndChild(element);
		}

		rootElement->InsertEndChild(texturesElement);

		if (node)
		{
			node->InsertEndChild(rootElement);
		}

		return rootElement;
	}

	bool Material::loadFromXML(tinyxml2::XMLNode* node)
	{
		bool ok = false;
		if (auto rootElement = node->ToElement())
		{
			if (strcmp(rootElement->Name(), "Material") == 0)
			{
				const char* name = rootElement->Attribute("name");
				m_name = name;
				if (auto shaderElement = rootElement->FirstChildElement("Shader"))
				{
					if (const char* desc = shaderElement->Attribute("desc"))
					{
						m_shaderName = desc;
						Resource r(desc);
						auto handle = ResourceManager::Instance().getHandle(r);
						auto shader = handle->getExtraTyped<Shader>();
						if (shader)
						{
							m_shader = shader;
							ok = true;
						}
					}
				}

				if (auto textureElement = rootElement->FirstChildElement("Textures"))
				{
					for (U32 i = 0; i < MaxTextures; i++)
					{
						char buf[50];
						snprintf(buf, 50, "slot%d\0", i+1);
						parseTexureSlot(textureElement, i, buf);
					}
				}
			}
		}
		return ok;
	}

	void Material::parseTexureSlot(tinyxml2::XMLElement* root, U32 slot, const char* name)
	{
		if (auto slotElement = root->FirstChildElement(name))
		{
			if (const char* desc = slotElement->Attribute("desc"))
			{
				Resource r(desc);
				auto handle = ResourceManager::Instance().getHandle(r);
				auto extra = handle->getExtraTyped<box::Texture>();
				if (extra)
				{
					m_textures[slot] = extra;
				}
			}
		}
	}

}
