#include "FederateStarter.h"
#include "CommonHeaders.h"
#include "StringOperation.h"

using namespace System::Diagnostics;
using namespace System::Threading;
FederateStarter::FederateStarter()
{
}

FederateStarter::~FederateStarter(void)
{
	check = false;
}

void FederateStarter::StartFederateListener()
{

	TcpListener^ fedListener = gcnew TcpListener(8889); 
	fedListener->Start(); 
	check = true;

	Console::WriteLine("FedListener baslatildi..."); 

	while(true)
	{
		Socket^ fedSocket = fedListener->AcceptSocket(); 
		Console::WriteLine("FedListener AcceptSocket...AktarýmBasladi"); 
		NetworkStream^ nStream = gcnew NetworkStream(fedSocket);

		// 1- Start new SimAgent Process.. 
		String^ fedType_Name = ReadString(nStream);
		cli::array<String^>^ fTNArray =  fedType_Name->Split('|');
		String^ fedType = fTNArray[0];
		String^ fedName = fTNArray[1];
		String^ fedTime = fTNArray[2];
		Process::Start("SimAgent.exe", fedType+" "+fedName+" "+fedTime);
		nStream->Flush() ; 
		fedSocket->Close();
		// string fedt = StringOperation::toStandardString(fedType);
		// string fedn =StringOperation::toStandardString(fedName);
		//Startt(fedt.c_str(),fedn.c_str(),2);
	}

}
/*
void FederateStarter::Startt(const char *fedType,const char *fedName, int fedState)
{
	RTI::RTIambassador *rtiamb = new RTI::RTIambassador();
	char* fedExecName = "HelloWorld";
	
	try{
		rtiamb->createFederationExecution(fedExecName, "HelloWorld.fed");
	}
	catch (RTI::FederationExecutionAlreadyExists& e){cout << "execution already exists";}
	
	SimAgent* agent = NULL;
	LBSDefaultFederate* lbsFed = NULL;
	
	lbsFed = ::CreateFederateInstance(fedType);
	lbsFed->SetName(fedName);
	agent = new SimAgent(lbsFed);
	agent->SetLBSFederateAmbassador(::GetFederateAmbassador(fedType));
	
	if(fedState == (int)FEDERATESTATE::MIGRATING){
		agent->GetLBSFederate()->SetFederateState(FEDERATESTATE::MIGRATING);
		TcpListener^ fedListener = gcnew TcpListener(8885); 
		fedListener->Start(); 
		Socket^ fedSocket = fedListener->AcceptSocket(); 
		NetworkStream^ nStream = gcnew NetworkStream(fedSocket);
		Console::WriteLine("Aktarim Basladi..."); 

		String^ rstate = ReadString(nStream);
		if(CheckFedState(rstate,"Saved"))
		{
			//Request saved state.
				SendString("RequestState",nStream);
			//Restore saved state.
				cli::array<Byte>^ savedStateByte = ReadByteFromStream(nStream);
				System::IO::File::WriteAllBytes("migratingFed",savedStateByte);
				delete savedStateByte;
			//Request messages.
				SendString("RequestMessages",nStream);
			//Restore messages.
				cli::array<Byte>^ savedMessagesByte = ReadByteFromStream(nStream);
				System::IO::File::WriteAllBytes("migratingReq",savedMessagesByte);
				//simAgent->RequestInteractions(*memArch2);
				delete savedMessagesByte;
			//Send success messages.
				SendString("Success",nStream);
				nStream->Close();
				fedSocket->Close();
		}
		fedListener->Stop();
	}

	//StartMigrationListener(agent);

	for(int tn = 0; tn < 5; tn++){
		try{
			RTI::FederateHandle federateID = rtiamb->joinFederationExecution
				(agent->GetLBSFederate()->GetName(),fedExecName,agent->GetLBSFederateAmbassador());
			break;
		}
		catch(RTI::Exception &e){
			Console::WriteLine("Federe ismi hala kullanýmda!!!");
			Thread::Sleep(500);
		}
	}
	agent->SetRTI(rtiamb);
	agent->Open();

	if(fedState == (int)FEDERATESTATE::MIGRATING){
		// open the archive
		std::ifstream ifs("migratingFed");
		assert(ifs.good());
		boost::archive::binary_iarchive ia(ifs);
		agent->FederateRestore(ia);
	}

	agent->Run();
}
*/
void FederateStarter::SendString(String^ message, NetworkStream^ networkStream)
{
	array<Byte>^ data = Encoding::Unicode->GetBytes(message);
	networkStream->Write(data, 0, data->Length);
	networkStream->Flush();
}

String^ FederateStarter::ReadString(NetworkStream^ nStream)
{
	cli::array<Byte>^ myTempBuffer = gcnew array<Byte>(1024);
	nStream->Read(myTempBuffer, 0, 1024);
	String^ message = String::Concat(message, System::Text::Encoding::Unicode->GetString(myTempBuffer, 0, myTempBuffer->Length));
	return message;
}
cli::array<Byte>^ FederateStarter::ReadByteFromStream(NetworkStream^ nStream)
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


bool FederateStarter::CheckFedState(String^ fedState, String^ controlState)
{
	const string constFedState = StringOperation::toStandardString(fedState).c_str();
	const string constControlState = StringOperation::toStandardString(controlState).c_str();
	if(strcmp(constFedState.c_str(),constControlState.c_str()) == 0 )
	{
		return true;
	}
	return false;
}