#include "stdafx.h"
#include "Tokenizer.h"

namespace Yal
{
	namespace Lexer
	{
		std::unordered_map< std::string, TokenId > s_tokenToTokenIdMap;

		void TokenizerInitialize()
		{
			s_tokenToTokenIdMap["sp"] = TokenId::TOKEN_STACK_POINTER;
			s_tokenToTokenIdMap["spadd"] = TokenId::TOKEN_SP_ADD;
			s_tokenToTokenIdMap["spsub"] = TokenId::TOKEN_SP_SUB;

			s_tokenToTokenIdMap["lea"] = TokenId::TOKEN_LOAD_EFFECTIVE_ADDRESS;
			s_tokenToTokenIdMap["ldi"] = TokenId::TOKEN_LOAD_IMMEDIATE;
			s_tokenToTokenIdMap["ld"] = TokenId::TOKEN_LOAD;
			s_tokenToTokenIdMap["st"] = TokenId::TOKEN_STORE;
			s_tokenToTokenIdMap["push"] = TokenId::TOKEN_PUSH;
			s_tokenToTokenIdMap["pop"] = TokenId::TOKEN_POP;

			s_tokenToTokenIdMap["mv"] = TokenId::TOKEN_MOVE;
			s_tokenToTokenIdMap["and"] = TokenId::TOKEN_AND;
			s_tokenToTokenIdMap["or"] = TokenId::TOKEN_OR;
			s_tokenToTokenIdMap["xor"] = TokenId::TOKEN_XOR;
			s_tokenToTokenIdMap["compl"] = TokenId::TOKEN_COMPLEMENT;

			s_tokenToTokenIdMap["land"] = TokenId::TOKEN_LOGICAL_AND;
			s_tokenToTokenIdMap["lor"] = TokenId::TOKEN_LOGICAL_OR;
			s_tokenToTokenIdMap["lxor"] = TokenId::TOKEN_LOGICAL_XOR;
			s_tokenToTokenIdMap["lnot"] = TokenId::TOKEN_LOGICAL_NOT;

			s_tokenToTokenIdMap["cmpeq"] = TokenId::TOKEN_COMPARE_EQUAL;
			s_tokenToTokenIdMap["cmplt"] = TokenId::TOKEN_COMPARE_LESS_THAN;
			s_tokenToTokenIdMap["cmple"] = TokenId::TOKEN_COMPARE_LESS_EQUAL;
			s_tokenToTokenIdMap["cmpgt"] = TokenId::TOKEN_COMPARE_GREATER_THAN;
			s_tokenToTokenIdMap["cmpge"] = TokenId::TOKEN_COMPARE_GREATER_EQUAL;

			s_tokenToTokenIdMap["jmp"] = TokenId::TOKEN_JUMP;
			s_tokenToTokenIdMap["jmpt"] = TokenId::TOKEN_JUMP_IF_TRUE;

			s_tokenToTokenIdMap["call"] = TokenId::TOKEN_CALL;
			s_tokenToTokenIdMap["ret"] = TokenId::TOKEN_RETURN;

			s_tokenToTokenIdMap["add"] = TokenId::TOKEN_ADD;
			s_tokenToTokenIdMap["sub"] = TokenId::TOKEN_SUBTRACT;
			s_tokenToTokenIdMap["inc"] = TokenId::TOKEN_INCREMENT;
			s_tokenToTokenIdMap["dec"] = TokenId::TOKEN_DECREMENT;
			s_tokenToTokenIdMap["mul"] = TokenId::TOKEN_MULTIPLY;
			s_tokenToTokenIdMap["div"] = TokenId::TOKEN_DIVIDE;
			s_tokenToTokenIdMap["abs"] = TokenId::TOKEN_ABSOLUTE;
			s_tokenToTokenIdMap["neg"] = TokenId::TOKEN_NEGATE;
			s_tokenToTokenIdMap["sr"] = TokenId::TOKEN_SHIFT_RIGHT;
			s_tokenToTokenIdMap["sl"] = TokenId::TOKEN_SHIFT_LEFT;
			s_tokenToTokenIdMap["casti"] = TokenId::TOKEN_CAST_TO_INTEGER;

			s_tokenToTokenIdMap["int8"] = TokenId::TOKEN_INT8;
			s_tokenToTokenIdMap["int16"] = TokenId::TOKEN_INT16;
			s_tokenToTokenIdMap["int32"] = TokenId::TOKEN_INT32;
			s_tokenToTokenIdMap["int64"] = TokenId::TOKEN_INT64;

			s_tokenToTokenIdMap["uint8"] = TokenId::TOKEN_UINT8;
			s_tokenToTokenIdMap["uint16"] = TokenId::TOKEN_UINT16;
			s_tokenToTokenIdMap["uint32"] = TokenId::TOKEN_UINT32;
			s_tokenToTokenIdMap["uint64"] = TokenId::TOKEN_UINT64;

			s_tokenToTokenIdMap["["] = TokenId::TOKEN_LEFT_SQUARE_BRACKET;
			s_tokenToTokenIdMap["]"] = TokenId::TOKEN_RIGHT_SQUARE_BRACKET;

			s_tokenToTokenIdMap["="] = TokenId::TOKEN_ASSIGN;
			s_tokenToTokenIdMap[","] = TokenId::TOKEN_COMMA;
			s_tokenToTokenIdMap[":"] = TokenId::TOKEN_COLUMN;
		}

		void TokenizerShutdown()
		{
			s_tokenToTokenIdMap.clear();
		}

		TokenId TokenToTokenId( const std::string &token )
		{
			auto it = s_tokenToTokenIdMap.find( token );
			if ( it != s_tokenToTokenIdMap.cend() )
				return it->second;

			if ( token[0] == 'r' )
				return TOKEN_REGISTER;
			if ( token.compare( 0, 2, "fr" ) == 0 )
				return TOKEN_FLOAT_REGISTER;
			if ( token.compare( 0, 3, "dfr" ) == 0 )
				return TOKEN_DOUBLE_REGISTER;
			if ( token.compare( 0, 3, "dfr" ) == 0 )
				return TOKEN_DOUBLE_REGISTER;
			
			return TOKENID_INVALID;
		}
	}
}