#include "lexer.hpp"

#include <vector>

namespace NNDSL::Parse {

	class Parser {
	public:
		explicit Parser(Lex::Lexer&&);

	private:
		std::vector<Lex::Token> tokens;
	};

} // namespace NNDSL::Parse
