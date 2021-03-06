#include "Editor.h"

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
	initTools();

	Exports::System::StartEngine(m_renderWindow->getHwnd(), 0, nullptr);

	m_recourcesWindow->refreshResourceCollection();
	m_runningEnvironmentWindow->updateEnvironmentSettings();
}

void winforms::MyForm::initTools()
{
	m_renderWindow = gcnew Editor::RenderWindow;
	m_renderWindow->Show(dockPanel1, WeifenLuo::WinFormsUI::Docking::DockState::Document);

	m_cheatWindow = gcnew Editor::CheatWindow;
	m_cheatWindow->Show(dockPanel1, WeifenLuo::WinFormsUI::Docking::DockState::Document);

	m_inputWindow = gcnew Editor::InputWindow;
	m_inputWindow->Show(dockPanel1, WeifenLuo::WinFormsUI::Docking::DockState::Document);

	m_networkWindow = gcnew Editor::NetworkWindow;
	m_networkWindow->Show(dockPanel1, WeifenLuo::WinFormsUI::Docking::DockState::Document);

	m_runningEnvironmentWindow = gcnew Editor::RunningEnvironmentWindow;
	m_runningEnvironmentWindow->Show(dockPanel1, WeifenLuo::WinFormsUI::Docking::DockState::Document);

	m_scriptingindow = gcnew Editor::ScriptingWindow;
	m_scriptingindow->Show(dockPanel1, WeifenLuo::WinFormsUI::Docking::DockState::Document);

	m_soundSystemWindow = gcnew Editor::SoundSystemWindow;
	m_soundSystemWindow->Show(dockPanel1, WeifenLuo::WinFormsUI::Docking::DockState::Document);

	m_properties = gcnew Editor::PropertiesWindow;
	m_properties->Show(dockPanel1, WeifenLuo::WinFormsUI::Docking::DockState::DockRight);

	m_sceneView = gcnew Editor::SceneView;
	m_sceneView->Show(dockPanel1, WeifenLuo::WinFormsUI::Docking::DockState::DockLeft);

	m_addObjectWindow = gcnew Editor::AddObject;
	m_addObjectWindow->Show(dockPanel1, WeifenLuo::WinFormsUI::Docking::DockState::DockLeft);

	m_recourcesWindow = gcnew Editor::RecourcesWindow(m_runningEnvironmentWindow, m_properties);
	m_recourcesWindow->Show(dockPanel1, WeifenLuo::WinFormsUI::Docking::DockState::DockLeft);


	m_globals = gcnew Editor::Globals;
	m_globals->m_recourcesWindow = m_recourcesWindow;
	m_globals->m_runningEnvironmentWindow = m_runningEnvironmentWindow;
	m_globals->m_propertiesWindow = m_properties;

	m_properties->setGlobals(m_globals);
	m_sceneView->setGlobals(m_globals);
}

void winforms::MyForm::deinit()
{
	Exports::System::StopEngine();
}
