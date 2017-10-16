#pragma once

namespace Yal
{
	namespace Assembler
	{
		static constexpr size_t KB = 1024;
		static constexpr size_t MB = KB * KB;

		struct Context
		{
			std::string								source;
			std::vector< uint8_t >					byteCode;
			std::vector< uint8_t >					data;
			std::unordered_map< std::string, int >	variables;
		};

		void Assemble( Context &context );
		void Disassemble( const Context &context, std::string &text );
	}
}
