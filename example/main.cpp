#include "nndsl_pch.hpp"

#include "file_io.hpp"
#include "lexer.hpp"
#include "parser.hpp"

auto main(int argc, char** argv) -> int
{
	auto read = NNDSL::IO::read_file("nndsl.txt");
	std::cout << read << "\n";
	NNDSL::Lexer lexer(read);
	auto tokens = lexer.lex();
	NNDSL::Parser parser(std::move(lexer));
	for (const auto& token : tokens) {
		try {
			std::cout << token << "\n";
		} catch (std::exception e) {
			(void)e;
		}
	}
    NNDSL::Interpreter interpreter(parser);

	return 0;
}