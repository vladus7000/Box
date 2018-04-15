#pragma once

namespace box
{
	namespace VFs
	{
		struct PosTcoordNorm
		{
			float pos[3];
			float tcoord[2];
			float norm[3];
		};

		struct Pos
		{
			float pos[3];
		};

		enum class Format
		{
			PosTCoordNorm,
			Pos
		};

	}
}