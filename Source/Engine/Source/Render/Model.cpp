#include "StdAfx.hpp"
#include "Render/Model.hpp"
#include "System/ResourceSystem/ResourceManager.hpp"

namespace box
{

	Model::Model(const std::string& name, const std::string& sourceFile)
		: m_name(name)
		, m_sourceFile(sourceFile)
	{
	}

	int Model::getSizeForXML() const
	{
		int ret = 0;

		for (const auto& mesh : m_meshes)
		{
			ret += mesh->getSizeForXML();
		}

		return ret;
	}

	tinyxml2::XMLNode* Model::serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const
	{
		tinyxml2::XMLElement* modelElement = doc.NewElement("Model");
		modelElement->SetAttribute("name", m_name.c_str());
		modelElement->SetAttribute("sourceFile", m_sourceFile.c_str());

		for (const auto& mesh : m_meshes)
		{
			mesh->serializeToXML(modelElement, doc);
		}

		if (node)
		{
			node->InsertEndChild(modelElement);
		}

		return modelElement;
	}

	bool Model::loadFromXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc)
	{
		if (auto element = node->ToElement())
		{
			if (strcmp(element->Name(), "Model") == 0)
			{
				if (const char* name = element->Attribute("name"))
				{
					m_name = name;
				}
				
				const char* sourceFile = element->Attribute("sourceFile");

				if (sourceFile)
				{
					m_sourceFile = sourceFile;

					std::string filePath("../Assets/Models/");
					filePath += sourceFile;
					ResourceManager::Instance().loadModel(filePath, *this);

					tinyxml2::XMLElement* meshElement = element->FirstChildElement("Mesh");
					for (const auto& mesh : m_meshes)
					{
						mesh->loadFromXML(meshElement, doc);
						meshElement = meshElement->NextSiblingElement("Mesh");
					}

					return true;
				}
			}
		}
		return false;
	}

}
