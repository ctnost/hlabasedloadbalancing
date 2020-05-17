#ifndef LBSDEFAULTFED_H_
#define LBSDEFAULTFED_H_

#include "RTI.hh"
#include <fedtime.hh>
#include <vector>
using std::ostream;
using namespace std;

#include "CommonHeaders.h"

/// <summary>
/// Simülasyon federe durumu tutulur.
/// </summary>
enum FEDERATESTATE
{
	DEFAULT,
	MIGRATING,
	SAVE,
	RESTORE,
	MIGRATED
};

struct LBSInteractionItem
{
public:
	RTI::InteractionClassHandle        theInteraction;
	RTI::ParameterHandleValuePairSet*  theParameters;
	double			                   theTime;
	int								   theTag;
	int								   type; //0: Receive , 1: Send
	int								   temp;
};

struct SortInteractionsByTime : public std::binary_function<LBSInteractionItem, LBSInteractionItem, bool>
{
	bool operator()(LBSInteractionItem left, LBSInteractionItem right) const
	{
		return(left.theTime < right.theTime);
	};
};

typedef list<LBSInteractionItem> LBSInteractionQueue;

class LBSDefaultFederate
{
public:
        LBSDefaultFederate(char* p_type);
        LBSDefaultFederate(RTI::ObjectHandle id, char* p_type);
        virtual ~LBSDefaultFederate();

		friend ostream& operator << (ostream &s,LBSDefaultFederate &v);
		friend ostream& operator << (ostream &s,LBSDefaultFederate *v);

		//ErdemControl
        //static LBSDefaultFederate*  Find(RTI::ObjectHandle objectID);
        virtual void      Init(RTI::RTIambassador* rtiAmb);
		static double convertTime( const RTI::FedTime& theTime );
		RTI::FedTime const& GetLastTime() { return m_lastTime;};
		RTI::FedTime const& GetLastTimePlusLookahead() 
		{
			m_TimePlusLookahead =m_lastTime;
			m_TimePlusLookahead += ms_lookahead;
			return m_TimePlusLookahead;
		};
		const char*          GetTypeName() { return m_type; };
		const char*          GetName() { return m_name; };
		RTI::ObjectHandle&   GetInstanceID(){ return m_instanceID; };

		RTI::FedTime&               GetLookahead()      { return ms_lookahead; };

		void SetLastTime(RTI::FedTime const& time)             { m_lastTime = time; };
		void SetName(const char*);
		void SetLookahead(RTI::FedTime& time) { ms_lookahead = time;  };

		FEDERATESTATE GetFederateState(){return fedState;};
		void SetFederateState(FEDERATESTATE fedSt){fedState = fedSt;};
		//LBSInteractionQueue* GetInteractionQueue() {return inQueue;};

		//Pure virtual methods
		virtual void	 FederateSave(OArchive& arch) const = 0;
		virtual void	 FederateRestore(InArchive& arch) = 0;
		virtual void	 RequestInteractions(OArchive& arch) const = 0;
		virtual void	 RestoreInteractions(InArchive& arch) = 0;
		virtual void	 ProcessInteractions() = 0;
        virtual void     PublishAndSubscribe() = 0;
        virtual void	 Register() = 0;
		virtual void	 updateAttributeValues() = 0;
		virtual void	 Close() = 0;
		virtual int		 CheckFederateState() = 0;
		virtual void     Update(RTI::FedTime& newTime) = 0;
		virtual void     Update(const RTI::AttributeHandleValuePairSet& theAttributes) = 0;
		virtual void	 JoinFederation() = 0;
 
protected:
		char*			  m_type;
        char*             m_name;
        RTI::ObjectHandle m_instanceID;
        RTI::FedTime&     m_lastTime;
        RTI::FedTime&     m_TimePlusLookahead;
        RTI::RTIambassador* ms_rtiAmb;
        //Lookahead Time value
        RTIfedTime   ms_lookahead;
		FEDERATESTATE fedState;
public:
		//InQueue
		LBSInteractionQueue* inQueue;
		LBSInteractionQueue* inStack;
		int lastInteractionNumber;
};
#endif