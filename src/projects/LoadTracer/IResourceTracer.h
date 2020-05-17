#pragma once

namespace LoadMonitor
{
	/// <summary>
	/// Y�k izleme aray�z�.
	/// </summary>
	class IResourceTracer
	{
	public:
		//ILoadTracer(void);

		//CPU Kuyrukta bekleyen i� say�s�
		virtual int GetCPU_Q() = 0;

		//CPU kullan�m durumu(%)
		virtual int GetCPU_Usage() = 0;

		//CPU h�z�
		virtual double GetCPU_S() = 0;

		//Kullan�lan bellek miktar�
		virtual double GetMEM_U() = 0;

		//Toplam sistem belle�i.
		virtual double GetMEM_S() = 0;

		//Toplam sistem belle�i kullan�m y�zdesi.
		virtual double GetMEM_Usage() = 0;

		//Kullan�lan bw miktar�
		virtual double GetBW_U() = 0;

		//BW h�z�.
		virtual double GetBW_S() = 0;

	public:
		//~ILoadTracer(void);
	};
}