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

		int64_t GetRegisterValue( size_t index ) const { return registers[index]; }
		double GetFloatRegisterValue( size_t index ) const { return floatRegisters[index]; }
		const std::vector< uint8_t > &GetData() const { return data; }

	private:
		std::vector< uint8_t >	byteCode;
		std::vector< uint8_t >	data;
		std::vector< int64_t >	registers;
		std::vector< double >	floatRegisters;
		size_t					ip;
		bool					compareResult;
		uint8_t					padding[7];

		void RegisterLoad();
		template< bool immediate > void RegisterStore();
		int64_t RegisterToALU( size_t ipOffset );
		void ALUToRegister( size_t ipOffset, int64_t value );
		double FloatRegisterToALU( size_t ipOffset );
		template< typename scalar_type, bool immediate > void Store( int64_t value );
		size_t GetAddress();
		void HandleJumpIfTrue();
	};
}