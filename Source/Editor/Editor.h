#pragma once
#include <System/Engine.hpp>

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
	private: System::Windows::Forms::Button^  button1;

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
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->panel1->Location = System::Drawing::Point(59, 35);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(487, 310);
			this->panel1->TabIndex = 0;
			this->panel1->Click += gcnew System::EventHandler(this, &MyForm::panel1_Click);
			this->panel1->Enter += gcnew System::EventHandler(this, &MyForm::panel1_Enter);
			this->panel1->MouseHover += gcnew System::EventHandler(this, &MyForm::panel1_MouseHover);
			this->panel1->PreviewKeyDown += gcnew System::Windows::Forms::PreviewKeyDownEventHandler(this, &MyForm::panel1_PreviewKeyDown);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(659, 90);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1272, 525);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->panel1);
			this->Name = L"MyForm";
			this->Text = L"Editor";
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void panel1_PreviewKeyDown(System::Object^  sender, System::Windows::Forms::PreviewKeyDownEventArgs^  e) {
		
		if (e->KeyCode == System::Windows::Forms::Keys::W)
		{
			System::Console::Write("W pressed ");
		}
		if (e->KeyCode == System::Windows::Forms::Keys::A)
		{
			System::Console::Write("A pressed ");
		}
		if (e->KeyCode == System::Windows::Forms::Keys::S)
		{
			System::Console::Write("S pressed ");
		}
		if (e->KeyCode == System::Windows::Forms::Keys::D)
		{
			System::Console::Write("D pressed ");
		}
		System::Console::WriteLine();
	}
	private: System::Void panel1_MouseHover(System::Object^  sender, System::EventArgs^  e) {

	}
	private: System::Void panel1_Enter(System::Object^  sender, System::EventArgs^  e) {
		System::Console::WriteLine("on focus");
	}
	private: System::Void panel1_Click(System::Object^  sender, System::EventArgs^  e) {
		this->panel1->Focus();
	}
	};
}
