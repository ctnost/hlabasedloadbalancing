// LoadTracer.h

#pragma once

#include "IResourceTracer.h"

using namespace System;
using namespace System::Management;

namespace LoadMonitor
{

	public class ResourceTracer : public IResourceTracer
	{
	public:
		ResourceTracer();
		~ResourceTracer();

		//CPU Kuyrukta bekleyen iþ sayýsý
		virtual int GetCPU_Q() override;

		//CPU kullaným durumu(%)
		virtual int GetCPU_Usage() override;

		//CPU hýzý
		virtual double GetCPU_S() override;

		//Kullanýlan bellek miktarý
		virtual double GetMEM_U() override;

		//Toplam sistem belleði.
		virtual double GetMEM_S() override;

		//Toplam sistem belleði kullaným yüzdesi.
		virtual double GetMEM_Usage() override;

		//Kullanýlan bw miktarý
		virtual double GetBW_U() override;

		//BW hýzý.
		virtual double GetBW_S() override;

	private:
		
	};
}
