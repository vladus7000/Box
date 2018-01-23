#pragma once

#include "System/ResourceSystem/ResourceExtraData.hpp"

#include <tinyxml2/tinyxml2.h>

namespace box
{
	class XMLResourceExtraData : public ResourceExtraData
	{
	public:
		~XMLResourceExtraData() {}

		virtual const std::string& getName() const { return "XMLResourceExtraData"; }
		tinyxml2::XMLNode* getXMLRoot() { return m_root; }
		tinyxml2::XMLDocument& getXMLDocument() { return m_xmlDoc; }

	private:
		friend class XMLResourceLoader;
		tinyxml2::XMLDocument m_xmlDoc;
		tinyxml2::XMLNode* m_root;
	};
}
