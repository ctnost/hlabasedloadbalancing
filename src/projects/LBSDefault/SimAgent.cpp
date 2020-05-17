#include "SimAgent.h"
//#include "RTIAmbassadorInstance.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <process.h>          // for _beginthread()
using namespace std;
using std::cout;
using std::cerr;
using std::endl;


std::string SynPoint("zhanghui");
std::string Syntag("tag");

RTI::FedTime &GrantTime = (*(RTI::FedTimeFactory::makeZero()));
/*
RTI::RTIambassador         rti_ambassador;
bool			           RTIAmbassadorInstance::rtiCreated = false;*/
SimAgenState *SimAgenState::s_instance = 0;
char* const	    		   fedExecName = "HelloWorld";
void WaitForUser();
SimAgent::SimAgent()
{

}

SimAgent::SimAgent(LBSDefaultFederate* lbsFed)
{
	this->lbsFederate = lbsFed;
}

SimAgent::~SimAgent()
{

}

void SimAgent::FederateSave(OArchive& arch )
{
	lbsFederate->FederateSave(arch);
	double lastTime =convertTime(lbsFedAmb->GrantTime);
	arch << lastTime;
}

void SimAgent::FederateRestore(InArchive& arch)
{
	lbsFederate->FederateRestore(arch);
	double lastTime = 0;
	arch >> lastTime;
	RTIfedTime requestTime(lastTime);
	lbsFedAmb->GrantTime = requestTime;
}

void SimAgent::RestoreInteractions(InArchive& arch)
{
	lbsFederate->RestoreInteractions(arch);
}

void SimAgent::ProcessInteractions()
{
	lbsFederate->ProcessInteractions();
}

void SimAgent::RequestInteractions(OArchive& arch)
{
	lbsFederate->RequestInteractions(arch);
}


void SimAgent::SetLBSFederateAmbassador(LBSDefaultFederateAmb* lbsFedAmb)
{
	this->lbsFedAmb = lbsFedAmb;
}

void SimAgent::JoinFederation()
{
	
}
void SimAgent::Open()
{
	/*RTI::FederateHandle federateID = rti_ambassador->joinFederationExecution
		(lbsFederate->GetName(),fedExecName,lbsFedAmb);*/
	//rti_ambassador->joinFederationExecution(lbsFederate->GetName(),fedExecName,lbsFedAmb);
	lbsFederate->Init(rti_ambassador);
	std::cout << "Federe baslatiliyor..." <<endl;
	lbsFedAmb->SetFederateInstance(lbsFederate);
	rti_ambassador->registerFederationSynchronizationPoint(SynPoint.c_str(),"");
	//rti_ambassador->tick(0.1,0.2);

	while (!lbsFedAmb->AnnounceSyncReceived)
	{
		rti_ambassador->tick(0.1,0.2);
	}
//WaitForUser();
	//wait until other federates have joined.
	/*
while (lbsFedAmb->discoverNum < 1)
	{
		rti_ambassador->tick(0.1,0.2);
	}
	cout<<" wait finished "<<endl<<endl;
*/

/*
	rti_ambassador->synchronizationPointAchieved(SynPoint.c_str());
	cout <<" synchronizationPointAchieved "<<endl<<endl;
	while (!lbsFedAmb->FederationalsSynchronized)
	{
		rti_ambassador->tick(0.1,0.2);
	}*/


	//enableTimePolicy();

    lbsFederate->PublishAndSubscribe();
	//WaitForUser();
    lbsFederate->Register();
   // rti_ambassador->tick(0.1,0.2);
	
	//wait until other federates have joined.
	//while (lbsFedAmb->discoverNum < 1)
	//{
	//	rti_ambassador->tick(0.1,0.2);
	//}
	//cout<<" wait finished "<<endl<<endl;

/*
    cout<<" SyncPointRegisterFailed "<<endl<<endl;
    rti_ambassador->tick(0.1,0.2);
    while (!lbsFedAmb->AnnounceSyncReceived)
    {
		rti_ambassador->tick(0.1,0.2);
    }

	cout<<"AnnounceSyncReceived  "<<endl<<endl;        
	rti_ambassador->tick(0.1,0.2);
	

	cout<<"-----FederationalsSynchronized-----"<<endl<<endl;

	rti_ambassador->tick(0.1,0.2);
	cout<<"-----------------------------------------------------"<<endl<<"  "<<endl<<endl;
*/
	lbsFedAmb->timeAdvGrant = true;

}

void SimAgent::Run()
{
	//int numberofTicks(500);
	int counter = 1;
	int loopCounter = 0;
	const RTIfedTime timeStep(1.0);
	SimAgenState::getInstance()->set_value(0);
	bool loopCond = true;
    while (loopCond)
    {
		//cout << " Time: " << convertTime(lbsFedAmb->GrantTime)<< endl;
		double ftime = convertTime(lbsFedAmb->GrantTime) ;//+ 1.0;
		RTIfedTime requestTime(ftime) ;
		if(SimAgenState::getInstance()->get_value() == 1)	break;
		if(SimAgenState::getInstance()->get_value() == 2){
			this->GetLBSFederate()->SetFederateState(FEDERATESTATE::MIGRATED);
			break;
		}
		///////////////////////////////////////////////lbsFederate->Update(requestTime);
		advanceTime();
		lbsFederate->updateAttributeValues();//(requestTime);       
		int fedstate = lbsFederate->CheckFederateState();
		if(fedstate == 1)	break;
        Sleep(1000);
	}

	lbsFederate->Close();
	Close();
}

void SimAgent::Close()
{  
	
	try
	{
		//rti_ambassador->disableTimeConstrained();
		lbsFedAmb->TimeConstrained = false;
	}
	catch (RTI::Exception& e)
	{
		cerr << "FED_HW: Err: " << &e <<endl;                        
	}

	try
	{
		//rti_ambassador->disableTimeRegulation();
		lbsFedAmb->TimeRegulation = false;
	}
	catch (RTI::Exception& e)
	{
		cerr << "FED_HW: Error:" << &e <<endl;
	}

	try
	{
		cout << "FED_HW: resign federation execution called" << endl;
		rti_ambassador->resignFederationExecution(RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
		cout << "FED_HW: successful resign federation execution called" << endl;
	}
	catch (RTI::Exception& e)
	{
		cerr << "FED_HW: Error: "<< &e <<endl;
	}
}

void SimAgent::enableTimePolicy()
{
	////////////////////////////
	// enable time regulation //
	////////////////////////////
	//lbsFedAmb->timeAdvGrant = false;
	try
	{

		rti_ambassador->enableTimeRegulation(GrantTime,lbsFederate->GetLookahead());
		lbsFedAmb->timeAdvGrant = false;
		lbsFedAmb->TimeRegulation = false;
		while (!lbsFedAmb->TimeRegulation)
		{
			rti_ambassador->tick(0.01,1.0);
		}
	}
	catch (RTI::Exception& e)
	{
		cerr <<"Error: " << &e <<endl;
	}

	/////////////////////////////
	// enable time constrained //
	/////////////////////////////
	rti_ambassador->enableTimeConstrained();
	lbsFedAmb->timeAdvGrant = false;
	lbsFedAmb->TimeConstrained = false;
	while (lbsFedAmb->TimeConstrained == false)
	{
		rti_ambassador->tick(0.01,1.0);
	}
}

void SimAgent::advanceTime()
{
	const RTIfedTime timeStep(1.0);
	// request the advance
	RTIfedTime requestTime(timeStep.getTime());
	requestTime +=lbsFedAmb->GrantTime;
	lbsFedAmb->timeAdvGrant =false;
	rti_ambassador->timeAdvanceRequest(requestTime);
	//cout << "timeAdvanceRequest= " <<convertTime(requestTime)<<endl;
	// wait for the time advance to be granted. ticking will tell the
	// LRC to start delivering callbacks to the federate
	while( !lbsFedAmb->timeAdvGrant )
	{
		rti_ambassador->tick();
	}
}
void WaitForUser()
{
	cout << " >>>>>>>>>> Press Enter to Continue <<<<<<<<<<" << endl;
	string line;
	getline( cin, line );
}
double SimAgent::convertTime( const RTI::FedTime& theTime )
{
	RTIfedTime castedTime = (RTIfedTime)theTime;
	return castedTime.getTime();
}

unsigned __stdcall SimAgent::ThreadStaticEntryPoint(void * pThis)
{
	SimAgent * pthX = (SimAgent*)pThis;   // the tricky cast
	pthX->Run();           // now call the true entry-point-function

	// A thread terminates automatically if it completes execution,
	// or it can terminate itself with a call to _endthread().
	return 1;          // the thread exit code
}
unsigned __stdcall SimAgent::ThreadStaticEntryPoint2(void * pThis)
{
	SimAgent * pthX = (SimAgent*)pThis;   // the tricky cast
	pthX->Open();           // now call the true entry-point-function

	// A thread terminates automatically if it completes execution,
	// or it can terminate itself with a call to _endthread().
	return 1;          // the thread exit code
}

