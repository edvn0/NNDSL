#include "parser.hpp"

#include "lexer.hpp"

namespace NNDSL {
	
    Parser::Parser(Lexer&& lexer): tokens(lexer.lex()) { }

} // namespace NNDSL
