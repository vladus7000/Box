#pragma once

#include "EngineExports/EngineExports.hpp"

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
			this->panel1->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &RenderWindow::panel1_Scroll);
			this->panel1->Click += gcnew System::EventHandler(this, &RenderWindow::panel1_Click);
			this->panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &RenderWindow::panel1_Paint);
			this->panel1->Enter += gcnew System::EventHandler(this, &RenderWindow::panel1_Enter);
			this->panel1->Leave += gcnew System::EventHandler(this, &RenderWindow::panel1_Leave);
			this->panel1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &RenderWindow::panel1_MouseDown);
			this->panel1->MouseLeave += gcnew System::EventHandler(this, &RenderWindow::panel1_MouseLeave);
			this->panel1->MouseHover += gcnew System::EventHandler(this, &RenderWindow::panel1_MouseHover);
			this->panel1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &RenderWindow::panel1_MouseMove);
			this->panel1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &RenderWindow::panel1_MouseUp);
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
			this->Enter += gcnew System::EventHandler(this, &RenderWindow::RenderWindow_Enter);
			this->Leave += gcnew System::EventHandler(this, &RenderWindow::RenderWindow_Leave);
			this->Resize += gcnew System::EventHandler(this, &RenderWindow::RenderWindow_Resize);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void RenderWindow_Resize(System::Object^  sender, System::EventArgs^  e)
	{
		(void)sender;
		(void)e;

		this->panel1->Size = this->ClientSize - System::Drawing::Size(24, 24);
		Exports::System::Resize(this->panel1->Size.Width, this->panel1->Size.Height);
	}
	private: System::Void panel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
		(void)sender;
		(void)e;
		this->panel1->Focus();
	}
	private: System::Void panel1_PreviewKeyDown(System::Object^  sender, System::Windows::Forms::PreviewKeyDownEventArgs^  e) {
		(void)sender;
		Exports::Input::OnKeyDown(static_cast<box::U32>(e->KeyCode));
	}
	private: System::Void panel1_Click(System::Object^  sender, System::EventArgs^  e)
	{
		(void)sender;
		(void)e;
		Exports::Editor::SetEnabledCameraInput(1);
	}
	private: System::Void panel1_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e) {
		(void)sender;
		(void)e;
	}
	private: System::Void panel1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		(void)sender;
		(void)e;
		Exports::Input::OnMouseButtonDown(e->X, e->Y, 0);
	}
	private: System::Void panel1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		(void)sender;
		(void)e;
		Exports::Input::OnMouseMove(e->X, e->Y);
	}
	private: System::Void panel1_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		(void)sender;
		(void)e;
		Exports::Input::OnMousebuttonUp(e->X, e->Y, 0);
	}
	private: System::Void RenderWindow_Enter(System::Object^  sender, System::EventArgs^  e)
	{
		(void)sender;
		(void)e;
		this->panel1->Focus();
	}
	private: System::Void RenderWindow_Leave(System::Object^  sender, System::EventArgs^  e)
	{
		(void)sender;
		(void)e;
	}
	private: System::Void panel1_Enter(System::Object^  sender, System::EventArgs^  e)
	{
		(void)sender;
		(void)e;
	}
	private: System::Void panel1_Leave(System::Object^  sender, System::EventArgs^  e)
	{
		(void)sender;
		(void)e;
	}
	protected: void OnGotFocus(System::EventArgs^  e) override
	{
		Invalidate();
		Exports::Editor::SetEnabledCameraInput(1);
		WeifenLuo::WinFormsUI::Docking::DockContent::OnGotFocus(e);
	}
	protected: void OnLostFocus(System::EventArgs^  e) override
	{
		Invalidate();
		Exports::Editor::SetEnabledCameraInput(0);
		WeifenLuo::WinFormsUI::Docking::DockContent::OnLostFocus(e);
	}

	protected: void OnDeactivate(System::EventArgs^  e) override
	{
		(void)e;
		Invalidate();
		Exports::Editor::SetEnabledCameraInput(0);
	}
	private: System::Void panel1_MouseHover(System::Object^  sender, System::EventArgs^  e)
	{
		(void)sender;
		(void)e;
		Exports::Editor::SetEnabledCameraInput(1);
	}
	private: System::Void panel1_MouseLeave(System::Object^  sender, System::EventArgs^  e)
	{
		(void)sender;
		(void)e;
		Exports::Editor::SetEnabledCameraInput(0);
	}
};
}
