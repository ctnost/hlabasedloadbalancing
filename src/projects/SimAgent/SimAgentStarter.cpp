#include "FederateCreater.h"
#include "StringOperation.h"
#include "SimAgent.h"
#include "MigrationManager.h"

#include <cstdlib> 
#include <ctime> 
#include <sstream>  
#include <ctime>
using namespace std;

using namespace System::Threading;
using namespace System::IO;
using namespace System::Text;
using namespace System::Net::Sockets;
using namespace System::Net;

MigrationManager^ StartMigrationListener(SimAgent* simAgent);
bool CheckFedState(String^ fedState, String^ controlState);
cli::array<Byte>^ ReadByteFromStream(NetworkStream^ nStream);
String^ ReadString(NetworkStream^ nStream);
void SendString(String^ message, NetworkStream^ networkStream);
String^ GetIP()
{
	String^ strHostName = System::Net::Dns::GetHostName();
	IPHostEntry^ ipEntry = System::Net::Dns::GetHostEntry(strHostName);
	cli::array<IPAddress^>^ addr = ipEntry->AddressList;
	return addr[addr.Length-1]->ToString();
}
void WriteBytes(String^ request,NetworkStream^ nStream, String^ fileName)
{
	SendString(request,nStream);
	//Restore saved state.
	cli::array<Byte>^ savedStateByte = ReadByteFromStream(nStream);
	System::IO::File::WriteAllBytes(fileName,savedStateByte);
	delete savedStateByte;
}
void SimAgentStarter(const char *fedType,const char *fedName, int fedState, int fedTime)
{
	long migrationStartTime = time(0);
	RTI::RTIambassador *rtiamb = new RTI::RTIambassador();
	char* fedExecName = "HelloWorld";
	FederationTracer^ fedTracer = gcnew FederationTracer();
	try{
		rtiamb->createFederationExecution(fedExecName, "HelloWorld.fed");
		fedTracer->CreateNewFederation();
	}
	catch (RTI::FederationExecutionAlreadyExists& e){cout << "execution already exists";}
	
	SimAgent* agent = NULL;
	LBSDefaultFederate* lbsFed = NULL;
	
	lbsFed = ::CreateFederateInstance(fedType);
	lbsFed->SetName(fedName);
	agent = new SimAgent(lbsFed);
	agent->SetLBSFederateAmbassador(::GetFederateAmbassador(fedType));
	lbsFed->SetFederateState((FEDERATESTATE)fedState);
	long startTime = time(0);
	String^ fedNameStr = StringOperation::toSystemString(fedName);
	String^ myIp = GetIP();
	fedTracer->InsertNewFederate(fedNameStr,startTime,myIp);
	NetworkStream^ nStream;
	Socket^ fedSocket;
	if(fedState == (int)FEDERATESTATE::MIGRATING){
		TcpListener^ fedListener = gcnew TcpListener(8885); 
		fedListener->Start(); 
		fedSocket = fedListener->AcceptSocket(); 
		nStream = gcnew NetworkStream(fedSocket);
		Console::WriteLine("Aktarim Basladi..."); 

		String^ rstate = ReadString(nStream);
		if(CheckFedState(rstate,"Saved"))
		{
			//Request saved state.
				WriteBytes("RequestState",nStream,"migratingFed");
			//Restore messages.
				WriteBytes("RequestMessages",nStream,"migratingReq");
			//Send success messages.
			//	SendString("Success",nStream);
		}
		fedListener->Stop();
	}

	MigrationManager^ mm = StartMigrationListener(agent);

	for(int tn = 1; tn < 11; tn++){
		try{
			RTI::FederateHandle federateID = rtiamb->joinFederationExecution
				(agent->GetLBSFederate()->GetName(),fedExecName,agent->GetLBSFederateAmbassador());
			break;
		}
		catch(RTI::Exception &e){
			Console::WriteLine("Federe ismi hala kullanýmda!!!");
			Thread::Sleep(500/tn);
		}
	}
	agent->SetRTI(rtiamb);
	agent->Open();

	if(fedState == (int)FEDERATESTATE::MIGRATING){
		//Todo
		//WriteBytes("ReRequestMessages",nStream,"migratingReReq");

		nStream->Close();
		fedSocket->Close();
		// open the archive
		int tryn = 0;
		std::ifstream ifs("migratingFed");
		while(tryn < 2){
			try{
			assert(ifs.good());
			boost::archive::binary_iarchive ia(ifs);
			agent->FederateRestore(ia);
			ifs.close();
			break;
			}catch(Exception^ ex){
				tryn++;
				ifs.close();
			}
		}
		//
		std::ifstream ifs2("migratingReq");
		while(tryn < 2){
			try{
				assert(ifs2.good());
				boost::archive::binary_iarchive ia2(ifs2);
				agent->RestoreInteractions(ia2);
				ifs2.close();
				break;
			}catch(Exception^ ex){
				tryn++;
				ifs2.close();
			}
		}
		//
		//std::ifstream ifs3("migratingReReq");
		//assert(ifs3.good());
		//boost::archive::binary_iarchive ia3(ifs3);
		//agent->RestoreInteractions(ia3);
		//
		agent->ProcessInteractions();

		lbsFed->SetFederateState(FEDERATESTATE::DEFAULT);
		LoadHistory::LoadTracer^ dao = gcnew LoadHistory::LoadTracer();
		dao->ConnectToDb();
		//long migrationStartTime = dao->GetMigrationTime();
		long migrationFinishTime = time(0);
		dao->UpdateMigrationTable(0);
		long migTime = (migrationFinishTime-migrationStartTime) + fedTime;
		fedTracer->InsertNewMigration(fedNameStr,migTime,myIp);
	}

	agent->Run();
	long endTime = time(0);
	fedTracer->UpdateFederate(fedNameStr,endTime,myIp);
	while(! mm->mmCanClose){
		Thread::Sleep(200);
	}
	//mmThread->Interrupt();
}

MigrationManager^ StartMigrationListener(SimAgent* simAgent)
{
	MigrationManager^ mm = gcnew MigrationManager(simAgent);
	ThreadStart^ threadDelegate1 = gcnew ThreadStart( mm, &MigrationManager::StartListener );
	Thread^ mmThread = gcnew Thread( threadDelegate1 );
	mmThread->Start();
	return mm;
}

void SendString(String^ message, NetworkStream^ networkStream)
{
	array<Byte>^ data = Encoding::Unicode->GetBytes(message);
	networkStream->Write(data, 0, data->Length);
	networkStream->Flush();
}
String^ ReadString(NetworkStream^ nStream)
{
	cli::array<Byte>^ myTempBuffer = gcnew array<Byte>(1024);
	nStream->Read(myTempBuffer, 0, 1024);
	String^ message = String::Concat(message, System::Text::Encoding::Unicode->GetString(myTempBuffer, 0, myTempBuffer->Length));
	return message;
}
cli::array<Byte>^ ReadByteFromStream(NetworkStream^ nStream)
{
	cli::array<Byte>^ myTempBuffer = gcnew array<Byte>(1024);
	cli::array<Byte>^ myReadBuffer = gcnew array<Byte>(0);
	String^ myCompleteMessage = "";
	int numberOfBytesRead = 0;
	int sumOfBytesRead = 0;

	// Incoming message may be larger than the buffer size.
	do
	{
		numberOfBytesRead = nStream->Read( myTempBuffer, 0, myTempBuffer->Length );
		Array::Resize(myReadBuffer,myReadBuffer->Length + numberOfBytesRead);
		Array::Copy(myTempBuffer,0,myReadBuffer,0,numberOfBytesRead);
		sumOfBytesRead += numberOfBytesRead;
	}
	while ( nStream->DataAvailable );

	return myReadBuffer;
}
bool CheckFedState(String^ fedState, String^ controlState)
{
	const string constFedState = StringOperation::toStandardString(fedState).c_str();
	const string constControlState = StringOperation::toStandardString(controlState).c_str();
	if(strcmp(constFedState.c_str(),constControlState.c_str()) == 0 )
	{
		return true;
	}
	return false;
}
void StartFederateListener()
{

	TcpListener^ fedListener = gcnew TcpListener(8889); 
	fedListener->Start(); 
	String^ message;
	Socket^ fedSocket = fedListener->AcceptSocket(); 
	Console::WriteLine("FedListener AcceptSocket...AktarýmBasladi"); 
	NetworkStream^ nStream = gcnew NetworkStream(fedSocket);

	// 1- Start new SimAgent Process.. 
	String^ fedType_Name = ReadString(nStream);
	cli::array<String^>^ fTNArray =  fedType_Name->Split('|');
	String^ fedType = fTNArray[0];
	String^ fedName = fTNArray[1];
	nStream->Flush() ; 
	fedSocket->Close();
	string fedt = StringOperation::toStandardString(fedType);
	string fedn =StringOperation::toStandardString(fedName);
	int fedStartType = (int)FEDERATESTATE::MIGRATING;
	SimAgentStarter(fedt.c_str(),fedn.c_str(),fedStartType,0);
}
 int main(int argc,char* argv[])
{
	string fedName,fedType;
	srand((unsigned)time(0)); 
	int random_integer = rand();
	stringstream sstream;
	int fedStartType = (int)FEDERATESTATE::DEFAULT;
	int fedTime = 0;
	if(argc == 1){
		cout << " >>>>>>>>>> Federate Name to Continue <<<<<<<<<<" << endl;
		getline( cin, fedType );
		if(strcmp(fedType.c_str(),"mm") == 0 )
			StartFederateListener();
		sstream << fedType << "-" << random_integer;
		fedName = const_cast<char*>(sstream.str().c_str());
	}
	else if(argc == 4){
		fedType = argv[1];
		fedName = argv[2];
		fedTime = Convert::ToInt64(argv[3]);
		Console::WriteLine("FEDTIME--- " + fedTime.ToString());
		fedStartType = (int)FEDERATESTATE::MIGRATING;
	}
	else{
		fedType = argv[1];
		sstream << fedType << "-" << random_integer;
		fedName = const_cast<char*>(sstream.str().c_str());
	}

	cout << "FedName :" << fedName<< endl;
	SimAgentStarter(fedType.c_str(),fedName.c_str(),fedStartType,fedTime);
	return 0;
}
