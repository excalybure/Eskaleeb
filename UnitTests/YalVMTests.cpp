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
					compeq r0, r1
					jumpt equal
					ldi r0, 0
					jump end
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
					compeq r0, r1
					jumpt equal
					ldi r0, 0
					jump end
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
					compgt r0, r1
					jumpt correct
					ldi r0, 0
					jump end
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
					compgt r0, r1
					jumpt correct
					ldi r0, 0
					jump end
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
					compge r0, r1
					jumpt correct
					ldi r0, 0
					jump end
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
					compge r0, r1
					jumpt correct
					ldi r0, 0
					jump end
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
					complt r0, r1
					jumpt correct
					ldi r0, 0
					jump end
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
					complt r0, r1
					jumpt correct
					ldi r0, 0
					jump end
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
					comple r0, r1
					jumpt correct
					ldi r0, 0
					jump end
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
					complt r0, r1
					jumpt correct
					ldi r0, 0
					jump end
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