#ifndef FEDERATESTARTER_H_
#define FEDERATESTARTER_H_

#include "FederateCreater.h"
#include "StringOperation.h"
#include "SimAgent.h"

using namespace System;
using namespace System::IO;
using namespace System::Text;
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
	bool CheckFedState(String^ fedState, String^ controlState);
};

#endif