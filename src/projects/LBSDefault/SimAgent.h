#ifndef SIMAGENT_H_
#define SIMAGENT_H_

#include "LBSDefaultFederate.h"
#include "LBSDefaultFederateAmb.h"
#include "SimAgentState.h"

/// <summary>
/// Simülasyon ajaný.
/// </summary>
class SimAgent
{
public:
        SimAgent();
        SimAgent(LBSDefaultFederate* lbsFed);
        virtual ~SimAgent();
 
		void FederateSave(OArchive& arch );
		void FederateRestore(InArchive& arch);
		void RequestInteractions(OArchive& arch);
		void RestoreInteractions(InArchive& arch);
		void ProcessInteractions();

		void JoinFederation();
		void Open();
		void Run();
		void Close();
		void enableTimePolicy();
		void advanceTime();
		void SetRTI(RTI::RTIambassador *rtiamb){rti_ambassador = rtiamb;}
		void SetLBSFederateAmbassador(LBSDefaultFederateAmb* lbsFedAmb);
		LBSDefaultFederate* GetLBSFederate(){return lbsFederate;};
		LBSDefaultFederateAmb* GetLBSFederateAmbassador(){return lbsFedAmb;};
		
		static unsigned __stdcall ThreadStaticEntryPoint(void * pThis);
		static unsigned __stdcall ThreadStaticEntryPoint2(void * pThis);


private:
	double convertTime( const RTI::FedTime& theTime );
private:
	LBSDefaultFederate* lbsFederate;
	LBSDefaultFederateAmb* lbsFedAmb;
	RTI::RTIambassador* rti_ambassador;
};
#endif