#ifndef LOADVIEWER_H_
#define LOADVIEWER_H_

#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;

#include "LoadUI.h"
#include "PCNodeUI.h"
#include "../LoadTracer/Defines.h"
#include "RandomFederateStarter.h"

namespace LoadManager {

	/// <summary>
	/// Summary for LoadViewer
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>

	public ref class LoadViewer : public System::Windows::Forms::Form
	{
	public:
		LoadViewer(void)
		{
			InitializeComponent();
			pcList = gcnew List<PCNodeStruct^>();
			this->CheckForIllegalCrossThreadCalls = false;
			simhasstarted = false;
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~LoadViewer()
		{
			if (components)
			{
				delete components;
			}
		}



	private: System::Windows::Forms::GroupBox^  groupBox3;

	private: System::Windows::Forms::GroupBox^  groupBox6;
	public: LoadManager::LoadUI^  loadUINetwork;

	private: 

	public: LoadUI^  loadUIMemory;



	public: LoadUI^  loadUICpu;
	private: System::Windows::Forms::Panel^  pcNodePanel;
	private: System::Windows::Forms::Label^  lblMessage;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TrackBar^  trackBar1;
	private: System::Windows::Forms::RichTextBox^  richTextBox1;
	public: System::Windows::Forms::CheckBox^  checkBoxLBSActive;
	private: System::Windows::Forms::TextBox^  textBox1;
	public: 
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::ComponentModel::IContainer^  components;
	public: 

	public: 


	public: 

	public: 







	protected: 

	private:
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
			this->components = (gcnew System::ComponentModel::Container());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->loadUINetwork = (gcnew LoadManager::LoadUI());
			this->loadUIMemory = (gcnew LoadManager::LoadUI());
			this->loadUICpu = (gcnew LoadManager::LoadUI());
			this->groupBox6 = (gcnew System::Windows::Forms::GroupBox());
			this->pcNodePanel = (gcnew System::Windows::Forms::Panel());
			this->lblMessage = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->checkBoxLBSActive = (gcnew System::Windows::Forms::CheckBox());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->groupBox3->SuspendLayout();
			this->groupBox6->SuspendLayout();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->BeginInit();
			this->SuspendLayout();
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->loadUINetwork);
			this->groupBox3->Controls->Add(this->loadUIMemory);
			this->groupBox3->Controls->Add(this->loadUICpu);
			this->groupBox3->Location = System::Drawing::Point(514, 143);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(59, 57);
			this->groupBox3->TabIndex = 3;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Bilgisayar Yük Deðerleri";
			// 
			// loadUINetwork
			// 
			this->loadUINetwork->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->loadUINetwork->Location = System::Drawing::Point(68, 23);
			this->loadUINetwork->Name = L"loadUINetwork";
			this->loadUINetwork->Size = System::Drawing::Size(26, 153);
			this->loadUINetwork->TabIndex = 7;
			// 
			// loadUIMemory
			// 
			this->loadUIMemory->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->loadUIMemory->Location = System::Drawing::Point(38, 23);
			this->loadUIMemory->Name = L"loadUIMemory";
			this->loadUIMemory->Size = System::Drawing::Size(24, 153);
			this->loadUIMemory->TabIndex = 6;
			// 
			// loadUICpu
			// 
			this->loadUICpu->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->loadUICpu->Location = System::Drawing::Point(7, 23);
			this->loadUICpu->Name = L"loadUICpu";
			this->loadUICpu->Size = System::Drawing::Size(25, 153);
			this->loadUICpu->TabIndex = 4;
			// 
			// groupBox6
			// 
			this->groupBox6->BackColor = System::Drawing::SystemColors::ControlDark;
			this->groupBox6->Controls->Add(this->pcNodePanel);
			this->groupBox6->Location = System::Drawing::Point(2, 91);
			this->groupBox6->Name = L"groupBox6";
			this->groupBox6->Size = System::Drawing::Size(709, 352);
			this->groupBox6->TabIndex = 3;
			this->groupBox6->TabStop = false;
			this->groupBox6->Text = L"Federe Listesi";
			// 
			// pcNodePanel
			// 
			this->pcNodePanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pcNodePanel->Location = System::Drawing::Point(3, 16);
			this->pcNodePanel->Name = L"pcNodePanel";
			this->pcNodePanel->Size = System::Drawing::Size(703, 333);
			this->pcNodePanel->TabIndex = 1;
			this->pcNodePanel->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &LoadViewer::pcNodePanel_Paint);
			// 
			// lblMessage
			// 
			this->lblMessage->AutoSize = true;
			this->lblMessage->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->lblMessage->ForeColor = System::Drawing::Color::Red;
			this->lblMessage->Location = System::Drawing::Point(10, 63);
			this->lblMessage->Name = L"lblMessage";
			this->lblMessage->Size = System::Drawing::Size(47, 15);
			this->lblMessage->TabIndex = 4;
			this->lblMessage->Text = L"label1";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->textBox2);
			this->groupBox1->Controls->Add(this->button1);
			this->groupBox1->Controls->Add(this->textBox1);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->checkBoxLBSActive);
			this->groupBox1->Controls->Add(this->richTextBox1);
			this->groupBox1->Controls->Add(this->lblMessage);
			this->groupBox1->Controls->Add(this->numericUpDown1);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->trackBar1);
			this->groupBox1->Location = System::Drawing::Point(3, 2);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(698, 83);
			this->groupBox1->TabIndex = 5;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Random Federe Baþlatma Sýklýðý (10 sn)";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(22, 22);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(130, 13);
			this->label3->TabIndex = 11;
			this->label3->Text = L"Federe Baþlama Sýklýðý(sn)";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(170, 19);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(39, 20);
			this->textBox2->TabIndex = 10;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(339, 34);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(22, 23);
			this->button1->TabIndex = 9;
			this->button1->Text = L">>";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &LoadViewer::button1_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(296, 36);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(39, 20);
			this->textBox1->TabIndex = 8;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(217, 39);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(79, 13);
			this->label2->TabIndex = 7;
			this->label2->Text = L"Sim Zamaný(sn)";
			// 
			// checkBoxLBSActive
			// 
			this->checkBoxLBSActive->AutoSize = true;
			this->checkBoxLBSActive->Checked = true;
			this->checkBoxLBSActive->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBoxLBSActive->Location = System::Drawing::Point(127, 45);
			this->checkBoxLBSActive->Name = L"checkBoxLBSActive";
			this->checkBoxLBSActive->Size = System::Drawing::Size(70, 17);
			this->checkBoxLBSActive->TabIndex = 6;
			this->checkBoxLBSActive->Text = L"LBS Aktif";
			this->checkBoxLBSActive->UseVisualStyleBackColor = true;
			// 
			// richTextBox1
			// 
			this->richTextBox1->Location = System::Drawing::Point(367, 11);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(325, 69);
			this->richTextBox1->TabIndex = 5;
			this->richTextBox1->Text = L"";
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
			this->numericUpDown1->Location = System::Drawing::Point(304, 11);
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(45, 20);
			this->numericUpDown1->TabIndex = 2;
			this->numericUpDown1->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {50, 0, 0, 0});
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(216, 14);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(85, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Baþlama Olasýlýðý";
			// 
			// trackBar1
			// 
			this->trackBar1->Location = System::Drawing::Point(9, 19);
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(210, 45);
			this->trackBar1->TabIndex = 0;
			this->trackBar1->Visible = false;
			// 
			// timer1
			// 
			this->timer1->Interval = 1000;
			this->timer1->Tick += gcnew System::EventHandler(this, &LoadViewer::timer1_Tick);
			// 
			// LoadViewer
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(713, 447);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->groupBox6);
			this->Controls->Add(this->groupBox3);
			this->Name = L"LoadViewer";
			this->Text = L"LoadViewer";
			this->Load += gcnew System::EventHandler(this, &LoadViewer::LoadViewer_Load);
			this->groupBox3->ResumeLayout(false);
			this->groupBox6->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: 
		System::Void LoadViewer_Load(System::Object^  sender, System::EventArgs^  e);

	public:
		void ShowLoadViewer();
	public:
		ref struct PCNodeStruct : IComparable<PCNodeStruct^>
		{
			String^ ipAddress;
			LOAD_STATES pcState,cpuState,memState,cpuqState,netState;
			List<String^>^ fedList;
			double cpu_u,cpuq_u,mem_u,net_u;
			PCNodeStruct(){
				fedList = gcnew List<String^>();
				pcState = LOAD_STATES::ST_NORMAL;
			}
		public:
			virtual int CompareTo(PCNodeStruct^ y)
			{
				String^ xIp = this->ipAddress;
				String^ yIp = y->ipAddress;
				int intXIp = xIp->LastIndexOf('.');
				int intYIp = yIp->LastIndexOf('.');
				String^ xNodeStr = xIp->Substring(intXIp+1,xIp->Length-1-intXIp);
				String^ yNodeStr = yIp->Substring(intYIp+1,yIp->Length-1-intYIp);
				int xNode = Convert::ToInt32(xNodeStr);
				int yNode = Convert::ToInt32(yNodeStr);
				if(xNode > yNode)
					return 1;
				else if(xNode == yNode)
					return 0;
				else
					return -1;
			
			}
		}; 
		ref struct PCNodeFedList
		{
			String^ ipAddress;
			List<String^>^ fedList;
		};
		List<PCNodeStruct^>^ pcList;
		List<PCNodeFedList^>^ pcOldFedList;
		double cpuAvg,cpuLowThreshold,cpuHighThreshold;
		double cpuqAvg,cpuqLowThreshold,cpuqHighThreshold;
		double memAvg,memLowThreshold,memHighThreshold;
		double netAvg,netLowThreshold,netHighThreshold;
		bool drawMigration;
		String^ myIp;
		String^ migratingTargetIp;
		String^ migratingFederate;
		int simTime;
		public: bool simhasstarted;
		void Reset()
		{
			pcList->Clear();
			drawMigration = false;
		}
		void AddNode(String^ ipAddress,LOAD_STATES ls,List<String^>^ fedList)
		{
			PCNodeStruct^ pcNode = gcnew PCNodeStruct();
			pcNode->ipAddress = ipAddress;
			pcNode->pcState = ls;
			pcNode->fedList = fedList;
			pcList->Add(pcNode);
		}

		void AddNode(PCNodeStruct^ pcNode)
		{
			pcList->Add(pcNode);
		}
		void RemoveNode(String^ ipAddress)
		{
			for each(PCNodeStruct^ node in pcList)
			{
				if(node->ipAddress == ipAddress){
					pcList->Remove(node);
					break;
				}
			}
		}

		void RefreshNode(String^ ipAddress, LOAD_STATES ls)
		{
			for each(PCNodeStruct^ node in pcList)
			{
				if(node->ipAddress == ipAddress){
					node->pcState = ls; break;
				}
			}
		}
		
		void RefreshGUI()
		{
			this->lblMessage->Text = "";
			this->pcNodePanel->Controls->Clear();
			int px = 0;
			int py = 0;
			int pw = 155;
			int ph = 155;
			pcList->Sort();
			for each(PCNodeStruct^ node in pcList)
			{
				LoadManager::PCNodeUI^ pcNode = gcnew LoadManager::PCNodeUI();
				pcNode->BackColor = System::Drawing::SystemColors::Control;
				pcNode->Location = System::Drawing::Point(px, py);
				pcNode->Name = node->ipAddress;
				pcNode->Size = System::Drawing::Size(pw, ph);
				pcNode->label1->Text = node->ipAddress;
				//PCNode Renk
				if(node->pcState == LOAD_STATES::ST_HIGH)
					pcNode->panelLS->BackColor = Color::Red;
				else if(node->pcState == LOAD_STATES::ST_NORMAL)
					pcNode->panelLS->BackColor = Color::Green;
				else if(node->pcState == LOAD_STATES::ST_LOW)
					pcNode->panelLS->BackColor = Color::Yellow;
				else//(node->ls == LOAD_STATES::ST_IDLE)
					pcNode->panelLS->BackColor = Color::White;

				for each(String^ fed in node->fedList){
					pcNode->lvFederates->Items->Add(fed);
				}
				//Check4MigrationOldList(node);
				AddNodeUI(pcNode);
				int nodeWidth = pcNodePanel->Width;
				if((nodeWidth -pw) > (px + pw+10))
					px += pw+10;
				else{
					px = 0;
					py += ph + 10;
				}
			}

			//pcOldFedList->Clear();
			//for each(PCNodeStruct^ node in pcList)
			//	Add2OldList(node->ipAddress,node->fedList);
		}
		delegate void AddControlHandler(LoadManager::PCNodeUI^ pcNode);
		void AddNodeUI(LoadManager::PCNodeUI^ pcNode)
		{
			if(this->pcNodePanel->InvokeRequired)
			{
				//AddControlDelegate^ d = gcnew AddControlDelegate(AddNodeUI);
				cli::array<Object^>^ oa = gcnew cli::array<Object^>(1);
				oa[0] = pcNode;
				this->pcNodePanel->Invoke(gcnew AddControlHandler(this,&LoadViewer::AddNodeUI),oa);
			}else
				this->pcNodePanel->Controls->Add(pcNode);
		}
		void DrawMigration(String^ myIp, String^ migratingTargetIp, String^ migratingFederate)
		{
			drawMigration = true;
			this->migratingTargetIp = migratingTargetIp;
			this->migratingFederate = migratingFederate;
			this->myIp = myIp;
			this->lblMessage->Text = migratingFederate +" is migrating to '" +  migratingTargetIp + "' !!!";
			this->richTextBox1->Text += this->lblMessage->Text + "\n";
		}
		void Add2OldList(String^ node, List<String^>^ nodeFedList)
		{
			PCNodeFedList^ pcNodeFedList = gcnew PCNodeFedList();
			pcNodeFedList->ipAddress = node;
			pcNodeFedList->fedList = nodeFedList;

			pcOldFedList->Add(pcNodeFedList);

		}
		void Check4MigrationOldList(PCNodeStruct^ node)
		{
			List<String^>^ oldFedList;
			for each(PCNodeFedList^ pcNode in pcOldFedList){
				if(pcNode->ipAddress == node->ipAddress){
					oldFedList = pcNode->fedList;
					break;
				}
			}
			if(oldFedList == nullptr)
				return;
			
			for each(String^ fed in oldFedList){				
				String^ newAddress = Check4OtherNode(fed);
				if(newAddress == "")	//Federe silinmiþ
					this->richTextBox1->Text += fed + " silinmiþ! \n";
				else if(newAddress == node->ipAddress)
					continue;
				else	//Buraya taþýnmýþ;
					this->richTextBox1->Text += fed +" is migrated to '" +  newAddress + "' !!!  \n";	
			}
		}
		String^ Check4OtherNode(String^ fed)
		{
			for each(PCNodeStruct^ node in pcList)
			{
				if(node->fedList->Contains(fed))
					return node->ipAddress;
			}
			return "";
		}
		System::Void pcNodePanel_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
		{
			//Graphics^ graphics = e->Graphics;
			//Pen^ pen = gcnew Pen(Color::Red,5);
			//graphics->DrawLine(pen,20,20,160,160);
			//if(!drawMigration)
			//	return;
			//
			//int x1,y1,x2,y2;
			//for each(PCNodeStruct^ node in pcList){
			//	cli::array<System::Windows::Forms::Control^>^ foundUI;
			//	if(node->ipAddress == this->migratingFederate){
			//			foundUI = this->pcNodePanel->Controls->Find(node->ipAddress,true);
			//			System::Windows::Forms::Control^ c = foundUI[0];
			//			x1 = c->Location.X;
			//			y1 = c->Location.Y;
			//	}
			//	if(node->ipAddress == this->migratingTargetIp){
			//			foundUI =this->pcNodePanel->Controls->Find(node->ipAddress,true);
			//			System::Windows::Forms::Control^ c = foundUI[0];
			//			x2 = c->Location.X;
			//			y2 = c->Location.Y;
			//	}
			//}
			//graphics->DrawLine(pen,20,20,160,160);
			//graphics->DrawLine(pen,x1,y1,x2,y2);
				//graphics->DrawString(rgbStr,fnt,brsh,x-r+(r/2)+5,y-r+(r/2)+5);
			
			
		}
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 simTime = Convert::ToInt32(textBox1->Text);
			 timer1->Enabled = true;
			 timer1->Start();
			 simhasstarted = true;
			 trackBar1->Value = (Convert::ToInt32(textBox2->Text)/10);
		 }
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(simTime == 0){
				 timer1->Stop();
				 trackBar1->Value = 0;
				 timer1->Enabled = false;
				 simhasstarted = false;
			 }
			 else{
				 simTime--;
				 textBox1->Text = simTime.ToString();
			 }
		 }
};

}
#endif