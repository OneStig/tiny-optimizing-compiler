#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <unordered_set>

enum class TokenType {
	IDENT, NUM, KEYWORD,
	RELOP, PUNCTUATION, ASSIGN,
	END_OF_FILE
};

struct Token {
    TokenType type;
    std::string name;
    int value;

	explicit Token(const TokenType t, std::string n = "", const int v = 0) :
		type{t}, name{std::move(n)}, value{v} {}
};

const std::unordered_set<std::string> keywords = {
	"let",
	"call",
	"if", "then", "else", "fi",
	"while", "do", "od",
	"return",
	"var",
	"void", "function",
	"main"
};

#endif //TOKEN_H
