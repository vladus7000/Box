#pragma once
#include <System/Engine.hpp>
#include "System\Input.hpp"

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

using namespace box;

namespace winforms {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	
	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
			: m_engine(nullptr)
		{
			InitializeComponent();
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
		box::Engine* m_engine;

		Editor::RenderWindow^ m_renderWindow;
		Editor::CheatWindow^ m_cheatWindow;
		Editor::InputWindow^ m_inputWindow;
		Editor::NetworkWindow^ m_networkWindow;
		Editor::RecourcesWindow^ m_recourcesWindow;
		Editor::RunningEnvironmentWindow^ m_runningEnvironmentWindow;
		Editor::ScriptingWindow^ m_scriptingindow;
		Editor::SoundSystemWindow^ m_soundSystemWindow;
		Editor::SceneView^ m_sceneView;
		Editor::Properties^ m_properties;
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
			this->buildToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->releaseBuildToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->debugToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->profileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->runGameToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->settingsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->renderWindowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->scriptingToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
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
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
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
			this->toolsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->renderWindowToolStripMenuItem,
					this->scriptingToolStripMenuItem
			});
			this->toolsToolStripMenuItem->Name = L"toolsToolStripMenuItem";
			this->toolsToolStripMenuItem->Size = System::Drawing::Size(47, 20);
			this->toolsToolStripMenuItem->Text = L"Tools";
			// 
			// renderWindowToolStripMenuItem
			// 
			this->renderWindowToolStripMenuItem->Name = L"renderWindowToolStripMenuItem";
			this->renderWindowToolStripMenuItem->Size = System::Drawing::Size(155, 22);
			this->renderWindowToolStripMenuItem->Text = L"RenderWindow";
			// 
			// scriptingToolStripMenuItem
			// 
			this->scriptingToolStripMenuItem->Name = L"scriptingToolStripMenuItem";
			this->scriptingToolStripMenuItem->Size = System::Drawing::Size(155, 22);
			this->scriptingToolStripMenuItem->Text = L"Scripting";
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
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	/*private: System::Void panel1_PreviewKeyDown(System::Object^  sender, System::Windows::Forms::PreviewKeyDownEventArgs^  e)
	{
		(void)sender;
		Input::Instance().test_register_key_event(static_cast<U32>(e->KeyCode));
	}
	private: System::Void panel1_MouseHover(System::Object^  sender, System::EventArgs^  e)
	{
		(void)sender;
		(void)e;
	}
	private: System::Void panel1_Enter(System::Object^  sender, System::EventArgs^  e)
	{
		(void)sender;
		(void)e;
		System::Console::WriteLine("on focus");
	}
	private: System::Void panel1_Click(System::Object^  sender, System::EventArgs^  e)
	{
		(void)sender;
		(void)e;
		this->panel1->Focus();
	}*/
	private: System::Void buildToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	}
};
}
