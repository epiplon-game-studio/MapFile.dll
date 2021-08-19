#pragma once
#include "CppUnitTest.h"
#include "FileReading.h"

// MF_LoadStatus

template<>
static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<MF_LoadStatus>(const MF_LoadStatus& status)
{
	switch (status)
	{
	case MF_LOAD_OK:
		return L"MF_LOAD_OK";
	case MF_LOAD_FILE_NOT_FOUND:
		return L"MF_LOAD_FILE_NOT_FOUND";
	case MF_LOAD_MEMORY_ERROR:
		return L"MF_LOAD_MEMORY_ERROR";
	case MF_LOAD_PARSE_ERROR:
		return L"MF_LOAD_PARSE_ERROR";
	default:
		return L"MF_UNKNOWN_LOAD_STATUS";
	}
}
