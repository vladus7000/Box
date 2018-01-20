#pragma once

#include "AppMsg.hpp"

namespace box
{
	class ScreenElement
	{
	public:
		ScreenElement() = default;
		virtual ~ScreenElement() = default;

		virtual S32 restore() = 0;
		virtual S32 render(F64 time, F32 delta) = 0;

		virtual void update(F32 delta) = 0;

		virtual S32 getZOrder() const = 0;
		virtual void setZOrder(S32 zOrder) = 0;

		virtual bool isVisible() const = 0;
		virtual void setVisible(bool visible) = 0;

		virtual S32 msgProc(const AppMsg& msg) = 0;

		virtual bool operator < (const ScreenElement& other) const { return getZOrder() < other.getZOrder(); }
	};
}
