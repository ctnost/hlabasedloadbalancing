#ifndef FEDERATETRIGGER_H_
#define FEDERATETRIGGER_H_
#pragma once

#include "StringOperation.h"
#include "SimAgent.h"

using namespace System;
using namespace System::IO;
using namespace System::Text;
using namespace System::Net::Sockets;

public ref class FederateTrigger
{
public:
	FederateTrigger();
	~FederateTrigger(void);
	void StartFederateTrigger();
private:
	cli::array<Byte>^ ReadByteFromStream(NetworkStream^ nStream);
	void	SendString(String^ message, NetworkStream^ networkStream);
	String^ ReadString(NetworkStream^ nStream);
};

#endif