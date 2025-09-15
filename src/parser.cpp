#include "parser.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& toks) : tokens(toks), pos(0) {}

const Token& Parser::peek() {
    if (pos < tokens.size()) return tokens[pos];
    static Token eof(TokenType::EndOfFile, "");
    return eof;
}

const Token& Parser::get() {
    if (pos < tokens.size()) return tokens[pos++];
    static Token eof(TokenType::EndOfFile, "");
    return eof;
}

std::vector<std::unique_ptr<ASTNode>> Parser::parse() {
    std::vector<std::unique_ptr<ASTNode>> stmts;
    while (peek().type != TokenType::EndOfFile) {
        stmts.push_back(statement());
    }
    return stmts;
}

std::unique_ptr<ASTNode> Parser::statement() {
    // NEW: if and while statements (single-statement bodies)
    if (peek().type == TokenType::Keyword && peek().value == "if") {
        get(); // consume 'if'
        if (get().type != TokenType::LParen) throw std::runtime_error("Expected '(' after if");
        auto cond = expression();
        if (get().type != TokenType::RParen) throw std::runtime_error("Expected ')' after if condition");

        auto thenStmt = statement();
        std::unique_ptr<ASTNode> elseStmt = nullptr;
        if (peek().type == TokenType::Keyword && peek().value == "else") {
            get(); // consume 'else'
            elseStmt = statement();
        }
        return std::make_unique<IfNode>(std::move(cond), std::move(thenStmt), std::move(elseStmt)); // [7]
    }
    if (peek().type == TokenType::Keyword && peek().value == "while") {
        get(); // consume 'while'
        if (get().type != TokenType::LParen) throw std::runtime_error("Expected '(' after while");
        auto cond = expression();
        if (get().type != TokenType::RParen) throw std::runtime_error("Expected ')' after while condition");

        auto body = statement();
        return std::make_unique<WhileNode>(std::move(cond), std::move(body)); // [15]
    }

    if (peek().type == TokenType::Identifier) {
        return assignment();
    }
    else if (peek().type == TokenType::Keyword && peek().value == "print") {
        return printStmt();
    }
    else {
        // NEW: allow expression statements like "2+2;"
        auto exprNode = expression();
        if (get().type != TokenType::Semicolon)
            throw std::runtime_error("Expected semicolon after expression");
        return exprNode;
    }
}

std::unique_ptr<ASTNode> Parser::assignment() {
    std::string name = get().value;
    if (peek().type != TokenType::Assign)
    throw std::runtime_error("Expected '=' after identifier");
    get(); // consume '='

    auto exprNode = expression();
    if (get().type != TokenType::Semicolon)
        throw std::runtime_error("Expected semicolon in assignment");
    return std::make_unique<AssignmentNode>(name, std::move(exprNode));
}

std::unique_ptr<ASTNode> Parser::printStmt() {
    get(); // consume 'print'
    auto exprNode = expression();
    if (get().type != TokenType::Semicolon)
    throw std::runtime_error("Expected semicolon after print");

    return std::make_unique<PrintNode>(std::move(exprNode));
}

std::unique_ptr<ASTNode> Parser::expression() {
    return logicalOr();  // top-level entry for logical expressions
}

// Logical OR (||)
std::unique_ptr<ASTNode> Parser::logicalOr() {
    auto node = logicalAnd();
    while (peek().type == TokenType::Operator && peek().value == "||") {
        std::string op = get().value;
        auto rhs = logicalAnd();
        node = std::make_unique<BinaryOpNode>(op, std::move(node), std::move(rhs));
    }
    return node;
}

// Logical AND (&&)
std::unique_ptr<ASTNode> Parser::logicalAnd() {
    auto node = comparison();
    while (peek().type == TokenType::Operator && peek().value == "&&") {
        std::string op = get().value;
        auto rhs = comparison();
        node = std::make_unique<BinaryOpNode>(op, std::move(node), std::move(rhs));
    }
    return node;
}

// Comparison operators (==, !=, <, <=, >, >=)
std::unique_ptr<ASTNode> Parser::comparison() {
    auto node = additive();
    while (peek().type == TokenType::Operator &&
           (peek().value == "==" || peek().value == "!=" ||
            peek().value == "<"  || peek().value == "<=" ||
            peek().value == ">"  || peek().value == ">=")) {
        std::string op = get().value;
        auto rhs = additive();
        node = std::make_unique<BinaryOpNode>(op, std::move(node), std::move(rhs));
    }
    return node;
}

// Additive operators (+, -)
std::unique_ptr<ASTNode> Parser::additive() {
    auto node = term();
    while (peek().type == TokenType::Operator &&
           (peek().value == "+" || peek().value == "-")) {
        std::string op = get().value;
        auto rhs = term();
        node = std::make_unique<BinaryOpNode>(op, std::move(node), std::move(rhs));
    }
    return node;
}

std::unique_ptr<ASTNode> Parser::term() {
    auto node = factor();
    while (peek().type == TokenType::Operator &&
           (peek().value == "*" || peek().value == "/" || peek().value == "%")) {
        std::string op = get().value;
        auto rhs = factor();
        node = std::make_unique<BinaryOpNode>(op, std::move(node), std::move(rhs));
    }
    return node;
}

std::unique_ptr<ASTNode> Parser::factor() {
    if (peek().type == TokenType::Operator && peek().value == "!") {
        get(); // consume '!'
        auto node = factor();
        return std::make_unique<UnaryOpNode>("!", std::move(node));
    }
    else if (peek().type == TokenType::Number) {
        int val = std::stoi(get().value);
        return std::make_unique<NumberNode>(val);
    }
    else if (peek().type == TokenType::Identifier) {
        return std::make_unique<IdentifierNode>(get().value);
    }
    else if (peek().type == TokenType::LParen) {
        get(); // consume '('
        auto exprNode = expression();
        if (get().type != TokenType::RParen)
            throw std::runtime_error("Expected ')'");
        return exprNode;
    }
    throw std::runtime_error("Unexpected token in factor: " + peek().value);
}
