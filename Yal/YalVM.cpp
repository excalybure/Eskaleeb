#include "stdafx.h"
#include "YalVM.h"
#include "Assembler.h"

namespace Yal
{
	template< typename scalar_type >
	static scalar_type LoadImmediate( const std::vector< uint8_t > &byteCode, size_t &ip )
	{
		scalar_type value;

		memcpy( &value, &byteCode[ip], sizeof( value ) );
		ip += sizeof( value );
		return value;
	}

	void VM::RegisterLoad()
	{
		RegisterType registerType = static_cast< RegisterType >( byteCode[ip++] );
		uint8_t registerIndex = byteCode[ip++];

		if ( registerType == REGISTER_TYPE_FLOAT )
		{
			// text += "fr";
		}
		else if ( registerType == REGISTER_TYPE_DOUBLE )
		{
			// text += "dfr";
		}
		else
		{
			switch ( registerType )
			{
			case REGISTER_TYPE_BYTE:
				registers[registerIndex] = static_cast< uint64_t >( LoadImmediate< int8_t >( byteCode, ip ) );
				break;
			case REGISTER_TYPE_UNSIGNED_BYTE:
				registers[registerIndex] = static_cast< uint64_t >( LoadImmediate< uint8_t >( byteCode, ip ) );
				break;
			case REGISTER_TYPE_WORD:
				registers[registerIndex] = static_cast< uint64_t >( LoadImmediate< int16_t >( byteCode, ip ) );
				break;
			case REGISTER_TYPE_UNSIGNED_WORD:
				registers[registerIndex] = static_cast< uint64_t >( LoadImmediate< uint16_t >( byteCode, ip ) );
				break;
			case REGISTER_TYPE_DWORD:
				registers[registerIndex] = static_cast< uint64_t >( LoadImmediate< int32_t >( byteCode, ip ) );
				break;
			case REGISTER_TYPE_UNSIGNED_DWORD:
				registers[registerIndex] = static_cast< uint64_t >( LoadImmediate< uint32_t >( byteCode, ip ) );
				break;
			case REGISTER_TYPE_NATIVE:
				registers[registerIndex] = static_cast< uint64_t >( LoadImmediate< int64_t >( byteCode, ip ) );
				break;
			case REGISTER_TYPE_UNSIGNED_NATIVE:
				registers[registerIndex] = static_cast< uint64_t >( LoadImmediate< uint64_t >( byteCode, ip ) );
				break;
			case REGISTER_TYPE_FLOAT:
				// text += 'f';
				break;
			default:
				throw std::exception( "INTERNAL ERROR: Invalid register type in code segment" );
			}
		}
	};


	void VM::Run()
	{
		ip = 0;
		while ( ip != byteCode.size() )
		{
			InstructionCode instruction = static_cast< InstructionCode >( byteCode[ip++] );
			switch ( instruction )
			{
			case InstructionCode::INSTR_CODE_LOAD_IMMEDIATE:
				RegisterLoad();
				break;
			default:
				break;
			}
		}
	}
}