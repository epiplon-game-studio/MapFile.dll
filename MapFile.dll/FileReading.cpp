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

	char* brushStart = NULL;
	int totalEntities = _MF_CountEntities(data, &brushStart);
	//int totalBrushes = _MF_CountBrushes(brushStart);

	MF_Entity* entities = (MF_Entity*)HeapAlloc(
		GetProcessHeap(),
		HEAP_ZERO_MEMORY,
		sizeof(MF_Entity) * totalEntities
	);


}

_Success_(return > 0)
int _MF_CountEntities(_In_ const char* text, _Out_ char** brushStart)
{
	if (text == NULL || *text == 0)
	{
		return MF_COUNT_ENTITIES_NULL_OR_EMPTY_TEXT; // null text
	}
	if (brushStart == NULL)
	{
		return MF_COUNT_ENTITIES_NULL_BRUSHSTART;
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
			else
			{
				*brushStart = lastEntityStart;
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
		return MF_COUNT_ENTITIES_DANGLING_OPEN_BRACKET;
	}
	else if (depth < 0)
	{
		return MF_COUNT_ENTITIES_DANGLING_CLOSE_BRACKET;
	}
	return total;
}

_Success_(return == MF_LOAD_OK)
MF_ParseStatus _MF_StartParseGeneralEntity(_In_ const char* text, _Out_ MF_Entity* entity, _Out_ char** endEntity)
{
	int properties = _MF_CountProperties(text);

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