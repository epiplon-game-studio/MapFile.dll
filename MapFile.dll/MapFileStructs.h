#pragma once

#include "glm/glm.hpp"

typedef struct _MF_RawPlane
{
	glm::vec3 a;
	glm::vec3 b;
	glm::vec3 c;
	
	char* textureName;
	float offsetX;
	float offsetY;

	float rotation;
	
	float scaleX;
	float scaleY;
} MF_RawPlane;

typedef struct _MF_Brush
{
	MF_RawPlane* planes;
} MF_Brush;

typedef struct _MF_EntityProperty
{
	char* key;
	char* value;
} MF_EntityProperty;

typedef struct _MF_Entity
{
	char* classname;
	/* Special property, since it's on basically every entity */
	glm::vec3 origin;

	size_t totalProperties;
	MF_EntityProperty* properties;

	size_t totalBrushes;
	MF_Brush* brushes;
} MF_Entity;

typedef struct _MF_Map
{
	int totalEntities;
	MF_Entity* entities;
} MF_Map;

typedef struct __MF_ParseContext
{

} _MF_ParseContext;