#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <unordered_set>

enum class TokenType {
	IDENT, NUM, KEYWORD,
	GENOP, RELOP, PUNCTUATION, ASSIGN,
	END_OF_FILE
};

struct Token {
    TokenType type;
    std::string name;
    int value;

	explicit Token(const TokenType t, std::string n = "", const int v = 0) :
		type{t}, name{std::move(n)}, value{v} {}

    [[nodiscard]] std::string toString() const {
        std::string typeStr = tokenTypeToString(type);
        std::string result = "Token(" + typeStr;

        result += ", name: " + name;
        result += ", value: " + std::to_string(value);
        result += ")";

        return result;
    }

private:
    static std::string tokenTypeToString(TokenType type) {
        switch (type) {
            case TokenType::IDENT: return "IDENT";
            case TokenType::NUM: return "NUM";
            case TokenType::KEYWORD: return "KEYWORD";
            case TokenType::GENOP: return "GENOP";
            case TokenType::RELOP: return "RELOP";
            case TokenType::PUNCTUATION: return "PUNCTUATION";
            case TokenType::ASSIGN: return "ASSIGN";
            case TokenType::END_OF_FILE: return "END_OF_FILE";
            default: return "UNKNOWN_TYPE";
        }
    }
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

const std::unordered_set<char> punctuation = {
	'(', ')', '{', '}',
	',', ';', '.'
};

const std::unordered_set<char> genericOperators = {
	'*', '/', '+', '-'
};

const std::unordered_set<std::string> relationalOperators = {
	"==", "!=", "<", "<=", ">", ">="
};

#endif //TOKEN_H
