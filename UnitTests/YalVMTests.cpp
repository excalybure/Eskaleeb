#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Yal/Assembler.h"
#include "../Yal/Tokenizer.h"
#include "../Yal/YalVM.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS( YalVMTests )
	{
	public:
		TEST_CLASS_INITIALIZE( Init )
		{
			Yal::Lexer::TokenizerInitialize();
		}

		TEST_CLASS_CLEANUP( Cleanup )
		{
			Yal::Lexer::TokenizerShutdown();
		}

		TEST_METHOD( TestLoadImmediate )
		{
			context.source = "ldi r0, 100";

			Yal::Assembler::Assemble( context );
			
			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 100 == vm.getRegisterValue( 0 ) );
		}
	
	private:
		Yal::Assembler::Context context;
		Yal::VM vm;
	};
}