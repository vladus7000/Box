#pragma once

#include <System/Engine.hpp>
#include "System\Input.hpp"

using namespace box;

namespace Editor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for RenderWindow
	/// </summary>
	public ref class RenderWindow : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		RenderWindow(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		void* getHwnd();

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~RenderWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Panel^  panel1;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->panel1->Location = System::Drawing::Point(12, 12);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(260, 237);
			this->panel1->TabIndex = 1;
			this->panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &RenderWindow::panel1_Paint);
			this->panel1->PreviewKeyDown += gcnew System::Windows::Forms::PreviewKeyDownEventHandler(this, &RenderWindow::panel1_PreviewKeyDown);
			// 
			// RenderWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 261);
			this->Controls->Add(this->panel1);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->HideOnClose = true;
			this->Name = L"RenderWindow";
			this->Text = L"RenderWindow";
			this->Resize += gcnew System::EventHandler(this, &RenderWindow::RenderWindow_Resize);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void RenderWindow_Resize(System::Object^  sender, System::EventArgs^  e) {
		this->panel1->Size = this->ClientSize - System::Drawing::Size(24, 24);
	}
	private: System::Void panel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
		(void)sender;
		(void)e;
		this->panel1->Focus();
	}
	private: System::Void panel1_PreviewKeyDown(System::Object^  sender, System::Windows::Forms::PreviewKeyDownEventArgs^  e) {
		(void)sender;
		Input::Instance().test_register_key_event(static_cast<U32>(e->KeyCode));
	}
	};
}
