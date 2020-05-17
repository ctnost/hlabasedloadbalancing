#pragma once
#include "StringOperation.h"
using namespace System;
using namespace System::Net::Sockets;

public ref class Utility
{
public:
	static void WriteString(String^ message, NetworkStream^ networkStream)
	{
		array<Byte>^ data = System::Text::Encoding::Unicode->GetBytes(message);
		networkStream->Write(data, 0, data->Length);
		networkStream->Flush();
	}

	static String^ ReadString(NetworkStream^ nStream)
	{
		cli::array<Byte>^ myTempBuffer = gcnew array<Byte>(1024);
		nStream->Read(myTempBuffer, 0, 1024);
		String^ message = String::Concat(message, System::Text::Encoding::Unicode->GetString(myTempBuffer, 0, myTempBuffer->Length));
		message = message->Replace("\0",String::Empty);
		return message;
	}

	static bool CheckFedState(String^ fedState, String^ controlState)
	{
		const string constFedState = StringOperation::toStandardString(fedState).c_str();
		const string constControlState = StringOperation::toStandardString(controlState).c_str();
		if(strcmp(constFedState.c_str(),constControlState.c_str()) == 0 )
		{
			return true;
		}
		return false;
	}
};

