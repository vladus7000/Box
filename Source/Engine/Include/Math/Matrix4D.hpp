#pragma once

#include <D3D11.h>
#include <d3dx9math.h>


namespace box
{
	class Matrix4D : public D3DXMATRIX
	{
	public:
		void serializeToXMLElement(tinyxml2::XMLElement* element) const
		{
			element->SetAttribute("E11", _11);
			element->SetAttribute("E12", _12);
			element->SetAttribute("E13", _13);
			element->SetAttribute("E14", _14);

			element->SetAttribute("E21", _21);
			element->SetAttribute("E22", _22);
			element->SetAttribute("E23", _23);
			element->SetAttribute("E24", _24);

			element->SetAttribute("E31", _31);
			element->SetAttribute("E32", _32);
			element->SetAttribute("E33", _33);
			element->SetAttribute("E34", _34);

			element->SetAttribute("E41", _41);
			element->SetAttribute("E42", _42);
			element->SetAttribute("E43", _43);
			element->SetAttribute("E44", _44);
		}

		bool loadFromXMLElement(tinyxml2::XMLElement* element)
		{
			element->QueryFloatAttribute("E11", &_11);
			element->QueryFloatAttribute("E12", &_12);
			element->QueryFloatAttribute("E13", &_13);
			element->QueryFloatAttribute("E14", &_14);

			element->QueryFloatAttribute("E21", &_21);
			element->QueryFloatAttribute("E22", &_22);
			element->QueryFloatAttribute("E23", &_23);
			element->QueryFloatAttribute("E24", &_24);

			element->QueryFloatAttribute("E31", &_31);
			element->QueryFloatAttribute("E32", &_32);
			element->QueryFloatAttribute("E33", &_33);
			element->QueryFloatAttribute("E34", &_34);

			element->QueryFloatAttribute("E41", &_41);
			element->QueryFloatAttribute("E42", &_42);
			element->QueryFloatAttribute("E43", &_43);
			element->QueryFloatAttribute("E44", &_44);
			return true;
		}
	};
}