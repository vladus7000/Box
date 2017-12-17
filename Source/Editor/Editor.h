#pragma once
#include <System/Engine.hpp>
#include "System\Input.hpp"

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
		void deinit();
		box::Engine* m_engine;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  RenderWindow;

	private: System::Windows::Forms::TabPage^  tabPage2;


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
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->RenderWindow = (gcnew System::Windows::Forms::TabPage());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->tabControl1->SuspendLayout();
			this->RenderWindow->SuspendLayout();
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->panel1->Location = System::Drawing::Point(19, 16);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(615, 441);
			this->panel1->TabIndex = 0;
			this->panel1->Click += gcnew System::EventHandler(this, &MyForm::panel1_Click);
			this->panel1->Enter += gcnew System::EventHandler(this, &MyForm::panel1_Enter);
			this->panel1->MouseHover += gcnew System::EventHandler(this, &MyForm::panel1_MouseHover);
			this->panel1->PreviewKeyDown += gcnew System::Windows::Forms::PreviewKeyDownEventHandler(this, &MyForm::panel1_PreviewKeyDown);
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->RenderWindow);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Location = System::Drawing::Point(12, 12);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->Padding = System::Drawing::Point(10, 3);
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(1248, 501);
			this->tabControl1->TabIndex = 2;
			// 
			// RenderWindow
			// 
			this->RenderWindow->Controls->Add(this->panel1);
			this->RenderWindow->Location = System::Drawing::Point(4, 22);
			this->RenderWindow->Name = L"RenderWindow";
			this->RenderWindow->Padding = System::Windows::Forms::Padding(3);
			this->RenderWindow->Size = System::Drawing::Size(1240, 475);
			this->RenderWindow->TabIndex = 0;
			this->RenderWindow->Text = L"RenderWindow";
			this->RenderWindow->UseVisualStyleBackColor = true;
			// 
			// tabPage2
			// 
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(1240, 475);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"tabPage2";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1272, 525);
			this->Controls->Add(this->tabControl1);
			this->Name = L"MyForm";
			this->Text = L"Editor";
			this->tabControl1->ResumeLayout(false);
			this->RenderWindow->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void panel1_PreviewKeyDown(System::Object^  sender, System::Windows::Forms::PreviewKeyDownEventArgs^  e)
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
	}
	};
}
