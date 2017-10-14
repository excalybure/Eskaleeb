#pragma once

enum TokenId
{
	TOKEN_RETURN,
	TOKENID_COUNT,
	TOKENID_INVALID = TOKENID_COUNT,
};

using TokenToTokenIdFn = TokenId (*)( const std::string::const_iterator &it, const std::vector< uint8_t > &token );

TokenId TokenToTokenId( const std::string &token );
