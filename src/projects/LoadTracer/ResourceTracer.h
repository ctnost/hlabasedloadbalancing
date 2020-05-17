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

		//CPU Kuyrukta bekleyen i� say�s�
		virtual int GetCPU_Q() override;

		//CPU kullan�m durumu(%)
		virtual int GetCPU_Usage() override;

		//CPU h�z�
		virtual double GetCPU_S() override;

		//Kullan�lan bellek miktar�
		virtual double GetMEM_U() override;

		//Toplam sistem belle�i.
		virtual double GetMEM_S() override;

		//Toplam sistem belle�i kullan�m y�zdesi.
		virtual double GetMEM_Usage() override;

		//Kullan�lan bw miktar�
		virtual double GetBW_U() override;

		//BW h�z�.
		virtual double GetBW_S() override;

	private:
		
	};
}
