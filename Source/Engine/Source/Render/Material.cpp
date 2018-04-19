#include "StdAfx.hpp"
#include "Render/Material.hpp"
#include "System/ResourceSystem/ResourceManager.hpp"
#include "Render\ResourceLoaders\TextureResourceExtraData.hpp"

namespace box
{

	Material::~Material()
	{
		for (U32 i = 0; i < MaxTextures; i++)
		{
			SAVE_RELEASE(m_textures[i]);
		}
	}

	bool Material::apply(ID3D11DeviceContext* context)
	{
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
					parseTexureSlot(textureElement, 0, "slot1");
					parseTexureSlot(textureElement, 1, "slot2");
					parseTexureSlot(textureElement, 2, "slot3");
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
				auto extra = handle->getExtraTyped<box::TextureResourceExtraData>();
				if (extra)
				{
					m_textures[slot] = extra->getTextureData().shaderResourceView;
					m_textures[slot]->AddRef();
				}
			}
		}
	}

}
