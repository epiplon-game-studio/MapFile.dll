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

		/* MF_CountEntities */
		TEST_METHOD(MF_CountEntities_NULL_Data)
		{
			Assert::AreEqual(
				MF_COUNT_ENTITIES_NULL_OR_EMPTY_TEXT,
				_MF_CountEntities(NULL, NULL)
			);
		}

		TEST_METHOD(MF_CountEntities_NULL_brushstart)
		{
			Assert::AreEqual(
				MF_COUNT_ENTITIES_NULL_BRUSHSTART,
				_MF_CountEntities("{}", NULL)
			);
		}

		TEST_METHOD(MF_CountEntities_Dangling_Bracket)
		{
			char* brushStart;
			Assert::AreEqual(
				MF_COUNT_ENTITIES_DANGLING_OPEN_BRACKET,
				_MF_CountEntities("{}\n{", &brushStart)
			);
		}

		TEST_METHOD(MF_CountEntities_Headless_Bracket)
		{
			char* brushStart;
			Assert::AreEqual(
				MF_COUNT_ENTITIES_DANGLING_CLOSE_BRACKET,
				_MF_CountEntities("}\n}", &brushStart)
			);
		}

		TEST_METHOD(MF_CountEntities_Embedded_Brushes)
		{
			char* brushStart;
			const char* text =	"//TestComment\n"
								"{\n"
								"\"classname\": \"worldspawn\"\n"
								"{}\n"
								"}\n"
								"{}\n"
								"{}";
			Assert::AreEqual(3, _MF_CountEntities(text, &brushStart));
			Assert::AreEqual(text + 14, brushStart);
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
		TEST_METHOD(MF_ReadFile_OK)
		{
			char* data = NULL;
			size_t size = 0;

			Assert::AreEqual(
				MF_LOAD_OK,
				_MF_ReadFile(
					"Sample/RandomData.map",
					&data,
					&size
				)
			);

			Assert::AreEqual((size_t)12, size);
			Assert::AreEqual(0, strcmp(data, "HELLO WORLD"));
		}
	};
}