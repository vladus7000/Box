#pragma once

#include "Math/Vector3D.hpp"
#include <tinyxml2/tinyxml2.h>

namespace box
{
	struct EnvironmentSettings
	{
		void updateEnvironmentSettings(tinyxml2::XMLNode* node)
		{
			loadFromXML(node);
		}

		int getSizeForXML() const
		{
			return 0;
		}

		tinyxml2::XMLNode* serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const
		{
			tinyxml2::XMLElement* root = doc.NewElement("EnvironmentSettings");
			tinyxml2::XMLElement* sun = doc.NewElement("Sun");

			{
				tinyxml2::XMLElement* position = doc.NewElement("Position");
				position->SetAttribute("X", sunPosition.x);
				position->SetAttribute("Y", sunPosition.y);
				position->SetAttribute("Z", sunPosition.z);
				sun->InsertEndChild(position);
			}

			{
				tinyxml2::XMLElement* color = doc.NewElement("Color");
				color->SetAttribute("R", sunColor.x);
				color->SetAttribute("G", sunColor.y);
				color->SetAttribute("B", sunColor.z);
				sun->InsertEndChild(color);
			}

			root->InsertEndChild(sun);

			if (node)
			{
				node->InsertEndChild(root);
			}

			return root;
		}

		bool loadFromXML(tinyxml2::XMLNode* node)
		{
			if (auto element = node->ToElement())
			{
				if (strcmp(element->Name(), "EnvironmentSettings") == 0)
				{
					if (auto sunSettings = element->FirstChildElement("Sun"))
					{
						if (auto position = sunSettings->FirstChildElement("Position"))
						{
							position->QueryFloatAttribute("X", &sunPosition.x);
							position->QueryFloatAttribute("Y", &sunPosition.y);
							position->QueryFloatAttribute("Z", &sunPosition.z);
						}
						if (auto color = sunSettings->FirstChildElement("Color"))
						{
							color->QueryFloatAttribute("R", &sunColor.x);
							color->QueryFloatAttribute("G", &sunColor.y);
							color->QueryFloatAttribute("B", &sunColor.z);
						}
						return true;
					}

				}
			}

			return false;
		}

		Vector3D sunPosition;
		Vector3D sunColor;
	};
}