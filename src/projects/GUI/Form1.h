#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "RandomFederateStarter.h";
#include <ctime>
#include <list>
//#include <windows.h>
#include <Windows.h>
using namespace System::IO;
using namespace System::Text;
using namespace System::Net::Sockets;
using namespace System::Runtime::InteropServices;
using namespace System::IO::Pipes;
using namespace System::Security;
using namespace System::Threading;
//#include "RTI.hh"

using std::ostream;
using namespace std;

namespace GUI {

	using namespace System;
	using namespace System::Text;
	using namespace System::IO;
	using namespace System::IO::Pipes;
	using namespace System::Security::Principal;
	using namespace System::Diagnostics;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	struct MigrationStatus
	{
		int migrationPoint;
		string ip;
	};
	struct SortMigrationStatus : public std::binary_function<MigrationStatus*, MigrationStatus*, bool>
	{
		bool operator()(MigrationStatus* left, MigrationStatus* right) const
		{
			return(left->migrationPoint < right->migrationPoint);
		};
	};
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
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
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
			  TcpListener^ serverSocket;

	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	public: System::Windows::Forms::TrackBar^  trackBar1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Button^  button2;
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
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(232, 48);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(167, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Federe Taþý";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(77, 47);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(134, 20);
			this->textBox1->TabIndex = 2;
			this->textBox1->Text = L"WS-";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(3, 50);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(67, 13);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Federe Ýsmi :";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(6, 25);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(55, 13);
			this->label2->TabIndex = 4;
			this->label2->Text = L"IP Adresi :";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(77, 22);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(134, 20);
			this->textBox2->TabIndex = 5;
			this->textBox2->Text = L"192.168.2.1";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->textBox3);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->textBox2);
			this->groupBox1->Controls->Add(this->button1);
			this->groupBox1->Controls->Add(this->textBox1);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Location = System::Drawing::Point(12, 7);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(420, 89);
			this->groupBox1->TabIndex = 6;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Federe Taþýma";
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(265, 22);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(134, 20);
			this->textBox3->TabIndex = 7;
			this->textBox3->Text = L"192.168.2.1";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(217, 25);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(42, 13);
			this->label4->TabIndex = 6;
			this->label4->Text = L"Hedef :";
			this->label4->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->trackBar1);
			this->groupBox2->Controls->Add(this->label3);
			this->groupBox2->Location = System::Drawing::Point(12, 102);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(420, 73);
			this->groupBox2->TabIndex = 7;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Federe Baþlatma";
			this->groupBox2->Enter += gcnew System::EventHandler(this, &Form1::groupBox2_Enter);
			// 
			// trackBar1
			// 
			this->trackBar1->Location = System::Drawing::Point(6, 19);
			this->trackBar1->Maximum = 100;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(408, 45);
			this->trackBar1->SmallChange = 10;
			this->trackBar1->TabIndex = 2;
			this->trackBar1->TickFrequency = 10;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(6, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(139, 13);
			this->label3->TabIndex = 0;
			this->label3->Text = L"Federe Baþlatma Sýklýðý (sn):";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(326, 98);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 8;
			this->button2->Text = L"button2";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(444, 203);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private:
		string ByteArrayToStr(array<Byte>^ byteArray)
		{
			int size = byteArray.Length;
			IntPtr pnt = Marshal::AllocHGlobal(size + 1);
			char* chararray = (char*)pnt.ToPointer();
			try
			{
				Marshal::Copy(byteArray, 0, pnt, size);
				*(chararray + size) = 0;
				return string(chararray);
			}
			finally
			{
				Marshal::FreeHGlobal(pnt);
			}
		}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 //DateTime dt = System::DateTime::Now;
			 String^ ms = "";
			 for (int i = 0; i < 500; i++)
			 {
				 const long double sysTime = time(0);
				 const long double sysTimeMs = sysTime*1000;
				 Console::WriteLine(sysTimeMs.ToString());
				 ms += sysTimeMs.ToString()+"\n";
			 }
	//		 MessageBox::Show(ms);
	
			cli::array<String^> ^ pipeList = System::IO::Directory::GetFiles("\\\\.\\pipe\\");
			for (int i = 0; i < pipeList->Length; i++)
			{
				String^ pipe = pipeList[i];
				if(pipe->Contains("WS"))
				{
					int indexx = pipe->IndexOf("WS");
					String^ fedNamex = pipe->Substring(indexx);
				}
			}
		/*	 NamedPipeClientStream ^pipeClient = gcnew 
				 NamedPipeClientStream("192.168.2.3","WS112",
				 PipeDirection::InOut, PipeOptions::None, TokenImpersonationLevel::Impersonation);
			 PipeSecurity^ ps = pipeClient->GetAccessControl();
			 PipeAccessRule^ par = gcnew PipeAccessRule("localuser",
				 PipeAccessRights::FullControl,AccessControl::AccessControlType::Allow);
			 ps->AddAccessRule(par);
			 pipeClient->SetAccessControl(ps);
			 pipeClient->Connect();
			 StreamString^ ss = gcnew StreamString(pipeClient);
			 ss->WriteString("192.168.2.4");
			 pipeClient->Close();*/
			 int requestCount = 0;
			 TcpClient^ clientSocket = gcnew TcpClient();
			 //clientSocket = serverSocket->AcceptTcpClient();
			 requestCount = 0;
			 try
			 {
				 clientSocket->Connect(textBox2->Text,8887);
				 requestCount = requestCount + 1;
				 NetworkStream^ networkStream = clientSocket->GetStream();
				 //StreamWriter^ sWriter = gcnew StreamWriter(networkStream);
				 String^ ipAddress = textBox3->Text;
				 String^ message = ipAddress +"|"+textBox1->Text;
				 array<Byte>^ data = Encoding::Unicode->GetBytes(message);
				 networkStream->Write(data, 0, data->Length);
				 networkStream->Flush();
			 }
			 catch (Exception^ ex)
			 {
				ex->ToString();
			 }
			 
			 clientSocket->Close();
		 }
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) 
			 {

				 list<MigrationStatus*> migStatus;	
				 MigrationStatus* m1 = new MigrationStatus();
				 m1->ip = "a"; m1->migrationPoint = 4;
				 MigrationStatus* m2 = new MigrationStatus();
				 m2->ip = "a"; m2->migrationPoint = 3;
				 MigrationStatus* m3 = new MigrationStatus();
				 m3->ip = "a"; m3->migrationPoint = 5;
				 //
				 migStatus.push_back(m1);
				 migStatus.push_back(m2);
				 migStatus.push_back(m3);
				 migStatus.pop_front();
				 migStatus.sort(SortMigrationStatus());
				 //

				Control::CheckForIllegalCrossThreadCalls = false;
				RandomFederateStarter^ fs = gcnew RandomFederateStarter(this->trackBar1);
				ThreadStart^ threadDelegate1 = gcnew ThreadStart( fs, &RandomFederateStarter::StartRandomFederateStarter );
				Thread^ newThread1 = gcnew Thread( threadDelegate1 );
				//System::Diagnostics::Debugger::Break();
				newThread1->Start();
			 }
private: System::Void groupBox2_Enter(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 String^ ss = System::DateTime::Now.ToLongTimeString();
			 SYSTEMTIME st;
			 st.wDay = System::DateTime::Now.Day;
			 st.wHour =  System::DateTime::Now.Hour;
			 st.wMilliseconds =  System::DateTime::Now.Millisecond;
			 st.wMinute =  System::DateTime::Now.Minute+2;
			 st.wMonth =  System::DateTime::Now.Month;
			 st.wSecond =  System::DateTime::Now.Second;
			 st.wYear =  System::DateTime::Now.Year;
			 SetLocalTime(&st);

		 }
};
	// Defines the data protocol for reading and writing strings on our stream 
	public ref class StreamString
{
private:
    Stream^ ioStream;
    UnicodeEncoding^ streamEncoding;

public:
    StreamString(Stream^ ioStream)
    {
        this->ioStream = ioStream;
        streamEncoding = gcnew UnicodeEncoding();
    }

    String^ ReadString()
    {
        int len;

        len = ioStream->ReadByte() * 256;
        len += ioStream->ReadByte();
        array<Byte>^ inBuffer = gcnew array<Byte>(len);
        ioStream->Read(inBuffer, 0, len);

        return streamEncoding->GetString(inBuffer);
    }

    int WriteString(String^ outString)
    {
        array<Byte>^ outBuffer = streamEncoding->GetBytes(outString);
        int len = outBuffer->Length;
        if (len > UInt16::MaxValue)
        {
            len = (int)UInt16::MaxValue;
        }
        ioStream->WriteByte((Byte)(len / 256));
        ioStream->WriteByte((Byte)(len & 255));
        ioStream->Write(outBuffer, 0, len);
        ioStream->Flush();

        return outBuffer->Length + 2;
    }
};
}

