#include "pch.h"
#include "CppUnitTest.h"

#include "FileReading.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MapFiledllTest
{
	TEST_CLASS(MapFileDLLParsing)
	{
	public:
		
		TEST_METHOD(MF_CountEntities_Test)
		{
			Assert::IsTrue(MF_LoadMap("", NULL) == MF_LOAD_MEMORY_ERROR);
		}
	};
}
