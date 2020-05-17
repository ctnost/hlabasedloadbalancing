using namespace System;
using namespace System::Threading;
using namespace System::Net;
using namespace System::Net::NetworkInformation;

public ref class PCTimeLogger{
public:
	void LogPCTime()
	{
		String^ strHostName = "";
		strHostName = System::Net::Dns::GetHostName();
		IPHostEntry^ ipEntry = System::Net::Dns::GetHostEntry(strHostName);
		cli::array<IPAddress^>^ addr = ipEntry->AddressList;
		String^ myIp = addr[addr.Length-1]->ToString();

		//This is Server 
	}
};