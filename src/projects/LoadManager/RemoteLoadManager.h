#pragma once

#include "RemoteNodeClient.h"
#include "RemoteNodeServer.h"
#include "LoadViewer.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace LoadHistory;
using namespace LoadManager;

ref class RemoteLoadManager
{
public:
	RemoteLoadManager(LoadTracer^ dao_);
	List<String^>^ GetRemoteUtilizations();
	List<String^>^ GetRemoteFederates(String^ ipAddress);
	void ManageLoads(String^ ipAddress, List<LoadViewer::PCNodeStruct^>^ lowLoadedNodeList,double cpuHighThreshold,double memHighThreshold);
	bool CheckNodeFutureLoad(String^ targetIp,List<LoadViewer::PCNodeStruct^>^ lowLoadedNodeList, String^ fedName,String^ ipAddress,double cpuHighThreshold,double memHighThreshold);
	void MigrateFederate(String^ myIp,String^ targetIp, String^ fedName);
	RemoteNodeClient^ rnc;
	RemoteNodeServer^ rns;
	long migStartTime;
	String^ migratingTargetIp;
	String^ migratingFederate;

	property LOAD_STATES NodeState{
		LOAD_STATES get(){
			return nodeState;
		}
		void set(LOAD_STATES lstate){
			nodeState = lstate;
			rns->nodeState = lstate;
		}
	}

	property List<String^>^ NodeList{
		List<String^>^ get(){
			return nodeList;
		}
		void set(List<String^>^ nList){
			nodeList = nList;
			rns->nodeList = nList;
		}
	}
private:
	String^ ChooseFederate(String^ ipAddress);
	LOAD_STATES nodeState;
	List<String^>^ nodeList;
	LoadTracer^ dao;
};

