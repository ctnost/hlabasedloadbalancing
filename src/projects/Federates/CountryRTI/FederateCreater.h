#ifndef _FEDLIBRARY_H_
#define _FEDLIBRARY_H_

#include <iostream>
#include <windows.h>
#include "WSN.h"
#include "WSNFederateAmb.h"
#include "WSFederate.h"
#include "WSFederateAmb.h"

#ifdef FEDLIBRARY_EXPORTS
#define FEDAPI __declspec(dllexport)
#else
#define FEDAPI __declspec(dllimport)
#endif

#ifdef __cplusplus
class LBSDefaultFederate;
class LBSDefaultFederateAmb;
typedef LBSDefaultFederate* LBSFEDERATEHANDLE;
typedef LBSDefaultFederateAmb* LBSFEDERATEAMBHANDLE;
#else   // C

#endif // __cplusplus

#ifdef __cplusplus
#   define EXTERN_C     extern "C"
#else
#   define EXTERN_C
#endif // __cplusplus

EXTERN_C
FEDAPI
LBSFEDERATEHANDLE
WINAPI CreateFederateInstance(const char* fedType );

EXTERN_C
FEDAPI
LBSFEDERATEAMBHANDLE
WINAPI GetFederateAmbassador(const char* fedName );

#endif