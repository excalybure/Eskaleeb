#include "stdafx.h"
#include "YalVM.h"
#include "Assembler.h"

namespace Yal
{
	template< typename scalarType >
	static int64_t MaskSignedValue( int64_t value )
	{
		if ( value >= 0 )
			return static_cast< int64_t >( value & ( ( static_cast< uint64_t >( 1 ) << sizeof( scalarType ) * 8 ) - 1 ) );
		return static_cast< int64_t >( value | ( 0xffffffffffffffff ^ ( ( static_cast< uint64_t >( 1 ) << sizeof( scalarType ) * 8 ) - 1 ) ) );
	}

	template< typename scalarType >
	static int64_t MaskUnsignedValue( int64_t value )
	{
		return static_cast< int64_t >( value & ( ( static_cast< uint64_t >( 1 ) << sizeof( scalarType ) * 8 ) - 1 ) );
	}

	template<> int64_t MaskSignedValue< int64_t >( int64_t value )
	{
		return value;
	}

	template<> int64_t MaskUnsignedValue< uint64_t >( int64_t value )
	{
		return static_cast< int64_t >( value & 0x7fffffffffffffff );
	}

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

		switch ( registerType )
		{
		case REGISTER_TYPE_BYTE:
			registers[registerIndex] = static_cast< int64_t >( LoadImmediate< int8_t >( byteCode, ip ) );
			break;
		case REGISTER_TYPE_UNSIGNED_BYTE:
			registers[registerIndex] = static_cast< int64_t >( LoadImmediate< uint8_t >( byteCode, ip ) );
			break;
		case REGISTER_TYPE_WORD:
			registers[registerIndex] = static_cast< int64_t >( LoadImmediate< int16_t >( byteCode, ip ) );
			break;
		case REGISTER_TYPE_UNSIGNED_WORD:
			registers[registerIndex] = static_cast< int64_t >( LoadImmediate< uint16_t >( byteCode, ip ) );
			break;
		case REGISTER_TYPE_DWORD:
			registers[registerIndex] = static_cast< int64_t >( LoadImmediate< int32_t >( byteCode, ip ) );
			break;
		case REGISTER_TYPE_UNSIGNED_DWORD:
			registers[registerIndex] = static_cast< int64_t >( LoadImmediate< uint32_t >( byteCode, ip ) );
			break;
		case REGISTER_TYPE_NATIVE:
			registers[registerIndex] = static_cast< int64_t >( LoadImmediate< int64_t >( byteCode, ip ) );
			break;
		case REGISTER_TYPE_UNSIGNED_NATIVE:
			registers[registerIndex] = static_cast< int64_t >( LoadImmediate< uint64_t >( byteCode, ip ) );
			break;
		case REGISTER_TYPE_FLOAT:
			floatRegisters[registerIndex] = LoadImmediate< float >( byteCode, ip );
			break;
		case REGISTER_TYPE_DOUBLE:
			floatRegisters[registerIndex] = LoadImmediate< double >( byteCode, ip );
			break;
		default:
			throw std::exception( "INTERNAL ERROR: Invalid register type in code segment" );
		}
	};

	template< typename scalar_type, bool immediate > void VM::Store( int64_t value )
	{
		if ( immediate )
		{
			int address;

			memcpy( &address, &byteCode[ip], sizeof( address ) );
			ip += sizeof( address );

			memcpy( &data[static_cast< size_t>( address )], &value, sizeof( scalar_type ) );
		}
		else
		{
			RegisterType registerType = static_cast< RegisterType >( byteCode[ip++] );
			uint8_t registerIndex = byteCode[ip++];
			if ( registerType != REGISTER_TYPE_NATIVE )
				throw std::exception( "INTERNAL ERROR: Invalid register type for store instruction" );

			memcpy( &data[static_cast< size_t>( registers[registerIndex] )], &value, sizeof( scalar_type ) );
		}
	}

	template< bool immediate >
	void VM::RegisterStore()
	{
		RegisterType registerType = static_cast< RegisterType >( byteCode[ip++] );
		uint8_t registerIndex = byteCode[ip++];

		switch ( registerType )
		{
		case REGISTER_TYPE_BYTE:
			Store< int8_t, immediate >( registers[registerIndex] );
			break;
		case REGISTER_TYPE_UNSIGNED_BYTE:
			Store< uint8_t, immediate >( registers[registerIndex] );
			break;
		case REGISTER_TYPE_WORD:
			Store< int16_t, immediate >( registers[registerIndex] );
			break;
		case REGISTER_TYPE_UNSIGNED_WORD:
			Store< uint16_t, immediate >( registers[registerIndex] );
			break;
		case REGISTER_TYPE_DWORD:
			Store< int32_t, immediate >( registers[registerIndex] );
			break;
		case REGISTER_TYPE_UNSIGNED_DWORD:
			Store< uint32_t, immediate >( registers[registerIndex] );
			break;
		case REGISTER_TYPE_NATIVE:
			Store< int64_t, immediate >( registers[registerIndex] );
			break;
		case REGISTER_TYPE_UNSIGNED_NATIVE:
			Store< uint64_t, immediate >( registers[registerIndex] );
			break;
		case REGISTER_TYPE_FLOAT:
			Store< float, immediate >( registers[registerIndex] );
			break;
		case REGISTER_TYPE_DOUBLE:
			Store< double, immediate >( registers[registerIndex] );
			break;
		default:
			throw std::exception( "INTERNAL ERROR: Invalid register type in code segment" );
		}
	};

	int64_t VM::RegisterToALU( size_t ipOffset )
	{
		RegisterType registerType = static_cast< RegisterType >( byteCode[ip + ipOffset] );
		uint8_t registerIndex = byteCode[ip + ipOffset + 1];
		switch ( registerType )
		{
		case REGISTER_TYPE_BYTE:
			return MaskSignedValue< int8_t >( registers[registerIndex] );
		case REGISTER_TYPE_UNSIGNED_BYTE:
			return MaskUnsignedValue< uint8_t >( registers[registerIndex] );
		case REGISTER_TYPE_WORD:
			return MaskSignedValue< int16_t >( registers[registerIndex] );;
		case REGISTER_TYPE_UNSIGNED_WORD:
			return MaskUnsignedValue< uint16_t >( registers[registerIndex] );
		case REGISTER_TYPE_DWORD:
			return MaskSignedValue< int32_t >( registers[registerIndex] );;
		case REGISTER_TYPE_UNSIGNED_DWORD:
			return MaskUnsignedValue< uint32_t >( registers[registerIndex] );
		case REGISTER_TYPE_NATIVE:
			return MaskSignedValue< int64_t >( registers[registerIndex] );
		case REGISTER_TYPE_UNSIGNED_NATIVE:
			return MaskUnsignedValue< uint64_t >( registers[registerIndex] );
		default:
			throw std::exception( "INTERNAL ERROR: Invalid register type in code segment" );
		}
	}

	void VM::ALUToRegister( size_t ipOffset, int64_t value )
	{
		RegisterType registerType = static_cast< RegisterType >( byteCode[ip + ipOffset] );
		uint8_t registerIndex = byteCode[ip + ipOffset + 1];
		switch ( registerType )
		{
		case REGISTER_TYPE_BYTE:
			registers[registerIndex] = MaskSignedValue< int8_t >( value );
			break;
		case REGISTER_TYPE_UNSIGNED_BYTE:
			registers[registerIndex] = MaskUnsignedValue< uint8_t >( value );
			break;
		case REGISTER_TYPE_WORD:
			registers[registerIndex] = MaskSignedValue< int16_t >( value );
			break;
		case REGISTER_TYPE_UNSIGNED_WORD:
			registers[registerIndex] = MaskUnsignedValue< uint16_t >( value );
			break;
		case REGISTER_TYPE_DWORD:
			registers[registerIndex] = MaskSignedValue< int32_t >( value );
			break;
		case REGISTER_TYPE_UNSIGNED_DWORD:
			registers[registerIndex] = MaskUnsignedValue< uint32_t >( value );
			break;
		case REGISTER_TYPE_NATIVE:
			registers[registerIndex] = MaskSignedValue< int64_t >( value );
			break;
		case REGISTER_TYPE_UNSIGNED_NATIVE:
			registers[registerIndex] = MaskUnsignedValue< uint64_t >( value );
			break;
		default:
			throw std::exception( "INTERNAL ERROR: Invalid register type in code segment" );
		}
	}

	void VM::Run()
	{
		ip = 0;
		while ( ip != byteCode.size() )
		{
			InstructionCode instruction = static_cast< InstructionCode >( byteCode[ip++] );
			switch ( instruction )
			{
			case InstructionCode::INSTR_CODE_LOAD_IMMEDIATE:
			case InstructionCode::INSTR_CODE_FLOAT_LOAD_IMMEDIATE:
			case InstructionCode::INSTR_CODE_DOUBLE_LOAD_IMMEDIATE:
				RegisterLoad();
				break;
			case InstructionCode::INSTR_CODE_STORE:
				RegisterStore< false >();
				break;
			case InstructionCode::INSTR_CODE_STORE_IMMEDIATE:
				RegisterStore< true >();
				break;
			case InstructionCode::INSTR_CODE_MOVE:
				ALUToRegister( 0, RegisterToALU( 2 ) );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_AND:
				ALUToRegister( 0, RegisterToALU( 2 ) & RegisterToALU( 4 ) );
				ip += 6;
				break;
			case InstructionCode::INSTR_CODE_OR:
				ALUToRegister( 0, RegisterToALU( 2 ) | RegisterToALU( 4 ) );
				ip += 6;
				break;
			case InstructionCode::INSTR_CODE_XOR:
				ALUToRegister( 0, RegisterToALU( 2 ) ^ RegisterToALU( 4 ) );
				ip += 6;
				break;
			case InstructionCode::INSTR_CODE_COMPLEMENT:
				ALUToRegister( 0, ~RegisterToALU( 2 ) );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_ADD:
				ALUToRegister( 0, RegisterToALU( 2 ) + RegisterToALU( 4 ) );
				ip += 6;
				break;
			case InstructionCode::INSTR_CODE_SUBTRACT:
				ALUToRegister( 0, RegisterToALU( 2 ) - RegisterToALU( 4 ) );
				ip += 6;
				break;
			case InstructionCode::INSTR_CODE_MULTIPLY:
				ALUToRegister( 0, RegisterToALU( 2 ) * RegisterToALU( 4 ) );
				ip += 6;
				break;
			case InstructionCode::INSTR_CODE_DIVIDE:
				ALUToRegister( 0, RegisterToALU( 2 ) / RegisterToALU( 4 ) );
				ip += 6;
				break;
			case InstructionCode::INSTR_CODE_LOGICAL_AND:
				ALUToRegister( 0, RegisterToALU( 2 ) && RegisterToALU( 4 ) );
				ip += 6;
				break;
			case InstructionCode::INSTR_CODE_LOGICAL_OR:
				ALUToRegister( 0, RegisterToALU( 2 ) || RegisterToALU( 4 ) );
				ip += 6;
				break;
			case InstructionCode::INSTR_CODE_LOGICAL_XOR:
				ALUToRegister( 0, ( RegisterToALU( 2 ) != 0 ) ^ ( RegisterToALU( 4 ) != 0 ) );
				ip += 6;
				break;
			case InstructionCode::INSTR_CODE_LOGICAL_NOT:
				ALUToRegister( 0, !RegisterToALU( 2 ) );
				ip += 4;
				break;
			default:
				break;
			}
		}
	}
}