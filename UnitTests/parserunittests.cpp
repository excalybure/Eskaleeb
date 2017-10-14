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

		TEST_METHOD( TestParseTokenUseToCommaAsSepartor )
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

			Assert::Fail( wmsg );
		}
	};
}