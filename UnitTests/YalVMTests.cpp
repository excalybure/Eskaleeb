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

		TEST_METHOD( TestLoadNativeImmediate )
		{
			context.source = "ldi r0, -10000000";

			Yal::Assembler::Assemble( context );
			
			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( -10000000 == static_cast< int64_t >( vm.GetRegisterValue( 0 ) ) );
		}
	
		TEST_METHOD( TestLoadByteImmediate )
		{
			context.source = "ldi rb0, -100";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( -100 == static_cast< int64_t >( vm.GetRegisterValue( 0 ) ) );
		}

		TEST_METHOD( TestLoadWordImmediate )
		{
			context.source = "ldi rw0, -1000";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( -1000 == static_cast< int64_t >( vm.GetRegisterValue( 0 ) ) );
		}

		TEST_METHOD( TestLoadDWordImmediate )
		{
			context.source = "ldi rd0, -100000";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( -100000 == static_cast< int64_t >( vm.GetRegisterValue( 0 ) ) );
		}

		TEST_METHOD( TestLoadUnsignedNativeImmediate )
		{
			context.source = "ldi ru0, 10000000";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 10000000 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestLoadUnsignedByteImmediate )
		{
			context.source = "ldi rbu0, 100";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 100 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestLoadUnsignedWordImmediate )
		{
			context.source = "ldi rwu0, 1000";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 1000 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestLoadUnsignedDWordImmediate )
		{
			context.source = "ldi rdu0, 100000";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 100000 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestFloatLoadImmediate )
		{
			context.source = "fldi fr0, 100000";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 100000 == vm.GetFloatRegisterValue( 0 ) );
		}

		TEST_METHOD( TestDoubleLoadImmediate )
		{
			context.source = "dfldi dfr0, 100000";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 100000 == vm.GetFloatRegisterValue( 0 ) );
		}

		TEST_METHOD( TestMove )
		{
			context.source = "ldi r0, 100000\nmv r1, r0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 100000 == vm.GetRegisterValue( 1 ) );
		}

		TEST_METHOD( TestStore )
		{
			context.source = "int64 foo; lea r1, foo\nldi r0, 100000\nst r0, r1";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 100000 == *reinterpret_cast< const int64_t * >( &vm.GetData()[0] ) );
		}

		TEST_METHOD( TestStoreImmediate )
		{
			context.source = "int64 foo; ldi r0, 100000\nsti r0, foo";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 100000 == *reinterpret_cast< const int64_t * >( &vm.GetData()[0] ) );
		}

		TEST_METHOD( TestIntegerIncrement )
		{
			context.source = "ldi r0, 3\ninc r0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 4 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestIntegerDecrement )
		{
			context.source = "ldi r0, 3\ndec r0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 2 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestIntegerAbsolute )
		{
			context.source = "ldi r0, -3\nabs r1, r0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 3 == vm.GetRegisterValue( 1 ) );
		}

		TEST_METHOD( TestIntegerNegative )
		{
			context.source = "ldi r0, 3\nneg r1, r0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( -3 == vm.GetRegisterValue( 1 ) );
		}

		TEST_METHOD( TestShiftRight )
		{
			context.source = "ldi r0, 16\nldi r1, 2\nsr r2, r0, r1";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 4 == vm.GetRegisterValue( 2 ) );
		}

		TEST_METHOD( TestShiftLeft )
		{
			context.source = "ldi r0, 16\nldi r1, 2\nsl r2, r0, r1";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 64 == vm.GetRegisterValue( 2 ) );
		}

		TEST_METHOD( TestIntegerAdd )
		{
			context.source = "ldi r0, 3\nldi r1, 4\nadd r2, r0, r1";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 7 == vm.GetRegisterValue( 2 ) );
		}

		TEST_METHOD( TestIntegerSubtract )
		{
			context.source = "ldi r0, 5\nldi r1, 3\nsub r2, r0, r1";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 2 == vm.GetRegisterValue( 2 ) );
		}

		TEST_METHOD( TestIntegerMultiply )
		{
			context.source = "ldi r0, 3\nldi r1, 4\nmul r2, r0, r1";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 12 == vm.GetRegisterValue( 2 ) );
		}

		TEST_METHOD( TestIntegerDivide )
		{
			context.source = "ldi r0, 12\nldi r1, 4\ndiv r2, r0, r1";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 3 == vm.GetRegisterValue( 2 ) );
		}

		TEST_METHOD( TestCastToInteger )
		{
			context.source = "fldi fr0, 123.0\ncasti r0, fr0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 123 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestAnd )
		{
			context.source = "ldi r0, 3\nldi r1, 2\nand r2, r0, r1";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 2 == vm.GetRegisterValue( 2 ) );
		}

		TEST_METHOD( TestIntegerOr )
		{
			context.source = "ldi r0, 1\nldi r1, 2\nor r2, r0, r1";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 3 == vm.GetRegisterValue( 2 ) );
		}

		TEST_METHOD( TestIntegerXor )
		{
			context.source = "ldi r0, 3\nldi r1, 2\nxor r2, r0, r1";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 1 == vm.GetRegisterValue( 2 ) );
		}

		TEST_METHOD( TestIntegerComplement )
		{
			context.source = "ldi r0, 127\ncompl rb1, rb0\nldi r0, 0\ncompl rb2, rb0\nldi r0, 1\ncompl rb3, rb0\n";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( -128 == vm.GetRegisterValue( 1 ) );
			Assert::IsTrue( -1 == vm.GetRegisterValue( 2 ) );
			Assert::IsTrue( -2 == vm.GetRegisterValue( 3 ) );
		}

		TEST_METHOD( TestLogicalAnd )
		{
			context.source = "ldi r0, 2\nldi r1, 4\nland r2, r0, r1\nldi r0, 0\nland r3, r0, r1\n";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 1 == vm.GetRegisterValue( 2 ) );
			Assert::IsTrue( 0 == vm.GetRegisterValue( 3 ) );
		}

		TEST_METHOD( TestLogicalOr )
		{
			context.source = "ldi r0, 2\nldi r1, 4\nlor r2, r0, r1\nldi r0, 0\nlor r3, r0, r1\nldi r1, 0\nlor r4, r0, r1\n";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 1 == vm.GetRegisterValue( 2 ) );
			Assert::IsTrue( 1 == vm.GetRegisterValue( 3 ) );
			Assert::IsTrue( 0 == vm.GetRegisterValue( 4 ) );
		}

		TEST_METHOD( TestLogicalXor )
		{
			context.source = "ldi r0, 2\nldi r1, 4\nlxor r2, r0, r1\nldi r0, 0\nlxor r3, r0, r1\nldi r1, 0\nlxor r4, r0, r1\n";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 0 == vm.GetRegisterValue( 2 ) );
			Assert::IsTrue( 1 == vm.GetRegisterValue( 3 ) );
			Assert::IsTrue( 0 == vm.GetRegisterValue( 4 ) );
		}

		TEST_METHOD( TestLogicalNot )
		{
			context.source = "ldi r0, 2\nlnot r1, r0\nldi r0, 0\nlnot r2, r0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 0 == vm.GetRegisterValue( 1 ) );
			Assert::IsTrue( 1 == vm.GetRegisterValue( 2 ) );
		}

		TEST_METHOD( TestCompareEqualWhenTrue )
		{
			context.source = R"(
					ldi r0, 2
					ldi r1, 2
					cmpeq r0, r1
					jmpt equal
					ldi r0, 0
					jmp end
				:equal
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 1 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestCompareEqualWhenNotTrue )
		{
			context.source = R"(
					ldi r0, 2
					ldi r1, 3
					cmpeq r0, r1
					jmpt equal
					ldi r0, 0
					jmp end
				:equal
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 0 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestCompareGreaterThanWhenTrue )
		{
			context.source = R"(
					ldi r0, 3
					ldi r1, 2
					cmpgt r0, r1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

				Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 1 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestCompareGreaterThanWhenNotTrue )
		{
			context.source = R"(
					ldi r0, 2
					ldi r1, 3
					cmpgt r0, r1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

				Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 0 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestCompareGreaterEqualWhenTrue )
		{
			context.source = R"(
					ldi r0, 2
					ldi r1, 2
					cmpge r0, r1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 1 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestCompareGreaterEqualWhenNotTrue )
		{
			context.source = R"(
					ldi r0, 2
					ldi r1, 3
					cmpge r0, r1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 0 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestCompareLessThanWhenTrue )
		{
			context.source = R"(
					ldi r0, 2
					ldi r1, 3
					cmplt r0, r1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 1 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestCompareLessThanWhenNotTrue )
		{
			context.source = R"(
					ldi r0, 3
					ldi r1, 2
					cmplt r0, r1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 0 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestCompareLessEqualWhenTrue )
		{
			context.source = R"(
					ldi r0, 2
					ldi r1, 2
					cmple r0, r1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 1 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestCompareLessEqualWhenNotTrue )
		{
			context.source = R"(
					ldi r0, 3
					ldi r1, 2
					cmple r0, r1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 0 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestFloatAdd )
		{
			context.source = "fldi fr0, 3\nfldi fr1, 4\nfadd fr2, fr0, fr1";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 7 == vm.GetFloatRegisterValue( 2 ) );
		}

		TEST_METHOD( TestFloatSub )
		{
			context.source = "fldi fr0, 3\nfldi fr1, 4\nfsub fr2, fr0, fr1";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( -1 == vm.GetFloatRegisterValue( 2 ) );
		}

		TEST_METHOD( TestFloatMul )
		{
			context.source = "fldi fr0, 3\nfldi fr1, 4\nfmul fr2, fr0, fr1";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 12 == vm.GetFloatRegisterValue( 2 ) );
		}

		TEST_METHOD( TestFloatDiv )
		{
			context.source = "fldi fr0, 12\nfldi fr1, 4\nfdiv fr2, fr0, fr1";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 3 == vm.GetFloatRegisterValue( 2 ) );
		}

		TEST_METHOD( TestFloatAbs )
		{
			context.source = "fldi fr0, -3\nfabs fr1, fr0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 3 == vm.GetFloatRegisterValue( 1 ) );
		}

		TEST_METHOD( TestFloatNeg )
		{
			context.source = "fldi fr0, 3\nfneg fr1, fr0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( -3 == vm.GetFloatRegisterValue( 1 ) );
		}

		TEST_METHOD( TestFloatCos )
		{
			context.source = "fldi fr0, 3\nfcos fr1, fr0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( static_cast< float >( cos( 3 ) ) == vm.GetFloatRegisterValue( 1 ) );
		}

		TEST_METHOD( TestFloatSin )
		{
			context.source = "fldi fr0, 3\nfsin fr1, fr0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( static_cast< float >( sin( 3 ) ) == vm.GetFloatRegisterValue( 1 ) );
		}

		TEST_METHOD( TestFloatSqrt )
		{
			context.source = "fldi fr0, 25\nfsqrt fr1, fr0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 5 == vm.GetFloatRegisterValue( 1 ) );
		}

		TEST_METHOD( TestFloatRound )
		{
			context.source = "fldi fr0, 3.7\nfrnd fr1, fr0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 4 == vm.GetFloatRegisterValue( 1 ) );
		}

		TEST_METHOD( TestFloatCast )
		{
			context.source = "ldi r0, 3\nfcast fr0, r0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 3 == vm.GetFloatRegisterValue( 0 ) );
		}

		TEST_METHOD( TestFloatCompareEqualWhenTrue )
		{
			context.source = R"(
					fldi fr0, 2
					fldi fr1, 2
					fcmpeq fr0, fr1
					jmpt equal
					ldi r0, 0
					jmp end
				:equal
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 1 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestFloatCompareEqualWhenNotTrue )
		{
			context.source = R"(
					fldi fr0, 2
					fldi fr1, 3
					fcmpeq fr0, fr1
					jmpt equal
					ldi r0, 0
					jmp end
				:equal
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 0 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestFloatCompareGreaterThanWhenTrue )
		{
			context.source = R"(
					fldi fr0, 3
					fldi fr1, 2
					fcmpgt fr0, fr1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 1 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestFloatCompareGreaterThanWhenNotTrue )
		{
			context.source = R"(
					fldi fr0, 2
					fldi fr1, 3
					fcmpgt fr0, fr1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 0 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestFloatCompareGreaterEqualWhenTrue )
		{
			context.source = R"(
					fldi fr0, 2
					fldi fr1, 2
					fcmpge fr0, fr1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 1 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestFloatCompareGreaterEqualWhenNotTrue )
		{
			context.source = R"(
					fldi fr0, 2
					fldi fr1, 3
					fcmpge fr0, fr1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 0 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestFloatCompareLessThanWhenTrue )
		{
			context.source = R"(
					fldi fr0, 2
					fldi fr1, 3
					fcmplt fr0, fr1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 1 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestFloatCompareLessThanWhenNotTrue )
		{
			context.source = R"(
					fldi fr0, 3
					fldi fr1, 2
					fcmplt fr0, fr1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 0 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestFloatCompareLessEqualWhenTrue )
		{
			context.source = R"(
					fldi fr0, 2
					fldi fr1, 2
					fcmple fr0, fr1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 1 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestFloatCompareLessEqualWhenNotTrue )
		{
			context.source = R"(
					fldi fr0, 3
					fldi fr1, 2
					fcmple fr0, fr1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 0 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestDoubleAdd )
		{
			context.source = "dfldi dfr0, 3\ndfldi dfr1, 4\ndfadd dfr2, dfr0, dfr1";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 7 == vm.GetFloatRegisterValue( 2 ) );
		}

		TEST_METHOD( TestDoubleSub )
		{
			context.source = "dfldi dfr0, 3\ndfldi dfr1, 4\ndfsub dfr2, dfr0, dfr1";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( -1 == vm.GetFloatRegisterValue( 2 ) );
		}

		TEST_METHOD( TestDoubleMul )
		{
			context.source = "dfldi dfr0, 3\ndfldi dfr1, 4\ndfmul dfr2, dfr0, dfr1";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 12 == vm.GetFloatRegisterValue( 2 ) );
		}

		TEST_METHOD( TestDoubleDiv )
		{
			context.source = "dfldi dfr0, 12\ndfldi dfr1, 4\ndfdiv dfr2, dfr0, dfr1";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 3 == vm.GetFloatRegisterValue( 2 ) );
		}

		TEST_METHOD( TestDoubleAbs )
		{
			context.source = "dfldi dfr0, -3\ndfabs dfr1, dfr0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 3 == vm.GetFloatRegisterValue( 1 ) );
		}

		TEST_METHOD( TestDoubleNeg )
		{
			context.source = "dfldi dfr0, 3\ndfneg dfr1, dfr0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( -3 == vm.GetFloatRegisterValue( 1 ) );
		}

		TEST_METHOD( TestDoubleCos )
		{
			context.source = "dfldi dfr0, 3\ndfcos dfr1, dfr0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( cos( 3 ) == vm.GetFloatRegisterValue( 1 ) );
		}

		TEST_METHOD( TestDoubleSin )
		{
			context.source = "dfldi dfr0, 3\ndfsin dfr1, dfr0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( sin( 3 ) == vm.GetFloatRegisterValue( 1 ) );
		}

		TEST_METHOD( TestDoubleSqrt )
		{
			context.source = "dfldi dfr0, 25\ndfsqrt dfr1, dfr0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 5 == vm.GetFloatRegisterValue( 1 ) );
		}

		TEST_METHOD( TestDoubleRound )
		{
			context.source = "dfldi dfr0, 3.7\ndfrnd dfr1, dfr0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 4 == vm.GetFloatRegisterValue( 1 ) );
		}

		TEST_METHOD( TestDoubleCast )
		{
			context.source = "ldi r0, 3\ndfcast dfr0, r0";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 3 == vm.GetFloatRegisterValue( 0 ) );
		}

		TEST_METHOD( TestDoubleCompareEqualWhenTrue )
		{
			context.source = R"(
					dfldi dfr0, 2
					dfldi dfr1, 2
					dfcmpeq dfr0, dfr1
					jmpt equal
					ldi r0, 0
					jmp end
				:equal
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 1 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestDoubleCompareEqualWhenNotTrue )
		{
			context.source = R"(
					dfldi dfr0, 2
					dfldi dfr1, 3
					dfcmpeq dfr0, dfr1
					jmpt equal
					ldi r0, 0
					jmp end
				:equal
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 0 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestDoubleCompareGreaterThanWhenTrue )
		{
			context.source = R"(
					dfldi dfr0, 3
					dfldi dfr1, 2
					dfcmpgt dfr0, dfr1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 1 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestDoubleCompareGreaterThanWhenNotTrue )
		{
			context.source = R"(
					dfldi dfr0, 2
					dfldi dfr1, 3
					dfcmpgt dfr0, dfr1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 0 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestDoubleCompareGreaterEqualWhenTrue )
		{
			context.source = R"(
					dfldi dfr0, 2
					dfldi dfr1, 2
					dfcmpge dfr0, dfr1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 1 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestDoubleCompareGreaterEqualWhenNotTrue )
		{
			context.source = R"(
					dfldi dfr0, 2
					dfldi dfr1, 3
					dfcmpge dfr0, dfr1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 0 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestDoubleCompareLessThanWhenTrue )
		{
			context.source = R"(
					dfldi dfr0, 2
					dfldi dfr1, 3
					dfcmplt dfr0, dfr1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 1 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestDoubleCompareLessThanWhenNotTrue )
		{
			context.source = R"(
					dfldi dfr0, 3
					dfldi dfr1, 2
					dfcmplt dfr0, dfr1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 0 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestDoubleCompareLessEqualWhenTrue )
		{
			context.source = R"(
					dfldi dfr0, 2
					dfldi dfr1, 2
					dfcmple dfr0, dfr1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 1 == vm.GetRegisterValue( 0 ) );
		}

		TEST_METHOD( TestDoubleCompareLessEqualWhenNotTrue )
		{
			context.source = R"(
					dfldi dfr0, 3
					dfldi dfr1, 2
					dfcmple dfr0, dfr1
					jmpt correct
					ldi r0, 0
					jmp end
				:correct
					ldi r0, 1
				:end)";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 0 == vm.GetRegisterValue( 0 ) );
		}
	private:
		Yal::Assembler::Context context;
		Yal::VM vm;
	};
}