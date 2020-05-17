#pragma once

namespace LoadMonitor
{
	/// <summary>
	/// Yük izleme arayüzü.
	/// </summary>
	class IResourceTracer
	{
	public:
		//ILoadTracer(void);

		//CPU Kuyrukta bekleyen iþ sayýsý
		virtual int GetCPU_Q() = 0;

		//CPU kullaným durumu(%)
		virtual int GetCPU_Usage() = 0;

		//CPU hýzý
		virtual double GetCPU_S() = 0;

		//Kullanýlan bellek miktarý
		virtual double GetMEM_U() = 0;

		//Toplam sistem belleði.
		virtual double GetMEM_S() = 0;

		//Toplam sistem belleði kullaným yüzdesi.
		virtual double GetMEM_Usage() = 0;

		//Kullanýlan bw miktarý
		virtual double GetBW_U() = 0;

		//BW hýzý.
		virtual double GetBW_S() = 0;

	public:
		//~ILoadTracer(void);
	};
}