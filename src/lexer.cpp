#include "nndsl_pch.hpp"

#include "lexer.hpp"

namespace NNDSL::Lex {

	static const auto keywords()
	{
		static const std::unordered_set<std::string> all_keywords { "Network", "Layers", "Input", "Output", "Nodes", "Size", "Function", "Loss",
			"return", "mod", "await", "Void", "Integer", "fn", "if", "Hooks", "const", "mut", "while", "import", "namespace" };
		return all_keywords;
	}

	constexpr auto is_numeric(char value) { return value >= '0' && value < '9'; }

	constexpr const char* to_string(Type type)
	{
		switch (type) {
		case Type::Character:
			return "Character";
		case Type::Number:
			return "Number";
		case Type::Comma:
			return "Comma";
		case Type::String:
			return "String";
		case Type::Identifier:
			return "Identifier";
		case Type::Keyword:
			return "Keyword";
		case Type::LeftBracket:
			return "LeftBracket";
		case Type::RightBracket:
			return "RightBracket";
		case Type::LeftParenthesis:
			return "LeftParenthesis";
		case Type::RightParenthesis:
			return "RightParenthesis";
		case Type::LeftSquareBracket:
			return "LeftSquareBracket";
		case Type::RightSquareBracket:
			return "RightSquareBracket";
		case Type::Equals:
			return "Equals";
		case Type::Ampersand:
			return "Ampersand";
		case Type::SemiColon:
			return "SemiColon";
		case Type::Colon:
			return "Colon";
		case Type::LeftAngleBracket:
			return "LeftAngleBracket";
		case Type::RightAngleBracket:
			return "RightAngleBracket";
		case Type::Dot:
			return "Dot";
		case Type::Exclamation:
			return "Exclamation";
		default:
			throw std::exception("Unmapped type");
		}
	}

	std::ostream& operator<<(std::ostream& os, const Token& dt)
	{
		os << "[" << dt.row << ", " << dt.column << "]: " << to_string(dt.token_type);
		const auto index = dt.data.index();
		switch (index) {
		case 0: {
			os << ": " << std::get<char>(dt.data);
			return os;
		}
		case 1: {
			auto as_int = std::get<int>(dt.data);
			os << ": " << as_int;
			return os;
		}
		case 2: {
			os << ": " << std::get<std::string>(dt.data);
			return os;
		}
		default: {
			throw std::exception("Variant holds unknown type.");
		}
		}
	}

	char Lexer::peek(int offset) { return program[offset + index]; }

	char Lexer::consume() { return program[index++]; }

	std::vector<Token> Lexer::lex()
	{
		std::vector<Token> out;
		static auto emit_token = [&]<class T>(Type t, T value) {
			Token token;
			token.column = column++;
			token.row = row;
			token.token_type = t;
			token.data = value;
			out.push_back(token);
		};

		while (index < program.size()) {
			char c = consume();
			if (c == ' ' || c == '\t') {
				column++;
				continue;
			}
			if (c == '\n') {
				row++;
				column = 0;
				continue;
			}
			if (c == '.') {
				emit_token(Type::Dot, c);
				continue;
			}
			if (c == '&') {
				emit_token(Type::Ampersand, c);
				continue;
			}
			if (c == ';') {
				emit_token(Type::SemiColon, c);
				continue;
			}
			if (c == ':') {
				emit_token(Type::Colon, c);
				continue;
			}
			if (c == '!') {
				emit_token(Type::Exclamation, c);
				continue;
			}
			if (c == '=') {
				emit_token(Type::Equals, c);
				continue;
			}
			if (c == '{') {
				emit_token(Type::LeftBracket, c);
				continue;
			}
			if (c == '}') {
				emit_token(Type::RightBracket, c);
				continue;
			}
			if (c == '[') {
				emit_token(Type::LeftSquareBracket, c);
				continue;
			}
			if (c == ']') {
				emit_token(Type::RightSquareBracket, c);
				continue;
			}
			if (c == '<') {
				emit_token(Type::LeftAngleBracket, c);
				continue;
			}
			if (c == '>') {
				emit_token(Type::RightAngleBracket, c);
				continue;
			}
			if (c == '(') {
				emit_token(Type::LeftParenthesis, c);
				continue;
			}
			if (c == ')') {
				emit_token(Type::RightParenthesis, c);
				continue;
			}
			if (c == ',') {
				emit_token(Type::Comma, c);
				continue;
			}
			if (c == '-' || is_numeric(c)) {
				char chosen = c;
				if (c == '-')
					chosen = consume();

				int to_move_past = 0;
				std::stringstream str;
				str << chosen;
				char k;
				while ((k = peek(to_move_past++)), is_numeric(k)) {
					str << k;
				}

				auto as_string = str.str();
				int data = stoi(as_string);
				if (c == '-')
					data *= -1;

				emit_token(Type::Number, data);
				move_past(to_move_past);

				continue;
			}
			if (isalpha(c)) {
				int to_move_past = 0;
				std::stringstream str;
				str << c;
				while (true) {
					char k = peek(to_move_past++);
					if (!isalpha(k))
						break;
					str << k;
				}

				auto to_string = str.str();
				emit_token(is_keyword(to_string) ? Type::Keyword : Type::Identifier, to_string);
				move_past(to_move_past);

				continue;
			}
			throw std::exception(&c);
		}
		return out;
	}

	void Lexer::move_past(int positions)
	{
		if (positions > 0)
			for (auto k = 0; k < positions - 1; k++)
				consume();
	}

	bool Lexer::is_keyword(const std::string& string) { return keywords().contains(string); }

} // namespace NNDSL::Lex
