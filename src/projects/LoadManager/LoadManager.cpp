#include "LoadManager.h"
#include "FederateTrigger.h"
#include "FederateStarter.h"
#include "ActivePCLogger.h"

using namespace LoadManager;
using namespace LoadHistory;

LoadController::LoadController(LoadViewer^ loadv)
{
	loadMM = gcnew LoadMonitorManager();

	dao = gcnew LoadTracer();
	dao->ConnectToDb();
	rlm = gcnew RemoteLoadManager(dao);
	lViewer = loadv;
	activeNodeList = gcnew List<String^>();
	InitLoadViewer();
}

void LoadController::InitLoadViewer()
{
	lViewer->loadUICpu->lblLoad->Text = "CPU";
	//lViewer->loadUICpuQueue->lblLoad->Text = "CPU Queue";
	lViewer->loadUIMemory->lblLoad->Text = "Memory";
	lViewer->loadUINetwork->lblLoad->Text = "Network";
	loadUICpuLoadGraphic = lViewer->loadUICpu->pnlLoad->CreateGraphics();
	//loadUICpuQueueLoadGraphic = lViewer->loadUICpuQueue->pnlLoad->CreateGraphics();
	loadUIMemoryLoadGraphic = lViewer->loadUIMemory->pnlLoad->CreateGraphics();
	loadUINetworkLoadGraphic = lViewer->loadUINetwork->pnlLoad->CreateGraphics();
	
}

void LoadController::StartMonitoring()
{
	FederateTrigger^ fedTrigger = gcnew FederateTrigger();
	ThreadStart^ threadDelegate1 = gcnew ThreadStart( fedTrigger, &FederateTrigger::StartFederateTrigger );
	Thread^ newThread1 = gcnew Thread( threadDelegate1 );
	newThread1->Start();

	FederateStarter^ fedStarter = gcnew FederateStarter();
	ThreadStart^ threadDelegate2 = gcnew ThreadStart( fedStarter, &FederateStarter::StartFederateListener );
	Thread^ newThread2 = gcnew Thread( threadDelegate2 );
	newThread2->Start();

	ActivePCLogger^ pcLogger = gcnew ActivePCLogger();
	ThreadStart^ threadDelegate3 = gcnew ThreadStart( pcLogger, &ActivePCLogger::LogActivePC );
	Thread^ newThread3 = gcnew Thread( threadDelegate3 );
	newThread3->Start();

	//ToDo:D1,2
	loadMM->StartMonitoring();
	TraceLoad();
}

void LoadController::PauseMonitoring()
{
	//Pause
}

void LoadController::StopMonitoring()
{
	//Stop
}

void LoadController::DrawUIByType(LOAD_TYPES lType,double usage,double avg,double highThreshold, double lowThreshold,LOAD_STATES state)
{
	System::Drawing::Graphics^ graphic; 
	System::Windows::Forms::Panel^ panel;
	System::Windows::Forms::Panel^ panelLoadState;
	switch(lType)
	{
	case LOAD_TYPES::CPU :
		panel = lViewer->loadUICpu->pnlLoad;
		graphic = loadUICpuLoadGraphic;
		panelLoadState = lViewer->loadUICpu->pnlLoadState;
		break ;
	case LOAD_TYPES::CPUQ :
		/*panel = lViewer->loadUICpuQueue->pnlLoad;
		graphic = loadUICpuQueueLoadGraphic;
		panelLoadState = lViewer->loadUICpuQueue->pnlLoadState;*/
		break ;
	case LOAD_TYPES::MEM :
		panel = lViewer->loadUIMemory->pnlLoad;
		graphic = loadUIMemoryLoadGraphic;
		panelLoadState = lViewer->loadUIMemory->pnlLoadState;
		break ;
	case LOAD_TYPES::BW :
		panel = lViewer->loadUINetwork->pnlLoad;
		graphic = loadUINetworkLoadGraphic;
		panelLoadState = lViewer->loadUINetwork->pnlLoadState;
		break ;
	}

	switch (state)
	{
	case LOAD_STATES::ST_IDLE:
		panelLoadState->BackColor = panelLoadState->DefaultBackColor;
		break;
	case LOAD_STATES::ST_LOW:
		panelLoadState->BackColor = System::Drawing::Color::Yellow;
		break;
	case LOAD_STATES::ST_HIGH:
		panelLoadState->BackColor = System::Drawing::Color::Red;
		break;
	default:
		panelLoadState->BackColor = System::Drawing::Color::Green;
	}

	graphic->Clear(panel->DefaultBackColor);
	Single coordinateY = panel->Height - ((usage)/100.0) * (panel->Height) ;
	Pen ^pen ;
	for (int i = panel->Height ; i >= coordinateY ; i-=1)
	{
		Single green = ((double)i/(double)panel->Height) * 255 ;	
		pen = gcnew Pen (::Color::FromArgb (255, (int)green, 0 ),1);
		graphic->DrawLine(pen, PointF (0.0f, i), PointF (panel->Width, i ));
		delete pen ;
	}

	if(highThreshold > 100)
		highThreshold = 100;
	if(lowThreshold < 0)
		lowThreshold = 0;

	Single coordinateYHT = panel->Height - ((highThreshold)/100.0) * (panel->Height);
	Pen ^penHT = gcnew Pen (::Color::Red,2);
	array<float>^ dashValues = { 1,2 };
	penHT->DashPattern = dashValues;
	graphic->DrawLine(penHT, PointF (0.0f, coordinateYHT), PointF (panel->Width, coordinateYHT ));

	Single coordinateYLT = panel->Height - ((lowThreshold)/100.0) * (panel->Height);
	Pen ^penLT = gcnew Pen (::Color::Black,2);
	penLT->DashPattern = dashValues;
	graphic->DrawLine(penLT, PointF (0.0f, coordinateYLT), PointF (panel->Width, coordinateYLT ));

	System::Drawing::Font ^fnt = gcnew System::Drawing::Font("Arial", 9);
	System::Drawing::SolidBrush ^brsh = gcnew System::Drawing::SolidBrush(Color::Black);
	graphic->DrawString(" %"+usage.ToString(),fnt,brsh,0,coordinateY-15);
}

void LoadController::TraceLoad()
{
	String^ myIp = loadMM->GetIP();
	int waitTime = LOAD_PERIOD * LOAD_NUMBER;
	rlm->rns->ipAddress = myIp;
	dao->UpdateMigrationTable(0);
	bool firstRun = true;
	while (true)
	{
		lViewer->Reset();
		List<String^>^ activePCList = dao->GetActivePCList();
		activePCList->Remove(myIp);
		rlm->NodeList  = activePCList;
		double cpu_u = loadMM->GetUtilization(LOAD_TYPES::CPU);
		double cpu_q = loadMM->GetUtilization(LOAD_TYPES::CPUQ);
		double mem_u = loadMM->GetUtilization(LOAD_TYPES::MEM);
		double bw_u = loadMM->GetUtilization(LOAD_TYPES::BW);
		rlm->rns->utilizations = cpu_u+" "+cpu_q+" "+mem_u+" "+bw_u;
		List<String^>^ remoteUtilizations = rlm->GetRemoteUtilizations();
		List<double>^ lst_cpu_u = gcnew List<double>();
		List<double>^ lst_cpu_q = gcnew List<double>();
		List<double>^ lst_mem_u = gcnew List<double>();
		List<double>^ lst_bw_u = gcnew List<double>();
		lst_cpu_u->Add(cpu_u);
		lst_cpu_q->Add(cpu_q);
		lst_mem_u->Add(mem_u);
		lst_bw_u->Add(bw_u);
		List<LoadViewer::PCNodeStruct^>^ pcList = gcnew List<LoadViewer::PCNodeStruct^>();
		for each(String^ node_u in remoteUtilizations){
			cli::array<String^>^ utils = node_u->Split(' ');
			double r_cpu_u = Convert::ToDouble(utils[0]);
			double r_cpu_q = Convert::ToDouble(utils[1]);
			double r_mem_u = Convert::ToDouble(utils[2]);
			double r_bw_u = Convert::ToDouble(utils[3]);
			String^ nodeIP = utils[4];
			List<String^>^ remoteFederates = rlm->GetRemoteFederates(nodeIP);
			lst_cpu_u->Add(r_cpu_u);
			lst_cpu_q->Add(r_cpu_q);
			lst_mem_u->Add(r_mem_u);
			lst_bw_u->Add(r_bw_u);
			LoadViewer::PCNodeStruct^ pcNodeSt = gcnew LoadViewer::PCNodeStruct();
			pcNodeSt->ipAddress = nodeIP;
			pcNodeSt->fedList = remoteFederates;
			pcNodeSt->cpu_u = r_cpu_u;
			pcNodeSt->cpuq_u = r_cpu_q;
			pcNodeSt->mem_u = r_mem_u;
			pcNodeSt->net_u = r_bw_u;
			pcList->Add(pcNodeSt);
		}

		//Cpu Threshold
		double cpuAvg = AvgList(lst_cpu_u);
		double cpuLowThreshold = cpuAvg * LOAD_LT;
		double cpuHighThreshold = cpuAvg * LOAD_HT;

		//Cpu Queue Threshold
		double cpuqAvg = AvgList(lst_cpu_q);
		double cpuqLowThreshold = cpuqAvg * LOAD_LT;
		double cpuqHighThreshold = cpuqAvg * LOAD_HT;

		//Memory Threshold
		double memAvg = AvgList(lst_mem_u);
		double memLowThreshold = memAvg * LOAD_LT;
		double memHighThreshold = memAvg * LOAD_HT;

		//Network Threshold
		double netAvg = AvgList(lst_bw_u);
		double netLowThreshold = netAvg * LOAD_LT;
		double netHighThreshold = netAvg * LOAD_HT;

		//Console::WriteLine("Cpu : " + cpuAvg + " , " + cpuLowThreshold+ " , " + cpuHighThreshold + " = \t" + cpu_u);
		//Console::WriteLine("CpuQ : " + cpuqAvg + " , " + cpuqLowThreshold+ " , " + cpuqHighThreshold + " = \t" + cpu_q);
		//Console::WriteLine("Mem : " + memAvg + " , " + memLowThreshold+ " , " + memHighThreshold + " = \t" + mem_u );
		//Console::WriteLine("Net : " + netAvg + " , " + netLowThreshold+ " , " + netHighThreshold + " = \t" + bw_u);
		LOAD_STATES cpuState, memState, bwState, cpuqState, pcState;

		cpuState = GetLoadState(cpu_u,cpuHighThreshold,cpuLowThreshold);
		cpuqState = GetLoadState(cpu_q,cpuqHighThreshold,cpuqLowThreshold);
		memState = GetLoadState(mem_u,memHighThreshold,memLowThreshold);
		bwState = GetLoadState(bw_u,netHighThreshold,netLowThreshold);
		//DrawUIByType(LOAD_TYPES::CPU,cpu_u,cpuAvg,cpuHighThreshold,cpuLowThreshold,cpuState);
		//DrawUIByType(LOAD_TYPES::CPUQ,cpu_q,cpuqAvg,cpuqHighThreshold,cpuqLowThreshold,cpuqState);
		//DrawUIByType(LOAD_TYPES::MEM,mem_u,memAvg,memHighThreshold,memLowThreshold,memState);
		//DrawUIByType(LOAD_TYPES::BW,bw_u,netAvg,netHighThreshold,netLowThreshold,bwState);

		pcState = GetPCState(cpuState,cpuqState,memState,bwState,cpu_u,cpu_q,mem_u,bw_u);
		rlm->NodeState = pcState;

		LoadViewer::PCNodeStruct^ pcNodeStruct = gcnew LoadViewer::PCNodeStruct();
		pcNodeStruct->ipAddress = myIp;
		pcNodeStruct->fedList = rlm->rns->GetActiveFedList();
		pcNodeStruct->cpuState = cpuState;;
		pcNodeStruct->cpuqState = cpuqState;
		pcNodeStruct->memState = memState;
		pcNodeStruct->netState = bwState;
		pcNodeStruct->pcState = pcState;
		lViewer->AddNode(pcNodeStruct);
		bool lowLoadedNode = false;
		List<LoadViewer::PCNodeStruct^>^ lowLoadedNodeList = gcnew List<LoadViewer::PCNodeStruct^>();
		for each(LoadViewer::PCNodeStruct^ pcListItem in pcList){
			pcListItem->cpuState = GetLoadState(pcListItem->cpu_u,cpuHighThreshold,cpuLowThreshold);
			pcListItem->cpuqState = GetLoadState(pcListItem->cpuq_u,cpuqHighThreshold,cpuqLowThreshold);
			pcListItem->memState = GetLoadState(pcListItem->mem_u,memHighThreshold,memLowThreshold);
			pcListItem->netState = GetLoadState(pcListItem->net_u,netHighThreshold,netLowThreshold);
			pcListItem->pcState = GetPCState(pcListItem->cpuState,pcListItem->cpuqState,pcListItem->memState,pcListItem->netState,
									pcListItem->cpu_u,pcListItem->cpuq_u,pcListItem->mem_u,pcListItem->net_u);
			if(pcListItem->pcState == LOAD_STATES::ST_IDLE || pcListItem->pcState == LOAD_STATES::ST_LOW){
				lowLoadedNode = true;
			}
			lowLoadedNodeList->Add(pcListItem);
			lViewer->AddNode(pcListItem);
		}
		pcList->Clear();
		lViewer->RefreshGUI();
		if(!firstRun){
		switch (pcState)
		{
		case LOAD_STATES::ST_IDLE:
			Console::WriteLine("PC STATE : IDLE");
			break;
		case LOAD_STATES::ST_LOW:
			Console::WriteLine("PC STATE : LOW");
			break;
		case LOAD_STATES::ST_HIGH:
			Console::WriteLine("PC STATE : HIGH");
			if(lowLoadedNode&&lViewer->checkBoxLBSActive->Checked == true){
				rlm->ManageLoads(myIp,lowLoadedNodeList,cpuHighThreshold,memHighThreshold);
				//lViewer
				if(rlm->migStartTime > 0)
					lViewer->DrawMigration(myIp,rlm->migratingTargetIp,rlm->migratingFederate);
			}
			break;
		default:
			Console::WriteLine("PC STATE : NORMAL");
		}
		}
		Console::WriteLine("----------------------------------------------------------------------");

		Thread::Sleep(waitTime);
		firstRun = false;
	}
}

double LoadController::AvgList(List<double>^ list)
{
	double sum = 0;
	for (int i = 0; i < list->Count; i++)
	{
		sum += list[i];
	}

	double avg = sum / list->Count;
	return avg;
}

LOAD_STATES LoadController::GetLoadState
		(double val, double highThreshold, double lowThreshold)
{
	if(val == 1)
		return LOAD_STATES::ST_IDLE;
	else if(val < lowThreshold)
		return LOAD_STATES::ST_LOW;
	else if (val > highThreshold)
		return LOAD_STATES::ST_HIGH;
	else
		return LOAD_STATES::ST_NORMAL;
}

LOAD_STATES LoadController::GetPCState(LOAD_STATES cpuState,LOAD_STATES cpuqState,LOAD_STATES memState,LOAD_STATES bwState,
											double cpu_u,double cpu_q,double mem_u,double bw_u)
{
	LOAD_STATES pcState;
	if((cpuState == LOAD_STATES::ST_IDLE) && (mem_u < 80))
	{
		pcState = LOAD_STATES::ST_IDLE;
	}
	else if(((mem_u > 80) && (cpuqState == LOAD_STATES::ST_HIGH)) || (mem_u > 90) ||
		(cpuState == LOAD_STATES::ST_HIGH && memState == LOAD_STATES::ST_HIGH))
	{
		pcState = LOAD_STATES::ST_HIGH;	
	}
	else if((cpuState == LOAD_STATES::ST_LOW) && (bwState == LOAD_STATES::ST_LOW) && (mem_u < 70) || (cpuqState == LOAD_STATES::ST_LOW))
	{
		pcState = LOAD_STATES::ST_LOW;
	}
	else
		pcState = LOAD_STATES::ST_NORMAL;

	return pcState;
}