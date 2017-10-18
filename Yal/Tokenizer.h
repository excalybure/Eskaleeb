#pragma once

namespace Yal
{
	namespace Lexer
	{
		enum TokenId
		{
			TOKEN_REGISTER,
			TOKEN_FLOAT_REGISTER,
			TOKEN_DOUBLE_REGISTER,

			TOKEN_STACK_POINTER,
			TOKEN_SP_ADD,
			TOKEN_SP_SUB,

			TOKEN_LOAD_EFFECTIVE_ADDRESS,
			TOKEN_LOAD_IMMEDIATE,
			TOKEN_LOAD,
			TOKEN_STORE,

			TOKEN_PUSH,
			TOKEN_POP,

			TOKEN_MOVE,
			TOKEN_AND,
			TOKEN_OR,
			TOKEN_XOR,
			TOKEN_COMPLEMENT,

			TOKEN_LOGICAL_AND,
			TOKEN_LOGICAL_OR,
			TOKEN_LOGICAL_XOR,
			TOKEN_LOGICAL_NOT,

			TOKEN_COMPARE_EQUAL,
			TOKEN_COMPARE_LESS_THAN,
			TOKEN_COMPARE_LESS_EQUAL,
			TOKEN_COMPARE_GREATER_THAN,
			TOKEN_COMPARE_GREATER_EQUAL,

			TOKEN_FLOAT_COMPARE_EQUAL,
			TOKEN_FLOAT_COMPARE_LESS_THAN,
			TOKEN_FLOAT_COMPARE_LESS_EQUAL,
			TOKEN_FLOAT_COMPARE_GREATER_THAN,
			TOKEN_FLOAT_COMPARE_GREATER_EQUAL,

			TOKEN_JUMP,
			TOKEN_JUMP_IF_TRUE,

			TOKEN_CALL,
			TOKEN_CALLI,
			TOKEN_RETURN,

			TOKEN_ADD,
			TOKEN_SUBTRACT,
			TOKEN_INCREMENT,
			TOKEN_DECREMENT,
			TOKEN_MULTIPLY,
			TOKEN_DIVIDE,
			TOKEN_ABSOLUTE,
			TOKEN_NEGATE,
			TOKEN_SHIFT_RIGHT,
			TOKEN_SHIFT_LEFT,
			TOKEN_CAST_TO_INTEGER,

			TOKEN_FLOAT_ADD,
			TOKEN_FLOAT_SUBTRACT,
			TOKEN_FLOAT_MULTIPLY,
			TOKEN_FLOAT_DIVIDE,
			TOKEN_FLOAT_ABSOLUTE,
			TOKEN_FLOAT_NEGATE,
			TOKEN_FLOAT_SQRT,
			TOKEN_FLOAT_COS,
			TOKEN_FLOAT_SIN,
			TOKEN_FLOAT_RND,
			TOKEN_FLOAT_CAST,

			TOKEN_INT8,
			TOKEN_INT16,
			TOKEN_INT32,
			TOKEN_INT64,

			TOKEN_UINT8,
			TOKEN_UINT16,
			TOKEN_UINT32,
			TOKEN_UINT64,

			TOKEN_LEFT_SQUARE_BRACKET,
			TOKEN_RIGHT_SQUARE_BRACKET,

			TOKEN_ASSIGN,
			TOKEN_COMMA,
			TOKEN_COLUMN,

			TOKENID_COUNT,
			TOKENID_INVALID = TOKENID_COUNT,
		};

		using TokenToTokenIdFn = TokenId( *)( const std::string::const_iterator &it, const std::vector< uint8_t > &token );

		void TokenizerInitialize();
		void TokenizerShutdown();

		TokenId TokenToTokenId( const std::string &token );
	}
}