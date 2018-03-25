#pragma once

namespace Editor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Xml;

	/// <summary>
	/// Summary for RunningEnvironmentWindow
	/// </summary>
	public ref class RunningEnvironmentWindow : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		RunningEnvironmentWindow(void)
		{
			InitializeComponent();

			m_XmlReaderSettings = gcnew XmlReaderSettings();
			m_doc = gcnew XmlDocument();
		}

		String^ getTextEditor();
		bool isTextFile(String^ file);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~RunningEnvironmentWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::TextBox^  TextEditor;
	protected:

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		XmlReader^ m_reader;
		XmlReaderSettings^ m_XmlReaderSettings;
		XmlDocument^ m_doc;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->TextEditor = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(68, 12);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Save";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// TextEditor
			// 
			this->TextEditor->Location = System::Drawing::Point(109, 51);
			this->TextEditor->Name = L"TextEditor";
			this->TextEditor->Size = System::Drawing::Size(342, 20);
			this->TextEditor->TabIndex = 1;
			this->TextEditor->Text = L"notepad.exe";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(33, 54);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(60, 13);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Text editor:";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(466, 49);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(23, 23);
			this->button2->TabIndex = 3;
			this->button2->Text = L"...";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &RunningEnvironmentWindow::button2_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// RunningEnvironmentWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(658, 496);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->TextEditor);
			this->Controls->Add(this->button1);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->HideOnClose = true;
			this->Name = L"RunningEnvironmentWindow";
			this->Text = L"RunningEnvironmentWindow";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e)
	{
		(void)sender;
		(void)e;
		openFileDialog1->Title = "Set text editor";
		openFileDialog1->FileName = "";
		openFileDialog1->ShowDialog();
		TextEditor->Text = openFileDialog1->FileName;
	}
};
}
