/**
 * \file FileReading.h
 * File reading and map parsing operations
 */

#pragma once
#include "framework.h"
#include "MapFileStructs.h"

// #defined magic numbers for count functions
#define MF_COUNT_NULL_OR_EMPTY_TEXT (-1)
#define MF_COUNT_NULL_BRUSHSTART (-2)
#define MF_COUNT_DANGLING_OPEN_BRACKET (-3)
#define MF_COUNT_DANGLING_CLOSE_BRACKET (-4)

enum MF_LoadStatus
{
	MF_LOAD_OK,
	MF_LOAD_FILE_NOT_FOUND,
	MF_LOAD_PERMISSIONS,
	MF_LOAD_MEMORY_ERROR,
	MF_LOAD_PARSE_ERROR,
	MF_LOAD_UNKNOWN_ERROR
};

enum MF_ParseStatus
{
	MF_PARSE_OK,
	MF_PARSE_SYNTAX_ERROR,
	MF_PARSE_MEMORY_ERROR,
	MF_PARSE_UNKNOWN_ERROR
};

extern "C"
{

/**
 * Loads a map into a set of headers representing the raw data in the map.
 * This creates a set of planes and entity's with properties, but does not
 * construct a triangle mesh from those planes
 * 
 * @param[in]	mapPath	The path to the map file
 * @param[out]	map		A pointer to the struct which will take in the map data
 * 
 * @return				An enum value representing the completion status of
 *						the operation. MF_LOAD_OK means success, all others
 *						mean failure.
 */
_Success_(return == MF_LOAD_OK)
DLL MF_LoadStatus MF_LoadMap(_In_ char* mapPath, _Out_ MF_Map* map);

/**
 * Counts the total number of entities in the map (brush and specific detail objects)
 * This is used for pre-allocating the memory necessary to parse out the brushes and 
 * entities.
 * 
 * @param[in]	text		A pointer to the start of the map file text, null terminated
 * 
 * @return					The total number of entities ( >0 ) on success, -1 on failure.
 */
_Success_(return > 0)
DLL int _MF_CountEntities(_In_ const char* text);

/**
 * Starts parsing out an entity (brush or detail object entity). Branches based on the
 * entity class
 * 
 * @param[in]	text		A pointer to the beginning bracket of the entity definition
 * @param[out]	entity		A pointer to a struct which will contain the entity information
 * @param[out]	endEntity	A pointer to one character after the end bracket of the entity
 *							definition
 * 
 * @return				An enum value representing the completion status of the opeartion.
 *						MF_PARSE_OK means success, all others mean failure.
 */
_Success_(return == MF_LOAD_OK)
DLL MF_ParseStatus _MF_StartParseGeneralEntity(_In_ const char* text, _In_ MF_Entity* entity, _Out_ char** endEntity);

/**
 * Counts the total number of brushes within a brush entity.
 * 
 * @param[in]	text	A pointer to the opening bracket of the brush entity definition
 * 
 * @return				The total number of brushes ( >0 ) on success, -1 on failure.
 */
_Success_(return > 0)
DLL int _MF_CountBrushes(_In_ const char* text);

/**
 * Starts parsing a detail object entity
 * 
 * @param[in]	text		A pointer to the opening bracket of the detail object definition
 * @param[out]	entity		A pointer to the entity struct which will contain the entity data
 * @param[out]	endEntity	A pointer to one character after the ending bracket of the entity
 *							definition
 * 
 * @return				An enum representing the completion status of the operation.
 *						MF_PARSE_OK on success, all others on failure.
 */
_Success_(return == MF_PARSE_OK)
DLL MF_ParseStatus _MF_StartParseEntity(_In_ const char* text, _In_ MF_Entity* entity, _Out_ char** endEntity);

/**
 * Starts parsing a brush entity
 * 
 * @param[in]	text		A pointer to the opening bracket of the brush object definition
 * @param[out]	entity		A pointer to the entity struct which will contain the brush data
 * @param[out]	endEntity	A pointer to one character after the ending bracket of the entity
 *							definition
 * 
 * @return				An enum representing the completion status of the operation.
 *						MF_PARSE_OK on success, all others on failure.
 */
_Success_(return == MF_PARSE_OK)
DLL MF_ParseStatus _MF_StartParseBrush(_In_ const char* text, _In_ MF_Entity* entity, _Out_ char** endEntity);

/**
 * Counts the total number of properties in an entity for pre-allocating the memory
 * to hold them
 * 
 * @param[in]	text	A pointer to the opening bracket of the entity definition
 * 
 * @return				The total number of properties in the entity definition
 *						( >0 ) on success, -1 on failure.
 */
_Success_(return > 0)
DLL int _MF_CountProperties(_In_ const char* text);

/**
 * Starts parsing all the properties of an entity, setting them on the provided entity itself.
 * 
 * @param[in]	text	A pointer to the opening bracket of the entity definition
 * @param[in]	entity	A pointer to the entity which will contain the new properties
 * 
 * @return				An enum representing the completion status of the operation.
 *						MF_PARSE_OK on success, all others on failure
 */
_Success_(return == MF_PARSE_OK)
DLL MF_ParseStatus _MF_ParseAllEntityProperties(_In_ const char* text, _In_ MF_Entity* entity);

/**
 * Parses a property out of an entity definition
 * 
 * @param[in]	text		A pointer to the beginning of the property definition
 *							(beginning of the line)
 * @param[out]	property	A pointer to the struct which will contain the property data
 * @param[out]	endProperty	A pointer to one character after the end of the property definition
 * 
 * @return					An enum representing the completion status of the operation.
 *							MF_PARSE_OK on success, all others on failure.
 */
_Success_(return == MF_PARSE_OK)
DLL MF_ParseStatus _MF_StartParseProperty(_In_ const char* text, _Out_ MF_EntityProperty* property, _Out_ char** endProperty);

/**
 * Reads a file into the provided data pointer. Puts the size of the string into the size
 * variable. This function allocates memory on the heap
 * 
 * @param[in]	mapPath		The path of the file
 * @param[out]	data		A pointer to the string where the file text will go
 * @param[out]	size		A pointer to the size_t variable which will take the
 *							value of the size of the string.
 * 
 * @return					An enum representing the completion status of the operation.
 *							MF_LOAD_OK on success, all others on failure.
 */
_Success_(return == MF_LOAD_OK)
DLL MF_LoadStatus _MF_ReadFile(_In_ char* mapPath, _Out_ char** data, _Out_ size_t* size);

}
