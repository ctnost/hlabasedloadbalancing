using namespace System;
using namespace System::Threading;
using namespace System::Net;
using namespace System::Net::NetworkInformation;

public ref class ActivePCLogger{
public:
	void LogActivePC()
	{
		LoadHistory::LoadTracer^ dao = gcnew LoadHistory::LoadTracer();
		dao->ConnectToDb();
		while(true){
			List<String^>^ oldActivePCList = dao->GetActivePCList();
			List<String^>^ newActivePCList = gcnew List<String^>();
			System::DirectoryServices::DirectoryEntry^ root = gcnew System::DirectoryServices::DirectoryEntry("WinNT:");
			for each (System::DirectoryServices::DirectoryEntry^ computers in root->Children){
				for each (System::DirectoryServices::DirectoryEntry^ computer in computers->Children){
					if(computer->Name == "Schema")
						continue;
					try{
						System::Net::IPHostEntry^ e = Dns::GetHostByName(computer->Name);
						cli::array<IPAddress^>^ addresslist =e->AddressList;
						for each (IPAddress^ theaddress in addresslist) { 
							String^ ip = theaddress->ToString();
							if(oldActivePCList->Contains(ip))
								oldActivePCList->Remove(ip);
							newActivePCList->Add(ip);
						} 
					}
					catch(Exception^ ex){
						continue;
					}
				}
			}
			dao->SetDeActivePCList(oldActivePCList);
			dao->SetActivePCList(newActivePCList);
			Thread::Sleep(5000);
		}
	}
};