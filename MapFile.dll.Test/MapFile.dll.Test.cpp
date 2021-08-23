#include "CppUnitTest.h"
#include "FileReading.h"

#include "ToString.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MapFiledllTest
{
	TEST_CLASS(MapFileDLLParsing)
	{
	public:
		/* MF_LoadMap */
		/*TEST_METHOD(MF_LoadMap_Path_NULL)
		{
			Assert::AreEqual(MF_LOAD_FILE_NOT_FOUND, MF_LoadMap(NULL, NULL));
		}

		TEST_METHOD(MF_LoadMap_Path_FNF)
		{
			Assert::AreEqual(MF_LOAD_FILE_NOT_FOUND, MF_LoadMap("C:\\BullshitFile.map", NULL));
		}

		TEST_METHOD(MF_LoadMap_MapPtr_NULL)
		{
			Assert::AreEqual(MF_LOAD_MEMORY_ERROR, MF_LoadMap("Sample\\Empty.map", NULL));
		}*/



		TEST_METHOD(_MF_ParseAllEntityBrushes_OK)
		{
			const char* text =	"{\n"
								"\"classname\" \"worldspawn\"\n"
								"\"name\" \"John\"\n"
								"{\n"
								"( -80 112 32 ) ( -144 0 32 ) ( -144 0 0 ) grass001_1024 96 -32 0 1 1\n"
								"( -144 0 32 ) ( 0 0 32 ) ( 0 0 0 ) grass001_1024 0 -32 0 1 1\n"
								"(-144 0 0) (0 0 0) (-64 112 0) grass001_1024 0 -96 0 1 1\n"
								"(-64 112 32) (0 0 32) (-144 0 32) grass001_1024 0 -96 0 1 1\n"
								"(-64 112 0) (-64 112 32) (-80 112 32) grass001_1024 0 -32 0 1 1\n"
								"(0 0 0) (0 0 32) (-64 112 32) grass001_1024 96 -32 0 1 1\n"
								"}\n"
								"{\n"
								"(48 48 64) (48 64 16) (48 64 64) Wood_planks001_512 0 0 90 0.1 0.1\n"
								"(144 48 64) (48 48 16) (48 48 64) Wood_planks001_512 0 0 90 0.1 0.1\n"
								"(144 64 16) (48 48 16) (144 48 16) Wood_planks001_512 0 0 90 0.1 0.1\n"
								"(144 64 64) (48 48 64) (48 64 64) Wood_planks001_512 0 0 90 0.1 0.1\n"
								"(144 64 64) (48 64 16) (144 64 16) Wood_planks001_512 0 0 90 0.1 0.1\n"
								"(144 64 64) (144 48 16) (144 48 64) Wood_planks001_512 0 0 90 0.1 0.1\n"
								"}\n"
								"}\n";
			MF_Entity entity = { 0 };
			char* end;
			MF_ParseStatus status = _MF_ParseAllEntityBrushes(text, &entity, &end);
			Assert::AreEqual(MF_PARSE_OK, status);

			Assert::AreEqual(entity.totalBrushes, (size_t)2);
			Assert::AreEqual(entity.brushes[1].faces[3].textureName, "Wood_planks001_512");
			Assert::AreEqual(entity.brushes[1].faces[2].verts[2].y, 48.0f);
		}
		
		TEST_METHOD(_MF_StartParseBrush_OK)
		{
			const char* text =	"{\n"
								"\"classname\" \"worldspawn\"\n"
								"\"name\" \"John\"\n"
								"{\n"
								"( -80 112 32 ) ( -144 0 32 ) ( -144 0 0 ) grass001_1024 96 -32 0 1 1\n"
								"( -144 0 32 ) ( 0 0 32 ) ( 0 0 0 ) grass001_1024 0 -32 0 1 1\n"
								"(-144 0 0) (0 0 0) (-64 112 0) grass001_1024 0 -96 0 1 1\n"
								"(-64 112 32) (0 0 32) (-144 0 32) grass001_1024 0 -96 0 1 1\n"
								"(-64 112 0) (-64 112 32) (-80 112 32) grass001_1024 0 -32 0 1 1\n"
								"(0 0 0) (0 0 32) (-64 112 32) grass001_1024 96 -32 0 1 1\n"
								"}\n"
								"{\n"
								"(48 48 64) (48 64 16) (48 64 64) Wood_planks001_512 0 0 90 0.1 0.1\n"
								"(144 48 64) (48 48 16) (48 48 64) Wood_planks001_512 0 0 90 0.1 0.1\n"
								"(144 64 16) (48 48 16) (144 48 16) Wood_planks001_512 0 0 90 0.1 0.1\n"
								"(144 64 64) (48 48 64) (48 64 64) Wood_planks001_512 0 0 90 0.1 0.1\n"
								"(144 64 64) (48 64 16) (144 64 16) Wood_planks001_512 0 0 90 0.1 0.1\n"
								"(144 64 64) (144 48 16) (144 48 64) Wood_planks001_512 0 0 90 0.1 0.1\n"
								"}\n"
								"}\n";
			MF_Brush brush = { 0 };
			char* c;
			MF_ParseStatus status = _MF_StartParseBrush(text, &brush, &c);
			Assert::AreEqual(status, MF_PARSE_OK);

			Assert::AreEqual((size_t)6, brush.totalFaces);
			
			// choose some random fields
			Assert::AreEqual("grass001_1024", brush.faces[3].textureName);
			Assert::AreEqual(-64.0f, brush.faces[2].verts[2].x);
			Assert::AreEqual(112.0f, brush.faces[5].verts[2].y);
		}

		TEST_METHOD(_MF_StartParseFace_OK)
		{
			const char* text =	"( -80 112 32 ) ( -144 0 32 ) ( -144 0 0 ) grass001_1024 96 -32 0 1 1\n"
								"( -144 0 32 ) ( 0 0 32 ) ( 0 0 0 ) grass001_1024 0 -32 0 1 1\n"
								"(-144 0 0) (0 0 0) (-64 112 0) grass001_1024 0 - 96 0 1 1\n"
								"(-64 112 32) (0 0 32) (-144 0 32) grass001_1024 0 - 96 0 1 1\n"
								"(-64 112 0) (-64 112 32) (-80 112 32) grass001_1024 0 - 32 0 1 1\n"
								"(0 0 0) (0 0 32) (-64 112 32) grass001_1024 96 - 32 0 1 1;\n";
			MF_Face face;
			char* end = NULL;
			MF_ParseStatus status = _MF_StartParseFace(text, &face, &end);
			Assert::AreEqual(MF_PARSE_OK, status);

			Assert::AreEqual(-80.0f, face.verts[0].x);
			Assert::AreEqual(112.0f, face.verts[0].y);
			Assert::AreEqual(32.0f, face.verts[0].z);

			Assert::AreEqual(-144.0f, face.verts[1].x);
			Assert::AreEqual(0.0f, face.verts[1].y);
			Assert::AreEqual(32.0f, face.verts[1].z);

			Assert::AreEqual(-144.0f, face.verts[2].x);
			Assert::AreEqual(0.0f, face.verts[2].y);
			Assert::AreEqual(0.0f, face.verts[2].z);

			Assert::AreEqual("grass001_1024", face.textureName);

			Assert::AreEqual(96.0f, face.texture.offsetX);
			Assert::AreEqual(-32.0f, face.texture.offsetY);
			Assert::AreEqual(0.0f, face.texture.rotation);
			Assert::AreEqual(1.0f, face.texture.scaleX);
			Assert::AreEqual(1.0f, face.texture.scaleY);
		}

		/* MF_CountEntities */
		TEST_METHOD(MF_CountEntities_NULL_Data)
		{
			Assert::AreEqual(
				MF_COUNT_NULL_OR_EMPTY_TEXT,
				_MF_CountEntities(NULL)
			);
		}


		TEST_METHOD(MF_CountEntities_Dangling_Bracket)
		{
			Assert::AreEqual(
				MF_COUNT_DANGLING_OPEN_BRACKET,
				_MF_CountEntities("{}\n{")
			);
		}

		TEST_METHOD(MF_CountEntities_Headless_Bracket)
		{
			Assert::AreEqual(
				MF_COUNT_DANGLING_CLOSE_BRACKET,
				_MF_CountEntities("}\n}")
			);
		}

		TEST_METHOD(MF_CountEntities_Embedded_Brushes)
		{
			const char* text =	"//TestComment\n"
								"{\n"
								"\"classname\": \"worldspawn\"\n"
								"{}\n"
								"}\n"
								"{}\n"
								"{}";
			Assert::AreEqual(3, _MF_CountEntities(text));
		}

		// MF_ReadFile

		TEST_METHOD(MF_ReadFile_NULL_Path)
		{
			Assert::AreEqual(MF_LOAD_FILE_NOT_FOUND, _MF_ReadFile(NULL, NULL, NULL));
		}

		TEST_METHOD(MF_ReadFile_NULL_Data)
		{
			Assert::AreEqual(
				MF_LOAD_MEMORY_ERROR,
				_MF_ReadFile(
					"BullshitFile.map",
					NULL, NULL
				)
			);
		}

		TEST_METHOD(MF_ReadFile_NULL_Size)
		{
			char* data = NULL;
			Assert::AreEqual(
				MF_LOAD_MEMORY_ERROR,
				_MF_ReadFile(
					"BullshitFile.map",
					&data,
					NULL
				)
			);
		}

		TEST_METHOD(MF_ReadFile_FileNotFound)
		{
			char* data = NULL;
			size_t size = 0;

			Assert::AreEqual(
				MF_LOAD_FILE_NOT_FOUND,
				_MF_ReadFile(
					"BullshitFile.map",
					&data,
					&size
				)
			);
		}

		TEST_METHOD(_MF_CountProperties_OK)
		{
			const char* text =	"{\n"
								"\"classname\" \"info_player_start\"\n"
								"\"health\" \"100\"\n"
								"{\n"
								"\"IgnoreThis\" \"Ignore\"\n"
								"(-64 - 64 - 16) (-64 - 63 - 16) (-64 - 64 - 15) __TB_empty 0 0 0 1 1\n"
								"(-64 - 64 - 16) (-64 - 64 - 15) (-63 - 64 - 16) __TB_empty 0 0 0 1 1\n"
								"(-64 - 64 - 16) (-63 - 64 - 16) (-64 - 63 - 16) __TB_empty 0 0 0 1 1\n"
								"}\n"
								"\"name\" \"ShortRound\"\n"
								"\"name2\" \"ShortRound2\"\n"
								"}\n"
								"\"name3\" \"ShortRound3 Ignore this \"\n";
					
			Assert::AreEqual(4, _MF_CountProperties(text));
		}

		TEST_METHOD(_MF_StartParseProperty_OK)
		{
			const char* text =	"{\n"
								"\"classname\" \"info_player_start\"\n"
								"\"name\" \"john\"\n"
								"}";
			MF_EntityProperty property;
			char* end = NULL;
			MF_ParseStatus status = _MF_StartParseProperty(text + 2, &property, &end);
			Assert::AreEqual((int)status, (int)MF_PARSE_OK);

			Assert::AreEqual(0, strcmp(property.key, "classname"));
			Assert::AreEqual(0, strcmp(property.value, "info_player_start"));

		}

		TEST_METHOD(_MF_ParseAllEntityProperties_OK)
		{
			const char* text = "{\n"
				"\"classname\" \"info_player_start\"\n\n"
				"\"name\" \"john\"\n"
				"}";
			MF_Entity entity;
			MF_ParseStatus status = _MF_ParseAllEntityProperties(text, &entity);
			Assert::AreEqual((int)status, (int)MF_PARSE_OK);

			Assert::AreEqual(2, (int)entity.totalProperties);

			Assert::AreEqual(0, strcmp(entity.properties[0].key, "classname"));
			Assert::AreEqual(0, strcmp(entity.properties[0].value, "info_player_start"));


			Assert::AreEqual(0, strcmp(entity.properties[1].key, "name"));
			Assert::AreEqual(0, strcmp(entity.properties[1].value, "john"));
		}

		/* This is the big one */
		TEST_METHOD(MF_LoadMap_OK)
		{
			MF_Map map;
			MF_LoadStatus status = MF_LoadMap("Sample/Test.map", &map);
			Assert::AreEqual(MF_LOAD_OK, status);

		}
	};
}