#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;
using namespace System;
using namespace System::Collections;

public class StringOperation
{
	/*String convert iþlemi.*/
public:
	static std::string toStandardString(System::String^ string)
	{
		using System::Runtime::InteropServices::Marshal;
		System::IntPtr pointer = Marshal::StringToHGlobalAnsi(string);
		char* charPointer = reinterpret_cast<char*>(pointer.ToPointer());
		std::string returnString(charPointer, string->Length);
		Marshal::FreeHGlobal(pointer);
		return returnString;
	}

	static System::String^ toSystemString(std::string string)
	{
		  String^ returnString = gcnew String(string.c_str());
		  return returnString;
	}
};
