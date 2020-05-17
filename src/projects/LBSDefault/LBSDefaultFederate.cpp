#include "LBSDefaultFederate.h"
#include "stdio.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <WinSock2.h>

using std::cout;
using std::cerr;
using std::endl;

LBSDefaultFederate::LBSDefaultFederate(char* p_type)
	: m_name(NULL), m_type(p_type),
	m_lastTime(*(RTI::FedTimeFactory::makeZero())),
	m_TimePlusLookahead(*(RTI::FedTimeFactory::makeZero()))
{
}
LBSDefaultFederate::LBSDefaultFederate(RTI::ObjectHandle id, char* p_type)
	: m_name(NULL),m_type(p_type),
	m_lastTime(*(RTI::FedTimeFactory::makeZero())),
	m_TimePlusLookahead(*(RTI::FedTimeFactory::makeZero()))
{
}

LBSDefaultFederate::~LBSDefaultFederate()
{
}

void LBSDefaultFederate::Init(RTI::RTIambassador* rtiAmb)
{
	ms_rtiAmb = rtiAmb;
	ms_lookahead = 0.5;
	cout<<"Init Finished"<<endl<<endl;
	inQueue = new LBSInteractionQueue();
	inStack = new LBSInteractionQueue();
	lastInteractionNumber = 0;
}

void LBSDefaultFederate::SetName(const char* name)
{
    delete [] m_name;

    if (name && strlen(name)>0)
    {
            m_name = new char[strlen(name)+1];
            strcpy(m_name,name);
    } else
    {
            m_name = NULL;
    }
}

ostream &operator <<(ostream &s, LBSDefaultFederate &v)
{
        const char* name =v.GetName();
        if (name == 0)
                name = "(unknow)";

		s <<"Name: " << name << " Population: "  << " Time: " << LBSDefaultFederate::convertTime( v.GetLastTime());

        return s;
}

ostream &operator <<(ostream &s,LBSDefaultFederate *v)
{
        if (!v)
        {
                return s;
        } 
        else
        {
                s << *v;
        }

        return s;
}

double LBSDefaultFederate::convertTime( const RTI::FedTime& theTime )
{
	RTIfedTime castedTime = (RTIfedTime)theTime;
	return castedTime.getTime();
}