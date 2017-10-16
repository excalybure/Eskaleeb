#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Yal/Tokenizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS( TokenizerTests )
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

		TEST_METHOD( TestNormalRegister )
		{
			std::string token( "r10" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_REGISTER == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestFloatingPointRegister )
		{
			std::string token( "fr10" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_FLOAT_REGISTER == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestDoublePointRegister )
		{
			std::string token( "dfr10" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_DOUBLE_REGISTER == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestStackPointer )
		{
			std::string token( "sp" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_STACK_POINTER == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestStackAdd )
		{
			std::string token( "spadd" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_SP_ADD == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestStackSub )
		{
			std::string token( "spsub" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_SP_SUB == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestLoadImmediate )
		{
			std::string token( "ldi" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_LOAD_IMMEDIATE == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestLoad )
		{
			std::string token( "ld" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_LOAD == Yal::Lexer::TokenToTokenId( token ) );
		}
		
		TEST_METHOD( TestStore )
		{
			std::string token( "st" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_STORE == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestPush )
		{
			std::string token( "push" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_PUSH == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestPop )
		{
			std::string token( "pop" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_POP == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestMove )
		{
			std::string token( "mv" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_MOVE == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestAnd )
		{
			std::string token( "and" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_AND == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestOr )
		{
			std::string token( "or" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_OR == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestXor )
		{
			std::string token( "xor" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_XOR == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestComplement )
		{
			std::string token( "compl" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_COMPLEMENT == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestLogicalAnd )
		{
			std::string token( "land" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_LOGICAL_AND == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestLogicalOr )
		{
			std::string token( "lor" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_LOGICAL_OR == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestLogicalXor )
		{
			std::string token( "lxor" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_LOGICAL_XOR == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestLogicalNot )
		{
			std::string token( "lnot" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_LOGICAL_NOT == Yal::Lexer::TokenToTokenId( token ) );
		}


		TEST_METHOD( TestCompareEqual )
		{
			std::string token( "cmpeq" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_COMPARE_EQUAL == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestLessThan )
		{
			std::string token( "cmplt" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_COMPARE_LESS_THAN == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestCompareLessEqual )
		{
			std::string token( "cmple" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_COMPARE_LESS_EQUAL == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestCompareGreaterThan )
		{
			std::string token( "cmpgt" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_COMPARE_GREATER_THAN == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestCompareGreaterEqual )
		{
			std::string token( "cmpge" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_COMPARE_GREATER_EQUAL == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestJump )
		{
			std::string token( "jmp" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_JUMP == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestJumpIfTrue )
		{
			std::string token( "jmpt" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_JUMP_IF_TRUE == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestCall )
		{
			std::string token( "call" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_CALL == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestReturn )
		{
			std::string token( "ret" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_RETURN == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestAdd )
		{
			std::string token( "add" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_ADD == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestSubtrack )
		{
			std::string token( "sub" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_SUBTRACT == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestIncrement )
		{
			std::string token( "inc" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_INCREMENT == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestDecrement )
		{
			std::string token( "dec" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_DECREMENT == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestMultiply )
		{
			std::string token( "mul" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_MULTIPLY == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestDivide )
		{
			std::string token( "div" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_DIVIDE == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestAbs )
		{
			std::string token( "abs" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_ABSOLUTE == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestNegate )
		{
			std::string token( "neg" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_NEGATE == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestShiftRight )
		{
			std::string token( "sr" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_SHIFT_RIGHT == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestShiftLeft )
		{
			std::string token( "sl" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_SHIFT_LEFT == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestCastI )
		{
			std::string token( "casti" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_CAST_TO_INTEGER == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestInt8 )
		{
			std::string token( "int8" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_INT8 == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestInt16 )
		{
			std::string token( "int16" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_INT16 == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestInt32 )
		{
			std::string token( "int32" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_INT32 == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestInt64 )
		{
			std::string token( "int64" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_INT64 == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestUInt8 )
		{
			std::string token( "uint8" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_UINT8 == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestUInt16 )
		{
			std::string token( "uint16" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_UINT16 == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestUInt32 )
		{
			std::string token( "uint32" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_UINT32 == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestUInt64 )
		{
			std::string token( "uint64" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_UINT64 == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestLeftSquareBracket )
		{
			std::string token( "[" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_LEFT_SQUARE_BRACKET == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestRightSquareBracket )
		{
			std::string token( "]" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_RIGHT_SQUARE_BRACKET == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestAssign )
		{
			std::string token( "=" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_ASSIGN == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestComma )
		{
			std::string token( "," );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_COMMA == Yal::Lexer::TokenToTokenId( token ) );
		}

		TEST_METHOD( TestColumn )
		{
			std::string token( ":" );
			Assert::IsTrue( Yal::Lexer::TokenId::TOKEN_COLUMN == Yal::Lexer::TokenToTokenId( token ) );
		}
	};
}