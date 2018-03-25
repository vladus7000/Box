#pragma once

#include <EngineExports/EngineExports.hpp>

#include "Windows\RenderWindow.h"
#include "Windows\CheatWindow.h"
#include "Windows\InputWindow.h"
#include "Windows\NetworkWindow.h"
#include "Windows\RecourcesWindow.h"
#include "Windows\RunningEnvironmentWindow.h"
#include "Windows\ScriptingWindow.h"
#include "Windows\SoundSystemWindow.h"
#include "Windows\SceneView.h"
#include "Windows\Properties.h"
#include "Windows\AddObject.h"

namespace winforms {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			Application::Idle += gcnew EventHandler(this,&MyForm::onIdle);
			init();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
			deinit();
		}

	private:
		void init();
		void initTools();
		void deinit();

		Editor::RenderWindow^ m_renderWindow;
		Editor::CheatWindow^ m_cheatWindow;
		Editor::InputWindow^ m_inputWindow;
		Editor::NetworkWindow^ m_networkWindow;
		Editor::RecourcesWindow^ m_recourcesWindow;
		Editor::RunningEnvironmentWindow^ m_runningEnvironmentWindow;
		Editor::ScriptingWindow^ m_scriptingindow;
		Editor::SoundSystemWindow^ m_soundSystemWindow;
		Editor::SceneView^ m_sceneView;
		Editor::PropertiesWindow^ m_properties;
		Editor::AddObject^ m_addObjectWindow;

	private: WeifenLuo::WinFormsUI::Docking::DockPanel^  dockPanel1;
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^  statusBar;
	private: System::Windows::Forms::ToolStripProgressBar^  progressBar;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  buildToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  releaseBuildToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  debugToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  runGameToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  profileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  settingsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  toolsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  renderWindowToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  scriptingToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  cheatToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showRenderWindowToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showScriptingWindowToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showCheatWindowToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  inputToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showInputWindowToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  networkToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showNetworkWindowToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  resourcesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showResourcesWindowToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  runInvironmentToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showWindowToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  soundToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showSoundSystemWindowToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  sceneViewToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showSceneViewWindowToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  propertiesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showPropertiesWindowToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  createObjectToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showCreateObjectWindowToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  importModelToolStripMenuItem;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::ToolStripMenuItem^  loadLevelToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveLevelToolStripMenuItem;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;


	private: System::ComponentModel::IContainer^  components;

		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			WeifenLuo::WinFormsUI::Docking::DockPanelSkin^  dockPanelSkin1 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPanelSkin());
			WeifenLuo::WinFormsUI::Docking::AutoHideStripSkin^  autoHideStripSkin1 = (gcnew WeifenLuo::WinFormsUI::Docking::AutoHideStripSkin());
			WeifenLuo::WinFormsUI::Docking::DockPanelGradient^  dockPanelGradient1 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPanelGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient1 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			WeifenLuo::WinFormsUI::Docking::DockPaneStripSkin^  dockPaneStripSkin1 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPaneStripSkin());
			WeifenLuo::WinFormsUI::Docking::DockPaneStripGradient^  dockPaneStripGradient1 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPaneStripGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient2 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			WeifenLuo::WinFormsUI::Docking::DockPanelGradient^  dockPanelGradient2 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPanelGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient3 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			WeifenLuo::WinFormsUI::Docking::DockPaneStripToolWindowGradient^  dockPaneStripToolWindowGradient1 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPaneStripToolWindowGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient4 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient5 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			WeifenLuo::WinFormsUI::Docking::DockPanelGradient^  dockPanelGradient3 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPanelGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient6 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient7 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			this->dockPanel1 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPanel());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->statusBar = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->progressBar = (gcnew System::Windows::Forms::ToolStripProgressBar());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->importModelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadLevelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveLevelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->buildToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->releaseBuildToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->debugToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->profileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->runGameToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->settingsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->renderWindowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showRenderWindowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->scriptingToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showScriptingWindowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cheatToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showCheatWindowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->inputToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showInputWindowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->networkToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showNetworkWindowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->resourcesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showResourcesWindowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->runInvironmentToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showWindowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->soundToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showSoundSystemWindowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->sceneViewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showSceneViewWindowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->propertiesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showPropertiesWindowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->createObjectToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showCreateObjectWindowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->statusStrip1->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// dockPanel1
			// 
			this->dockPanel1->BackColor = System::Drawing::SystemColors::AppWorkspace;
			this->dockPanel1->DockBackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->dockPanel1->Location = System::Drawing::Point(0, 27);
			this->dockPanel1->Name = L"dockPanel1";
			this->dockPanel1->Size = System::Drawing::Size(1451, 635);
			dockPanelGradient1->EndColor = System::Drawing::SystemColors::ControlLight;
			dockPanelGradient1->StartColor = System::Drawing::SystemColors::ControlLight;
			autoHideStripSkin1->DockStripGradient = dockPanelGradient1;
			tabGradient1->EndColor = System::Drawing::SystemColors::Control;
			tabGradient1->StartColor = System::Drawing::SystemColors::Control;
			tabGradient1->TextColor = System::Drawing::SystemColors::ControlDarkDark;
			autoHideStripSkin1->TabGradient = tabGradient1;
			autoHideStripSkin1->TextFont = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			dockPanelSkin1->AutoHideStripSkin = autoHideStripSkin1;
			tabGradient2->EndColor = System::Drawing::SystemColors::ControlLightLight;
			tabGradient2->StartColor = System::Drawing::SystemColors::ControlLightLight;
			tabGradient2->TextColor = System::Drawing::SystemColors::ControlText;
			dockPaneStripGradient1->ActiveTabGradient = tabGradient2;
			dockPanelGradient2->EndColor = System::Drawing::SystemColors::Control;
			dockPanelGradient2->StartColor = System::Drawing::SystemColors::Control;
			dockPaneStripGradient1->DockStripGradient = dockPanelGradient2;
			tabGradient3->EndColor = System::Drawing::SystemColors::ControlLight;
			tabGradient3->StartColor = System::Drawing::SystemColors::ControlLight;
			tabGradient3->TextColor = System::Drawing::SystemColors::ControlText;
			dockPaneStripGradient1->InactiveTabGradient = tabGradient3;
			dockPaneStripSkin1->DocumentGradient = dockPaneStripGradient1;
			dockPaneStripSkin1->TextFont = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			tabGradient4->EndColor = System::Drawing::SystemColors::ActiveCaption;
			tabGradient4->LinearGradientMode = System::Drawing::Drawing2D::LinearGradientMode::Vertical;
			tabGradient4->StartColor = System::Drawing::SystemColors::GradientActiveCaption;
			tabGradient4->TextColor = System::Drawing::SystemColors::ActiveCaptionText;
			dockPaneStripToolWindowGradient1->ActiveCaptionGradient = tabGradient4;
			tabGradient5->EndColor = System::Drawing::SystemColors::Control;
			tabGradient5->StartColor = System::Drawing::SystemColors::Control;
			tabGradient5->TextColor = System::Drawing::SystemColors::ControlText;
			dockPaneStripToolWindowGradient1->ActiveTabGradient = tabGradient5;
			dockPanelGradient3->EndColor = System::Drawing::SystemColors::ControlLight;
			dockPanelGradient3->StartColor = System::Drawing::SystemColors::ControlLight;
			dockPaneStripToolWindowGradient1->DockStripGradient = dockPanelGradient3;
			tabGradient6->EndColor = System::Drawing::SystemColors::InactiveCaption;
			tabGradient6->LinearGradientMode = System::Drawing::Drawing2D::LinearGradientMode::Vertical;
			tabGradient6->StartColor = System::Drawing::SystemColors::GradientInactiveCaption;
			tabGradient6->TextColor = System::Drawing::SystemColors::InactiveCaptionText;
			dockPaneStripToolWindowGradient1->InactiveCaptionGradient = tabGradient6;
			tabGradient7->EndColor = System::Drawing::Color::Transparent;
			tabGradient7->StartColor = System::Drawing::Color::Transparent;
			tabGradient7->TextColor = System::Drawing::SystemColors::ControlDarkDark;
			dockPaneStripToolWindowGradient1->InactiveTabGradient = tabGradient7;
			dockPaneStripSkin1->ToolWindowGradient = dockPaneStripToolWindowGradient1;
			dockPanelSkin1->DockPaneStripSkin = dockPaneStripSkin1;
			this->dockPanel1->Skin = dockPanelSkin1;
			this->dockPanel1->TabIndex = 3;
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) { this->statusBar, this->progressBar });
			this->statusStrip1->Location = System::Drawing::Point(0, 665);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(1451, 22);
			this->statusStrip1->TabIndex = 6;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// statusBar
			// 
			this->statusBar->Name = L"statusBar";
			this->statusBar->Size = System::Drawing::Size(59, 17);
			this->statusBar->Text = L"statusText";
			// 
			// progressBar
			// 
			this->progressBar->Name = L"progressBar";
			this->progressBar->Size = System::Drawing::Size(100, 16);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->fileToolStripMenuItem,
					this->buildToolStripMenuItem, this->runGameToolStripMenuItem, this->settingsToolStripMenuItem, this->toolsToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1451, 24);
			this->menuStrip1->TabIndex = 7;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->importModelToolStripMenuItem,
					this->loadLevelToolStripMenuItem, this->saveLevelToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// importModelToolStripMenuItem
			// 
			this->importModelToolStripMenuItem->Name = L"importModelToolStripMenuItem";
			this->importModelToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->importModelToolStripMenuItem->Text = L"ImportModel";
			this->importModelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::importModelToolStripMenuItem_Click);
			// 
			// loadLevelToolStripMenuItem
			// 
			this->loadLevelToolStripMenuItem->Name = L"loadLevelToolStripMenuItem";
			this->loadLevelToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->loadLevelToolStripMenuItem->Text = L"loadLevel";
			this->loadLevelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::loadLevelToolStripMenuItem_Click);
			// 
			// saveLevelToolStripMenuItem
			// 
			this->saveLevelToolStripMenuItem->Name = L"saveLevelToolStripMenuItem";
			this->saveLevelToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->saveLevelToolStripMenuItem->Text = L"saveLevel";
			this->saveLevelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::saveLevelToolStripMenuItem_Click);
			// 
			// buildToolStripMenuItem
			// 
			this->buildToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->releaseBuildToolStripMenuItem,
					this->debugToolStripMenuItem, this->profileToolStripMenuItem
			});
			this->buildToolStripMenuItem->Name = L"buildToolStripMenuItem";
			this->buildToolStripMenuItem->Size = System::Drawing::Size(77, 20);
			this->buildToolStripMenuItem->Text = L"BuildGame";
			this->buildToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::buildToolStripMenuItem_Click);
			// 
			// releaseBuildToolStripMenuItem
			// 
			this->releaseBuildToolStripMenuItem->Name = L"releaseBuildToolStripMenuItem";
			this->releaseBuildToolStripMenuItem->Size = System::Drawing::Size(113, 22);
			this->releaseBuildToolStripMenuItem->Text = L"Release";
			// 
			// debugToolStripMenuItem
			// 
			this->debugToolStripMenuItem->Name = L"debugToolStripMenuItem";
			this->debugToolStripMenuItem->Size = System::Drawing::Size(113, 22);
			this->debugToolStripMenuItem->Text = L"Debug";
			// 
			// profileToolStripMenuItem
			// 
			this->profileToolStripMenuItem->Name = L"profileToolStripMenuItem";
			this->profileToolStripMenuItem->Size = System::Drawing::Size(113, 22);
			this->profileToolStripMenuItem->Text = L"Profile";
			// 
			// runGameToolStripMenuItem
			// 
			this->runGameToolStripMenuItem->Name = L"runGameToolStripMenuItem";
			this->runGameToolStripMenuItem->Size = System::Drawing::Size(71, 20);
			this->runGameToolStripMenuItem->Text = L"RunGame";
			// 
			// settingsToolStripMenuItem
			// 
			this->settingsToolStripMenuItem->Name = L"settingsToolStripMenuItem";
			this->settingsToolStripMenuItem->Size = System::Drawing::Size(61, 20);
			this->settingsToolStripMenuItem->Text = L"Settings";
			// 
			// toolsToolStripMenuItem
			// 
			this->toolsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(11) {
				this->renderWindowToolStripMenuItem,
					this->scriptingToolStripMenuItem, this->cheatToolStripMenuItem, this->inputToolStripMenuItem, this->networkToolStripMenuItem,
					this->resourcesToolStripMenuItem, this->runInvironmentToolStripMenuItem, this->soundToolStripMenuItem, this->sceneViewToolStripMenuItem,
					this->propertiesToolStripMenuItem, this->createObjectToolStripMenuItem
			});
			this->toolsToolStripMenuItem->Name = L"toolsToolStripMenuItem";
			this->toolsToolStripMenuItem->Size = System::Drawing::Size(47, 20);
			this->toolsToolStripMenuItem->Text = L"Tools";
			// 
			// renderWindowToolStripMenuItem
			// 
			this->renderWindowToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->showRenderWindowToolStripMenuItem });
			this->renderWindowToolStripMenuItem->Name = L"renderWindowToolStripMenuItem";
			this->renderWindowToolStripMenuItem->Size = System::Drawing::Size(160, 22);
			this->renderWindowToolStripMenuItem->Text = L"Render";
			// 
			// showRenderWindowToolStripMenuItem
			// 
			this->showRenderWindowToolStripMenuItem->Name = L"showRenderWindowToolStripMenuItem";
			this->showRenderWindowToolStripMenuItem->Size = System::Drawing::Size(184, 22);
			this->showRenderWindowToolStripMenuItem->Text = L"ShowRenderWindow";
			this->showRenderWindowToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::showRenderWindowToolStripMenuItem_Click);
			// 
			// scriptingToolStripMenuItem
			// 
			this->scriptingToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->showScriptingWindowToolStripMenuItem });
			this->scriptingToolStripMenuItem->Name = L"scriptingToolStripMenuItem";
			this->scriptingToolStripMenuItem->Size = System::Drawing::Size(160, 22);
			this->scriptingToolStripMenuItem->Text = L"Scripting";
			// 
			// showScriptingWindowToolStripMenuItem
			// 
			this->showScriptingWindowToolStripMenuItem->Name = L"showScriptingWindowToolStripMenuItem";
			this->showScriptingWindowToolStripMenuItem->Size = System::Drawing::Size(194, 22);
			this->showScriptingWindowToolStripMenuItem->Text = L"ShowScriptingWindow";
			this->showScriptingWindowToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::showScriptingWindowToolStripMenuItem_Click);
			// 
			// cheatToolStripMenuItem
			// 
			this->cheatToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->showCheatWindowToolStripMenuItem });
			this->cheatToolStripMenuItem->Name = L"cheatToolStripMenuItem";
			this->cheatToolStripMenuItem->Size = System::Drawing::Size(160, 22);
			this->cheatToolStripMenuItem->Text = L"Cheat";
			// 
			// showCheatWindowToolStripMenuItem
			// 
			this->showCheatWindowToolStripMenuItem->Name = L"showCheatWindowToolStripMenuItem";
			this->showCheatWindowToolStripMenuItem->Size = System::Drawing::Size(178, 22);
			this->showCheatWindowToolStripMenuItem->Text = L"ShowCheatWindow";
			this->showCheatWindowToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::showCheatWindowToolStripMenuItem_Click);
			// 
			// inputToolStripMenuItem
			// 
			this->inputToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->showInputWindowToolStripMenuItem });
			this->inputToolStripMenuItem->Name = L"inputToolStripMenuItem";
			this->inputToolStripMenuItem->Size = System::Drawing::Size(160, 22);
			this->inputToolStripMenuItem->Text = L"Input";
			// 
			// showInputWindowToolStripMenuItem
			// 
			this->showInputWindowToolStripMenuItem->Name = L"showInputWindowToolStripMenuItem";
			this->showInputWindowToolStripMenuItem->Size = System::Drawing::Size(175, 22);
			this->showInputWindowToolStripMenuItem->Text = L"ShowInputWindow";
			this->showInputWindowToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::showInputWindowToolStripMenuItem_Click);
			// 
			// networkToolStripMenuItem
			// 
			this->networkToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->showNetworkWindowToolStripMenuItem });
			this->networkToolStripMenuItem->Name = L"networkToolStripMenuItem";
			this->networkToolStripMenuItem->Size = System::Drawing::Size(160, 22);
			this->networkToolStripMenuItem->Text = L"Network";
			// 
			// showNetworkWindowToolStripMenuItem
			// 
			this->showNetworkWindowToolStripMenuItem->Name = L"showNetworkWindowToolStripMenuItem";
			this->showNetworkWindowToolStripMenuItem->Size = System::Drawing::Size(192, 22);
			this->showNetworkWindowToolStripMenuItem->Text = L"ShowNetworkWindow";
			this->showNetworkWindowToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::showNetworkWindowToolStripMenuItem_Click);
			// 
			// resourcesToolStripMenuItem
			// 
			this->resourcesToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->showResourcesWindowToolStripMenuItem });
			this->resourcesToolStripMenuItem->Name = L"resourcesToolStripMenuItem";
			this->resourcesToolStripMenuItem->Size = System::Drawing::Size(160, 22);
			this->resourcesToolStripMenuItem->Text = L"Resources";
			// 
			// showResourcesWindowToolStripMenuItem
			// 
			this->showResourcesWindowToolStripMenuItem->Name = L"showResourcesWindowToolStripMenuItem";
			this->showResourcesWindowToolStripMenuItem->Size = System::Drawing::Size(200, 22);
			this->showResourcesWindowToolStripMenuItem->Text = L"ShowResourcesWindow";
			this->showResourcesWindowToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::showResourcesWindowToolStripMenuItem_Click);
			// 
			// runInvironmentToolStripMenuItem
			// 
			this->runInvironmentToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->showWindowToolStripMenuItem });
			this->runInvironmentToolStripMenuItem->Name = L"runInvironmentToolStripMenuItem";
			this->runInvironmentToolStripMenuItem->Size = System::Drawing::Size(160, 22);
			this->runInvironmentToolStripMenuItem->Text = L"RunInvironment";
			// 
			// showWindowToolStripMenuItem
			// 
			this->showWindowToolStripMenuItem->Name = L"showWindowToolStripMenuItem";
			this->showWindowToolStripMenuItem->Size = System::Drawing::Size(147, 22);
			this->showWindowToolStripMenuItem->Text = L"ShowWindow";
			this->showWindowToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::showWindowToolStripMenuItem_Click);
			// 
			// soundToolStripMenuItem
			// 
			this->soundToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->showSoundSystemWindowToolStripMenuItem });
			this->soundToolStripMenuItem->Name = L"soundToolStripMenuItem";
			this->soundToolStripMenuItem->Size = System::Drawing::Size(160, 22);
			this->soundToolStripMenuItem->Text = L"Sound";
			// 
			// showSoundSystemWindowToolStripMenuItem
			// 
			this->showSoundSystemWindowToolStripMenuItem->Name = L"showSoundSystemWindowToolStripMenuItem";
			this->showSoundSystemWindowToolStripMenuItem->Size = System::Drawing::Size(219, 22);
			this->showSoundSystemWindowToolStripMenuItem->Text = L"ShowSoundSystemWindow";
			this->showSoundSystemWindowToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::showSoundSystemWindowToolStripMenuItem_Click);
			// 
			// sceneViewToolStripMenuItem
			// 
			this->sceneViewToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->showSceneViewWindowToolStripMenuItem });
			this->sceneViewToolStripMenuItem->Name = L"sceneViewToolStripMenuItem";
			this->sceneViewToolStripMenuItem->Size = System::Drawing::Size(160, 22);
			this->sceneViewToolStripMenuItem->Text = L"SceneView";
			// 
			// showSceneViewWindowToolStripMenuItem
			// 
			this->showSceneViewWindowToolStripMenuItem->Name = L"showSceneViewWindowToolStripMenuItem";
			this->showSceneViewWindowToolStripMenuItem->Size = System::Drawing::Size(203, 22);
			this->showSceneViewWindowToolStripMenuItem->Text = L"ShowSceneViewWindow";
			this->showSceneViewWindowToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::showSceneViewWindowToolStripMenuItem_Click);
			// 
			// propertiesToolStripMenuItem
			// 
			this->propertiesToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->showPropertiesWindowToolStripMenuItem });
			this->propertiesToolStripMenuItem->Name = L"propertiesToolStripMenuItem";
			this->propertiesToolStripMenuItem->Size = System::Drawing::Size(160, 22);
			this->propertiesToolStripMenuItem->Text = L"Properties";
			// 
			// showPropertiesWindowToolStripMenuItem
			// 
			this->showPropertiesWindowToolStripMenuItem->Name = L"showPropertiesWindowToolStripMenuItem";
			this->showPropertiesWindowToolStripMenuItem->Size = System::Drawing::Size(200, 22);
			this->showPropertiesWindowToolStripMenuItem->Text = L"ShowPropertiesWindow";
			this->showPropertiesWindowToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::showPropertiesWindowToolStripMenuItem_Click);
			// 
			// createObjectToolStripMenuItem
			// 
			this->createObjectToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->showCreateObjectWindowToolStripMenuItem });
			this->createObjectToolStripMenuItem->Name = L"createObjectToolStripMenuItem";
			this->createObjectToolStripMenuItem->Size = System::Drawing::Size(160, 22);
			this->createObjectToolStripMenuItem->Text = L"CreateObject";
			// 
			// showCreateObjectWindowToolStripMenuItem
			// 
			this->showCreateObjectWindowToolStripMenuItem->Name = L"showCreateObjectWindowToolStripMenuItem";
			this->showCreateObjectWindowToolStripMenuItem->Size = System::Drawing::Size(216, 22);
			this->showCreateObjectWindowToolStripMenuItem->Text = L"ShowCreateObjectWindow";
			this->showCreateObjectWindowToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::showCreateObjectWindowToolStripMenuItem_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1451, 687);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->menuStrip1);
			this->Controls->Add(this->dockPanel1);
			this->IsMdiContainer = true;
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"Editor";
			this->Resize += gcnew System::EventHandler(this, &MyForm::MyForm_Resize);
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void buildToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		(void)sender;
		(void)e;
	}
private: System::Void showRenderWindowToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e){
	(void)sender;
	(void)e;
	m_renderWindow->Show();
}
private: System::Void showScriptingWindowToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e){
	(void)sender;
	(void)e;
	m_scriptingindow->Show();
}
private: System::Void showCheatWindowToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	(void)sender;
	(void)e;
	m_cheatWindow->Show();
}
private: System::Void showInputWindowToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	(void)sender;
	(void)e;
	m_inputWindow->Show();
}
private: System::Void showNetworkWindowToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	(void)sender;
	(void)e;
	m_networkWindow->Show();
}
private: System::Void showResourcesWindowToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	(void)sender;
	(void)e;
	m_recourcesWindow->Show();
}
private: System::Void showWindowToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	(void)sender;
	(void)e;
	m_runningEnvironmentWindow->Show();
}
private: System::Void showSoundSystemWindowToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	(void)sender;
	(void)e;
	m_soundSystemWindow->Show();
}
private: System::Void showSceneViewWindowToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	(void)sender;
	(void)e;
	m_sceneView->Show();
}
private: System::Void showPropertiesWindowToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	(void)sender;
	(void)e;
	m_properties->Show();
}
private: System::Void showCreateObjectWindowToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	(void)sender;
	(void)e;
	m_addObjectWindow->Show();
}
private: System::Void MyForm_Resize(System::Object^  sender, System::EventArgs^  e) {
	(void)sender;
	(void)e;
	this->dockPanel1->Size = this->ClientSize - System::Drawing::Size(0, 52);
}
private: System::Void importModelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
	(void)sender;
	(void)e;
	openFileDialog1->Title = "Import Model";
	openFileDialog1->FileName = "";
	openFileDialog1->ShowDialog();
	System::String^ fileName = openFileDialog1->FileName;

	{
		char* name = static_cast<char*>(Marshal::StringToHGlobalAnsi(fileName).ToPointer());
		Exports::Resources::ImportModel(name);
		Marshal::FreeHGlobal(static_cast<IntPtr>(name));
	}
}

	private: System::Void onIdle(System::Object^  sender, System::EventArgs^  e) {
		(void)sender;
		(void)e;
		Exports::System::WndProc(m_renderWindow->getHwnd(), 0, 0, 0);
		Exports::System::RenderFrame(1.0f / 60.0f);
		Invalidate();
	}
private: System::Void loadLevelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
	(void)sender;
	(void)e;

	openFileDialog1->Title = "Load Level";
	openFileDialog1->FileName = "";
	openFileDialog1->ShowDialog();
	System::String^ fileName = openFileDialog1->FileName;

	{
		char* name = static_cast<char*>(Marshal::StringToHGlobalAnsi(fileName).ToPointer());
		Exports::Resources::LoadLevelFromXMLFile(name);
		Marshal::FreeHGlobal(static_cast<IntPtr>(name));
	}
}
private: System::Void saveLevelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
	(void)sender;
	(void)e;

	saveFileDialog1->Title = "Save Level";
	saveFileDialog1->FileName = "level.xml";
	saveFileDialog1->ShowDialog();
	System::String^ fileName = saveFileDialog1->FileName;

	{
		char* name = static_cast<char*>(Marshal::StringToHGlobalAnsi(fileName).ToPointer());
		Exports::Resources::SaveLevelToXMLFile(name);
		Marshal::FreeHGlobal(static_cast<IntPtr>(name));
	}
}
};
}
