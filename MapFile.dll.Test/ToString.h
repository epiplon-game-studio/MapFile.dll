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
	case MF_LOAD_PERMISSIONS:
		return L"MF_LOAD_PERMISSIONS";
	case MF_LOAD_UNKNOWN_ERROR:
		return L"MF_LOAD_UNKNOWN_ERROR";
	default:
		return L"MF_UNKNOWN_LOAD_STATUS";
	}
}

// MF_ParseStatus

template<>
static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<MF_ParseStatus>(const MF_ParseStatus& status)
{
	switch (status)
	{
	case MF_PARSE_OK:				return L"MF_PARSE_OK";
	case MF_PARSE_SYNTAX_ERROR:		return L"MF_PARSE_SYNTAX_ERROR";
	case MF_PARSE_MEMORY_ERROR:		return L"MF_PARSE_MEMORY_ERROR";
	case MF_PARSE_UNKNOWN_ERROR:	return L"MF_PARSE_UNKNOWN_ERROR";
	default:						return L"MF_PARSE_UNDEFINED_ERROR";
	}
}
