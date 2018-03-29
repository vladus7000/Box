#pragma once

#include <EngineExports/EngineExports.hpp>

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
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::TextBox^  CameraFovTextBox;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  ZFarTextBox;

	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  ZNearTextBox;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::TextBox^  CameraSpeedTextBox;
	private: System::Windows::Forms::Label^  label5;
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
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->ZFarTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->ZNearTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->CameraFovTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->CameraSpeedTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->groupBox1->SuspendLayout();
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
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->CameraSpeedTextBox);
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Controls->Add(this->button3);
			this->groupBox1->Controls->Add(this->ZFarTextBox);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->ZNearTextBox);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->CameraFovTextBox);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Location = System::Drawing::Point(46, 104);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(522, 154);
			this->groupBox1->TabIndex = 4;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"CameraSettings";
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(63, 125);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->TabIndex = 5;
			this->button3->Text = L"Update";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &RunningEnvironmentWindow::button3_Click);
			// 
			// ZFarTextBox
			// 
			this->ZFarTextBox->Location = System::Drawing::Point(63, 86);
			this->ZFarTextBox->Name = L"ZFarTextBox";
			this->ZFarTextBox->Size = System::Drawing::Size(115, 20);
			this->ZFarTextBox->TabIndex = 8;
			this->ZFarTextBox->Text = L"500";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(19, 86);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(27, 13);
			this->label4->TabIndex = 9;
			this->label4->Text = L"zFar";
			// 
			// ZNearTextBox
			// 
			this->ZNearTextBox->Location = System::Drawing::Point(63, 60);
			this->ZNearTextBox->Name = L"ZNearTextBox";
			this->ZNearTextBox->Size = System::Drawing::Size(115, 20);
			this->ZNearTextBox->TabIndex = 6;
			this->ZNearTextBox->Text = L"0.5";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(19, 60);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(35, 13);
			this->label3->TabIndex = 7;
			this->label3->Text = L"zNear";
			// 
			// CameraFovTextBox
			// 
			this->CameraFovTextBox->Location = System::Drawing::Point(63, 34);
			this->CameraFovTextBox->Name = L"CameraFovTextBox";
			this->CameraFovTextBox->Size = System::Drawing::Size(115, 20);
			this->CameraFovTextBox->TabIndex = 5;
			this->CameraFovTextBox->Text = L"1.0472";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(19, 34);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(22, 13);
			this->label2->TabIndex = 5;
			this->label2->Text = L"fov";
			// 
			// CameraSpeedTextBox
			// 
			this->CameraSpeedTextBox->Location = System::Drawing::Point(307, 34);
			this->CameraSpeedTextBox->Name = L"CameraSpeedTextBox";
			this->CameraSpeedTextBox->Size = System::Drawing::Size(115, 20);
			this->CameraSpeedTextBox->TabIndex = 10;
			this->CameraSpeedTextBox->Text = L"0.03";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(214, 34);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(87, 13);
			this->label5->TabIndex = 11;
			this->label5->Text = L"movementSpeed";
			// 
			// RunningEnvironmentWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(658, 496);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->TextEditor);
			this->Controls->Add(this->button1);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->HideOnClose = true;
			this->Name = L"RunningEnvironmentWindow";
			this->Text = L"RunningEnvironmentWindow";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
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
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e)
	{
		(void)sender;
		(void)e;
		Exports::Editor::SetCameraFov((float)System::Convert::ToDouble(this->CameraFovTextBox->Text));
		Exports::Editor::SetCameraZNear((float)System::Convert::ToDouble(this->ZNearTextBox->Text));
		Exports::Editor::SetCameraZFar((float)System::Convert::ToDouble(this->ZFarTextBox->Text));
		Exports::Editor::SetCameraMovementSpeed((float)System::Convert::ToDouble(this->CameraSpeedTextBox->Text));
	}
};
}
