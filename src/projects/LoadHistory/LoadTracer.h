#pragma once
#include "dao.h"
using namespace System::Data;
using namespace System::Data::SqlClient;
namespace LoadHistory
{
	public ref class LoadTracer : public DAO
	{
	public:
		LoadTracer(void);
		virtual ~LoadTracer(void);

		void ConnectToDb();
		void DisconnectToDb();

		//A�daki t�m akrif pclerin CPU y�kleri liste ile getirilir.
		virtual List<double>^ GetCPUWeights() override;
		virtual double GetCPUMaxLoad() override;

		//A�daki t�m aktif pclerin CPU y�k y�zdeleri liste ile getirilir.
		virtual List<double>^ GetCPUUsages() override;

		//A�daki t�m akrif pclerin bellek y�kleri liste ile getirilir.
		virtual List<double>^ GetMEMWeights() override;
		virtual double GetMEMMaxLoad() override;
		//A�daki t�m aktif pclerin bellek y�k y�zdeleri liste ile getirilir.
		virtual List<double>^ GetMEMUsages() override;

		//A�daki t�m akrif pclerin a� y�kleri liste ile getirilir.
		virtual List<double>^ GetNETWeights() override;
		virtual double GetNETMaxLoad() override;

		virtual void SetCPULoad(double load, String^ pcIP) override;
		virtual void SetCPUQueue(double load, String^ pcIP) override;
		virtual void SetCPUWeight(double weight, String^ pcIP) override;

		virtual void SetMEMLoad(double load, String^ pcIP) override;
		virtual void SetMEMWeight(double weight, String^ pcIP) override;

		virtual void SetNETLoad(double load, String^ pcIP) override;
		virtual void SetNETWeight(double weight, String^ pcIP) override;

		virtual List<String^>^ GetActivePCList() override;
		virtual void SetDeActivePCList(List<String^>^ pcList) override;
		virtual void SetActivePCList(List<String^>^ pcList) override;
		virtual List<List<String^>^>^ GetActivePCInfo() override;

		virtual void InsertFederation(String ^tag) override;
		virtual void InsertFederate(String^ fedId, String^ pcIp) override;
		virtual void RemoveFederate(String^ fedId, String^ pcIp) override;
		virtual void UpdateFederateMigrateStatus(String^ fedId, String^ pcIp, bool canMigrate) override;
		virtual void UpdateFederateLife(String^ fedId, String^ pcIp, double fedLife) override;
		virtual List<double>^ GetFederateResourceUsages(String^ fedId, String^ pcIp) override;
		virtual void UpdateFederateUsages(String^ fedId, String^ pcIp, double cpuU, double memU) override;
		virtual double GetFederateLife(String^ fedId, String^ pcIp) override;

		virtual List<String^>^ GetFederateListByIp(String^ pcIP) override;
		virtual List<String^>^ GetMigratableFederates(String^ pcIP, List<String^>^ activeFedList) override;
		virtual void UpdateMigrationTable(long migTime) override;
		virtual long GetMigrationTime() override;
	private:
		SqlConnection^ conn;
		List<String^>^ GetFederationAndIpID(String ^ip);
		bool QueryFederate4Update(int federationId, String^ fedId);
		bool IsPCInDB(String^ node);
	};

}