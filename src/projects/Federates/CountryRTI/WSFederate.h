#ifndef WS_H_
#define WS_H_

#include "RTI.hh"
#include <fedtime.hh>
#include "LBSDefaultFederate.h"
#include "WirelessSensor.h"
#include "Defaults.h"
#include <map>
#include <vcclr.h>

using std::ostream;

struct RSSTATE
{
	int receivedTag;
	int sendingTag;
};

class WSFederate : public LBSDefaultFederate
{
public:
        WSFederate(char* p_type);
        virtual ~WSFederate();

		//Virtual Methods
		virtual void	 FederateSave(OArchive& arch) const;
		virtual void	 FederateRestore(InArchive& arch);
		virtual void	 RequestInteractions(OArchive& arch) const;
		virtual void	 RestoreInteractions(InArchive& arch);
		virtual void	 ProcessInteractions();
		virtual void     PublishAndSubscribe();
		virtual void	 Register();
		virtual void     JoinFederation(){}
		virtual void     Init(RTI::RTIambassador* rtiAmb);
		virtual void	 Close();

		void			 updateAttributeValues();
		int				 CheckFederateState();
		void			 SendDetectedData(bool newData);
        void             Update(RTI::FedTime& newTime);
		void             Update(const RTI::AttributeHandleValuePairSet& theAttributes);
        static void		 Update(RTI::InteractionClassHandle theInteraction,const RTI::ParameterHandleValuePairSet& theParameters);
		void			 ReceiveData(RTI::InteractionClassHandle theInteraction,const RTI::ParameterHandleValuePairSet& theParameters,const char *theTag);
        static RTI::ObjectClassHandle      GetWSRtiID()   { return ms_WSTypeID; };
        static RTI::AttributeHandle        GetNameRtiID()      { return ms_nameTypeID; };
//        static RTI::InteractionClassHandle GetCommRtiID()      { return ms_commTypeID; };
        //static RTI::ParameterHandle        GetMessageRtiID()   { return ms_commMsgTypeID;};
        //static RTI::FedTime&               GetLookahead()      { return ms_lookahead; };

		void SetUpdateControl(RTI::Boolean status, const RTI::AttributeHandleSet& attrs);
		double Expntl(double x);
		std::string itostring(int i);
		RTI::AttributeHandleValuePairSet* CreateAttributeSet();
		void UpdateFederateOnDB();
        static void SetInteractionControl(RTI::Boolean status, RTI::InteractionClassHandle theClass);
		int GetLastMessageFromParameters(const RTI::ParameterHandleValuePairSet& theParameters);
        static const double ms_growthRate;
        static unsigned int ms_extentCardinality;
public:
		static WirelessSensor* ws;
		RSSTATE* rs;
private:
        //Run-time Type Identification data
        static RTI::ObjectClassHandle      ms_WSTypeID;
        static RTI::AttributeHandle        ms_nameTypeID;
        static RTI::AttributeHandle        ah_posx;
		static RTI::AttributeHandle        ah_posy;
		static RTI::AttributeHandle        ah_power;
		static RTI::AttributeHandle        ah_rc;
		static RTI::AttributeHandle        ah_sc;
        static RTI::InteractionClassHandle ich_SendDataTypeID;
		static RTI::InteractionClassHandle ich_LBSCheck;
        static RTI::ParameterHandle        ph_posx;
		static RTI::ParameterHandle        ph_posy;
		static RTI::ParameterHandle        ph_radius;
		static RTI::ParameterHandle        ph_LastMessageTag;
		static RTI::ParameterHandle		   ph_Target;
		static RTI::ParameterHandle		   ph_Source;
		static RTI::ParameterHandle		   ph_LastMessageId;
		double nextDetectionTime;
		double nextSendingTime;
		double localTime;
		double proc_timeLast,sys_timeLast;
		double systemTotalMemory;
        //Names for querying RTTI values
        static char* const  ms_WSTypeStr;
        static char* const  ms_nameTypeStr;
        static char* const  ah_posxTypeStr;
		static char* const  ah_posyTypeStr;
		static char* const  ah_powerTypeStr;
        static char* const  ich_SendDataTypeStr;
		static char* const  ich_LBSCheckStr;
        static char* const  ph_posxTypeStr;
		static char* const  ph_posyTypeStr;
		static char* const  ph_radiusTypeStr;
		static char* const  ph_TargetStr;
		static char* const  ph_SourceStr;
		static char* const  ph_LastMessageTagStr;
		static char* const  ah_rcTypeStr;
		static char* const  ah_scTypeStr;
		gcroot<LoadHistory::LoadTracer^> dao;
};
#endif