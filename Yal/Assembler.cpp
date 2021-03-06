#include "stdafx.h"
#include "Tokenizer.h"
#include "Lexer.h"
#include "Assembler.h"
#include "Cast.h"

namespace Yal
{
	namespace Assembler
	{
		using AddressToVariableNameMap = std::unordered_map< int, std::string >;

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

		InstructionCode TokenIdToInstructionCode[] =
		{
			INSTR_CODE_INVALID,					// TOKEN_REGISTER,
			INSTR_CODE_INVALID,					// TOKEN_FLOAT_REGISTER,
			INSTR_CODE_INVALID,					// TOKEN_DOUBLE_REGISTER,

			INSTR_CODE_INVALID,					// TOKEN_STACK_POINTER,
			INSTR_CODE_SP_ADD,					// TOKEN_SP_ADD,
			INSTR_CODE_SP_SUB,					// TOKEN_SP_SUB,

			INSTR_CODE_LOAD_EFFECTIVE_ADDRESS,	// TOKEN_LOAD_EFFECTIVE_ADDRESS,
			INSTR_CODE_LOAD_CODE_ADDRESS,		// TOKEN_LOAD_CODE_ADDRESS,
			INSTR_CODE_LOAD_IMMEDIATE,			// TOKEN_LOAD_IMMEDIATE,
			INSTR_CODE_FLOAT_LOAD_IMMEDIATE,	// TOKEN_FLOAT_LOAD_IMMEDIATE,
			INSTR_CODE_DOUBLE_LOAD_IMMEDIATE,	// TOKEN_DOUBLE_LOAD_IMMEDIATE,
			INSTR_CODE_LOAD,					// TOKEN_LOAD,
			INSTR_CODE_LOAD_ADDRESS,			// TOKEN_LOAD_ADDRESS,
			INSTR_CODE_STORE,					// TOKEN_STORE,
			INSTR_CODE_STORE_ADDRESS,			// TOKEN_STORE_ADDRESS,

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

			INSTR_CODE_FLOAT_COMPARE_EQUAL,			// TOKEN_FLOAT_COMPARE_EQUAL,
			INSTR_CODE_FLOAT_COMPARE_LESS_THAN,		// TOKEN_FLOAT_COMPARE_LESS_THAN,
			INSTR_CODE_FLOAT_COMPARE_LESS_EQUAL,	// TOKEN_FLOAT_COMPARE_LESS_EQUAL,
			INSTR_CODE_FLOAT_COMPARE_GREATER_THAN,	// TOKEN_FLOAT_COMPARE_GREATER_THAN,
			INSTR_CODE_FLOAT_COMPARE_GREATER_EQUAL,	// TOKEN_FLOAT_COMPARE_GREATER_EQUAL,

			INSTR_CODE_DOUBLE_COMPARE_EQUAL,			// TOKEN_DOUBLE_COMPARE_EQUAL,
			INSTR_CODE_DOUBLE_COMPARE_LESS_THAN,		// TOKEN_DOUBLE_COMPARE_LESS_THAN,
			INSTR_CODE_DOUBLE_COMPARE_LESS_EQUAL,	// TOKEN_DOUBLE_COMPARE_LESS_EQUAL,
			INSTR_CODE_DOUBLE_COMPARE_GREATER_THAN,	// TOKEN_DOUBLE_COMPARE_GREATER_THAN,
			INSTR_CODE_DOUBLE_COMPARE_GREATER_EQUAL,	// TOKEN_DOUBLE_COMPARE_GREATER_EQUAL,

			INSTR_CODE_JUMP,					// TOKEN_JUMP,
			INSTR_CODE_JUMP_IF_TRUE,			// TOKEN_JUMP_IF_TRUE,

			INSTR_CODE_CALL,					// TOKEN_CALL,
			INSTR_CODE_CALL_INDIRECT,			// TOKEN_CALL_INDIRECT,
			INSTR_CODE_CALL_NATIVE,				// TOKEN_CALL_NATIVE,
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

			INSTR_CODE_FLOAT_ADD,				// TOKEN_FLOAT_ADD,
			INSTR_CODE_FLOAT_SUBTRACT,			// TOKEN_FLOAT_SUBTRACT,
			INSTR_CODE_FLOAT_MULTIPLY,			// TOKEN_FLOAT_MULTIPLY,
			INSTR_CODE_FLOAT_DIVIDE,			// TOKEN_FLOAT_DIVIDE,
			INSTR_CODE_FLOAT_ABSOLUTE,			// TOKEN_FLOAT_ABSOLUTE,
			INSTR_CODE_FLOAT_NEGATE,			// TOKEN_FLOAT_NEGATE,
			INSTR_CODE_FLOAT_SQRT,				// TOKEN_FLOAT_SQRT,
			INSTR_CODE_FLOAT_COS,				// TOKEN_FLOAT_COS,
			INSTR_CODE_FLOAT_SIN,				// TOKEN_FLOAT_SIN,
			INSTR_CODE_FLOAT_RND,				// TOKEN_FLOAT_RND,
			INSTR_CODE_FLOAT_CAST,				// TOKEN_FLOAT_CAST,

			INSTR_CODE_DOUBLE_ADD,				// TOKEN_DOUBLE_ADD,
			INSTR_CODE_DOUBLE_SUBTRACT,			// TOKEN_DOUBLE_SUBTRACT,
			INSTR_CODE_DOUBLE_MULTIPLY,			// TOKEN_DOUBLE_MULTIPLY,
			INSTR_CODE_DOUBLE_DIVIDE,			// TOKEN_DOUBLE_DIVIDE,
			INSTR_CODE_DOUBLE_ABSOLUTE,			// TOKEN_DOUBLE_ABSOLUTE,
			INSTR_CODE_DOUBLE_NEGATE,			// TOKEN_DOUBLE_NEGATE,
			INSTR_CODE_DOUBLE_SQRT,				// TOKEN_DOUBLE_SQRT,
			INSTR_CODE_DOUBLE_COS,				// TOKEN_DOUBLE_COS,
			INSTR_CODE_DOUBLE_SIN,				// TOKEN_DOUBLE_SIN,
			INSTR_CODE_DOUBLE_RND,				// TOKEN_DOUBLE_RND,
			INSTR_CODE_DOUBLE_CAST,				// TOKEN_DOUBLE_CAST,

			INSTR_CODE_INVALID,					// TOKEN_INT8,
			INSTR_CODE_INVALID,					// TOKEN_INT16,
			INSTR_CODE_INVALID,					// TOKEN_INT32,
			INSTR_CODE_INVALID,					// TOKEN_INT64,

			INSTR_CODE_INVALID,					// TOKEN_UINT8,
			INSTR_CODE_INVALID,					// TOKEN_UINT16,
			INSTR_CODE_INVALID,					// TOKEN_UINT32,
			INSTR_CODE_INVALID,					// TOKEN_UINT64,

			INSTR_CODE_INVALID,					// TOKEN_FLOAT,
			INSTR_CODE_INVALID,					// TOKEN_DOUBLE,
				
			INSTR_CODE_INVALID,					// TOKEN_LEFT_SQUARE_BRACKET,
			INSTR_CODE_INVALID,					// TOKEN_RIGHT_SQUARE_BRACKET,

			INSTR_CODE_INVALID,					// TOKEN_ASSIGN,
			INSTR_CODE_INVALID,					// TOKEN_COMMA,
			INSTR_CODE_INVALID,					// TOKEN_COLUMN,
		};
		static_assert( _countof( TokenIdToInstructionCode ) == Lexer::TokenId::TOKENID_COUNT, "Too few entries in TokenIdToInstructionCode" );
		

		InstructionDesc InstructionCodeToIntructionDesc[] =
		{
			InstructionDesc( ARG_TYPE_INT ),											// TOKEN_SP_ADD,
			InstructionDesc( ARG_TYPE_INT ),											// TOKEN_SP_SUB,

			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_ADDRESS ),						// TOKEN_LOAD_EFFECTIVE_ADDRESS,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_ADDRESS ),						// TOKEN_LOAD_CODE_ADDRESS,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_INT ),							// TOKEN_LOAD_IMMEDIATE,
			InstructionDesc( ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_FLOAT ),					// TOKEN_FLOAT_LOAD_IMMEDIATE,
			InstructionDesc( ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_DOUBLE ),				// TOKEN_DOUBLE_LOAD_IMMEDIATE,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),					// TOKEN_LOAD,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_ADDRESS ),						// TOKEN_LOAD_ADDRESS,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_REGISTER ),					// TOKEN_STORE,
			InstructionDesc( ARG_TYPE_REGISTER, ARG_TYPE_ADDRESS ),						// TOKEN_STORE_ADDRESS,

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

			InstructionDesc( ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_FLOAT_REGISTER ),		// TOKEN_FLOAT_COMPARE_EQUAL,
			InstructionDesc( ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_FLOAT_REGISTER ),		// TOKEN_FLOAT_COMPARE_LESS_THAN,
			InstructionDesc( ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_FLOAT_REGISTER ),		// TOKEN_FLOAT_COMPARE_LESS_EQUAL,
			InstructionDesc( ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_FLOAT_REGISTER ),		// TOKEN_FLOAT_COMPARE_GREATER_THAN,
			InstructionDesc( ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_FLOAT_REGISTER ),		// TOKEN_FLOAT_COMPARE_GREATER_EQUAL,

			InstructionDesc( ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_DOUBLE_REGISTER ),		// TOKEN_DOUBLE_COMPARE_EQUAL,
			InstructionDesc( ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_DOUBLE_REGISTER ),		// TOKEN_DOUBLE_COMPARE_LESS_THAN,
			InstructionDesc( ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_DOUBLE_REGISTER ),		// TOKEN_DOUBLE_COMPARE_LESS_EQUAL,
			InstructionDesc( ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_DOUBLE_REGISTER ),		// TOKEN_DOUBLE_COMPARE_GREATER_THAN,
			InstructionDesc( ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_DOUBLE_REGISTER ),		// TOKEN_DOUBLE_COMPARE_GREATER_EQUAL,

			InstructionDesc( ARG_TYPE_ADDRESS ),										// TOKEN_COMPARE_JUMP,
			InstructionDesc( ARG_TYPE_ADDRESS ),										// TOKEN_COMPARE_JUMP_IF_TRUE,

			InstructionDesc( ARG_TYPE_ADDRESS ),										// TOKEN_CALL,
			InstructionDesc( ARG_TYPE_REGISTER ),										// TOKEN_INDIRECT_CALL,
			InstructionDesc( ARG_TYPE_ADDRESS ),										// TOKEN_NATIVE_CALL,
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

			InstructionDesc( ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_FLOAT_REGISTER ),	// TOKEN_FLOAT_ADD,
			InstructionDesc( ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_FLOAT_REGISTER ),	// TOKEN_FLOAT_SUBTRACT,
			InstructionDesc( ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_FLOAT_REGISTER ),	// TOKEN_FLOAT_MULTIPLY,
			InstructionDesc( ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_FLOAT_REGISTER ),	// TOKEN_FLOAT_DIVIDE,
			InstructionDesc( ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_FLOAT_REGISTER ),		// TOKEN_FLOAT_ABSOLUTE,
			InstructionDesc( ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_FLOAT_REGISTER ),		// TOKEN_FLOAT_NEGATE,
			InstructionDesc( ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_FLOAT_REGISTER ),		// TOKEN_FLOAT_SQRT,
			InstructionDesc( ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_FLOAT_REGISTER ),		// TOKEN_FLOAT_COS,
			InstructionDesc( ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_FLOAT_REGISTER ),		// TOKEN_FLOAT_SIN,
			InstructionDesc( ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_FLOAT_REGISTER ),		// TOKEN_FLOAT_RND,
			InstructionDesc( ARG_TYPE_FLOAT_REGISTER, ARG_TYPE_REGISTER ),				// TOKEN_FLOAT_CAST,

			InstructionDesc( ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_DOUBLE_REGISTER ),	// TOKEN_DOUBLE_ADD,
			InstructionDesc( ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_DOUBLE_REGISTER ),	// TOKEN_DOUBLE_SUBTRACT,
			InstructionDesc( ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_DOUBLE_REGISTER ),	// TOKEN_DOUBLE_MULTIPLY,
			InstructionDesc( ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_DOUBLE_REGISTER ),	// TOKEN_DOUBLE_DIVIDE,
			InstructionDesc( ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_DOUBLE_REGISTER ),		// TOKEN_DOUBLE_ABSOLUTE,
			InstructionDesc( ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_DOUBLE_REGISTER ),		// TOKEN_DOUBLE_NEGATE,
			InstructionDesc( ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_DOUBLE_REGISTER ),		// TOKEN_DOUBLE_SQRT,
			InstructionDesc( ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_DOUBLE_REGISTER ),		// TOKEN_DOUBLE_COS,
			InstructionDesc( ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_DOUBLE_REGISTER ),		// TOKEN_DOUBLE_SIN,
			InstructionDesc( ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_DOUBLE_REGISTER ),		// TOKEN_DOUBLE_RND,
			InstructionDesc( ARG_TYPE_DOUBLE_REGISTER, ARG_TYPE_REGISTER ),				// TOKEN_DOUBLE_CAST,
		};
		static_assert( _countof( InstructionCodeToIntructionDesc )  == INSTR_CODE_COUNT, "Too few entries in InstructionCodeToIntructionDesc" );

		const char *InstructionCodeToString[]
		{
			"spadd",		// INSTR_CODE_SP_ADD,
			"spsub",		// INSTR_CODE_SP_SUB,

			"lea",			// INSTR_CODE_LOAD_EFFECTIVE_ADDRESS
			"lca",			// INSTR_CODE_LOAD_CODE_ADDRESS
			"ldi",			// INSTR_CODE_LOAD_IMMEDIATE,
			"fldi",			// INSTR_CODE_FLOAT_LOAD_IMMEDIATE,
			"dfldi",		// INSTR_CODE_DOUBLE_LOAD_IMMEDIATE,
			"ld",			// INSTR_CODE_LOAD,
			"lda",			// INSTR_CODE_LOAD_ADDRESS,
			"st",			// INSTR_CODE_STORE,
			"sta",			// INSTR_CODE_STORE_ADDRESS,

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

			"fcmpeq",		// INSTR_CODE_FLOAT_COMPARE_EQUAL,
			"fcmplt",		// INSTR_CODE_FLOAT_COMPARE_LESS_THAN,
			"fcmple",		// INSTR_CODE_FLOAT_COMPARE_LESS_EQUAL,
			"fcmpgt",		// INSTR_CODE_FLOAT_COMPARE_GREATER_THAN,
			"fcmpge",		// INSTR_CODE_FLOAT_COMPARE_GREATER_EQUAL,

			"dfcmpeq",		// INSTR_CODE_DOUBLE_COMPARE_EQUAL,
			"dfcmplt",		// INSTR_CODE_DOUBLE_COMPARE_LESS_THAN,
			"dfcmple",		// INSTR_CODE_DOUBLE_COMPARE_LESS_EQUAL,
			"dfcmpgt",		// INSTR_CODE_DOUBLE_COMPARE_GREATER_THAN,
			"dfcmpge",		// INSTR_CODE_DOUBLE_COMPARE_GREATER_EQUAL,

			"jmp",			// INSTR_CODE_COMPARE_JUMP,
			"jmpt",			// INSTR_CODE_COMPARE_JUMP_IF_TRUE,

			"call",			// INSTR_CODE_CALL,
			"calli",		// INSTR_CODE_CALL_INDIRECT,
			"calln",		// INSTR_CODE_CALL_NATIVE,
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

			"fadd",			// INSTR_CODE_FLOAT_ADD,
			"fsub",			// INSTR_CODE_FLOAT_SUBTRACT,
			"fmul",			// INSTR_CODE_FLOAT_MULTIPLY,
			"fdiv",			// INSTR_CODE_FLOAT_DIVIDE,
			"fabs",			// INSTR_CODE_FLOAT_ABSOLUTE,
			"fneg",			// INSTR_CODE_FLOAT_NEGATE,
			"fsqrt",		// INSTR_CODE_FLOAT_SQRT,
			"fcos",			// INSTR_CODE_FLOAT_COS,
			"fsin",			// INSTR_CODE_FLOAT_SIN,
			"frnd",			// INSTR_CODE_FLOAT_RND,
			"fcast",		// INSTR_CODE_FLOAT_CAST,

			"dfadd",		// INSTR_CODE_DOUBLE_ADD,
			"dfsub",		// INSTR_CODE_DOUBLE_SUBTRACT,
			"dfmul",		// INSTR_CODE_DOUBLE_MULTIPLY,
			"dfdiv",		// INSTR_CODE_DOUBLE_DIVIDE,
			"dfabs",		// INSTR_CODE_DOUBLE_ABSOLUTE,
			"dfneg",		// INSTR_CODE_DOUBLE_NEGATE,
			"dfsqrt",		// INSTR_CODE_DOUBLE_SQRT,
			"dfcos",		// INSTR_CODE_DOUBLE_COS,
			"dfsin",		// INSTR_CODE_DOUBLE_SIN,
			"dfrnd",		// INSTR_CODE_DOUBLE_RND,
			"dfcast",		// INSTR_CODE_DOUBLE_CAST,
		};
		static_assert( _countof( InstructionCodeToString ) == INSTR_CODE_COUNT, "Too few entries in InstructionCodeToString" );

		template< typename scalar_type > scalar_type TokenToScalarType( const std::string &token )
		{
			int64_t result = _atoi64( &token[0] );
			return Cast< scalar_type >( result );
		};

		template<> float TokenToScalarType< float >( const std::string &token )
		{
			return static_cast< float >( atof( &token[0] ) );
		};

		template<> double TokenToScalarType< double >( const std::string &token )
		{
			return atof( &token[0] );
		};

		template< typename scalar_type >
		void AppendScalar( const scalar_type value, std::vector< uint8_t > &buffer, size_t arraySize = 1 )
		{
			size_t startOffset = buffer.size();
			buffer.resize( startOffset + sizeof( value ) * arraySize );
			for( size_t index = 0; index < arraySize; ++index )
				memcpy( &buffer[startOffset + sizeof( value ) * index], &value, sizeof( value ) );
		}

		template< typename scalar_type >
		void AppendScalar( const std::string &token, std::vector< uint8_t > &buffer )
		{
			AppendScalar( TokenToScalarType< scalar_type >( token ), buffer );
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

		static void AppendAddress( Context &context, const std::string &variableName )
		{
			if ( context.variables.find( variableName ) == context.variables.cend() )
				throw std::exception( "Trying to reference an unknown variable" );

			int address = context.variables[variableName];
			AppendScalar< int32_t >( address, context.byteCode );
		}

		static void HandleCodeLabel( Context &context, const std::string &labelName )
		{
			context.deferredLabelResolutions.emplace_back( labelName, context.byteCode.size() );
			AppendScalar< int32_t >( 0, context.byteCode );
		}

		static void ExpectToken( std::string::const_iterator &it, const std::string::const_iterator &end, const char *expectedToken )
		{
			std::string token = Lexer::ParseToken( it, end );
			if ( token != expectedToken )
				throw std::exception( "Syntax error. Got unexpected token" );
		}

		template< typename scalar_type >
		static void ParseVariableDefinition( Context &context, std::string::const_iterator &it, const std::string::const_iterator &end )
		{
			size_t arraySize = 1;
			bool isArray = false;

			std::string variableName = Lexer::ParseToken( it, end );

			if ( context.variables.find( variableName ) != context.variables.cend() )
				throw std::exception( "A variable by the same name was already created" );
			context.variables[variableName] = static_cast< int >( context.data.size() );

			std::string token = Lexer::ParseToken( it, end );
			if ( token == "[" )
			{
				isArray = true;
				token = Lexer::ParseToken( it, end );
				arraySize = TokenToScalarType<uint32_t>( token );
				ExpectToken( it, end, "]" );
				token = Lexer::ParseToken( it, end );
			}

			if ( token == ";" )
			{
				AppendScalar< scalar_type >( 0, context.data, arraySize );
				return;
			}

			if ( token != "=" )
				throw std::exception( "Expected '=' but got something else" );

			if ( isArray )
			{
				ExpectToken( it, end, "{" );
				for ( size_t elementIndex = 0; elementIndex < arraySize; ++elementIndex )
				{
					if ( elementIndex != 0 )
						ExpectToken( it, end, "," );
					AppendScalar< scalar_type >( it, end, context.data );
				}
				ExpectToken( it, end, "}" );
			}
			else
			{
				AppendScalar< scalar_type >( it, end, context.data );
			}

			ExpectToken( it, end, ";" );
		}

		template< typename scalar_type >
		static void DisassembleScalar( std::string &text, std::vector< uint8_t >::const_iterator &codeIt )
		{
			scalar_type value;

			memcpy( &value, &codeIt[0], sizeof( value ) );
			codeIt += sizeof( value );
			text += std::to_string( value );
		}

		static void ParseLabel( Context &context, std::string::const_iterator &it, const std::string::const_iterator &end )
		{
			std::string labelName = Lexer::ParseToken( it, end );
			if ( context.labels.find( labelName ) != context.labels.cend() )
				throw std::exception( "A label by the same name was already created" );
			context.labels[labelName] = static_cast< int >( context.byteCode.size() );
		}

		static bool WantsCodeAddress( InstructionCode code )
		{
			switch ( code )
			{
			case INSTR_CODE_LOAD_CODE_ADDRESS:
			case INSTR_CODE_JUMP:
			case INSTR_CODE_JUMP_IF_TRUE:
			case INSTR_CODE_CALL:
				return true;
			default:
				break;
			}
			return false;
		}

		void Assemble( Context &context )
		{
			uint8_t registerIndex;

			context.byteCode.clear();
			context.byteCode.reserve( 1 * MB );

			context.data.clear();
			context.data.reserve( 1 * MB );

			auto it = context.source.cbegin();
			auto end = context.source.cend();

			auto parseNextToken = [&it, end] ()
			{
				std::string token = Lexer::ParseToken( it, end );
				if ( token.empty() )
					throw std::exception( "Unexpectedly encountered end of file" );
				return token;
			};

			auto extractRegisterIndex = []( const std::string &token, size_t offset )->uint8_t
			{
				int registerIndex = atoi( &token[offset] );
				if ( registerIndex < 0 || registerIndex > 255 )
					throw std::exception( "Invalid register index" );
				return static_cast< uint8_t >( registerIndex );
			};

			auto tokenToRegisterIndex = [&extractRegisterIndex]( const std::string &token, RegisterType &registerType ) -> uint8_t
			{
				size_t index;

				if ( token[0] == 'f' )
				{
					registerType = REGISTER_TYPE_FLOAT;
					if ( token[1] != 'r' )
						throw std::exception( "Malformed floating point register" );
					index = 2;
				}
				else if ( token[0] == 'd' )
				{
					registerType = REGISTER_TYPE_DOUBLE;
					if ( token[1] != 'f' || token[2] != 'r' )
						throw std::exception( "Malformed floating point register" );
					index = 3;
				}
				else
				{
					if ( token[0] != 'r' )
						throw std::exception( "Malformed register" );
					
					index = 1;

					registerType = REGISTER_TYPE_NATIVE;
					switch ( token[index] )
					{
					case 'b':
						registerType = REGISTER_TYPE_BYTE;
						++index;
						break;
					case 'w':
						registerType = REGISTER_TYPE_WORD;
						++index;
						break;
					case 'd':
						registerType = REGISTER_TYPE_DWORD;
						++index;
						break;
					}
					if ( token[index] == 'u' )
					{
						registerType = static_cast< RegisterType >( registerType + 1 );
						index++;
					}
				}
				return extractRegisterIndex( token, index );
			};

			auto tokenToFloatRegisterIndex = [&extractRegisterIndex] ( const std::string &token ) -> uint8_t
			{
				if ( token[0] != 'f' || token[1] != 'r' )
					throw std::exception( "Malformed floating point register" );
				return extractRegisterIndex( token, 2 );
			};

			auto tokenToDoubleRegisterIndex = [&extractRegisterIndex] ( const std::string &token ) -> uint8_t
			{
				if ( token[0] != 'd' || token[1] != 'f' || token[2] != 'r' )
					throw std::exception( "Malformed double register" );
				return extractRegisterIndex( token, 3 );
			};
			
			while ( it != end )
			{
				RegisterType registerType = REGISTER_TYPE_DWORD;
				std::string token = Lexer::ParseToken( it, end );
				if ( token.empty() )
					break;

				Lexer::TokenId tokenId = Lexer::TokenToTokenId( token );
				const InstructionCode &code = tokenId != Lexer::TokenId::TOKENID_INVALID ? TokenIdToInstructionCode[tokenId] : INSTR_CODE_INVALID;
				if ( code != INSTR_CODE_INVALID )
				{
					context.byteCode.emplace_back( code );
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
						if ( token == "-" )
							token += parseNextToken();

						switch ( instructionDesc.args[argIndex] )
						{
						case ARG_TYPE_REGISTER:
							registerIndex = tokenToRegisterIndex( token, registerType );
							context.byteCode.emplace_back( registerType );
							context.byteCode.emplace_back( registerIndex );
							break;
						case ARG_TYPE_FLOAT_REGISTER:
							context.byteCode.emplace_back( REGISTER_TYPE_FLOAT );
							context.byteCode.emplace_back( tokenToFloatRegisterIndex( token ) );
							break;
						case ARG_TYPE_DOUBLE_REGISTER:
							context.byteCode.emplace_back( REGISTER_TYPE_DOUBLE );
							context.byteCode.emplace_back( tokenToDoubleRegisterIndex( token ) );
							break;
						case ARG_TYPE_INT:
							switch ( registerType )
							{
							case REGISTER_TYPE_BYTE:
								AppendScalar< int8_t >( token, context.byteCode );
								break;
							case REGISTER_TYPE_UNSIGNED_BYTE:
								AppendScalar< uint8_t >( token, context.byteCode );
								break;
							case REGISTER_TYPE_WORD:
								AppendScalar< int16_t >( token, context.byteCode );
								break;
							case REGISTER_TYPE_UNSIGNED_WORD:
								AppendScalar< uint16_t >( token, context.byteCode );
								break;
							case REGISTER_TYPE_DWORD:
								AppendScalar< int32_t >( token, context.byteCode );
								break;
							case REGISTER_TYPE_UNSIGNED_DWORD:
								AppendScalar< uint32_t >( token, context.byteCode );
								break;
							case REGISTER_TYPE_NATIVE:
								AppendScalar< int64_t >( token, context.byteCode );
								break;
							case REGISTER_TYPE_UNSIGNED_NATIVE:
								AppendScalar< uint64_t >( token, context.byteCode );
								break;
							default:
								throw std::exception( "INTERNAL ERROR: Register type was not set" );
							}
							break;
						case ARG_TYPE_FLOAT:
							AppendScalar< float >( token, context.byteCode );
							break;
						case ARG_TYPE_DOUBLE:
							AppendScalar< double >( token, context.byteCode );
							break;
						case ARG_TYPE_ADDRESS:
							if ( WantsCodeAddress( code ) )
								HandleCodeLabel( context, token );
							else
								AppendAddress( context, token );
							break;
						}
					}
				}
				else
				{
					switch ( tokenId )
					{
					case Lexer::TokenId::TOKEN_INT8:
						ParseVariableDefinition< int8_t >( context, it, end );
						break;
					case Lexer::TokenId::TOKEN_INT16:
						ParseVariableDefinition< int16_t >( context, it, end );
						break;
					case Lexer::TokenId::TOKEN_INT32:
						ParseVariableDefinition< int32_t >( context, it, end );
						break;
					case Lexer::TokenId::TOKEN_INT64:
						ParseVariableDefinition< int64_t >( context, it, end );
						break;
					case Lexer::TokenId::TOKEN_UINT8:
						ParseVariableDefinition< uint8_t >( context, it, end );
						break;
					case Lexer::TokenId::TOKEN_UINT16:
						ParseVariableDefinition< uint16_t >( context, it, end );
						break;
					case Lexer::TokenId::TOKEN_UINT32:
						ParseVariableDefinition< uint32_t >( context, it, end );
						break;
					case Lexer::TokenId::TOKEN_UINT64:
						ParseVariableDefinition< uint64_t >( context, it, end );
						break;
					case Lexer::TokenId::TOKEN_FLOAT:
						ParseVariableDefinition< float >( context, it, end );
						break;
					case Lexer::TokenId::TOKEN_DOUBLE:
						ParseVariableDefinition< double >( context, it, end );
						break;
					case Lexer::TokenId::TOKEN_COLUMN:
						ParseLabel( context, it, end );
						break;
					default:
						throw std::exception( "Unrecognized instruction" );
					}
				}
			}

			for ( const auto &label : context.deferredLabelResolutions )
			{
				const auto labelIt = context.labels.find( std::get< 0 >( label ) );
				if ( labelIt == context.labels.cend() )
					throw std::exception( "Trying to reference an unknown variable" );

				int address = labelIt->second;
				memcpy( &context.byteCode[std::get< 1 >( label )], &address, sizeof( address ) );
			}

			context.byteCode.shrink_to_fit();
		}

		void Disassemble( const Context &context, std::string &text )
		{
			Context::AddressToNameMap addressToVariableNameMap;
			Context::AddressToNameMap addressToLabelNameMap;

			auto computeAddressToNameMap = [] ( const NameToAddressMap &nameToAddressMap, Context::AddressToNameMap &addressToNameMap )
			{
				addressToNameMap.reserve( nameToAddressMap.size() );
				for ( const auto &nameToAdress : nameToAddressMap )
					addressToNameMap[nameToAdress.second] = nameToAdress.first;
			};

			auto disassembleAddress = []( std::string &text, std::vector< uint8_t >::const_iterator &codeIt, Context::AddressToNameMap &addressToNameMap )
			{
				int address;

				memcpy( &address, &codeIt[0], sizeof( address ) );
				codeIt += sizeof( address );

				auto nameIt = addressToNameMap.find( address );
				if ( nameIt == addressToNameMap.cend() )
					throw std::exception( "There is no variable/label at this address" );
				text += nameIt->second;
			};

			auto disassembleRegister = []( std::string &text, std::vector< uint8_t >::const_iterator &codeIt, RegisterType &registerType )
			{
				registerType = static_cast< RegisterType >( *codeIt );
				++codeIt;
				uint8_t registerIndex = *codeIt;
				++codeIt;
				if ( registerType == REGISTER_TYPE_FLOAT )
				{
					text += "fr";
				}
				else if ( registerType == REGISTER_TYPE_DOUBLE )
				{
					text += "dfr";
				}
				else
				{
					text += 'r';
					switch ( registerType )
					{
					case REGISTER_TYPE_BYTE:
						text += 'b';
						break;
					case REGISTER_TYPE_UNSIGNED_BYTE:
						text += "bu";
						break;
					case REGISTER_TYPE_WORD:
						text += 'w';
						break;
					case REGISTER_TYPE_UNSIGNED_WORD:
						text += "wu";
						break;
					case REGISTER_TYPE_DWORD:
						text += 'd';
						break;
					case REGISTER_TYPE_UNSIGNED_DWORD:
						text += "du";
						break;
					case REGISTER_TYPE_NATIVE:
						break;
					case REGISTER_TYPE_UNSIGNED_NATIVE:
						text += 'u';
						break;
					case REGISTER_TYPE_FLOAT:
						text += 'f';
						break;
					default:
						throw std::exception( "INTERNAL ERROR: Invalid register type in code segment" );
					}
				}
				text += std::to_string( registerIndex );
			};

			auto appendLabel = [&addressToLabelNameMap] ( const Context &context, std::string &text, std::vector< uint8_t >::const_iterator &codeIt )
			{
				auto labelIt = addressToLabelNameMap.find( static_cast< int >( std::distance( context.byteCode.cbegin(), codeIt ) ) );
				if ( labelIt != addressToLabelNameMap.cend() )
				{
					text += ":";
					text += labelIt->second;
					text += "\n";
				}
			};

			text.clear();
			text.reserve( 1 * MB );

			computeAddressToNameMap( context.variables, addressToVariableNameMap );
			computeAddressToNameMap( context.labels, addressToLabelNameMap );

			auto it = context.byteCode.cbegin();
			auto end = context.byteCode.cend();

			while ( it != end )
			{
				RegisterType registerType = REGISTER_TYPE_DWORD;

				appendLabel( context, text, it );

				InstructionCode code = static_cast< InstructionCode >( *it );
				++it;

				text += InstructionCodeToString[code];

				const InstructionDesc &instructionDesc = InstructionCodeToIntructionDesc[code];
				if ( instructionDesc.argCount > 0 )
				{
					text += " ";

					for ( int argIndex = 0; argIndex < instructionDesc.argCount; ++argIndex )
					{
						if ( argIndex > 0 )
							text += ", ";
						switch ( instructionDesc.args[argIndex] )
						{
						case ARG_TYPE_REGISTER:
						case ARG_TYPE_FLOAT_REGISTER:
						case ARG_TYPE_DOUBLE_REGISTER:
							disassembleRegister( text, it, registerType );
							break;
						case ARG_TYPE_INT:
							switch ( registerType )
							{
							case REGISTER_TYPE_BYTE:
								DisassembleScalar< int8_t >( text, it );
								break;
							case REGISTER_TYPE_UNSIGNED_BYTE:
								DisassembleScalar< uint8_t >( text, it );
								break;
							case REGISTER_TYPE_WORD:
								DisassembleScalar< int16_t >( text, it );
								break;
							case REGISTER_TYPE_UNSIGNED_WORD:
								DisassembleScalar< uint16_t >( text, it );
								break;
							case REGISTER_TYPE_DWORD:
								DisassembleScalar< int32_t >( text, it );
								break;
							case REGISTER_TYPE_UNSIGNED_DWORD:
								DisassembleScalar< uint32_t >( text, it );
								break;
							case REGISTER_TYPE_NATIVE:
								DisassembleScalar< int64_t >( text, it );
								break;
							case REGISTER_TYPE_UNSIGNED_NATIVE:
								DisassembleScalar< uint64_t >( text, it );
								break;
							default:
								throw std::exception( "INTERNAL ERROR: Invalid register type in code segment" );
							}
							break;
						case ARG_TYPE_FLOAT:
							DisassembleScalar< float >( text, it );
							break;
						case ARG_TYPE_DOUBLE:
							DisassembleScalar< double >( text, it );
							break;
						case ARG_TYPE_ADDRESS:
							if ( WantsCodeAddress( code ) )
								disassembleAddress( text, it, addressToLabelNameMap );
							else
								disassembleAddress( text, it, addressToVariableNameMap );
							break;
						}
					}
				}

				text += '\n';
			}

			appendLabel( context, text, it );

			text.shrink_to_fit();
		}
	}
}
