#pragma once

#include <D3D11.h>
#include <d3dx9math.h>

namespace box
{
	class Plane : public D3DXPLANE
	{
	public:
		Plane() = default;
		Plane(F32 a, F32 b, F32 c, F32 d)
			: D3DXPLANE(a, b, c, d)
		{}
	};
}