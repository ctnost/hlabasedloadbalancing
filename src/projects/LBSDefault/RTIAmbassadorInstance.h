#ifndef RTIAMBASSADORINSTANCE_H_
#define RTIAMBASSADORINSTANCE_H_

#include <iostream>
#include <string>
using namespace std;
#include "RTI.hh"

public class RTIAmbassadorInstance
{
public:
	static RTI::RTIambassador GetInstance();
	static void Destroy();
	static char* const GetFedExecName();
public:
	static RTI::RTIambassador rtiAmb;
	static char* const fedExecName;
	static bool rtiCreated;
	//static CRITICAL_SECTION cs;
};
#endif