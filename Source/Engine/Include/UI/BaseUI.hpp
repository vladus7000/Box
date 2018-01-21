#pragma once

#include <System\ScreenElement.hpp>
#include <System/AppMsg.hpp>

namespace box
{
	class BaseUI : public ScreenElement
	{
	public:
		BaseUI() : m_zOrder(0), m_visible(true) {}
		virtual ~BaseUI() = default;

		virtual bool restore() = 0;
		virtual S32 render(F64 time, F32 delta) = 0;

		virtual void update(F32 delta) = 0;
		virtual AppMsg::Status msgProc(const AppMsg& msg) = 0;

		virtual S32 getZOrder() const override { return m_zOrder; }
		virtual void setZOrder(S32 zOrder) override { m_zOrder = zOrder; }

		virtual bool isVisible() const override { return m_visible; }
		virtual void setVisible(bool visible) override { m_visible = visible; }
	private:
		S32 m_zOrder;
		bool m_visible;
	};
}
