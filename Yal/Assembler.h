#pragma once

namespace Yal
{
	namespace Assembler
	{
		static constexpr size_t KB = 1024;
		static constexpr size_t MB = KB * KB;

		void Assemble( const std::string &text, std::vector< uint8_t > &prog, std::vector< uint8_t > &data );
		void Disassemble( const std::vector< uint8_t > &prog, std::string &text );
	}
}
