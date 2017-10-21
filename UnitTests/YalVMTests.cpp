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

			Assert::IsTrue( -10000000 == static_cast< int64_t >( vm.getRegisterValue( 0 ) ) );
		}
	
		TEST_METHOD( TestLoadByteImmediate )
		{
			context.source = "ldi rb0, -100";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( -100 == static_cast< int64_t >( vm.getRegisterValue( 0 ) ) );
		}

		TEST_METHOD( TestLoadWordImmediate )
		{
			context.source = "ldi rw0, -1000";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( -1000 == static_cast< int64_t >( vm.getRegisterValue( 0 ) ) );
		}

		TEST_METHOD( TestLoadDWordImmediate )
		{
			context.source = "ldi rd0, -100000";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( -100000 == static_cast< int64_t >( vm.getRegisterValue( 0 ) ) );
		}

		TEST_METHOD( TestLoadUnsignedNativeImmediate )
		{
			context.source = "ldi ru0, 10000000";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 10000000 == vm.getRegisterValue( 0 ) );
		}

		TEST_METHOD( TestLoadUnsignedByteImmediate )
		{
			context.source = "ldi rbu0, 100";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 100 == vm.getRegisterValue( 0 ) );
		}

		TEST_METHOD( TestLoadUnsignedWordImmediate )
		{
			context.source = "ldi rwu0, 1000";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 1000 == vm.getRegisterValue( 0 ) );
		}

		TEST_METHOD( TestLoadUnsignedDWordImmediate )
		{
			context.source = "ldi rdu0, 100000";

			Yal::Assembler::Assemble( context );

			vm.Init( 10, 10, context.byteCode, context.data );
			vm.Run();

			Assert::IsTrue( 100000 == vm.getRegisterValue( 0 ) );
		}

	private:
		Yal::Assembler::Context context;
		Yal::VM vm;
	};
}