#include "lexer.h"
#include <cctype>

Lexer::Lexer(const std::string& src) : source(src), pos(0) {}

char Lexer::peek() {
    return pos < source.size() ? source[pos] : '\0';
}

char Lexer::get() {
    return pos < source.size() ? source[pos++] : '\0';
}

void Lexer::skipWhitespace() {
    while (std::isspace(static_cast<unsigned char>(peek()))) get();
}

Token Lexer::identifier() {
    std::string result;
    while (std::isalnum(static_cast<unsigned char>(peek()))) result += get();

    if (result == "print" || result == "if" || result == "while" || result == "else")
        return Token(TokenType::Keyword, result); // else now recognized

    return Token(TokenType::Identifier, result);
}


Token Lexer::number() {
    std::string result;
    while (std::isdigit(static_cast<unsigned char>(peek()))) result += get();
    return Token(TokenType::Number, result);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (true) {
        skipWhitespace();
        char c = peek();

        if (c == '\0') {
            tokens.emplace_back(TokenType::EndOfFile, "");
            break;
        } else if (std::isalpha(static_cast<unsigned char>(c))) {
            tokens.push_back(identifier());
        } else if (std::isdigit(static_cast<unsigned char>(c))) {
            tokens.push_back(number());
        } else if (c == '=') {
            get();
            if (peek() == '=') {
                get();
                tokens.emplace_back(TokenType::Operator, "==");
            } else {
                tokens.emplace_back(TokenType::Assign, "=");
            }
        } else if (c == '!') {
            get();
            if (peek() == '=') {
                get();
                tokens.emplace_back(TokenType::Operator, "!=");
            } else {
                tokens.emplace_back(TokenType::Operator, "!");
            }
        } else if (c == '<') {
            get();
            if (peek() == '=') {
                get();
                tokens.emplace_back(TokenType::Operator, "<=");
            } else {
                tokens.emplace_back(TokenType::Operator, "<");
            }
        } else if (c == '>') {
            get();
            if (peek() == '=') {
                get();
                tokens.emplace_back(TokenType::Operator, ">=");
            } else {
                tokens.emplace_back(TokenType::Operator, ">");
            }
        } else if (c == '&') {
            // bounds-safe lookahead for &&
            if (pos + 1 < source.size() && source[pos + 1] == '&') {
                get(); get();
                tokens.emplace_back(TokenType::Operator, "&&");
            } else {
                // treat single '&' as unknown for now
                get();
                tokens.emplace_back(TokenType::Unknown, "&");
            }
        } else if (c == '|') {
            // bounds-safe lookahead for ||
            if (pos + 1 < source.size() && source[pos + 1] == '|') {
                get(); get();
                tokens.emplace_back(TokenType::Operator, "||");
            } else {
                get();
                tokens.emplace_back(TokenType::Unknown, "|");
            }
        } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
            tokens.emplace_back(TokenType::Operator, std::string(1, get()));
        } else if (c == ';') {
            get();
            tokens.emplace_back(TokenType::Semicolon, ";");
        } else if (c == '(') {
            get();
            tokens.emplace_back(TokenType::LParen, "(");
        } else if (c == ')') {
            get();
            tokens.emplace_back(TokenType::RParen, ")");
        } else {
            tokens.emplace_back(TokenType::Unknown, std::string(1, get()));
        }
    }

    return tokens;
}
