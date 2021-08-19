/**
 * \file FileReading.h
 * File reading and map parsing operations
 */

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
 * @param[out]	brushStart	A pointer to a pointer representing the start of the brush
 *							(worldspawn) entity, for future lookup when counting the
 *							total number of brushes.
 * 
 * @return					The total number of entities ( >0 ) on success, -1 on failure.
 */
_Success_(return > 0)
DLL int _MF_CountEntities(_In_ const char* text, _Out_ char** brushStart);

/**
 * Starts parsing out an entity (brush or detail object entity). Branches based on the
 * entity class
 * 
 * @param[in]	text	A pointer to the beginning bracket of the entity definition
 * @param[out]	entity	A pointer to a struct which will contain the entity information
 * 
 * @return				An enum value representing the completion status of the opeartion.
 *						MF_PARSE_OK means success, all others mean failure.
 */
_Success_(return == MF_LOAD_OK)
DLL MF_ParseStatus _MF_StartParseGeneralEntity(_In_ const char* text, _Out_ MF_Entity* entity);

/**
 * Counts the total number of brushes within the "worldspawn" entity.
 * 
 * @param[in]	text	A pointer to the opening bracket of the worldspawn entity definition
 * 
 * @return				The total number of brushes ( >0 ) on success, -1 on failure.
 */
_Success_(return > 0)
DLL int _MF_CountBrushes(_In_ const char* text);

/**
 * Starts parsing a detail object entity
 * 
 * @param[in]	text	A pointer to the opening bracket of the detail object definition
 * @param[out]	entity	A pointer to the entity struct which will contain the entity data
 * 
 * @return				An enum representing the completion status of the operation.
 *						MF_PARSE_OK on success, all others on failure.
 */
_Success_(return == MF_PARSE_OK)
DLL MF_ParseStatus _MF_StartParseEntity(_In_ const char* text, _Out_ MF_Entity* entity);

/**
 * Starts parsing a brush entity
 * 
 * @param[in]	text	A pointer to the opening bracket of the brush object definition
 * @param[out]	entity	A pointer to the entity struct which will contain the brush data
 * 
 * @return				An enum representing the completion status of the operation.
 *						MF_PARSE_OK on success, all others on failure.
 */
_Success_(return == MF_PARSE_OK)
DLL MF_ParseStatus _MF_StartParseBrush(_In_ const char* text, _Out_ MF_Entity* entity);

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
 * Parses a property out of an entity definition
 * 
 * @param[in]	text		A pointer to the beginning of the property definition
 *							(beginning of the line)
 * @param[out]	property	A pointer to the struct which will contain the property data
 * 
 * @return					An enum representing the completion status of the operation.
 *							MF_PARSE_OK on success, all others on failure.
 */
_Success_(return == MF_PARSE_OK)
DLL MF_ParseStatus _MF_StartParseProperty(_In_ const char* text, _Out_ MF_EntityProperty* property);

}