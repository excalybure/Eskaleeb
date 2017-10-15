#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Yal/Assembler.h"
#include "../Yal/Tokenizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS( AssemblerTests )
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

		TEST_METHOD( TestLoadImediate )
		{
			std::string text( "ldi r10, 100" );
			std::string expectedResult( "ldi\tr10, 100\n" );
			std::string disassembledText;
			std::vector< uint8_t > byteCode;
			
			Yal::Assembler::Assemble( text, byteCode );
			Yal::Assembler::Disassemble( byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "ldi\tr10, 100\n" );
		}
	};
}