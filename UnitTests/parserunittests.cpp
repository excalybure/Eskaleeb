#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Yal/Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(ParserUnitTests)
	{
	public:
		
		TEST_METHOD(TestParseTokenCanParseASimpleLiteral)
		{
			std::string testString = "Hello";
			auto it = testString.cbegin();
			std::string token = ParseToken( it, testString.cend() );
			AssertString( token, "Hello" );
		}

		TEST_METHOD( TestParseTokenReturnsFirstLiteral )
		{
			std::string testString = "Hello World";
			auto it = testString.cbegin();
			std::string token = ParseToken( it, testString.cend() );
			AssertString( token, "Hello" );
		}

		TEST_METHOD( TestParseTokenReturnsSecondLiteral )
		{
			std::string testString = "Hello World";
			auto it = testString.cbegin();
			ParseToken( it, testString.cend() );
			std::string token = ParseToken( it, testString.cend() );
			AssertString( token, "World" );
		}

		TEST_METHOD( TestParseTokenSkipsComments )
		{
			std::string testString = "// Foo bar\nHello";
			auto it = testString.cbegin();
			std::string token = ParseToken( it, testString.cend() );
			AssertString( token, "Hello" );
		}

		TEST_METHOD( TestParseTokenUseCommaAsSepartor )
		{
			std::string token;
			std::string testString = "Hello,World";
			auto it = testString.cbegin();
			
			token = ParseToken( it, testString.cend() );
			AssertString( token, "Hello" );
			
			token = ParseToken( it, testString.cend() );
			AssertString( token, "," );

			token = ParseToken( it, testString.cend() );
			AssertString( token, "World" );
		}

		TEST_METHOD( TestParseTokenUseEqualAsSepartor )
		{
			std::string token;
			std::string testString = "Hello=World";
			auto it = testString.cbegin();

			token = ParseToken( it, testString.cend() );
			AssertString( token, "Hello" );

			token = ParseToken( it, testString.cend() );
			AssertString( token, "=" );

			token = ParseToken( it, testString.cend() );
			AssertString( token, "World" );
		}

		TEST_METHOD( TestLiteralTokensCanContainDigits )
		{
			std::string token;
			std::string testString = "Hello123";
			auto it = testString.cbegin();

			token = ParseToken( it, testString.cend() );
			Assert::IsTrue( token == "Hello123" );
		}

		TEST_METHOD( TestLiteralThrowsExceptionOnInvalidNumber )
		{
			std::string token;
			std::string testString = "123Hello";
			auto it = testString.cbegin();

			Assert::ExpectException< std::exception >( [&] { ParseToken( it, testString.cend() ); } );
		}

		TEST_METHOD( TestDoubles )
		{
			std::string token;
			std::string testString = "123.456";
			auto it = testString.cbegin();

			token = ParseToken( it, testString.cend() );
			Assert::IsTrue( token == "123.456" );
		}

		TEST_METHOD( TestThrowOnInvalidFloatingPoints )
		{
			std::string token;
			std::string testString = "123.456.789";
			auto it = testString.cbegin();

			Assert::ExpectException< std::exception >( [&] { ParseToken( it, testString.cend() ); } );
		}

		TEST_METHOD( TestFloatingPoints )
		{
			std::string token;
			std::string testString = "123.456f";
			auto it = testString.cbegin();

			token = ParseToken( it, testString.cend() );
			Assert::IsTrue( token == "123.456f" );
		}

	private:
		void AssertString( const std::string &result, const std::string &expected )
		{
			char msg[1024];
			wchar_t wmsg[1024];
			size_t charConverted;

			if ( result == expected )
				return;
			
			sprintf_s( msg, sizeof( msg ), "'%s' expected but got '%s' instead", expected.c_str(), result.c_str() );
			mbstowcs_s( &charConverted, wmsg, sizeof( wmsg ), msg, strlen( msg ) );

			__LineInfo lineInfo( __WFILE__, __FUNCTION__, __LINE__ );

			Assert::IsTrue( result == expected, wmsg, &lineInfo );
		}
	};
}