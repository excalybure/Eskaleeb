// Yal.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Tokenizer.h"
#include "Lexer.h"
#include "Assembler.h"

/*
TokenToTokenIdFn tokenParsers[100];

void Assembler( const std::string &text, std::vector< byte > &prog )
{
	std::string token;

	auto it = text.cbegin();
	auto end = text.cend();

	while ( it != end )
	{
		token = Yal::Lexer::ParseToken( it, text.cend() );
		const TokenId tokenId = TokenToTokenId( token );
		tokenParsers[tokenId]( it, prog );
	}
	tokenParsers[TOKEN_RETURN]( it, prog );
}
*/