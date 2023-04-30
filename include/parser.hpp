#include "lexer.hpp"

#include <vector>

namespace NNDSL {

	class Parser {
	public:
		explicit Parser(Lexer&&);

    private:
        std::vector<Token> tokens;
    };

} // namespace NNDSL
