#pragma once

#include <EngineExports/EngineExports.hpp>
#include "Globals.hpp"

namespace Editor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;
	using namespace System::Xml;

	/// <summary>
	/// Summary for Properties
	/// </summary>
	public ref class PropertiesWindow : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		PropertiesWindow()
		{
			InitializeComponent();
			
			m_inCollection = false;
		}

		void setGlobals(Globals^ globals) { m_globals = globals; }
		void showInfoAboutFile(String^ file, String^ type, bool inCollection);
		void disableImportForm();
		System::String^ getResourcePathFromFullPath(System::String^ fullPath);

		void addResourceToComboBoxes(String^ fullPath);
		void addShader(String^ descName);
		void addMaterial(String^ descName);
		void addTexture(String^ descName);
		void clearComboBoxes();
		void updateMaterialUI(String^ fileName);

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
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  typeLabel;
	private: System::Windows::Forms::Button^  addToAssetsButton;

	protected:
	protected:

	private:
		bool m_inCollection;
		Globals^ m_globals;
		System::String^ m_descriptionPath;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  assetSrcPathTextBox;

	private: System::Windows::Forms::TextBox^  assetPathTextBox;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  importStatusLabel;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  modelNameTextBox;
	private: System::Windows::Forms::TabPage^  tabPage3;
	private: System::Windows::Forms::TabControl^  tabControl2;
	private: System::Windows::Forms::TabPage^  tabPage4;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::ComboBox^  selectMaterialComboBox;

	private: System::Windows::Forms::TabPage^  tabPage5;
	private: System::Windows::Forms::TabPage^  tabPage6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::ComboBox^  comboBox2;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::ComboBox^  materialSelectShaderComboBox;


	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::ComboBox^  textureSlot3ComboBox;

	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::ComboBox^  textureSlot2ComboBox;

	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::ComboBox^  textureSlot1ComboBox;

	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::ComboBox^  shaderSelectShaderComboBox;

	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::TextBox^  materialNameTextBox;

	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::TextBox^  materialDescPathTextBox;


	private: System::Windows::Forms::Label^  label14;
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
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->typeLabel = (gcnew System::Windows::Forms::Label());
			this->addToAssetsButton = (gcnew System::Windows::Forms::Button());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->modelNameTextBox = (gcnew System::Windows::Forms::TextBox());
			this->importStatusLabel = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->assetSrcPathTextBox = (gcnew System::Windows::Forms::TextBox());
			this->assetPathTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->tabControl2 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->comboBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->selectMaterialComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->tabPage5 = (gcnew System::Windows::Forms::TabPage());
			this->materialDescPathTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->materialNameTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->textureSlot3ComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->textureSlot2ComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->textureSlot1ComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->materialSelectShaderComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->tabPage6 = (gcnew System::Windows::Forms::TabPage());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->shaderSelectShaderComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->tabPage3->SuspendLayout();
			this->tabControl2->SuspendLayout();
			this->tabPage4->SuspendLayout();
			this->tabPage5->SuspendLayout();
			this->tabPage6->SuspendLayout();
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
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(33, 43);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(34, 13);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Type:";
			// 
			// typeLabel
			// 
			this->typeLabel->AutoSize = true;
			this->typeLabel->Location = System::Drawing::Point(87, 43);
			this->typeLabel->Name = L"typeLabel";
			this->typeLabel->Size = System::Drawing::Size(27, 13);
			this->typeLabel->TabIndex = 3;
			this->typeLabel->Text = L"type";
			// 
			// addToAssetsButton
			// 
			this->addToAssetsButton->Location = System::Drawing::Point(9, 136);
			this->addToAssetsButton->Name = L"addToAssetsButton";
			this->addToAssetsButton->Size = System::Drawing::Size(89, 23);
			this->addToAssetsButton->TabIndex = 4;
			this->addToAssetsButton->Text = L"Add to assets";
			this->addToAssetsButton->UseVisualStyleBackColor = true;
			this->addToAssetsButton->Click += gcnew System::EventHandler(this, &PropertiesWindow::button1_Click);
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Controls->Add(this->tabPage3);
			this->tabControl1->Location = System::Drawing::Point(12, 68);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(329, 492);
			this->tabControl1->TabIndex = 5;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->label5);
			this->tabPage1->Controls->Add(this->modelNameTextBox);
			this->tabPage1->Controls->Add(this->importStatusLabel);
			this->tabPage1->Controls->Add(this->label4);
			this->tabPage1->Controls->Add(this->assetSrcPathTextBox);
			this->tabPage1->Controls->Add(this->assetPathTextBox);
			this->tabPage1->Controls->Add(this->label3);
			this->tabPage1->Controls->Add(this->addToAssetsButton);
			this->tabPage1->Enabled = false;
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(321, 466);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Import";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(6, 85);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(66, 13);
			this->label5->TabIndex = 10;
			this->label5->Text = L"model Name";
			// 
			// modelNameTextBox
			// 
			this->modelNameTextBox->Location = System::Drawing::Point(82, 78);
			this->modelNameTextBox->Name = L"modelNameTextBox";
			this->modelNameTextBox->Size = System::Drawing::Size(233, 20);
			this->modelNameTextBox->TabIndex = 9;
			// 
			// importStatusLabel
			// 
			this->importStatusLabel->AutoSize = true;
			this->importStatusLabel->ForeColor = System::Drawing::SystemColors::ControlText;
			this->importStatusLabel->Location = System::Drawing::Point(104, 141);
			this->importStatusLabel->Name = L"importStatusLabel";
			this->importStatusLabel->Size = System::Drawing::Size(37, 13);
			this->importStatusLabel->TabIndex = 6;
			this->importStatusLabel->Text = L"Status";
			this->importStatusLabel->Visible = false;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(6, 59);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(76, 13);
			this->label4->TabIndex = 8;
			this->label4->Text = L"asset src path:";
			// 
			// assetSrcPathTextBox
			// 
			this->assetSrcPathTextBox->Location = System::Drawing::Point(82, 52);
			this->assetSrcPathTextBox->Name = L"assetSrcPathTextBox";
			this->assetSrcPathTextBox->Size = System::Drawing::Size(233, 20);
			this->assetSrcPathTextBox->TabIndex = 7;
			// 
			// assetPathTextBox
			// 
			this->assetPathTextBox->Location = System::Drawing::Point(82, 26);
			this->assetPathTextBox->Name = L"assetPathTextBox";
			this->assetPathTextBox->Size = System::Drawing::Size(233, 20);
			this->assetPathTextBox->TabIndex = 6;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(17, 29);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(59, 13);
			this->label3->TabIndex = 5;
			this->label3->Text = L"asset path:";
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->button2);
			this->tabPage2->Controls->Add(this->button1);
			this->tabPage2->Enabled = false;
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(321, 466);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"addToScene";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(133, 17);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(93, 23);
			this->button2->TabIndex = 1;
			this->button2->Text = L"addToScene";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &PropertiesWindow::addToSceneClick);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(20, 17);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(92, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"previewModel";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &PropertiesWindow::previewModelClick);
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->tabControl2);
			this->tabPage3->Location = System::Drawing::Point(4, 22);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Size = System::Drawing::Size(321, 466);
			this->tabPage3->TabIndex = 2;
			this->tabPage3->Text = L"Properties";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// tabControl2
			// 
			this->tabControl2->Controls->Add(this->tabPage4);
			this->tabControl2->Controls->Add(this->tabPage5);
			this->tabControl2->Controls->Add(this->tabPage6);
			this->tabControl2->Location = System::Drawing::Point(3, 3);
			this->tabControl2->Name = L"tabControl2";
			this->tabControl2->SelectedIndex = 0;
			this->tabControl2->Size = System::Drawing::Size(315, 460);
			this->tabControl2->TabIndex = 0;
			// 
			// tabPage4
			// 
			this->tabPage4->Controls->Add(this->label7);
			this->tabPage4->Controls->Add(this->comboBox2);
			this->tabPage4->Controls->Add(this->button3);
			this->tabPage4->Controls->Add(this->label6);
			this->tabPage4->Controls->Add(this->selectMaterialComboBox);
			this->tabPage4->Location = System::Drawing::Point(4, 22);
			this->tabPage4->Name = L"tabPage4";
			this->tabPage4->Padding = System::Windows::Forms::Padding(3);
			this->tabPage4->Size = System::Drawing::Size(307, 434);
			this->tabPage4->TabIndex = 0;
			this->tabPage4->Text = L"Model";
			this->tabPage4->UseVisualStyleBackColor = true;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(20, 22);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(65, 13);
			this->label7->TabIndex = 4;
			this->label7->Text = L"Select mesh";
			// 
			// comboBox2
			// 
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Location = System::Drawing::Point(91, 19);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(188, 21);
			this->comboBox2->TabIndex = 3;
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(83, 393);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->TabIndex = 2;
			this->button3->Text = L"Apply";
			this->button3->UseVisualStyleBackColor = true;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(9, 70);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(76, 13);
			this->label6->TabIndex = 1;
			this->label6->Text = L"Select material";
			// 
			// selectMaterialComboBox
			// 
			this->selectMaterialComboBox->FormattingEnabled = true;
			this->selectMaterialComboBox->Location = System::Drawing::Point(91, 67);
			this->selectMaterialComboBox->Name = L"selectMaterialComboBox";
			this->selectMaterialComboBox->Size = System::Drawing::Size(188, 21);
			this->selectMaterialComboBox->TabIndex = 0;
			// 
			// tabPage5
			// 
			this->tabPage5->Controls->Add(this->materialDescPathTextBox);
			this->tabPage5->Controls->Add(this->label14);
			this->tabPage5->Controls->Add(this->materialNameTextBox);
			this->tabPage5->Controls->Add(this->label13);
			this->tabPage5->Controls->Add(this->button6);
			this->tabPage5->Controls->Add(this->button4);
			this->tabPage5->Controls->Add(this->label11);
			this->tabPage5->Controls->Add(this->textureSlot3ComboBox);
			this->tabPage5->Controls->Add(this->label10);
			this->tabPage5->Controls->Add(this->textureSlot2ComboBox);
			this->tabPage5->Controls->Add(this->label9);
			this->tabPage5->Controls->Add(this->textureSlot1ComboBox);
			this->tabPage5->Controls->Add(this->label8);
			this->tabPage5->Controls->Add(this->materialSelectShaderComboBox);
			this->tabPage5->Location = System::Drawing::Point(4, 22);
			this->tabPage5->Name = L"tabPage5";
			this->tabPage5->Padding = System::Windows::Forms::Padding(3);
			this->tabPage5->Size = System::Drawing::Size(307, 434);
			this->tabPage5->TabIndex = 1;
			this->tabPage5->Text = L"Material";
			this->tabPage5->UseVisualStyleBackColor = true;
			// 
			// materialDescPathTextBox
			// 
			this->materialDescPathTextBox->Location = System::Drawing::Point(93, 13);
			this->materialDescPathTextBox->Name = L"materialDescPathTextBox";
			this->materialDescPathTextBox->Size = System::Drawing::Size(188, 20);
			this->materialDescPathTextBox->TabIndex = 18;
			this->materialDescPathTextBox->Text = L"desc/default.material";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(35, 16);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(52, 13);
			this->label14->TabIndex = 17;
			this->label14->Text = L"descPath";
			// 
			// materialNameTextBox
			// 
			this->materialNameTextBox->Location = System::Drawing::Point(93, 45);
			this->materialNameTextBox->Name = L"materialNameTextBox";
			this->materialNameTextBox->Size = System::Drawing::Size(188, 20);
			this->materialNameTextBox->TabIndex = 16;
			this->materialNameTextBox->Text = L"default";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(51, 48);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(35, 13);
			this->label13->TabIndex = 15;
			this->label13->Text = L"Name";
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(112, 396);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(108, 23);
			this->button6->TabIndex = 14;
			this->button6->Text = L"New/Upd material";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &PropertiesWindow::newMaterial_Click);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(16, 396);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(75, 23);
			this->button4->TabIndex = 13;
			this->button4->Text = L"Apply";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &PropertiesWindow::button4_Click);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(15, 207);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(71, 13);
			this->label11->TabIndex = 12;
			this->label11->Text = L"Texture slot 3";
			// 
			// textureSlot3ComboBox
			// 
			this->textureSlot3ComboBox->FormattingEnabled = true;
			this->textureSlot3ComboBox->Location = System::Drawing::Point(93, 204);
			this->textureSlot3ComboBox->Name = L"textureSlot3ComboBox";
			this->textureSlot3ComboBox->Size = System::Drawing::Size(188, 21);
			this->textureSlot3ComboBox->TabIndex = 11;
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(15, 168);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(71, 13);
			this->label10->TabIndex = 10;
			this->label10->Text = L"Texture slot 2";
			// 
			// textureSlot2ComboBox
			// 
			this->textureSlot2ComboBox->FormattingEnabled = true;
			this->textureSlot2ComboBox->Location = System::Drawing::Point(93, 165);
			this->textureSlot2ComboBox->Name = L"textureSlot2ComboBox";
			this->textureSlot2ComboBox->Size = System::Drawing::Size(188, 21);
			this->textureSlot2ComboBox->TabIndex = 9;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(15, 129);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(71, 13);
			this->label9->TabIndex = 8;
			this->label9->Text = L"Texture slot 1";
			// 
			// textureSlot1ComboBox
			// 
			this->textureSlot1ComboBox->FormattingEnabled = true;
			this->textureSlot1ComboBox->Location = System::Drawing::Point(93, 126);
			this->textureSlot1ComboBox->Name = L"textureSlot1ComboBox";
			this->textureSlot1ComboBox->Size = System::Drawing::Size(188, 21);
			this->textureSlot1ComboBox->TabIndex = 7;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(15, 82);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(72, 13);
			this->label8->TabIndex = 6;
			this->label8->Text = L"Select shader";
			// 
			// materialSelectShaderComboBox
			// 
			this->materialSelectShaderComboBox->FormattingEnabled = true;
			this->materialSelectShaderComboBox->Location = System::Drawing::Point(93, 79);
			this->materialSelectShaderComboBox->Name = L"materialSelectShaderComboBox";
			this->materialSelectShaderComboBox->Size = System::Drawing::Size(188, 21);
			this->materialSelectShaderComboBox->TabIndex = 5;
			// 
			// tabPage6
			// 
			this->tabPage6->Controls->Add(this->button5);
			this->tabPage6->Controls->Add(this->label12);
			this->tabPage6->Controls->Add(this->shaderSelectShaderComboBox);
			this->tabPage6->Location = System::Drawing::Point(4, 22);
			this->tabPage6->Name = L"tabPage6";
			this->tabPage6->Size = System::Drawing::Size(307, 434);
			this->tabPage6->TabIndex = 2;
			this->tabPage6->Text = L"Shader";
			this->tabPage6->UseVisualStyleBackColor = true;
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(21, 382);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(75, 23);
			this->button5->TabIndex = 9;
			this->button5->Text = L"compile";
			this->button5->UseVisualStyleBackColor = true;
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(18, 19);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(72, 13);
			this->label12->TabIndex = 8;
			this->label12->Text = L"Select shader";
			// 
			// shaderSelectShaderComboBox
			// 
			this->shaderSelectShaderComboBox->FormattingEnabled = true;
			this->shaderSelectShaderComboBox->Location = System::Drawing::Point(96, 16);
			this->shaderSelectShaderComboBox->Name = L"shaderSelectShaderComboBox";
			this->shaderSelectShaderComboBox->Size = System::Drawing::Size(188, 21);
			this->shaderSelectShaderComboBox->TabIndex = 7;
			// 
			// PropertiesWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(353, 583);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->typeLabel);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->fileNameLabel);
			this->Controls->Add(this->label1);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->HideOnClose = true;
			this->Name = L"PropertiesWindow";
			this->Text = L"Properties";
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->tabPage3->ResumeLayout(false);
			this->tabControl2->ResumeLayout(false);
			this->tabPage4->ResumeLayout(false);
			this->tabPage4->PerformLayout();
			this->tabPage5->ResumeLayout(false);
			this->tabPage5->PerformLayout();
			this->tabPage6->ResumeLayout(false);
			this->tabPage6->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void addToAssets_Click(System::Object^  sender, System::EventArgs^  e)
	{
		(void)sender;
		(void)e;
	}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void previewModelClick(System::Object^  sender, System::EventArgs^  e);
	private: System::Void addToSceneClick(System::Object^  sender, System::EventArgs^  e);
	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e)
	{
		(void)sender;
		(void)e;
	}

private: System::Void newMaterial_Click(System::Object^  sender, System::EventArgs^  e);
};
}
