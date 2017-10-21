#include "stdafx.h"
#include "YalVM.h"
#include "Assembler.h"

namespace Yal
{
	void VM::Run()
	{
		uint8_t registerIndex;
		Assembler::RegisterType registerType;

		size_t ip = 0;
		while ( ip != byteCode.size() )
		{
			Assembler::InstructionCode instruction = static_cast< Assembler::InstructionCode >( byteCode[ip++] );
			switch ( instruction )
			{
			case Assembler::InstructionCode::INSTR_CODE_LOAD_IMMEDIATE:
				registerType = static_cast< Assembler::RegisterType >( byteCode[ip++] );
				registerIndex = byteCode[ip++];
				switch ( registerType )
				{
				case Assembler::RegisterType::REGISTER_TYPE_BYTE:
					registers[registerIndex] = static_cast< uint64_t >( byteCode[ip++] );
					break;
				case Assembler::RegisterType::REGISTER_TYPE_NATIVE:
					registers[registerIndex] = *reinterpret_cast< uint64_t* >( &byteCode[ip] );
					ip += sizeof( uint64_t );
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}
	}
}