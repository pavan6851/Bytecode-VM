#pragma once
#include <string>
#include <vector>

enum class TokenType {
    Identifier,
    Number,
    Keyword,
    Operator,
    Assign,
    Semicolon,
    LParen,     // (
    RParen,     // )
    EndOfFile,
    Unknown
};

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType t, std::string v) : type(t), value(std::move(v)) {}
};


class Lexer {
public:
    explicit Lexer(const std::string& src);
    std::vector<Token> tokenize();

private:
    std::string source;
    size_t pos;

    char peek();
    char get();
    void skipWhitespace();
    Token identifier();
    Token number();
};
