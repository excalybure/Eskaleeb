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
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "spadd 100\n" );
		}

		TEST_METHOD( TestStackPointerSubtract )
		{
			context.source = "spsub 100";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "spsub 100\n" );
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

		TEST_METHOD( TestExceptionThrownIfAssignmentOperatorIsMissing )
		{
			context.source = "uint8 foo 123456789";

			Assert::ExpectException< std::exception >( [&] { Yal::Assembler::Assemble( context ); } );
		}

		TEST_METHOD( TestExceptionThrownWhenCreatingTwoVariablesByTheSameName )
		{
			context.source = "uint32 foo = 123456789\nuint32 foo = 123456789";

			Assert::ExpectException< std::exception >( [&] { Yal::Assembler::Assemble( context ); } );
		}

		TEST_METHOD( TestLoadEffectiveAddress )
		{
			context.source = "uint32 foo = 123456789\nlea r10, foo";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "lea r10, foo\n" );
		}

		TEST_METHOD( TestExceptionIsThrownTryingToReferenceNonExistentVariable )
		{
			context.source = "uint32 foo = 123456789\nlea r10, bar";

			Assert::ExpectException< std::exception >( [&] { Yal::Assembler::Assemble( context ); } );
		}

		TEST_METHOD( TestLoadImmediate )
		{
			context.source = "ldi r10, 100";
			
			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "ldi r10, 100\n" );
		}

		TEST_METHOD( TestLoadImmediateWithByteRegister )
		{
			context.source = "ldi rb10, 100";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "ldi rb10, 100\n" );
		}

		TEST_METHOD( TestLoadImmediateWithWordRegister )
		{
			context.source = "ldi rw10, 1000";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "ldi rw10, 1000\n" );
		}

		TEST_METHOD( TestLoadImmediateWithDWordRegister )
		{
			context.source = "ldi rd10, 100000";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "ldi rd10, 100000\n" );
		}

		TEST_METHOD( TestLoadImmediateThrowsExceptionWhenValueDoesNotFitInRegister )
		{
			context.source = "ldi rb10, 1000";

			Assert::ExpectException< std::exception >( [&] { Yal::Assembler::Assemble( context ); } );
		}

		TEST_METHOD( TestLoadImmediateAcceptsNegativeNumbers )
		{
			context.source = "ldi rb10, -100";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "ldi rb10, -100\n" );
		}

		TEST_METHOD( TestLoadImmediateWithUnsignedByte )
		{
			context.source = "ldi rbu10, 255";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "ldi rbu10, 255\n" );
		}

		TEST_METHOD( TestLoadImmediateWithUnsignedWord )
		{
			context.source = "ldi rwu10, 65535";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "ldi rwu10, 65535\n" );
		}

		TEST_METHOD( TestLoadImmediateWithUnsignedDword )
		{
			context.source = "ldi rdu10, 4294967295";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "ldi rdu10, 4294967295\n" );
		}

		TEST_METHOD( TestLoadImmediateWithUnsignedNative )
		{
			context.source = "ldi ru10, 9223372036854775807";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "ldi ru10, 9223372036854775807\n" );
		}

		TEST_METHOD( TestLoad )
		{
			context.source = "ld r10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "ld r10, r11\n" );
		}

		TEST_METHOD( TestStore )
		{
			context.source = "st r10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "st r10, r11\n" );
		}

		TEST_METHOD( TestPush )
		{
			context.source = "push r10";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "push r10\n" );
		}

		TEST_METHOD( TestPop )
		{
			context.source = "pop r10";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "pop r10\n" );
		}

		TEST_METHOD( TestMove )
		{
			context.source = "mv r10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "mv r10, r11\n" );
		}

		TEST_METHOD( TestAnd )
		{
			context.source = "and r10, r11, r12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "and r10, r11, r12\n" );
		}

		TEST_METHOD( TestOr )
		{
			context.source = "or r10, r11, r12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "or r10, r11, r12\n" );
		}

		TEST_METHOD( TestXor )
		{
			context.source = "xor r10, r11, r12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "xor r10, r11, r12\n" );
		}

		TEST_METHOD( TestComplement )
		{
			context.source = "compl r10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "compl r10, r11\n" );
		}

		TEST_METHOD( TestLogicalAnd )
		{
			context.source = "land r10, r11, r12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "land r10, r11, r12\n" );
		}

		TEST_METHOD( TestLogicalOr )
		{
			context.source = "lor r10, r11, r12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "lor r10, r11, r12\n" );
		}

		TEST_METHOD( TestLogicalXor )
		{
			context.source = "lxor r10, r11, r12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "lxor r10, r11, r12\n" );
		}

		TEST_METHOD( TestLogicalNot )
		{
			context.source = "lnot r10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "lnot r10, r11\n" );
		}

		TEST_METHOD( TestJump )
		{
			context.source = ":foo\njmp foo";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == ":foo\njmp foo\n" );
		}

		TEST_METHOD( TestJumpIfTrue )
		{
			context.source = ":foo\njmpt foo";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == ":foo\njmpt foo\n" );
		}

		TEST_METHOD( TestExceptionIsThrownOnUnrecognizedInstruction )
		{
			context.source = "foo bar";

			Assert::ExpectException< std::exception >( [&] { Yal::Assembler::Assemble( context ); } );
		}

		TEST_METHOD( TestCompareEqual )
		{
			context.source = "cmpeq r10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "cmpeq r10, r11\n" );
		}

		TEST_METHOD( TestCompareLessThan )
		{
			context.source = "cmplt r10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "cmplt r10, r11\n" );
		}

		TEST_METHOD( TestCompareLessEqual )
		{
			context.source = "cmple r10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "cmple r10, r11\n" );
		}

		TEST_METHOD( TestCompareGreaterThan )
		{
			context.source = "cmpgt r10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "cmpgt r10, r11\n" );
		}

		TEST_METHOD( TestCompareGreaterEqual )
		{
			context.source = "cmpge r10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "cmpge r10, r11\n" );
		}

		TEST_METHOD( TestAdd )
		{
			context.source = "add r10, r11, r12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "add r10, r11, r12\n" );
		}

		TEST_METHOD( TestSub )
		{
			context.source = "sub r10, r11, r12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "sub r10, r11, r12\n" );
		}

		TEST_METHOD( TestMul )
		{
			context.source = "mul r10, r11, r12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "mul r10, r11, r12\n" );
		}

		TEST_METHOD( TestDiv )
		{
			context.source = "div r10, r11, r12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "div r10, r11, r12\n" );
		}

		TEST_METHOD( TestShiftRight )
		{
			context.source = "sr r10, r11, r12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "sr r10, r11, r12\n" );
		}

		TEST_METHOD( TestShiftLeft )
		{
			context.source = "sl r10, r11, r12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "sl r10, r11, r12\n" );
		}

		TEST_METHOD( TestInc )
		{
			context.source = "inc r10";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "inc r10\n" );
		}

		TEST_METHOD( TestDec )
		{
			context.source = "dec r10";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "dec r10\n" );
		}

		TEST_METHOD( TestAbs )
		{
			context.source = "abs r10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "abs r10, r11\n" );
		}

		TEST_METHOD( TestNeg )
		{
			context.source = "neg r10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "neg r10, r11\n" );
		}

		TEST_METHOD( TestCall )
		{
			context.source = ":foo\ncall foo";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == ":foo\ncall foo\n" );
		}

		TEST_METHOD( TestIndirectCall )
		{
			context.source = "calli r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "calli r11\n" );
		}

		TEST_METHOD( TestReturn )
		{
			context.source = "ret";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "ret\n" );
		}

		TEST_METHOD( TestLoadFloatingPointRegister )
		{
			context.source = "ld fr10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "ld fr10, r11\n" );
		}

		TEST_METHOD( TestLoadDoubleFloatingPointRegister )
		{
			context.source = "ld dfr10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "ld dfr10, r11\n" );
		}

		TEST_METHOD( TestLoadMalformedRegisterThrowsException )
		{
			context.source = "ld 10, r11";

			Assert::ExpectException< std::exception >( [&] { Yal::Assembler::Assemble( context ); } );
		}

		TEST_METHOD( TestLoadMalformedFloatingPointRegisterThrowsException )
		{
			context.source = "ld f10, r11";

			Assert::ExpectException< std::exception >( [&] { Yal::Assembler::Assemble( context ); } );
		}

		TEST_METHOD( TestLoadMalformedDoubleFloatingPointRegisterThrowsException )
		{
			context.source = "ld df10, r11";

			Assert::ExpectException< std::exception >( [&] { Yal::Assembler::Assemble( context ); } );
		}

		TEST_METHOD( TestFloatCompareEqual )
		{
			context.source = "fcmpeq fr10, fr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "fcmpeq fr10, fr11\n" );
		}

		TEST_METHOD( TestFloatCompareLessThan )
		{
			context.source = "fcmplt fr10, fr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "fcmplt fr10, fr11\n" );
		}

		TEST_METHOD( TestFloatCompareLessEqual )
		{
			context.source = "fcmple fr10, fr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "fcmple fr10, fr11\n" );
		}

		TEST_METHOD( TestFloatCompareGreaterThan )
		{
			context.source = "fcmpgt fr10, fr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "fcmpgt fr10, fr11\n" );
		}

		TEST_METHOD( TestFloatCompareGreaterEqual )
		{
			context.source = "fcmpge fr10, fr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "fcmpge fr10, fr11\n" );
		}

		TEST_METHOD( TestFloatAdd )
		{
			context.source = "fadd fr10, fr11, fr12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "fadd fr10, fr11, fr12\n" );
		}

		TEST_METHOD( TestFloatSub )
		{
			context.source = "fsub fr10, fr11, fr12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "fsub fr10, fr11, fr12\n" );
		}

		TEST_METHOD( TestFloatMul )
		{
			context.source = "fmul fr10, fr11, fr12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "fmul fr10, fr11, fr12\n" );
		}

		TEST_METHOD( TestFloatDiv )
		{
			context.source = "fdiv fr10, fr11, fr12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "fdiv fr10, fr11, fr12\n" );
		}

		TEST_METHOD( TestFloatAbs )
		{
			context.source = "fabs fr10, fr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "fabs fr10, fr11\n" );
		}

		TEST_METHOD( TestFloatNeg )
		{
			context.source = "fneg fr10, fr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "fneg fr10, fr11\n" );
		}

		TEST_METHOD( TestFloatSqrt )
		{
			context.source = "fsqrt fr10, fr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "fsqrt fr10, fr11\n" );
		}

		TEST_METHOD( TestFloatCos )
		{
			context.source = "fcos fr10, fr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "fcos fr10, fr11\n" );
		}

		TEST_METHOD( TestFloatSin )
		{
			context.source = "fsin fr10, fr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "fsin fr10, fr11\n" );
		}

		TEST_METHOD( TestFloatRnd )
		{
			context.source = "frnd fr10, fr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "frnd fr10, fr11\n" );
		}

		TEST_METHOD( TestFloatCast )
		{
			context.source = "fcast fr10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "fcast fr10, r11\n" );
		}

		TEST_METHOD( TestDoubleCompareEqual )
		{
			context.source = "dfcmpeq dfr10, dfr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "dfcmpeq dfr10, dfr11\n" );
		}

		TEST_METHOD( TestDoubleCompareLessThan )
		{
			context.source = "dfcmplt dfr10, dfr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "dfcmplt dfr10, dfr11\n" );
		}

		TEST_METHOD( TestDoubleCompareLessEqual )
		{
			context.source = "dfcmple dfr10, dfr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "dfcmple dfr10, dfr11\n" );
		}

		TEST_METHOD( TestDoubleCompareGreaterThan )
		{
			context.source = "dfcmpgt dfr10, dfr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "dfcmpgt dfr10, dfr11\n" );
		}

		TEST_METHOD( TestDoubleCompareGreaterEqual )
		{
			context.source = "dfcmpge dfr10, dfr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "dfcmpge dfr10, dfr11\n" );
		}

		TEST_METHOD( TestDoubleAdd )
		{
			context.source = "dfadd dfr10, dfr11, dfr12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "dfadd dfr10, dfr11, dfr12\n" );
		}

		TEST_METHOD( TestDoubleSub )
		{
			context.source = "dfsub dfr10, dfr11, dfr12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "dfsub dfr10, dfr11, dfr12\n" );
		}

		TEST_METHOD( TestDoubleMul )
		{
			context.source = "dfmul dfr10, dfr11, dfr12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "dfmul dfr10, dfr11, dfr12\n" );
		}

		TEST_METHOD( TestDoubleDiv )
		{
			context.source = "dfdiv dfr10, dfr11, dfr12";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "dfdiv dfr10, dfr11, dfr12\n" );
		}

		TEST_METHOD( TestDoubleAbs )
		{
			context.source = "dfabs dfr10, dfr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "dfabs dfr10, dfr11\n" );
		}

		TEST_METHOD( TestDoubleNeg )
		{
			context.source = "dfneg dfr10, dfr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "dfneg dfr10, dfr11\n" );
		}

		TEST_METHOD( TestDoubleSqrt )
		{
			context.source = "dfsqrt dfr10, dfr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "dfsqrt dfr10, dfr11\n" );
		}

		TEST_METHOD( TestDoubleCos )
		{
			context.source = "dfcos dfr10, dfr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "dfcos dfr10, dfr11\n" );
		}

		TEST_METHOD( TestDoubleSin )
		{
			context.source = "dfsin dfr10, dfr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "dfsin dfr10, dfr11\n" );
		}

		TEST_METHOD( TestDoubleRnd )
		{
			context.source = "dfrnd dfr10, dfr11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "dfrnd dfr10, dfr11\n" );
		}

		TEST_METHOD( TestDoubleCast )
		{
			context.source = "dfcast dfr10, r11";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "dfcast dfr10, r11\n" );
		}

		TEST_METHOD( TestAddingCreatingFloatPointVariable )
		{
			context.source = "float foo = 123.0f";

			Yal::Assembler::Assemble( context );

			Assert::IsTrue( context.data.size() == sizeof( float ) );
			Assert::IsTrue( *reinterpret_cast< float * >( &context.data[0] ) == 123.0f );
		}

		TEST_METHOD( TestAddingCreatingDoubleVariable )
		{
			context.source = "double foo = 123.0";

			Yal::Assembler::Assemble( context );

			Assert::IsTrue( context.data.size() == sizeof( double ) );
			Assert::IsTrue( *reinterpret_cast< double * >( &context.data[0] ) == 123.0 );
		}

		TEST_METHOD( TestExceptionThrownOnInvalidFloatRegister )
		{
			context.source = "frnd f10, fr11";

			Assert::ExpectException< std::exception >( [&] { Yal::Assembler::Assemble( context ); } );
		}

		TEST_METHOD( TestExceptionThrownOnInvalidDoubleRegister )
		{
			context.source = "dfrnd fr10, dfr11";

			Assert::ExpectException< std::exception >( [&] { Yal::Assembler::Assemble( context ); } );
		}

		TEST_METHOD( TestFloatLoadImmediate )
		{
			context.source = "fldi fr10, 100.0f";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "fldi fr10, 100.000000\n" );
		}

		TEST_METHOD( TestDoubleLoadImmediate )
		{
			context.source = "dfldi dfr10, 100.0";

			Yal::Assembler::Assemble( context );
			Yal::Assembler::Disassemble( context, disassembledText );

			Assert::IsTrue( disassembledText == "dfldi dfr10, 100.000000\n" );
		}

	private:
		Yal::Assembler::Context context;
		std::string disassembledText;
	};
}