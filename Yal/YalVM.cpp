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

	template< typename scalar_type, bool atAddress > scalar_type VM::Load()
	{
		scalar_type value;

		if ( atAddress )
		{
			memcpy( &value, &data[GetAddress()], sizeof( value ) );
		}
		else
		{
			memcpy( &value, &byteCode[ip], sizeof( value ) );
			ip += sizeof( value );
		}
		return value;
	}

	template< bool atAddress > void VM::RegisterLoad()
	{
		RegisterType registerType = static_cast< RegisterType >( byteCode[ip++] );
		uint8_t registerIndex = byteCode[ip++];

		switch ( registerType )
		{
		case REGISTER_TYPE_BYTE:
			registers[registerIndex] = static_cast< int64_t >( Load< int8_t, atAddress >() );
			break;
		case REGISTER_TYPE_UNSIGNED_BYTE:
			registers[registerIndex] = static_cast< int64_t >( Load< uint8_t, atAddress >() );
			break;
		case REGISTER_TYPE_WORD:
			registers[registerIndex] = static_cast< int64_t >( Load< int16_t, atAddress >() );
			break;
		case REGISTER_TYPE_UNSIGNED_WORD:
			registers[registerIndex] = static_cast< int64_t >( Load< uint16_t, atAddress >() );
			break;
		case REGISTER_TYPE_DWORD:
			registers[registerIndex] = static_cast< int64_t >( Load< int32_t, atAddress >() );
			break;
		case REGISTER_TYPE_UNSIGNED_DWORD:
			registers[registerIndex] = static_cast< int64_t >( Load< uint32_t, atAddress >() );
			break;
		case REGISTER_TYPE_NATIVE:
			registers[registerIndex] = static_cast< int64_t >( Load< int64_t, atAddress >() );
			break;
		case REGISTER_TYPE_UNSIGNED_NATIVE:
			registers[registerIndex] = static_cast< int64_t >( Load< uint64_t, atAddress >() );
			break;
		case REGISTER_TYPE_FLOAT:
			floatRegisters[registerIndex] = Load< float, atAddress >();
			break;
		case REGISTER_TYPE_DOUBLE:
			floatRegisters[registerIndex] = Load< double, atAddress >();
			break;
		default:
			throw std::exception( "INTERNAL ERROR: Invalid register type in code segment" );
		}
	};

	size_t VM::GetAddress()
	{
		int address;

		memcpy( &address, &byteCode[ip], sizeof( address ) );
		ip += sizeof( address );

		return static_cast< size_t >( address );
	}

	template< typename scalar_type, bool atAddress > void VM::Store( int64_t value )
	{
		if ( atAddress )
		{
			memcpy( &data[GetAddress()], &value, sizeof( scalar_type ) );
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

	template< bool atAddress >
	void VM::RegisterStore()
	{
		RegisterType registerType = static_cast< RegisterType >( byteCode[ip++] );
		uint8_t registerIndex = byteCode[ip++];

		switch ( registerType )
		{
		case REGISTER_TYPE_BYTE:
			Store< int8_t, atAddress >( registers[registerIndex] );
			break;
		case REGISTER_TYPE_UNSIGNED_BYTE:
			Store< uint8_t, atAddress >( registers[registerIndex] );
			break;
		case REGISTER_TYPE_WORD:
			Store< int16_t, atAddress >( registers[registerIndex] );
			break;
		case REGISTER_TYPE_UNSIGNED_WORD:
			Store< uint16_t, atAddress >( registers[registerIndex] );
			break;
		case REGISTER_TYPE_DWORD:
			Store< int32_t, atAddress >( registers[registerIndex] );
			break;
		case REGISTER_TYPE_UNSIGNED_DWORD:
			Store< uint32_t, atAddress >( registers[registerIndex] );
			break;
		case REGISTER_TYPE_NATIVE:
			Store< int64_t, atAddress >( registers[registerIndex] );
			break;
		case REGISTER_TYPE_UNSIGNED_NATIVE:
			Store< uint64_t, atAddress >( registers[registerIndex] );
			break;
		case REGISTER_TYPE_FLOAT:
			Store< float, atAddress >( registers[registerIndex] );
			break;
		case REGISTER_TYPE_DOUBLE:
			Store< double, atAddress >( registers[registerIndex] );
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

	double VM::FloatRegisterToALU( size_t ipOffset )
	{
		RegisterType registerType = static_cast< RegisterType >( byteCode[ip + ipOffset] );
		uint8_t registerIndex = byteCode[ip + ipOffset + 1];
		switch ( registerType )
		{
		case REGISTER_TYPE_FLOAT:
			return static_cast< float >( floatRegisters[registerIndex] );
		case REGISTER_TYPE_DOUBLE:
			return floatRegisters[registerIndex];
		default:
			throw std::exception( "INTERNAL ERROR: Invalid foating point register type in code segment" );
		}
	}

	void VM::ALUToFloatRegister( size_t ipOffset, double value )
	{
		RegisterType registerType = static_cast< RegisterType >( byteCode[ip + ipOffset] );
		uint8_t registerIndex = byteCode[ip + ipOffset + 1];
		switch ( registerType )
		{
		case REGISTER_TYPE_FLOAT:
			floatRegisters[registerIndex] = static_cast< float >( value );
			break;
		case REGISTER_TYPE_DOUBLE:
			floatRegisters[registerIndex] = value;
			break;
		default:
			throw std::exception( "INTERNAL ERROR: Invalid foating point register type in code segment" );
		}
	}

	void VM::HandleJumpIfTrue()
	{
		size_t address = GetAddress();
		if ( compareResult )
			ip = address;
	}

	void VM::HandleCall()
	{
		size_t address = GetAddress();
		stack.emplace_back( static_cast< int64_t >( ip ) );
		ip = address;
	}

	void VM::HandleCallIndirect()
	{
		size_t address = static_cast< size_t >( RegisterToALU( 0 ) );
		stack.emplace_back( ip + 2 );
		ip = address;
	}

	void VM::HandleCallNative()
	{
		NativeFunctionPointer nativeFunction;

		size_t address = GetAddress();
		stack.emplace_back( ip );

		memcpy( &nativeFunction, &data[address], sizeof( nativeFunction ) );

		nativeFunction( *this );
	}

	void VM::Run()
	{
		ip = 0;
		compareResult = false;
		while ( ip != byteCode.size() )
		{
			InstructionCode instruction = static_cast< InstructionCode >( byteCode[ip++] );
			switch ( instruction )
			{
			case InstructionCode::INSTR_CODE_LOAD_IMMEDIATE:
			case InstructionCode::INSTR_CODE_LOAD_CODE_ADDRESS:
			case InstructionCode::INSTR_CODE_FLOAT_LOAD_IMMEDIATE:
			case InstructionCode::INSTR_CODE_DOUBLE_LOAD_IMMEDIATE:
				RegisterLoad< false >();
				break;
			case InstructionCode::INSTR_CODE_LOAD_ADDRESS:
				RegisterLoad< true >();
				break;
			case InstructionCode::INSTR_CODE_STORE:
				RegisterStore< false >();
				break;
			case InstructionCode::INSTR_CODE_STORE_ADDRESS:
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
			case InstructionCode::INSTR_CODE_INCREMENT:
				ALUToRegister( 0, RegisterToALU( 0 ) + 1 );
				ip += 2;
				break;
			case InstructionCode::INSTR_CODE_ABSOLUTE:
				ALUToRegister( 0, abs( RegisterToALU( 2 ) ) );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_NEGATE:
				ALUToRegister( 0, -RegisterToALU( 2 ) );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_DECREMENT:
				ALUToRegister( 0, RegisterToALU( 0 ) - 1 );
				ip += 2;
				break;
			case InstructionCode::INSTR_CODE_CAST_TO_INTEGER:
				ALUToRegister( 0, static_cast< int64_t >( FloatRegisterToALU( 2 ) ) );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_SHIFT_LEFT:
				ALUToRegister( 0, RegisterToALU( 2 ) << RegisterToALU( 4 ) );
				ip += 6;
				break;
			case InstructionCode::INSTR_CODE_SHIFT_RIGHT:
				ALUToRegister( 0, RegisterToALU( 2 ) >> RegisterToALU( 4 ) );
				ip += 6;
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
			case InstructionCode::INSTR_CODE_COMPARE_EQUAL:
				compareResult = RegisterToALU( 0 ) == RegisterToALU( 2 );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_COMPARE_GREATER_THAN:
				compareResult = RegisterToALU( 0 ) > RegisterToALU( 2 );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_COMPARE_GREATER_EQUAL:
				compareResult = RegisterToALU( 0 ) >= RegisterToALU( 2 );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_COMPARE_LESS_THAN:
				compareResult = RegisterToALU( 0 ) < RegisterToALU( 2 );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_COMPARE_LESS_EQUAL:
				compareResult = RegisterToALU( 0 ) <= RegisterToALU( 2 );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_JUMP:
				ip = GetAddress();
				break;
			case InstructionCode::INSTR_CODE_JUMP_IF_TRUE:
				HandleJumpIfTrue();
				break;
			case InstructionCode::INSTR_CODE_FLOAT_ADD:
			case InstructionCode::INSTR_CODE_DOUBLE_ADD:
				ALUToFloatRegister( 0, FloatRegisterToALU( 2 ) + FloatRegisterToALU( 4 ) );
				ip += 6;
				break;
			case InstructionCode::INSTR_CODE_FLOAT_SUBTRACT:
			case InstructionCode::INSTR_CODE_DOUBLE_SUBTRACT:
				ALUToFloatRegister( 0, FloatRegisterToALU( 2 ) - FloatRegisterToALU( 4 ) );
				ip += 6;
				break;
			case InstructionCode::INSTR_CODE_FLOAT_MULTIPLY:
			case InstructionCode::INSTR_CODE_DOUBLE_MULTIPLY:
				ALUToFloatRegister( 0, FloatRegisterToALU( 2 ) * FloatRegisterToALU( 4 ) );
				ip += 6;
				break;
			case InstructionCode::INSTR_CODE_FLOAT_DIVIDE:
			case InstructionCode::INSTR_CODE_DOUBLE_DIVIDE:
				ALUToFloatRegister( 0, FloatRegisterToALU( 2 ) / FloatRegisterToALU( 4 ) );
				ip += 6;
				break;
			case InstructionCode::INSTR_CODE_FLOAT_COS:
			case InstructionCode::INSTR_CODE_DOUBLE_COS:
				ALUToFloatRegister( 0, cos( FloatRegisterToALU( 2 ) ) );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_FLOAT_SIN:
			case InstructionCode::INSTR_CODE_DOUBLE_SIN:
				ALUToFloatRegister( 0, sin( FloatRegisterToALU( 2 ) ) );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_FLOAT_SQRT:
			case InstructionCode::INSTR_CODE_DOUBLE_SQRT:
				ALUToFloatRegister( 0, sqrt( FloatRegisterToALU( 2 ) ) );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_FLOAT_ABSOLUTE:
			case InstructionCode::INSTR_CODE_DOUBLE_ABSOLUTE:
				ALUToFloatRegister( 0, abs( FloatRegisterToALU( 2 ) ) );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_FLOAT_NEGATE:
			case InstructionCode::INSTR_CODE_DOUBLE_NEGATE:
				ALUToFloatRegister( 0, -FloatRegisterToALU( 2 ) );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_FLOAT_RND:
			case InstructionCode::INSTR_CODE_DOUBLE_RND:
				ALUToFloatRegister( 0, round( FloatRegisterToALU( 2 ) ) );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_FLOAT_CAST:
			case InstructionCode::INSTR_CODE_DOUBLE_CAST:
				ALUToFloatRegister( 0, static_cast< double >( RegisterToALU( 2 ) ) );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_FLOAT_COMPARE_EQUAL:
			case InstructionCode::INSTR_CODE_DOUBLE_COMPARE_EQUAL:
				compareResult = FloatRegisterToALU( 0 ) == FloatRegisterToALU( 2 );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_FLOAT_COMPARE_GREATER_THAN:
			case InstructionCode::INSTR_CODE_DOUBLE_COMPARE_GREATER_THAN:
				compareResult = FloatRegisterToALU( 0 ) > FloatRegisterToALU( 2 );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_FLOAT_COMPARE_GREATER_EQUAL:
			case InstructionCode::INSTR_CODE_DOUBLE_COMPARE_GREATER_EQUAL:
				compareResult = FloatRegisterToALU( 0 ) >= FloatRegisterToALU( 2 );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_FLOAT_COMPARE_LESS_THAN:
			case InstructionCode::INSTR_CODE_DOUBLE_COMPARE_LESS_THAN:
				compareResult = FloatRegisterToALU( 0 ) < FloatRegisterToALU( 2 );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_FLOAT_COMPARE_LESS_EQUAL:
			case InstructionCode::INSTR_CODE_DOUBLE_COMPARE_LESS_EQUAL:
				compareResult = FloatRegisterToALU( 0 ) <= FloatRegisterToALU( 2 );
				ip += 4;
				break;
			case InstructionCode::INSTR_CODE_CALL:
				HandleCall();
				break;;
			case InstructionCode::INSTR_CODE_CALL_INDIRECT:
				HandleCallIndirect();
				break;;
			case InstructionCode::INSTR_CODE_CALL_NATIVE:
				HandleCallNative();
				break;
			case InstructionCode::INSTR_CODE_RETURN:
				if ( stack.empty() )
					return;
				ip = static_cast< size_t >( stack.back() );
				stack.pop_back();
				break;
			case InstructionCode::INSTR_CODE_PUSH:
				stack.push_back( RegisterToALU( 0 ) );
				ip += 2;
			case InstructionCode::INSTR_CODE_POP:
				if ( stack.empty() )
					throw std::exception( "Yal::VM::Run(Pop) - Stack is empty" );
				ALUToRegister( 0, stack.back() );
				ip += 2;
			default:
				break;
			}
		}
	}

	void VM::SetData( const std::string& name, int64_t value )
	{
		auto it = variables.find( name );
		if ( it == variables.cend() )
			throw std::exception( "Yal::VM::SetData - Invalid varaible name specified" );
		memcpy( &data[static_cast< size_t >( it->second )], &value, sizeof( value ) );
	}
}