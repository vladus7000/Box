#pragma once

namespace Editor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Properties
	/// </summary>
	public ref class PropertiesWindow : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		PropertiesWindow(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		void showInfoAboutFile(String^ file);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~PropertiesWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  fileNameLabel;
	protected:
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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->fileNameLabel = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(41, 19);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(26, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"File:";
			// 
			// fileNameLabel
			// 
			this->fileNameLabel->AutoSize = true;
			this->fileNameLabel->Location = System::Drawing::Point(87, 19);
			this->fileNameLabel->Name = L"fileNameLabel";
			this->fileNameLabel->Size = System::Drawing::Size(48, 13);
			this->fileNameLabel->TabIndex = 1;
			this->fileNameLabel->Text = L"fileName";
			// 
			// PropertiesWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 261);
			this->Controls->Add(this->fileNameLabel);
			this->Controls->Add(this->label1);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->HideOnClose = true;
			this->Name = L"PropertiesWindow";
			this->Text = L"Properties";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
