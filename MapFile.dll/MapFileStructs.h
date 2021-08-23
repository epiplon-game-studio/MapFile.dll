#pragma once

#include "glm/glm.hpp"

typedef union _MF_FaceTextureParameters
{
	struct
	{
		float offsetX;
		float offsetY;
		float rotation;
		float scaleX;
		float scaleY;
	};
	float parameters[5];
} MF_FaceTextureParameters;

typedef struct _MF_Face
{
	glm::vec3 verts[3];

	char* textureName;
	MF_FaceTextureParameters texture;
} MF_Face;

typedef struct _MF_Brush
{
	size_t totalFaces;
	MF_Face* faces;
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