#pragma once

#include "Assembler.h"

namespace Yal
{
	class VM
	{
	public:
		using NativeFunctionPointer = void(*)( const VM &vm );

		VM() {}

		void Init( size_t registerFileSize, size_t floatRegisterFileSize, const std::vector< uint8_t > &inByteCode, const std::vector< uint8_t > &inData, const NameToAddressMap& inVariables )
		{
			registers.resize( registerFileSize );
			floatRegisters.resize( floatRegisterFileSize );

			byteCode = inByteCode;
			data = inData;
			variables = inVariables;
		}

		void Run();

		int64_t GetRegisterValue( size_t index ) const { return registers[index]; }
		double GetFloatRegisterValue( size_t index ) const { return floatRegisters[index]; }
		const std::vector< uint8_t > &GetData() const { return data; }

		void SetData( const std::string& name, int64_t value );

	private:
		std::vector< uint8_t >	byteCode;
		std::vector< uint8_t >	data;
		NameToAddressMap		variables;
		std::vector< int64_t >	registers;
		std::vector< size_t >	stack;
		std::vector< double >	floatRegisters;
		size_t					ip;
		bool					compareResult;

		template< bool atAddress >							void RegisterLoad();
		template< bool atAddress >							void RegisterStore();
		template< typename scalar_type, bool atAddress >	void Store( int64_t value );
		template< typename scalar_type, bool atAddress >	scalar_type Load();

		int64_t RegisterToALU( size_t ipOffset );
		void	ALUToRegister( size_t ipOffset, int64_t value );
		double	FloatRegisterToALU( size_t ipOffset );
		void	ALUToFloatRegister( size_t ipOffset, double value );
		size_t	GetAddress();
		void	HandleJumpIfTrue();
		void	HandleCall();
		void	HandleCallIndirect();
		void	HandleCallNative();
	};
}