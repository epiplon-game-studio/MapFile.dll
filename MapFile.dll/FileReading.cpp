#include "FileReading.h"

#include "winbase.h"

_Success_(return == MF_LOAD_OK)
MF_LoadStatus MF_LoadMap(_In_ char* mapPath, _Out_ MF_Map *map)
{
	if (mapPath == NULL)
	{
		return MF_LOAD_FILE_NOT_FOUND;
	}
	if (map == NULL)
	{
		return MF_LOAD_MEMORY_ERROR;
	}

	char* data = NULL;
	size_t size = 0;
	MF_LoadStatus status = _MF_ReadFile(mapPath, &data, &size);

	if (status != MF_LOAD_OK)
	{
		return status;
	}

	int totalEntities = _MF_CountEntities(data);
	MF_Entity* entities = (MF_Entity*)HeapAlloc(
		GetProcessHeap(),
		HEAP_ZERO_MEMORY,
		sizeof(MF_Entity) * totalEntities
	);
	int entityOffset = 0;
	for (char* cursor = data; entityOffset != totalEntities; cursor++)
	{
		if (*cursor == '{')
		{
			_MF_StartParseGeneralEntity(cursor, entities + entityOffset++, &cursor);
		}
	}
}

_Success_(return > 0)
int _MF_CountEntities(_In_ const char* text)
{
	if (text == NULL || *text == 0)
	{
		return MF_COUNT_NULL_OR_EMPTY_TEXT;
	}

	int depth = 0;
	int total = 0;
	char* lastEntityStart = NULL;
	for (char* cursor = (char*)text; *cursor != 0; cursor++)
	{
		if (*cursor == '{')
		{
			if (depth == 0)
			{
				lastEntityStart = cursor;
				total++;
			}

			depth++;
		}
		else if (*cursor == '}')
		{
			depth--;
		}
	}
	if (depth > 0)
	{
		return MF_COUNT_DANGLING_OPEN_BRACKET;
	}
	else if (depth < 0)
	{
		return MF_COUNT_DANGLING_CLOSE_BRACKET;
	}
	return total;
}

_Success_(return == MF_LOAD_OK)
MF_ParseStatus _MF_StartParseGeneralEntity(_In_ const char* text, _In_ MF_Entity* entity, _Out_ char** endEntity)
{
	if (text == NULL || *text == NULL)
	{
		return MF_PARSE_SYNTAX_ERROR;
	}
	if (entity == NULL || endEntity == NULL)
	{
		return MF_PARSE_MEMORY_ERROR;
	}

	MF_ParseStatus status = _MF_ParseAllEntityProperties(text, entity);
	status = _MF_ParseAllEntityBrushes(text, entity);
	/*
	entity->totalBrushes = _MF_CountBrushes(text);
	entity->brushes = (MF_Brush*)HeapAlloc(
		GetProcessHeap(),
		HEAP_ZERO_MEMORY,
		sizeof(MF_Brush) * entity->totalBrushes
	);*/
	
	return MF_PARSE_OK;
}

_Success_(return == MF_PARSE_OK)
DLL MF_ParseStatus _MF_ParseAllEntityProperties(_In_ const char* text, _In_ MF_Entity* entity)
{
	entity->totalProperties = _MF_CountProperties(text);
	if (entity->totalProperties < 0)
	{
		return MF_PARSE_SYNTAX_ERROR;
	}
	else if (entity->totalProperties == 0)
	{
		return MF_PARSE_MEMORY_ERROR;
	}

	entity->properties = (MF_EntityProperty*)HeapAlloc(
		GetProcessHeap(),
		HEAP_ZERO_MEMORY,
		sizeof(MF_EntityProperty) * entity->totalProperties
	);
	if (entity->properties == NULL)
	{
		return MF_PARSE_MEMORY_ERROR;
	}

	int propertyOffset = 0;
	for (char* startProperty = (char*)text; propertyOffset < entity->totalProperties && *startProperty != 0; startProperty++)
	{
		if (*startProperty == '"')
		{
			_MF_StartParseProperty(startProperty, entity->properties + propertyOffset++, &startProperty);
		}
	}

	return MF_PARSE_OK;
}

_Success_(return == MF_PARSE_OK)
DLL MF_ParseStatus _MF_StartParseProperty(_In_ const char* text, _Out_ MF_EntityProperty* property, _Out_ char** endProperty)
{
	if (endProperty == NULL)
	{
		return MF_PARSE_MEMORY_ERROR;
	}

	char* key = NULL;
	UINT64 keyLength = 0;
	char* value = NULL;
	UINT64 valueLength = 0;
	for (char* cursor = (char*)text + 1; *cursor != NULL; cursor++)
	{
		if (*cursor == '"')
		{
			if (key == NULL)
			{
				key = (char*)text + 1;
				keyLength = (UINT64)(cursor - key);
			}
			else if (value == NULL)
			{
				value = cursor + 1;
			}
			else
			{
				valueLength = (UINT64)(cursor - value);
			}
		}
		if (*cursor == '\n')
		{
			*endProperty = cursor;
			break;
		}
	}
	if (key == NULL || keyLength == 0 || value == NULL || valueLength == 0 || *endProperty == NULL)
	{
		return MF_PARSE_SYNTAX_ERROR;
	}
	property->key = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, keyLength + 1);
	if (property->key == NULL)
	{
		return MF_PARSE_MEMORY_ERROR;
	}
	property->value = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, valueLength + 1);
	if (property->value == NULL)
	{
		return MF_PARSE_MEMORY_ERROR;
	}

	strncpy_s(property->key, keyLength + 1, key, keyLength);
	strncpy_s(property->value, valueLength + 1, value, valueLength);

	return MF_PARSE_OK;
}


_Success_(return > 0)
int _MF_CountProperties(_In_ const char* text)
{
	bool newLine = true;
	int total = 0;
	int depth = 0;
	for (char* cursor = (char*)text + 1; *cursor != 0; cursor++)
	{
		if (*cursor == '\n' || *cursor == '\r')
		{
			newLine = true;
		}
		else if (*cursor == '"' && newLine && depth == 0)
		{
			total++;
			newLine = false;
		}
		else if (*cursor == '{')
		{
			depth++;
		}
		else if (*cursor == '}')
		{
			depth--;
		}
		
		if (depth == -1) // end
		{
			break;
		}
	}
	return total;
}

_Success_(return > 0)
int _MF_CountBrushes(_In_ const char* text)
{
	if (text == NULL || *text == NULL)
	{
		return MF_COUNT_NULL_OR_EMPTY_TEXT;
	}
	bool newLine = true;
	int total = 0;
	int depth = 0;
	for (char* cursor = (char*)text + 1; *cursor != 0; cursor++)
	{
		if (*cursor == '{')
		{
			if (depth == 0)
			{
				total++;
			}
			depth++;
		}
		else if (*cursor == '}')
		{
			depth--;
			if (depth == -1)
				break;
		}
	}
	if (depth > 0)
	{
		return MF_COUNT_DANGLING_OPEN_BRACKET;
	}
	return total;
}

_Success_(return == MF_PARSE_OK)
MF_ParseStatus _MF_ParseAllEntityBrushes(_In_ const char* text, _In_ MF_Entity* entity)
{
	if (text == NULL || *text == '\0' || entity == NULL)
	{
		return MF_PARSE_MEMORY_ERROR;
	}
	entity->totalBrushes = _MF_CountBrushes(text);
	if (entity->totalBrushes == 0)
	{
		return MF_PARSE_OK;
	}
	entity->brushes = (MF_Brush*)HeapAlloc(
		GetProcessHeap(),
		HEAP_ZERO_MEMORY,
		sizeof(MF_Brush) * entity->totalBrushes
	);
	if (entity->brushes == NULL)
	{
		return MF_PARSE_MEMORY_ERROR;
	}

	int brushOffset = 0;
	for (char* cursor = (char*)text + 1; brushOffset < entity->totalBrushes; cursor++)
	{
		if (*cursor == '{')
		{
			_MF_StartParseBrush(cursor, entity->brushes + brushOffset++, &cursor);
		}
	}
}

_Success_(return == MF_PARSE_OK)
MF_ParseStatus _MF_StartParseBrush(_In_ const char* text, _In_ MF_Brush * brush, _Out_ char** endEntity)
{
	return MF_PARSE_OK;
}

// VERY private helpers

_Success_(return == MF_LOAD_OK)
MF_LoadStatus _MF_ReadFile(_In_ char* mapPath, _Out_ char** data, _Out_ size_t* size)
{
	// Initializing everything at the top to avoid
	// issues with goto
	MF_LoadStatus status = MF_LOAD_OK;
	DWORD numRead = 0;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD sizeLow = 0;

	if (mapPath == NULL)
	{
		return MF_LOAD_FILE_NOT_FOUND;
	}

	if (data == NULL || size == NULL)
	{
		return MF_LOAD_MEMORY_ERROR;
	}
	
	hFile = CreateFileA(
		mapPath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		switch (GetLastError())
		{
		case ERROR_FILE_NOT_FOUND:
		{
			return MF_LOAD_FILE_NOT_FOUND;
		}
		case ERROR_ACCESS_DENIED:
		{
			return MF_LOAD_PERMISSIONS;
		}
		default:
		{
			return MF_LOAD_UNKNOWN_ERROR;
		}
		}
	}

	sizeLow = GetFileSize(hFile, NULL) + 1; // +1 for the null terminator
	if (sizeLow == 0)
	{
		status = MF_LOAD_MEMORY_ERROR;
		goto close;
	}
	*size = (size_t)sizeLow;
	*data = (char*) HeapAlloc(
		GetProcessHeap(),
		HEAP_ZERO_MEMORY,
		(SIZE_T)(sizeLow * sizeof(char))
	);

	if (*data == NULL)
	{
		status = MF_LOAD_MEMORY_ERROR;
		goto close;
	}

	if (!ReadFile(
			hFile,
			*data,
			sizeLow,
			&numRead,
			NULL
		)
	)
	{
		status = MF_LOAD_UNKNOWN_ERROR;
	}
close:
	CloseHandle(hFile);
	return status;
}
