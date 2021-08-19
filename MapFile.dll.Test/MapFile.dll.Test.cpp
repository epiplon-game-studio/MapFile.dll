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
		TEST_METHOD(MF_LoadMap_Path_NULL)
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
		}

		/* MF_CountEntities */
		TEST_METHOD(MF_CountEntities_NULL_Data)
		{
			Assert::AreEqual(-1, _MF_CountEntities(NULL, NULL));
		}

		TEST_METHOD(MF_CountEntities_NULL_brushstart)
		{
			Assert::AreEqual(-1, _MF_CountEntities("{}", NULL));
		}

		TEST_METHOD(MF_CountEntities_Dangling_Bracket)
		{
			char* brushStart;
			Assert::AreEqual(-1, _MF_CountEntities("{}\n{", &brushStart));
		}

		TEST_METHOD(MF_CountEntities_Headless_Bracket)
		{
			char* brushStart;
			Assert::AreEqual(-1, _MF_CountEntities("}\n}", &brushStart));
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
			Assert::AreEqual(3, _MF_CountEntities("//TestComment\n{\n\"classname\":\"worldspawn\"\n{}\n}{}{}", &brushStart));
			Assert::AreEqual(text + 15, brushStart);
		}
	};
}