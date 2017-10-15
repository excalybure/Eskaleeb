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

		TEST_METHOD( TestStackPointerAdd )
		{
			context.source = "spadd 100";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context.byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "spadd\t100\n" );
		}

		TEST_METHOD( TestStackPointerSubtract )
		{
			context.source = "spsub 100";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context.byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "spsub\t100\n" );
		}

		TEST_METHOD( TestAddingCreatingUInt8Variable )
		{
			context.source = "uint8 foo = 123";

			Yal::Assembler::Assemble( context );

			Assert::IsTrue( context.data.size() == 1 );
			Assert::IsTrue( context.data[0] == 123 );
		}

		TEST_METHOD( TestAddingCreatingUInt16Variable )
		{
			context.source = "uint16 foo = 12345";

			Yal::Assembler::Assemble( context );

			Assert::IsTrue( context.data.size() == 2 );
			Assert::IsTrue( *reinterpret_cast< uint16_t * >( &context.data[0] ) == 12345 );
		}

		TEST_METHOD( TestAddingCreatingUInt32Variable )
		{
			context.source = "uint32 foo = 123456789";

			Yal::Assembler::Assemble( context );
			
			Assert::IsTrue( context.data.size() == 4 );
			Assert::IsTrue( *reinterpret_cast< uint32_t * >( &context.data[0] ) == 123456789 );
		}

		TEST_METHOD( TestAddingCreatingUInt64Variable )
		{
			context.source = "uint64 foo = 123456789123";

			Yal::Assembler::Assemble( context );

			Assert::IsTrue( context.data.size() == 8 );
			Assert::IsTrue( *reinterpret_cast< uint64_t * >( &context.data[0] ) == 123456789123 );
		}

		TEST_METHOD( TestAddingCreatingInt8Variable )
		{
			context.source = "int8 foo = -123";

			Yal::Assembler::Assemble( context );

			Assert::IsTrue( context.data.size() == 1 );
			Assert::IsTrue( *reinterpret_cast< int8_t * >( &context.data[0] ) == -123 );
		}

		TEST_METHOD( TestAddingCreatingInt16Variable )
		{
			context.source = "int16 foo = -12345";

			Yal::Assembler::Assemble( context );

			Assert::IsTrue( context.data.size() == 2 );
			Assert::IsTrue( *reinterpret_cast< int16_t * >( &context.data[0] ) == -12345 );
		}

		TEST_METHOD( TestAddingCreatingInt32Variable )
		{
			context.source = "int32 foo = -123456789";

			Yal::Assembler::Assemble( context );

			Assert::IsTrue( context.data.size() == 4 );
			Assert::IsTrue( *reinterpret_cast< int32_t * >( &context.data[0] ) == -123456789 );
		}

		TEST_METHOD( TestAddingCreatingInt64Variable )
		{
			context.source = "int64 foo = -123456789123";

			Yal::Assembler::Assemble( context );

			Assert::IsTrue( context.data.size() == 8 );
			Assert::IsTrue( *reinterpret_cast< int64_t * >( &context.data[0] ) == -123456789123 );
		}

		TEST_METHOD( TestExceptionThrownOnNegativeNumberWhenOnlyPositiveNumbersAllowed )
		{
			context.source = "uint32 foo = -123456789";

			Assert::ExpectException< std::exception >( [&] { Yal::Assembler::Assemble( context ); } );
		}

		TEST_METHOD( TestExceptionThrownOnTooBigNumber)
		{
			context.source = "uint8 foo = 123456789";

			Assert::ExpectException< std::exception >( [&] { Yal::Assembler::Assemble( context ); } );
		}

		TEST_METHOD( TestLoadEffectiveAddress )
		{
			context.source = "uint32 foo = 123456789\nlea r10, foo";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context.byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "lea\tr10, 0\n" );
		}

		TEST_METHOD( TestLoadImmediate )
		{
			context.source = "ldi r10, 100";
			
			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context.byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "ldi\tr10, 100\n" );
		}

		TEST_METHOD( TestLoad )
		{
			context.source = "ld r10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context.byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "ld\tr10, r11\n" );
		}

		TEST_METHOD( TestStore )
		{
			context.source = "st r10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context.byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "st\tr10, r11\n" );
		}

		TEST_METHOD( TestPush )
		{
			context.source = "push r10";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context.byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "push\tr10\n" );
		}

		TEST_METHOD( TestPop )
		{
			context.source = "pop r10";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context.byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "pop\tr10\n" );
		}

		TEST_METHOD( TestMove )
		{
			context.source = "mv r10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context.byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "mv\tr10, r11\n" );
		}

		TEST_METHOD( TestAnd )
		{
			context.source = "and r10, r11, r12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context.byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "and\tr10, r11, r12\n" );
		}

		TEST_METHOD( TestOr )
		{
			context.source = "or r10, r11, r12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context.byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "or\tr10, r11, r12\n" );
		}

		TEST_METHOD( TestXor )
		{
			context.source = "xor r10, r11, r12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context.byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "xor\tr10, r11, r12\n" );
		}

		TEST_METHOD( TestComplement )
		{
			context.source = "compl r10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context.byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "compl\tr10, r11\n" );
		}

		TEST_METHOD( TestLogicalAnd )
		{
			context.source = "land r10, r11, r12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context.byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "land\tr10, r11, r12\n" );
		}

		TEST_METHOD( TestLogicalOr )
		{
			context.source = "lor r10, r11, r12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context.byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "lor\tr10, r11, r12\n" );
		}

		TEST_METHOD( TestLogicalXor )
		{
			context.source = "lxor r10, r11, r12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context.byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "lxor\tr10, r11, r12\n" );
		}

		TEST_METHOD( TestLogicalNot )
		{
			context.source = "lnot r10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context.byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "lnot\tr10, r11\n" );
		}

	private:
		Yal::Assembler::Context context;
		std::string disassembledText;
	};
}