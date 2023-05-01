#pragma once

#include <cstdint>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

namespace NNDSL::Lex {

	enum class Type : std::uint8_t {
		Character,
		Number,
		Comma,
		String,
		Identifier,
		Keyword,
		LeftBracket,
		RightBracket,
		LeftParenthesis,
		RightParenthesis,
		LeftSquareBracket,
		RightSquareBracket,
		Equals,
		Ampersand,
		SemiColon,
		Colon,
		LeftAngleBracket,
		RightAngleBracket,
		Dot,
		Exclamation
	};
	constexpr const char* to_string(Type);

	struct Token {
		std::variant<char, int, std::string> data;
		Type token_type;
		std::uint16_t column;
		std::uint16_t row;

		friend std::ostream& operator<<(std::ostream& os, const Token& dt);
	};

	class Lexer {
	public:
		template <class T>
		explicit Lexer(T&& in_program) noexcept
			: program(std::forward<T>(in_program))
		{
		}

		std::vector<Token> lex();

	private:
		char peek(int offset = 0);
		char consume();
		void move_past(int positions);

		std::size_t index { 0 };
		std::uint32_t row { 0 };
		std::uint32_t column { 0 };
		std::string program;
		bool is_keyword(const std::string& string);
	};

} // namespace NNDSL::Lex
