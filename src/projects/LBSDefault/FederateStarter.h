#ifndef FEDERATESTARTER_H_
#define FEDERATESTARTER_H_

#include "SimAgent.h"
#include "StringOperation.h"
#include "SimAgentStarter.h"
#include "Persistence.h"

using namespace System;
using namespace System::IO;
using namespace System::Net::Sockets;

ref class FederateStarter
{
public:
	FederateStarter();
	~FederateStarter(void);
	void StartFederateListener();
private:
	bool check;
	cli::array<Byte>^ ReadByteFromStream(NetworkStream^ nStream);
	void	SendString(String^ message, NetworkStream^ networkStream);
	String^ ReadString(NetworkStream^ nStream);
	//FederateFactory* myFedFactory;
};

#endif