#pragma once 

using namespace System;
using namespace System::Collections::Generic;

namespace LoadHistory
{

	//Data Access Object
	public interface class DAO
	{

		//Get Data

		//Aðdaki tüm akrif pclerin CPU yükleri liste ile getirilir.
		virtual List<double>^ GetCPUWeights() = 0;
		virtual double GetCPUMaxLoad() = 0;

		//Aðdaki tüm aktif pclerin CPU yük yüzdeleri liste ile getirilir.
		virtual List<double>^ GetCPUUsages() = 0;

		//Aðdaki tüm akrif pclerin bellek yükleri liste ile getirilir.
		virtual List<double>^ GetNETWeights() = 0;
		virtual double GetNETMaxLoad() = 0;

		//Aðdaki tüm akrif pclerin að yükleri liste ile getirilir.
		virtual List<double>^ GetMEMWeights() = 0;
		virtual double GetMEMMaxLoad() = 0;
		//Aðdaki tüm aktif pclerin bellek yük yüzdeleri liste ile getirilir.
		virtual List<double>^ GetMEMUsages() = 0;
		//Set Data
		virtual void SetCPULoad(double load, String^ pcIP) = 0;
		virtual void SetCPUQueue(double load, String^ pcIP) = 0;
		virtual void SetCPUWeight(double weight, String^ pcIP) = 0;

		virtual void SetMEMLoad(double load, String^ pcIP) = 0;
		virtual void SetMEMWeight(double weight, String^ pcIP) = 0;

		virtual void SetNETLoad(double load, String^ pcIP) = 0;
		virtual void SetNETWeight(double weight, String^ pcIP) = 0;


		virtual List<String^>^ GetActivePCList() = 0;
		virtual void SetDeActivePCList(List<String^>^ pcList) = 0;
		virtual void SetActivePCList(List<String^>^ pcList) = 0;
		virtual List<List<String^>^>^ GetActivePCInfo() = 0;


		virtual void InsertFederation(String ^tag) = 0;
		virtual void InsertFederate(String^ fedId, String^ pcIp) = 0;
		virtual void RemoveFederate(String^ fedId, String^ pcIp) = 0;

		virtual void UpdateFederateMigrateStatus(String^ fedId, String^ pcIp, bool canMigrate) = 0;
		virtual void UpdateFederateLife(String^ fedId, String^ pcIp, double fedLife) = 0;
		virtual void UpdateFederateUsages(String^ fedId, String^ pcIp, double cpuU, double memU) = 0;
		virtual double GetFederateLife(String^ fedId, String^ pcIp) = 0;
		virtual List<double>^ GetFederateResourceUsages(String^ fedId, String^ pcIp) = 0;

		virtual List<String^>^ GetFederateListByIp(String^ pcIP) = 0;
		virtual List<String^>^ GetMigratableFederates(String^ pcIP, List<String^>^ activeFedList) = 0;
		virtual void UpdateMigrationTable(long migTime) = 0;
		virtual long GetMigrationTime() = 0;
	};
}