#include "StdAfx.hpp"
#include <windows.h>
#include "Render\DXUTHelper.hpp"
#include "UI/Dialog.hpp"

namespace box
{
	void CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
	{
		if (pUserContext)
		{
			Dialog* This =  reinterpret_cast<Dialog*>(pUserContext);
			
			Dialog::Answer answer = (Dialog::Answer)nControlID;
			switch (answer)
			{
			case Dialog::Answer::Yes:
			case Dialog::Answer::No:
				This->setAnswer(answer);
				break;
			default:
				This->setAnswer(Dialog::Answer::NotReady);
				break;
			}
		}
	}

	Dialog::Dialog(Type type)
		: m_type(type)
		, m_answer(Answer::NotReady)
	{}

	bool Dialog::restore()
	{
		auto& dialogResourceManager = DXUT::GetDialogResourceManager();
		m_dialog.Init(&dialogResourceManager);
		m_dialog.SetCallback(OnGUIEvent, this); int iY = 100;
		m_dialog.AddButton((int)Answer::Yes, L"Yes", 50, iY, 170, 23);
		m_dialog.AddButton((int)Answer::No, L"No", 50 + 170 + 5, iY, 170, 23);
		m_dialog.SetLocation(20, 0);
		m_dialog.SetSize(170, 170);
		m_answer = Answer::NotReady;
		return true;
	}

	S32 Dialog::render(F64 time, F32 delta)
	{
		if (isVisible())
		{
			m_dialog.OnRender(delta);
		}
		return 0;
	}

	void Dialog::update(F32 delta)
	{

	}

	AppMsg::Status Dialog::msgProc(const AppMsg& msg)
	{
		if (isVisible())
		{
			if (m_dialog.MsgProc(msg.hwnd, msg.uMsg, msg.wParam, msg.lParam))
			{
				return AppMsg::Status::Processed;
			}
		}
		return AppMsg::Status::DefaultAction;
	}

	Dialog::Answer Dialog::getAnswer() const
	{
		return m_answer;
	}

	void Dialog::resetAnswer()
	{
		m_answer = Answer::NotReady;
	}
}
