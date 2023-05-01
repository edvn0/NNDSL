#include "parser.hpp"

#include "lexer.hpp"

namespace NNDSL::Parse {

	Parser::Parser(Lex::Lexer&& lexer)
		: tokens(lexer.lex())
	{
	}

} // namespace NNDSL::Parse
