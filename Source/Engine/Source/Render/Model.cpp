#include "StdAfx.hpp"
#include "Render/Model.hpp"

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
		return false;
	}

}
