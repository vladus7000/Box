#pragma once

#include <DXUT11\Core\DXUT.h>
#include "DXUT11\Optional\DXUTgui.h"
#include "BaseUI.hpp"

namespace box
{
	class Dialog : public BaseUI
	{
	public:
		enum class Type
		{
			YesNo,
			YesNoCancel,
			ContinueBreakExit,
		};
		enum class Answer
		{
			Yes,
			No,
			Cancel,
			Continue,
			Break,
			Exit,
			NotReady
		};

		Dialog(Type type = Type::YesNo);
		virtual bool restore() override;
		virtual S32 render(F64 time, F32 delta) override;
		virtual void update(F32 delta) override;
		virtual AppMsg::Status msgProc(const AppMsg& msg) override;

		Answer getAnswer() const;
		void setAnswer(Answer answer) { m_answer = answer; }
		void resetAnswer();
	private:
		CDXUTDialog m_dialog;
		Type m_type;
		Answer m_answer;
	};
}
