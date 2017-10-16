#pragma once

namespace Yal
{
	namespace Assembler
	{
		static constexpr size_t KB = 1024;
		static constexpr size_t MB = KB * KB;

		struct Context
		{
			using NameToAddressMap = std::unordered_map< std::string, int >;
			using AddressToNameMap = std::unordered_map< int, std::string >;

			std::string				source;
			std::vector< uint8_t >	byteCode;
			std::vector< uint8_t >	data;
			NameToAddressMap		variables;
			NameToAddressMap		labels;
		};

		void Assemble( Context &context );
		void Disassemble( const Context &context, std::string &text );
	}
}
