#include "FederateStarter.h"
#include "CommonHeaders.h"
//int SimAgent::agentstate;

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

	Socket^ fedSocket = fedListener->AcceptSocket(); 
	SimAgent* simAgent = NULL;
	LBSDefaultFederate* lbsFed = NULL;
	String^ message;
	while(true)
	{
		NetworkStream^ nStream = gcnew NetworkStream(fedSocket);

		//try
		//{
			// Five defined actions to perform the migration.
			// 1- Launches a new SimAgent  
			String^ fedType_Name = ReadString(nStream);
			cli::array<String^>^ fTNArray =  fedType_Name->Split('|');
			const string fedType = StringOperation::toStandardString(fTNArray[0]).c_str();
			const string fedName = StringOperation::toStandardString(fTNArray[1]).c_str();
			
			lbsFed = ::CreateFederateInstance(fedType.c_str());
			lbsFed->SetName(fedName.c_str());
			simAgent = new SimAgent(lbsFed);
			simAgent->SetLBSFederateAmbassador(::GetFederateAmbassador(fedType.c_str()));
			//StartMigrationListener(simAgent);

			//// Federate state is migrating!!!
			simAgent->GetLBSFederate()->SetFederateState(FEDERATESTATE::MIGRATING);

			String^ rstate = ReadString(nStream);
			if(CheckFedState(rstate,"Saved"))
			{
				//Register federate to rti.
				//Request saved state.
					SendString("RequestState",nStream);
				//Restore saved state.
					cli::array<Byte>^ savedStateByte = ReadByteFromStream(nStream);
					System::IO::File::WriteAllBytes("migratingFed",savedStateByte);
					// open the archive
					std::ifstream ifs("migratingFed");
					assert(ifs.good());
					boost::archive::binary_iarchive ia(ifs);
					simAgent->FederateRestore(ia);
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
					RTI::RTIambassador *rtiamb = new RTI::RTIambassador();
					char* fedExecName = "HelloWorld";

					try{
						rtiamb->createFederationExecution(fedExecName, "HelloWorld.fed");
					}
					catch (RTI::FederationExecutionAlreadyExists& e){cout << "execution already exists";}

					RTI::FederateHandle federateID = rtiamb->joinFederationExecution
						(simAgent->GetLBSFederate()->GetName(),fedExecName,simAgent->GetLBSFederateAmbassador());
					simAgent->SetRTI(rtiamb);
					simAgent->Open();
					simAgent->Run();
			}

			nStream->Flush() ; 
/*
		}
		catch(Exception ^ex)
		{
			Console::WriteLine("Sunucu kapatiliyor...");
			return ;
		}*/


	} //End While

	fedSocket->Close();
}

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