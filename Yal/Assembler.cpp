#include "stdafx.h"
#include "Tokenizer.h"
#include "Lexer.h"
#include "Assembler.h"
#include "Cast.h"

namespace Yal
{
	namespace Assembler
	{
		enum InstructionCode : uint8_t
		{
			INSTR_CODE_SP_ADD,
			INSTR_CODE_SP_SUB,

			INSTR_CODE_LOAD_EFFECTIVE_ADDRESS,
			INSTR_CODE_LOAD_IMMEDIATE,
			INSTR_CODE_LOAD,
			INSTR_CODE_STORE,

			INSTR_CODE_PUSH,
			INSTR_CODE_POP,

			INSTR_CODE_MOVE,
			INSTR_CODE_AND,
			INSTR_CODE_OR,
			INSTR_CODE_XOR,
			INSTR_CODE_COMPLEMENT,

			INSTR_CODE_LOGICAL_AND,
			INSTR_CODE_LOGICAL_OR,
			INSTR_CODE_LOGICAL_XOR,
			INSTR_CODE_LOGICAL_NOT,

			INSTR_CODE_COMPARE_EQUAL,
			INSTR_CODE_COMPARE_LESS_THAN,
			INSTR_CODE_COMPARE_LESS_EQUAL,
			INSTR_CODE_COMPARE_GREATER_THAN,
			INSTR_CODE_COMPARE_GREATER_EQUAL,

			INSTR_CODE_COMPARE_JUMP,
			INSTR_CODE_COMPARE_JUMP_IF_TRUE,

			INSTR_CODE_CALL,
			INSTR_CODE_CALL_INDIRECT,
			INSTR_CODE_RETURN,

			INSTR_CODE_ADD,
			INSTR_CODE_SUBTRACT,
			INSTR_CODE_INCREMENT,
			INSTR_CODE_DECREMENT,
			INSTR_CODE_MULTIPLY,
			INSTR_CODE_DIVIDE,
			INSTR_CODE_ABSOLUTE,
			INSTR_CODE_NEGATE,
			INSTR_CODE_SHIFT_RIGHT,
			INSTR_CODE_SHIFT_LEFT,
			INSTR_CODE_CAST_TO_INTEGER,

			INSTR_CODE_COUNT,
			INSTR_CODE_INVALID = INSTR_CODE_COUNT,
		};

		enum ArgType : uint8_t
		{
			ARG_TYPE_REGISTER,
			ARG_TYPE_FLOAT_REGISTER,
			ARG_TYPE_DOUBLE_REGISTER,
			ARG_TYPE_INT,
			ARG_TYPE_FLOAT,
			ARG_TYPE_DOUBLE,
			ARG_TYPE_ADDRESS,
		};

		struct InstructionDesc
		{
			static constexpr int MAX_ARGS = 3;

			byte argCount;
			ArgType args[MAX_ARGS];

			explicit InstructionDesc()
				: argCount( 0 )
			{
			}

			explicit InstructionDesc( ArgType arg0 )
				: argCount( 1 )
			{
				args[0] = arg0;
			}

			explicit InstructionDesc( ArgType arg0, ArgType arg1 )
				: argCount( 2 )
			{
				args[0] = arg0;
				args[1] = arg1;
			}

			explicit InstructionDesc( ArgType arg0, ArgType arg1, ArgType arg2 )
				: argCount( 3 )
			{
				args[0] = arg0;
				args[1] = arg1;
				args[2] = arg2;
			}
		};

		InstructionCode TokenIdToInstructionCode[Lexer::TokenId::TOKENID_COUNT] =
		{
			INSTR_CODE_INVALID,					// TOKEN_REGISTER,
			INSTR_CODE_INVALID,					// TOKEN_FLOAT_REGISTER,
			INSTR_CODE_INVALID,					// TOKEN_DOUBLE_REGISTER,

			INSTR_CODE_INVALID,					// TOKEN_STACK_POINTER,
			INSTR_CODE_SP_ADD,					// TOKEN_SP_ADD,
			INSTR_CODE_SP_SUB,					// TOKEN_SP_SUB,

			INSTR_CODE_LOAD_EFFECTIVE_ADDRESS,	// TOKEN_LOAD_EFFECTIVE_ADDRESS,
			INSTR_CODE_LOAD_IMMEDIATE,			// TOKEN_LOAD_IMMEDIATE,
			INSTR_CODE_LOAD,					// TOKEN_LOAD,
			INSTR_CODE_STORE,					// TOKEN_STORE,

			INSTR_CODE_PUSH,					// TOKEN_PUSH,
			INSTR_CODE_POP,						// TOKEN_POP,

			INSTR_CODE_MOVE,					// TOKEN_MOVE,
			INSTR_CODE_AND,						// TOKEN_AND,
			INSTR_CODE_OR,						// TOKEN_OR,
			INSTR_CODE_XOR,						// TOKEN_XOR,
			INSTR_CODE_COMPLEMENT,				// TOKEN_COMPLEMENT,

			INSTR_CODE_LOGICAL_AND,				// TOKEN_LOGICAL_AND,
			INSTR_CODE_LOGICAL_OR,				// TOKEN_LOGICAL_OR,
			INSTR_CODE_LOGICAL_XOR,				// TOKEN_LOGICAL_XOR,
			INSTR_CODE_LOGICAL_NOT,				// TOKEN_LOGICAL_NOT,

			INSTR_CODE_COMPARE_EQUAL,			// TOKEN_COMPARE_EQUAL,
			INSTR_CODE_COMPARE_LESS_THAN,		// TOKEN_COMPARE_LESS_THAN,
			INSTR_CODE_COMPARE_LESS_EQUAL,		// TOKEN_COMPARE_LESS_EQUAL,
			INSTR_CODE_COMPARE_GREATER_THAN,	// TOKEN_COMPARE_GREATER_THAN,
			INSTR_CODE_COMPARE_GREATER_EQUAL,	// TOKEN_COMPARE_GREATER_EQUAL,

			INSTR_CODE_COMPARE_JUMP,			// TOKEN_COMPARE_JUMP,
			INSTR_CODE_COMPARE_JUMP_IF_TRUE,	// TOKEN_COMPARE_JUMP_IF_TRUE,

			INSTR_CODE_CALL,					// TOKEN_CALL,
			INSTR_CODE_CALL_INDIRECT,			// TOKEN_CALL_INDIRECT,
			INSTR_CODE_RETURN,					// TOKEN_RETURN,

			INSTR_CODE_ADD,						// TOKEN_ADD,
			INSTR_CODE_SUBTRACT,				// TOKEN_SUBTRACT,
			INSTR_CODE_INCREMENT,				// TOKEN_INCREMENT,
			INSTR_CODE_DECREMENT,				// TOKEN_DECREMENT,
			INSTR_CODE_MULTIPLY,				// TOKEN_MULTIPLY,
			INSTR_CODE_DIVIDE,					// TOKEN_DIVIDE,
			INSTR_CODE_ABSOLUTE,				// TOKEN_ABSOLUTE,
			INSTR_CODE_NEGATE,					// TOKEN_NEGATE,
			INSTR_CODE_SHIFT_RIGHT,				// TOKEN_SHIFT_RIGHT,
			INSTR_CODE_SHIFT_LEFT,				// TOKEN_SHIFT_LEFT,
			INSTR_CODE_CAST_TO_INTEGER,			// TOKEN_CAST_TO_INTEGER,

			INSTR_CODE_INVALID,					// TOKEN_INT8,
			INSTR_CODE_INVALID,					// TOKEN_INT16,
			INSTR_CODE_INVALID,					// TOKEN_INT32,
			INSTR_CODE_INVALID,					// TOKEN_INT64,

			INSTR_CODE_INVALID,					// TOKEN_UINT8,
			INSTR_CODE_INVALID,					// TOKEN_UINT16,
			INSTR_CODE_INVALID,					// TOKEN_UINT32,
			INSTR_CODE_INVALID,					// TOKEN_UINT64,

			INSTR_CODE_INVALID,					// TOKEN_LEFT_SQUARE_BRACKET,
			INSTR_CODE_INVALID,					// TOKEN_RIGHT_SQUARE_BRACKET,

			INSTR_CODE_INVALID,					// TOKEN_ASSIGN,
			INSTR_CODE_INVALID,					// TOKEN_COMMA,
			INSTR_CODE_INVALID,					// TOKEN_COLUMN,
		};

		InstructionDesc InstructionCodeToIntructionDesc[INSTR_CODE_COUNT] =
		{
			InstructionDesc( ARG_TYPE_INT ),											// TOKEN_SP_ADD,
			InstructionDesc( ARG_TYPE_INT ),											// TOKEN_SP_SUB,

			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_ADDRESS ),						// TOKEN_LOAD_EFFECTIVE_ADDRESS,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_INT ),							// TOKEN_LOAD_IMMEDIATE,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),					// TOKEN_LOAD,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),					// TOKEN_STORE,

			InstructionDesc( ARG_TYPE_REGISTER ),										// TOKEN_PUSH,
			InstructionDesc( ARG_TYPE_REGISTER ),										// TOKEN_POP,

			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),					// TOKEN_MOVE,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),	// TOKEN_AND,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),	// TOKEN_OR,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),	// TOKEN_XOR,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),					// TOKEN_COMPLEMENT,

			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),	// TOKEN_LOGICAL_AND,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),	// TOKEN_LOGICAL_OR,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),	// TOKEN_LOGICAL_XOR,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),					// TOKEN_LOGICAL_NOT,

			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),					// TOKEN_COMPARE_EQUAL,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),					// TOKEN_COMPARE_LESS_THAN,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),					// TOKEN_COMPARE_LESS_EQUAL,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),					// TOKEN_COMPARE_GREATER_THAN,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),					// TOKEN_COMPARE_GREATER_EQUAL,

			InstructionDesc( ARG_TYPE_ADDRESS ),										// TOKEN_COMPARE_JUMP,
			InstructionDesc( ARG_TYPE_ADDRESS ),										// TOKEN_COMPARE_JUMP_IF_TRUE,

			InstructionDesc( ARG_TYPE_ADDRESS ),										// TOKEN_CALL,
			InstructionDesc( ARG_TYPE_REGISTER ),										// TOKEN_INDIRECT_CALL,
			InstructionDesc(),															// TOKEN_RETURN,

			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),	// TOKEN_ADD,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),	// TOKEN_SUBTRACT,
			InstructionDesc( ARG_TYPE_REGISTER ),										// TOKEN_INCREMENT,
			InstructionDesc( ARG_TYPE_REGISTER ),										// TOKEN_DECREMENT,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),	// TOKEN_MULTIPLY,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),	// TOKEN_DIVIDE,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),					// TOKEN_ABSOLUTE,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),					// TOKEN_NEGATE,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),	// TOKEN_SHIFT_RIGHT,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),	// TOKEN_SHIFT_LEFT,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_FLOAT_REGISTER ),				// TOKEN_CAST_TO_INTEGER,
		};

		const char *InstructionCodeToString[INSTR_CODE_COUNT]
		{
			"spadd",		// INSTR_CODE_SP_ADD,
			"spsub",		// INSTR_CODE_SP_SUB,

			"lea",			// INSTR_CODE_LOAD_EFFECTIVE_ADDRESS
			"ldi",			// INSTR_CODE_LOAD_IMMEDIATE,
			"ld",			// INSTR_CODE_LOAD,
			"st",			// INSTR_CODE_STORE,

			"push",			// INSTR_CODE_PUSH,
			"pop",			// INSTR_CODE_POP,

			"mv",			// INSTR_CODE_MOVE,
			"and",			// INSTR_CODE_AND,
			"or",			// INSTR_CODE_OR,
			"xor",			// INSTR_CODE_XOR,
			"compl",		// INSTR_CODE_COMPLEMENT,

			"land",			// INSTR_CODE_LOGICAL_AND,
			"lor",			// INSTR_CODE_LOGICAL_OR,
			"lxor",			// INSTR_CODE_LOGICAL_XOR,
			"lnot",			// INSTR_CODE_LOGICAL_NOT,

			"cmpeq",		// INSTR_CODE_COMPARE_EQUAL,
			"cmplt",		// INSTR_CODE_COMPARE_LESS_THAN,
			"cmple",		// INSTR_CODE_COMPARE_LESS_EQUAL,
			"cmpgt",		// INSTR_CODE_COMPARE_GREATER_THAN,
			"cmpge",		// INSTR_CODE_COMPARE_GREATER_EQUAL,

			"jmp",			// INSTR_CODE_COMPARE_JUMP,
			"jmpt",			// INSTR_CODE_COMPARE_JUMP_IF_TRUE,

			"call",			// INSTR_CODE_CALL,
			"calli",		// INSTR_CODE_CALL_INDIRECT,
			"ret",			// INSTR_CODE_RETURN,

			"add",			// INSTR_CODE_ADD,
			"sub",			// INSTR_CODE_SUBTRACT,
			"inc",			// INSTR_CODE_INCREMENT,
			"dec",			// INSTR_CODE_DECREMENT,
			"mul",			// INSTR_CODE_MULTIPLY,
			"div",			// INSTR_CODE_DIVIDE,
			"abs",			// INSTR_CODE_ABSOLUTE,
			"neg",			// INSTR_CODE_NEGATE,
			"sr",			// INSTR_CODE_SHIFT_RIGHT,
			"sl",			// INSTR_CODE_SHIFT_LEFT,
			"casti",		// INSTR_CODE_CAST_TO_INTEGER,
		};

		template< typename scalar_type > scalar_type TokenToScalarType( const std::string &token )
		{
			int64_t result = _atoi64( &token[0] );
			return Cast< scalar_type >( result );
		};

		template< typename scalar_type >
		void AppendScalar( const std::string &token, std::vector< uint8_t > &buffer )
		{
			scalar_type value;

			value = TokenToScalarType< scalar_type >( token );
			buffer.resize( buffer.size() + sizeof( value ) );
			memcpy( &buffer[buffer.size() - sizeof( value )], &value, sizeof( value ) );
		}

		template< typename scalar_type >
		void AppendScalar( std::string::const_iterator &it, const std::string::const_iterator &end, std::vector< uint8_t > &buffer )
		{
			std::string token;

			token = Lexer::ParseToken( it, end );
			if ( token == "-" )
				token += Lexer::ParseToken( it, end );

			AppendScalar< scalar_type >( token, buffer );
		}

		template< typename scalar_type >
		void ParseVariableDefinition( std::string::const_iterator &it, const std::string::const_iterator &end, std::vector< uint8_t > &dataSegment )
		{
			std::string token;

			token = Lexer::ParseToken( it, end ); // name
			token = Lexer::ParseToken( it, end ); // =
			
			AppendScalar< scalar_type >( it, end, dataSegment );
		}

		void Assemble( const std::string &text, std::vector< uint8_t > &prog, std::vector< uint8_t > &dataSegment )
		{
			uint8_t registerIndex;

			prog.clear();
			prog.reserve( 1 * MB );

			dataSegment.clear();
			dataSegment.reserve( 1 * MB );

			auto it = text.cbegin();
			auto end = text.cend();

			auto parseNextToken = [&it, end] ()
			{
				std::string token = Lexer::ParseToken( it, end );
				if ( token.empty() )
					throw std::exception( "Unexpectedly encountered end of file" );
				return token;
			};
			auto tokenToRegisterIndex = []( const std::string &token ) -> uint8_t
			{
				int index = atoi( &token[1] );
				if ( index < 0 || index > 255 )
					throw std::exception( "Invalid register index" );
				return static_cast< uint8_t >( index );
			};

			while ( it != end )
			{
				std::string token = Lexer::ParseToken( it, end );
				if ( token.empty() )
					break;

				Lexer::TokenId tokenId = Lexer::TokenToTokenId( token );
				const InstructionCode &code = TokenIdToInstructionCode[tokenId];
				if ( code != INSTR_CODE_INVALID )
				{
					prog.emplace_back( code );
					const InstructionDesc &instructionDesc = InstructionCodeToIntructionDesc[code];
					for ( int argIndex = 0; argIndex < instructionDesc.argCount; ++argIndex )
					{
						token = parseNextToken();
						if ( argIndex > 0 )
						{
							if ( token != "," )
								throw std::exception( "Missing comma" );
							token = parseNextToken();
						}
						switch ( instructionDesc.args[argIndex] )
						{
						case ARG_TYPE_REGISTER:
							registerIndex = tokenToRegisterIndex( token );
							prog.emplace_back( registerIndex );
							break;
						case ARG_TYPE_FLOAT_REGISTER:
							break;
						case ARG_TYPE_DOUBLE_REGISTER:
							break;
						case ARG_TYPE_INT:
							AppendScalar< int32_t >( token, prog );
							break;
						case ARG_TYPE_FLOAT:
							break;
						case ARG_TYPE_DOUBLE:
							break;
						case ARG_TYPE_ADDRESS:
							break;
						}
					}
				}
				else
				{
					switch ( tokenId )
					{
					case Lexer::TokenId::TOKEN_INT8:
						ParseVariableDefinition< int8_t >( it, end, dataSegment );
						break;
					case Lexer::TokenId::TOKEN_INT16:
						ParseVariableDefinition< int16_t >( it, end, dataSegment );
						break;
					case Lexer::TokenId::TOKEN_INT32:
						ParseVariableDefinition< int32_t >( it, end, dataSegment );
						break;
					case Lexer::TokenId::TOKEN_INT64:
						ParseVariableDefinition< int64_t >( it, end, dataSegment );
						break;
					case Lexer::TokenId::TOKEN_UINT8:
						ParseVariableDefinition< uint8_t >( it, end, dataSegment );
						break;
					case Lexer::TokenId::TOKEN_UINT16:
						ParseVariableDefinition< uint16_t >( it, end, dataSegment );
						break;
					case Lexer::TokenId::TOKEN_UINT32:
						ParseVariableDefinition< uint32_t >( it, end, dataSegment );
						break;
					case Lexer::TokenId::TOKEN_UINT64:
						ParseVariableDefinition< uint64_t >( it, end, dataSegment );
						break;
					default:
						break;
					}
				}
			}

			prog.shrink_to_fit();
		}

		void Disassemble( const std::vector< uint8_t > &prog, std::string &text )
		{
			text.clear();
			text.reserve( 1 * MB );
			auto it = prog.cbegin();
			auto end = prog.cend();

			while ( it != end )
			{
				int registerIndex;
				int integerValue;
				InstructionCode code = static_cast< InstructionCode >( *it );
				++it;
				text += InstructionCodeToString[code];
				text += "\t";

				const InstructionDesc &instructionDesc = InstructionCodeToIntructionDesc[code];
				for ( int argIndex = 0; argIndex < instructionDesc.argCount; ++argIndex )
				{
					if ( argIndex > 0 )
						text += ", ";
					switch ( instructionDesc.args[argIndex] )
					{
					case ARG_TYPE_REGISTER:
						registerIndex = *it;
						++it;
						text += "r" + std::to_string( registerIndex );
						break;
					case ARG_TYPE_FLOAT_REGISTER:
						break;
					case ARG_TYPE_DOUBLE_REGISTER:
						break;
					case ARG_TYPE_INT:
						memcpy( &integerValue, &it[0], sizeof( integerValue ) );
						it += sizeof( integerValue );
						text += std::to_string( integerValue );
						break;
					case ARG_TYPE_FLOAT:
						break;
					case ARG_TYPE_DOUBLE:
						break;
					case ARG_TYPE_ADDRESS:
						break;
					}
				}

				text += '\n';
			}

			text.shrink_to_fit();
		}
	}
}
