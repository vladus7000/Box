#pragma once

#include "Math/Plane.hpp"
#include "Math/Vector3D.hpp"

namespace box
{
	class Frustum
	{
	public:
		enum class Side
		{
			Near,
			Far,
			Left,
			Right,
			Top,
			Bottom,

			Count
		};
		enum class TestResult
		{
			inside,
			outside,
			intersect
		};
		enum IntersectionMask
		{
			None = 0,

			Near = 1,
			Far = 2,
			Left = 4,
			Right = 8,
			Top = 16,
			Bottom = 32,

			All = 63
		};
	public:
		Frustum() = default;
		Frustum(const Plane planes[6])
		{
			for (U32 i = 0; i < 6; ++i)
			{
				m_plane[i] = planes[i];
			}
		}

		void setPlane(const Plane& plane, Side side)
		{
			const U32 idx = side < Side::Count ? static_cast<U32>(side) : 0;
			m_plane[idx] = plane;
		}

		const Plane& getPlane(Side side) const
		{
			const U32 idx = side < Side::Count ? static_cast<U32>(side) : 0;
			return m_plane[idx];
		}

		/*
		* outMask is bitwise representing intersection with planes.
		* order is Bottom|Top|Right|Left|Far|Near.
		* ex. 0x01 - near, 0x02 - Far, 0x03 - Far|Near, etc.
		*/
		TestResult testSphere(const Vector3D& pos, F32 radius, U32& outMask) const
		{
			outMask = static_cast<U32>(IntersectionMask::None);
			return TestResult::inside;
		}

	private:
		Plane m_plane[6];
	};
}
