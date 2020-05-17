#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Data;
using namespace System::Data::SqlClient;

using namespace LoadHistory;

#include "WSN.h"
#include "Defaults.h"
#include "StringOperation.h"

namespace WSNGUI {

	/// <summary>
	/// Summary for WSNBoard
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class WSNBoard : public System::Windows::Forms::Form
	{
	public:
		WSNBoard(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~WSNBoard()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	protected: 
	private: System::Windows::Forms::GroupBox^  groupBox1;
	public: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::DataGridView^  dgvWS;

	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column3;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column4;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ListView^  listView1;

	public: 
	private: System::ComponentModel::IContainer^  components;

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
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->dgvWS = (gcnew System::Windows::Forms::DataGridView());
			this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column4 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->listView1 = (gcnew System::Windows::Forms::ListView());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dgvWS))->BeginInit();
			this->groupBox2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox1->Location = System::Drawing::Point(3, 16);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(594, 381);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &WSNBoard::pictureBox1_Paint);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->pictureBox1);
			this->groupBox1->Location = System::Drawing::Point(228, 3);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(600, 400);
			this->groupBox1->TabIndex = 1;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Wireless Sensor Network";
			// 
			// timer1
			// 
			this->timer1->Interval = 20;
			this->timer1->Tick += gcnew System::EventHandler(this, &WSNBoard::timer1_Tick);
			// 
			// pictureBox2
			// 
			this->pictureBox2->Location = System::Drawing::Point(835, 12);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(39, 388);
			this->pictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox2->TabIndex = 2;
			this->pictureBox2->TabStop = false;
			// 
			// dgvWS
			// 
			this->dgvWS->AllowUserToAddRows = false;
			this->dgvWS->AllowUserToDeleteRows = false;
			this->dgvWS->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dgvWS->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(4) {this->Column1, this->Column2, 
				this->Column3, this->Column4});
			this->dgvWS->Location = System::Drawing::Point(8, 9);
			this->dgvWS->Name = L"dgvWS";
			this->dgvWS->ReadOnly = true;
			this->dgvWS->RowHeadersVisible = false;
			this->dgvWS->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dgvWS->Size = System::Drawing::Size(214, 388);
			this->dgvWS->TabIndex = 3;
			this->dgvWS->CellClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &WSNBoard::dgvWS_CellClick);
			// 
			// Column1
			// 
			this->Column1->HeaderText = L"Name";
			this->Column1->Name = L"Column1";
			this->Column1->ReadOnly = true;
			// 
			// Column2
			// 
			this->Column2->HeaderText = L"SC";
			this->Column2->Name = L"Column2";
			this->Column2->ReadOnly = true;
			this->Column2->Width = 30;
			// 
			// Column3
			// 
			this->Column3->HeaderText = L"RC";
			this->Column3->Name = L"Column3";
			this->Column3->ReadOnly = true;
			this->Column3->Width = 30;
			// 
			// Column4
			// 
			this->Column4->HeaderText = L"State";
			this->Column4->Name = L"Column4";
			this->Column4->ReadOnly = true;
			this->Column4->Width = 50;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->groupBox3);
			this->groupBox2->Controls->Add(this->listView1);
			this->groupBox2->Location = System::Drawing::Point(8, 403);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(863, 298);
			this->groupBox2->TabIndex = 4;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"PC Nodes";
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->label1);
			this->groupBox3->Location = System::Drawing::Point(573, 13);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(284, 279);
			this->groupBox3->TabIndex = 2;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"PC Info";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(7, 20);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(67, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"IP Address : ";
			// 
			// listView1
			// 
			this->listView1->Location = System::Drawing::Point(6, 19);
			this->listView1->Name = L"listView1";
			this->listView1->Size = System::Drawing::Size(561, 273);
			this->listView1->TabIndex = 1;
			this->listView1->UseCompatibleStateImageBehavior = false;
			this->listView1->SelectedIndexChanged += gcnew System::EventHandler(this, &WSNBoard::listView1_SelectedIndexChanged);
			// 
			// WSNBoard
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(883, 712);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->dgvWS);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->groupBox1);
			this->Name = L"WSNBoard";
			this->Text = L"WSNBoard";
			this->Load += gcnew System::EventHandler(this, &WSNBoard::WSNBoard_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->groupBox1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dgvWS))->EndInit();
			this->groupBox2->ResumeLayout(false);
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private:
		List<String^> ^selectedWSList;
		LoadTracer^ dao;
		ImageList^ imgList;

	private: System::Void pictureBox1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
		 {
			 Graphics^ graphics = e->Graphics;
			 map<RTI::ObjectHandle,WirelessSensor*>::iterator it;
			 int maxPow = MaxPower;
			 for (it = WSN::wSensors.begin(); it != WSN::wSensors.end(); it++)
			 {
				 int x = it->second->posx;
				 int y = it->second->posy;
				 int r = MaxRadius;
				 int pow = it->second->power;
				 int red = (255*(1+maxPow-pow))/100;
				 int g = (255*pow)/100;
				 int b = 0;
				 if(red < 0) red = 0; if(red > 255) red = 255;
				 if(g < 0) g = 0; if(g > 255) g = 255;
				 if(b < 0) b = 0; if(b > 255) b = 255;
				 String^ rgbStr = pow.ToString();//red.ToString() +" " + g.ToString() + " " + b.ToString();
				 graphics->FillEllipse(gcnew SolidBrush(Color::FromArgb(red,g,b)),x-r,y-r,r*2,r*2);
				 System::Drawing::Font ^fnt = gcnew System::Drawing::Font("Arial", 14);
				 System::Drawing::SolidBrush ^brsh = gcnew System::Drawing::SolidBrush(Color::Black);
				 graphics->DrawString(rgbStr,fnt,brsh,x-r+(r/2)+5,y-r+(r/2)+5);
				 if(selectedWSList->Count > 0){
					 for each(String^ selectedWS in selectedWSList){
						String^ fedName = gcnew String(it->second->name.c_str());
						if(fedName == selectedWS)
						{
							graphics->DrawEllipse(gcnew Pen(Color::FromArgb(0,0,0),2),x-r,y-r,r*2,r*2);
						}
					 }
				 }
				 
			 }
			 updateDGVList();
		 }
	private: void updateDGVList()
			 {
				 for each(DataGridViewRow^ dgvRow in dgvWS->Rows)
				 {
					 DataGridViewTextBoxCell^ dgvCell = (DataGridViewTextBoxCell^)dgvRow->Cells[3];
					 dgvCell->Value = "Dead";
				 }
				 map<RTI::ObjectHandle,WirelessSensor*>::iterator it;
				 for (it = WSN::wSensors.begin(); it != WSN::wSensors.end(); it++)
				 {
					 String^ wsName = StringOperation::toSystemString(it->second->name);
					 bool wsFound = false;
					 for each(DataGridViewRow^ dgvRow in dgvWS->Rows)
					 {
						 DataGridViewTextBoxCell^ dgvCell = (DataGridViewTextBoxCell^)dgvRow->Cells[0];
						 if((String^)dgvCell->Value == wsName){
							 DataGridViewTextBoxCell^ dgvCell1 = (DataGridViewTextBoxCell^)dgvRow->Cells[1];
							 DataGridViewTextBoxCell^ dgvCell2 = (DataGridViewTextBoxCell^)dgvRow->Cells[2];
							 DataGridViewTextBoxCell^ dgvCell3 = (DataGridViewTextBoxCell^)dgvRow->Cells[3];
							 dgvCell1->Value = it->second->sc;
							 dgvCell2->Value = it->second->rc;
							 dgvCell3->Value = "Alive";
							 wsFound = true;
							 break;
						 }

					 }
					 if(!wsFound)
						 dgvWS->Rows->Add(wsName,"0","0","Alive");
				 }
			 }
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) 
			 {
				 pictureBox1->Refresh();
				
				 List<List<String^>^>^ pcList = dao->GetActivePCInfo();
				 if(pcList->Count != listView1->Items->Count)
				 {	 
					 listView1->Items->Clear();
					 int i = 0;
					 for each(List<String^>^ pc in pcList)
					 {
						ListViewItem^ lvItem = gcnew ListViewItem();
						lvItem->ImageIndex = 0;
						lvItem->Text = pc[1];
						listView1->Items->Add(lvItem);
						i++;
					 }
				 }
				
			 }
	private: System::Void WSNBoard_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->groupBox1->Size = System::Drawing::Size(MapWidth,MapHeight);
				 timer1->Start();
				 timer1->Enabled = true;
			
				 imgList = gcnew ImageList();
				 imgList->ImageSize = Drawing::Size(64,64);
				 imgList->Images->Add(Image::FromFile("Icon_pc.png"));
				 listView1->LargeImageList = imgList;
				 selectedWSList = gcnew List<String^>();

				 dao = gcnew LoadTracer();
				 dao->ConnectToDb();
				 dao->InsertFederation("WSN");
			 }
	public: void StartForm()
			 {
				 this->ShowDialog();
			 }

	private: System::Void dgvWS_CellClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
		 {
			 DataGridViewRow^ dgvRow = dgvWS->SelectedRows[0];
			 DataGridViewTextBoxCell^ dgvCell = (DataGridViewTextBoxCell^)dgvRow->Cells[0];
			 String^ selectedWSName = (String^)dgvCell->Value;
			 selectedWSList->Clear();
			 selectedWSList->Add(selectedWSName);
		 }
	private: System::Void listView1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 //if(listView1->SelectedItems->Count > 0)
			 //{
				// ListViewItem^ lwItem = listView1->SelectedItems[0];
				// List<String^>^ lstFed = dao->GetFederateListByIp(lwItem->Text);
				// selectedWSList->Clear();
				// for each(String^ fed in lstFed)
				// {
				//	selectedWSList->Add(fed);
				// }
			 //}
		 }
};
}
