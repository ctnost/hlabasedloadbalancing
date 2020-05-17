#include "StdAfx.h"
#include "LoadViewer.h"
#include "LoadManager.h"
using namespace LoadManager;
using namespace System::Drawing::Drawing2D;

System::Void LoadViewer::LoadViewer_Load(System::Object^  sender, System::EventArgs^  e)
{
	RandomFederateStarter^ fs = gcnew RandomFederateStarter(this->trackBar1,this->numericUpDown1);
	ThreadStart^ threadDelegate1 = gcnew ThreadStart( fs, &RandomFederateStarter::StartRandomFederateStarter );
	Thread^ newThread1 = gcnew Thread( threadDelegate1 );
	newThread1->Start();
	pcOldFedList = gcnew List<PCNodeFedList^>();
}

void LoadViewer::ShowLoadViewer()
{
	this->Show();
}