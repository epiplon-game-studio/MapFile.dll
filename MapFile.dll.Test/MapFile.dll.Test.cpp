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
	};
}