#include "FederateCreater.h"
#include "WSNBoard.h"
using namespace System::Threading;

FEDAPI LBSFEDERATEHANDLE  APIENTRY CreateFederateInstance(const char* fedType )
{
	LBSDefaultFederate* lbsFed = nullptr;
	if (strcmp(fedType,"WSN") == 0  )
	{
		lbsFed = new WSN("WSN");
		WSNGUI::WSNBoard^ wsnBoard = gcnew WSNGUI::WSNBoard();
		ThreadStart^ threadDelegate1 = gcnew ThreadStart( wsnBoard, &WSNGUI::WSNBoard::StartForm );
		Thread^ newThread1 = gcnew Thread( threadDelegate1 );
		newThread1->Start();

		std::cout << "WirelessSensorNetwork yaratildi :" << &lbsFed <<endl;
		return lbsFed;
	}
	else if (strcmp(fedType,"WS") == 0  )
	{
		lbsFed = new WSFederate("WS");
		std::cout << "WirelessSensor yaratildi :" << &lbsFed <<endl;
		return lbsFed;
	}

	return lbsFed;
}

FEDAPI LBSFEDERATEAMBHANDLE APIENTRY GetFederateAmbassador(const char* fedType )
{
	if (strcmp(fedType,"WSN") == 0  )
	{
		WSNFederateAmb* fedAmb = new WSNFederateAmb();
		std::cout << "WirelessSensorNetwork Amb yaratildi :" << fedAmb <<endl;
		return fedAmb;
	}
	else if (strcmp(fedType,"WS") == 0  )
	{
		WSFederateAmb* fedAmb = new WSFederateAmb();
		std::cout << "WirelessSensor Amb yaratildi :" << fedAmb <<endl;
		return fedAmb;
	}

	LBSDefaultFederateAmb fedAmb;
	return &fedAmb;
}
