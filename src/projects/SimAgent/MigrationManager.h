#pragma once

#include "SimAgent.h"

using namespace System;
using namespace System::IO;
using namespace System::Net::Sockets;

//Federelerin ta��nmas� i�lemini ger�ekle�tiren s�n�f.
ref class MigrationManager
{
public:
	MigrationManager(SimAgent *agent);
	void MigarateFederate(String^ ipAddress);
	void StartListener();
	bool mmCanClose;
private:
	bool CheckFedState(String^ fedState, String^ controlState);
	cli::array<Byte>^ ReadByteFromStream(NetworkStream^ nStream);
	void	WriteString(String^ message, NetworkStream^ networkStream);
	String^ ReadString(NetworkStream^ nStream);
	SimAgent* simAgent;
};
