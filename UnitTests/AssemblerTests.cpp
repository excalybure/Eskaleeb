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
			std::string text( "spadd 100" );

			Yal::Assembler::Assemble( text, byteCode, data );
			Yal::Assembler::Disassemble( byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "spadd\t100\n" );
		}

		TEST_METHOD( TestStackPointerSubtract )
		{
			std::string text( "spsub 100" );

			Yal::Assembler::Assemble( text, byteCode, data );
			Yal::Assembler::Disassemble( byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "spsub\t100\n" );
		}

		TEST_METHOD( TestAddingCreatingUInt8Variable )
		{
			std::string text( "uint8 foo = 123" );

			Yal::Assembler::Assemble( text, byteCode, data );

			Assert::IsTrue( data.size() == 1 );
			Assert::IsTrue( data[0] == 123 );
		}

		TEST_METHOD( TestAddingCreatingUInt16Variable )
		{
			std::string text( "uint16 foo = 12345" );

			Yal::Assembler::Assemble( text, byteCode, data );

			Assert::IsTrue( data.size() == 2 );
			Assert::IsTrue( *reinterpret_cast< uint16_t * >( &data[0] ) == 12345 );
		}

		TEST_METHOD( TestAddingCreatingUInt32Variable )
		{
			std::string text( "uint32 foo = 123456789" );

			Yal::Assembler::Assemble( text, byteCode, data );
			
			Assert::IsTrue( data.size() == 4 );
			Assert::IsTrue( *reinterpret_cast< uint32_t * >( &data[0] ) == 123456789 );
		}

		TEST_METHOD( TestAddingCreatingUInt64Variable )
		{
			std::string text( "uint64 foo = 123456789123" );

			Yal::Assembler::Assemble( text, byteCode, data );

			Assert::IsTrue( data.size() == 8 );
			Assert::IsTrue( *reinterpret_cast< uint64_t * >( &data[0] ) == 123456789123 );
		}

		TEST_METHOD( TestAddingCreatingInt8Variable )
		{
			std::string text( "int8 foo = -123" );

			Yal::Assembler::Assemble( text, byteCode, data );

			Assert::IsTrue( data.size() == 1 );
			Assert::IsTrue( *reinterpret_cast< int8_t * >( &data[0] ) == -123 );
		}

		TEST_METHOD( TestAddingCreatingInt16Variable )
		{
			std::string text( "int16 foo = -12345" );

			Yal::Assembler::Assemble( text, byteCode, data );

			Assert::IsTrue( data.size() == 2 );
			Assert::IsTrue( *reinterpret_cast< int16_t * >( &data[0] ) == -12345 );
		}

		TEST_METHOD( TestAddingCreatingInt32Variable )
		{
			std::string text( "int32 foo = -123456789" );

			Yal::Assembler::Assemble( text, byteCode, data );

			Assert::IsTrue( data.size() == 4 );
			Assert::IsTrue( *reinterpret_cast< int32_t * >( &data[0] ) == -123456789 );
		}

		TEST_METHOD( TestAddingCreatingInt64Variable )
		{
			std::string text( "int64 foo = -123456789123" );

			Yal::Assembler::Assemble( text, byteCode, data );

			Assert::IsTrue( data.size() == 8 );
			Assert::IsTrue( *reinterpret_cast< int64_t * >( &data[0] ) == -123456789123 );
		}

		TEST_METHOD( TestExceptionThrownOnNegativeNumberWhenOnlyPositiveNumbersAllowed )
		{
			std::string text( "uint32 foo = -123456789" );

			Assert::ExpectException< std::exception >( [&] { Yal::Assembler::Assemble( text, byteCode, data ); } );
		}

		TEST_METHOD( TestExceptionThrownOnTooBigNumber)
		{
			std::string text( "uint8 foo = 123456789" );

			Assert::ExpectException< std::exception >( [&] { Yal::Assembler::Assemble( text, byteCode, data ); } );
		}

		TEST_METHOD( TestLoadEffectiveAddress )
		{
			std::string text( "lea r10, foo" );

			Yal::Assembler::Assemble( text, byteCode, data );
			Yal::Assembler::Disassemble( byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "lea\tr10, foor\n" );
		}

		TEST_METHOD( TestLoadImmediate )
		{
			std::string text( "ldi r10, 100" );
			
			Yal::Assembler::Assemble( text, byteCode, data );
			Yal::Assembler::Disassemble( byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "ldi\tr10, 100\n" );
		}

		TEST_METHOD( TestLoad )
		{
			std::string text( "ld r10, r11" );

			Yal::Assembler::Assemble( text, byteCode, data );
			Yal::Assembler::Disassemble( byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "ld\tr10, r11\n" );
		}

		TEST_METHOD( TestStore )
		{
			std::string text( "st r10, r11" );

			Yal::Assembler::Assemble( text, byteCode, data );
			Yal::Assembler::Disassemble( byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "st\tr10, r11\n" );
		}

		TEST_METHOD( TestPush )
		{
			std::string text( "push r10" );

			Yal::Assembler::Assemble( text, byteCode, data );
			Yal::Assembler::Disassemble( byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "push\tr10\n" );
		}

		TEST_METHOD( TestPop )
		{
			std::string text( "pop r10" );

			Yal::Assembler::Assemble( text, byteCode, data );
			Yal::Assembler::Disassemble( byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "pop\tr10\n" );
		}

		TEST_METHOD( TestMove )
		{
			std::string text( "mv r10, r11" );

			Yal::Assembler::Assemble( text, byteCode, data );
			Yal::Assembler::Disassemble( byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "mv\tr10, r11\n" );
		}

		TEST_METHOD( TestAnd )
		{
			std::string text( "and r10, r11, r12" );

			Yal::Assembler::Assemble( text, byteCode, data );
			Yal::Assembler::Disassemble( byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "and\tr10, r11, r12\n" );
		}

		TEST_METHOD( TestOr )
		{
			std::string text( "or r10, r11, r12" );

			Yal::Assembler::Assemble( text, byteCode, data );
			Yal::Assembler::Disassemble( byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "or\tr10, r11, r12\n" );
		}

		TEST_METHOD( TestXor )
		{
			std::string text( "xor r10, r11, r12" );

			Yal::Assembler::Assemble( text, byteCode, data );
			Yal::Assembler::Disassemble( byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "xor\tr10, r11, r12\n" );
		}

		TEST_METHOD( TestComplement )
		{
			std::string text( "compl r10, r11" );

			Yal::Assembler::Assemble( text, byteCode, data );
			Yal::Assembler::Disassemble( byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "compl\tr10, r11\n" );
		}

		TEST_METHOD( TestLogicalAnd )
		{
			std::string text( "land r10, r11, r12" );

			Yal::Assembler::Assemble( text, byteCode, data );
			Yal::Assembler::Disassemble( byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "land\tr10, r11, r12\n" );
		}

		TEST_METHOD( TestLogicalOr )
		{
			std::string text( "lor r10, r11, r12" );

			Yal::Assembler::Assemble( text, byteCode, data );
			Yal::Assembler::Disassemble( byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "lor\tr10, r11, r12\n" );
		}

		TEST_METHOD( TestLogicalXor )
		{
			std::string text( "lxor r10, r11, r12" );

			Yal::Assembler::Assemble( text, byteCode, data );
			Yal::Assembler::Disassemble( byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "lxor\tr10, r11, r12\n" );
		}

		TEST_METHOD( TestLogicalNot )
		{
			std::string text( "lnot r10, r11" );

			Yal::Assembler::Assemble( text, byteCode, data );
			Yal::Assembler::Disassemble( byteCode, disassembledText );

			Assert::IsTrue( disassembledText == "lnot\tr10, r11\n" );
		}

	private:
		std::string disassembledText;
		std::vector< uint8_t > byteCode;
		std::vector< uint8_t > data;
	};
}