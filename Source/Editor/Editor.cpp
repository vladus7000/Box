#include "Editor.h"
#include "Window\Window.hpp"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main(array<String^>^ args)
{
	(void)args;
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	winforms::MyForm form;

	Application::Run(%form);
}

void winforms::MyForm::init()
{
	m_renderWindow = gcnew Editor::RenderWindow;
	m_renderWindow->Show(dockPanel1, WeifenLuo::WinFormsUI::Docking::DockState::Document);
	
	m_cheatWindow = gcnew Editor::CheatWindow;
	m_cheatWindow->Show(dockPanel1, WeifenLuo::WinFormsUI::Docking::DockState::Document);

	m_inputWindow = gcnew Editor::InputWindow;
	m_inputWindow->Show(dockPanel1, WeifenLuo::WinFormsUI::Docking::DockState::Document);

	m_networkWindow = gcnew Editor::NetworkWindow;
	m_networkWindow->Show(dockPanel1, WeifenLuo::WinFormsUI::Docking::DockState::Document);

	m_recourcesWindow = gcnew Editor::RecourcesWindow;
	m_recourcesWindow->Show(dockPanel1, WeifenLuo::WinFormsUI::Docking::DockState::Document);

	m_runningEnvironmentWindow = gcnew Editor::RunningEnvironmentWindow;
	m_runningEnvironmentWindow->Show(dockPanel1, WeifenLuo::WinFormsUI::Docking::DockState::Document);

	m_scriptingindow = gcnew Editor::ScriptingWindow;
	m_scriptingindow->Show(dockPanel1, WeifenLuo::WinFormsUI::Docking::DockState::Document);

	m_soundSystemWindow = gcnew Editor::SoundSystemWindow;
	m_soundSystemWindow->Show(dockPanel1, WeifenLuo::WinFormsUI::Docking::DockState::Document);

	if (!m_engine)
	{
		m_engine = new box::Engine();

		m_engine->startup(m_renderWindow->getHwnd());
	}
}

void winforms::MyForm::deinit()
{
	if (m_engine)
	{
		m_engine->shutdown();
		delete m_engine;
		m_engine = nullptr;
	}
}
