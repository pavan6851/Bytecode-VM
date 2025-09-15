#pragma once
#include "lexer.h"
#include <memory>
#include <vector>
#include <string>

// Base AST Node
struct ASTNode {
    virtual ~ASTNode() = default;
};

// Expressions
struct NumberNode : ASTNode {
    int value;
    explicit NumberNode(int v) : value(v) {}
};

struct IdentifierNode : ASTNode {
    std::string name;
    explicit IdentifierNode(std::string n) : name(std::move(n)) {}
};

struct BinaryOpNode : ASTNode {
    std::string op;  
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    BinaryOpNode(std::string o, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r)
        : op(std::move(o)), left(std::move(l)), right(std::move(r)) {}
};

// NEW: Unary operator node (for !, - etc.)
struct UnaryOpNode : ASTNode {
    std::string op;
    std::unique_ptr<ASTNode> expr;

    UnaryOpNode(std::string o, std::unique_ptr<ASTNode> e)
        : op(std::move(o)), expr(std::move(e)) {}
};

// Statements
struct AssignmentNode : ASTNode {
    std::string varName;
    std::unique_ptr<ASTNode> expr;
    AssignmentNode(std::string v, std::unique_ptr<ASTNode> e)
        : varName(std::move(v)), expr(std::move(e)) {}
};

struct PrintNode : ASTNode {
    std::unique_ptr<ASTNode> expr;
    explicit PrintNode(std::unique_ptr<ASTNode> e) : expr(std::move(e)) {}
};

// New statement nodes
struct IfNode : ASTNode {
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> thenBranch;
    std::unique_ptr<ASTNode> elseBranch; // may be null
    IfNode(std::unique_ptr<ASTNode> c,
           std::unique_ptr<ASTNode> t,
           std::unique_ptr<ASTNode> e = nullptr)
        : condition(std::move(c)), thenBranch(std::move(t)), elseBranch(std::move(e)) {}
};

struct WhileNode : ASTNode {
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> body;
    WhileNode(std::unique_ptr<ASTNode> c, std::unique_ptr<ASTNode> b)
        : condition(std::move(c)), body(std::move(b)) {}
};

// Optional: block of statements (used as body for if/while)
struct BlockNode : ASTNode {
    std::vector<std::unique_ptr<ASTNode>> statements;
    explicit BlockNode(std::vector<std::unique_ptr<ASTNode>> s)
        : statements(std::move(s)) {}
};

// In Parser class public section, add:
std::unique_ptr<ASTNode> ifStmt();
std::unique_ptr<ASTNode> whileStmt();
std::unique_ptr<ASTNode> block(); // parses { ... }


class Parser {
    std::vector<Token> tokens;
    size_t pos;

public:
    Parser(const std::vector<Token>& toks);

    const Token& peek();
    const Token& get();

    std::vector<std::unique_ptr<ASTNode>> parse();
    std::unique_ptr<ASTNode> statement();
    std::unique_ptr<ASTNode> assignment();
    std::unique_ptr<ASTNode> printStmt();
    std::unique_ptr<ASTNode> expression();
    std::unique_ptr<ASTNode> factor();

    // Logical + comparison precedence chain
    std::unique_ptr<ASTNode> logicalOr();
    std::unique_ptr<ASTNode> logicalAnd();
    std::unique_ptr<ASTNode> comparison();
    std::unique_ptr<ASTNode> additive();
    std::unique_ptr<ASTNode> term();
};
