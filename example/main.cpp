#include "nndsl_pch.hpp"

#include "file_io.hpp"
#include "lexer.hpp"
#include "parser.hpp"

using namespace NNDSL;

auto main(int argc, char** argv) -> int
{
	auto read = IO::read_file("nndsl.txt");
	std::cout << read << "\n";
	Lex::Lexer lexer(read);
	auto tokens = lexer.lex();
	Parse::Parser parser(std::move(lexer));
	for (const auto& token : tokens) {
		std::cout << token << "\n";
	}
	// NNDSL::Interpreter interpreter(parser);

	return 0;
}
