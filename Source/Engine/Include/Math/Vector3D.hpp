#pragma once

#include <D3D11.h>
#include <d3dx9math.h>
#include <tinyxml2/tinyxml2.h>

namespace box
{
	class Vector3D : public D3DXVECTOR3
	{
	public:
		Vector3D()
			: D3DXVECTOR3(0.0f, 0.0f, 0.0f)
		{}
		Vector3D(F32 x, F32 y, F32 z)
			: D3DXVECTOR3(x, y, z)
		{}
		Vector3D(const D3DXVECTOR3& xmvector)
			: D3DXVECTOR3(xmvector)
		{}

		inline void set(F32 _x, F32 _y, F32 _z)
		{
			x = _x;
			y = _y;
			z = _z;
		}

		inline void set(const D3DXVECTOR3& xmvector)
		{
			x = xmvector.x;
			y = xmvector.y;
			z = xmvector.z;
		}

		inline F32 dot(const Vector3D& other) const
		{
			return dot(*this, other);
		}

		inline Vector3D cross(const Vector3D& other) const
		{
			return cross(*this, other);
		}

		inline void normalize()
		{
			D3DXVec3Normalize(this, this);
		}

		inline const Vector3D& getNormalized() const
		{
			Vector3D ret;
			D3DXVec3Normalize(&ret, this);
			return ret;
		}

		inline F32 getLenght() const
		{
			return D3DXVec3Length(this);
		}

		inline F32 getLenghtSQ() const
		{
			return D3DXVec3LengthSq(this);
		}

		static inline F32 dot(const Vector3D& v1, const Vector3D& v2)
		{
			return D3DXVec3Dot(&v1, &v2);
		}

		static inline Vector3D cross(const Vector3D& v1, const Vector3D& v2)
		{
			Vector3D ret;
			D3DXVec3Cross(&ret, &v1, &v2);
			return ret;
		}

		void serializeToXMLElement(tinyxml2::XMLElement* element) const
		{
			element->SetAttribute("X", x);
			element->SetAttribute("Y", y);
			element->SetAttribute("Z", z);
		}

		bool loadFromXMLElement(tinyxml2::XMLElement* element)
		{
			element->QueryFloatAttribute("X", &x);
			element->QueryFloatAttribute("Y", &y);
			element->QueryFloatAttribute("Z", &z);
			return true;
		}
	};
}
