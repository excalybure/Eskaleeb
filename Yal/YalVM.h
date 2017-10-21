#pragma once

namespace Yal
{

	class VM
	{
	public:
		VM() {}

		void Init( size_t registerFileSize, size_t floatRegisterFileSize, const std::vector< uint8_t > &inByteCode, const std::vector< uint8_t > &inData )
		{
			registers.resize( registerFileSize );
			floatRegisters.resize( floatRegisterFileSize );

			byteCode = inByteCode;
			data = inData;
		}

		void Run();

		uint64_t getRegisterValue( size_t index ) const { return registers[index]; }
		double getFloatRegisterValue( size_t index ) const { return floatRegisters[index]; }

	private:
		std::vector< uint8_t >	byteCode;
		std::vector< uint8_t >	data;
		std::vector< uint64_t >	registers;
		std::vector< double >	floatRegisters;
	};
}