#pragma once
#include "framework.h"

#include "MapFileStructs.h"

enum MF_LoadStatus
{
	MF_LOAD_OK,
	MF_LOAD_FILE_NOT_FOUND,
	MF_LOAD_MEMORY_ERROR,
	MF_LOAD_PARSE_ERROR
};

enum MF_ParseStatus
{
	MF_PARSE_OK,
	MF_PARSE_SYNTAX_ERROR,
	MF_PARSE_MEMORY_ERROR
};

extern "C"
{

_Success_(return == MF_LOAD_OK)
DLL MF_LoadStatus MF_LoadMap(_In_ char* mapPath, _Out_ MF_Map* map);

_Success_(return > 0)
DLL int _MF_CountEntities(_In_ const char* text, _Out_ char* brushStart);

_Success_(return == MF_LOAD_OK)
DLL MF_ParseStatus _MF_StartParseGeneralEntity(_In_ const char* text, _Out_ MF_Map* map);

_Success_(return > 0)
DLL int _MF_CountBrushes(_In_ const char* text);

_Success_(return == MF_PARSE_OK)
DLL MF_ParseStatus _MF_StartParseEntity(_In_ const char* text, _Out_ MF_Entity* map);

_Success_(return == MF_PARSE_OK)
DLL MF_ParseStatus _MF_StartParseBrush(_In_ const char* text, _Out_ MF_Brush* map);

_Success_(return > 0)
DLL int _MF_CountProperties(_In_ const char* text);

_Success_(return == MF_PARSE_OK)
DLL MF_ParseStatus _MF_StartParseProperty(_In_ const char* text, _Out_ MF_EntityProperty* property);

}