#pragma once
#include "parser.h"
#include "bytecode.h"
#include <vector>
#include <memory>

// The Compiler turns AST into Bytecode instructions
class Compiler {
public:
    // Compile a whole program (list of AST nodes/statements)
    std::vector<Instruction> compile(const std::vector<std::unique_ptr<ASTNode>>& program);

private:
    // Dispatch based on node type
    void compileNode(const ASTNode* node, std::vector<Instruction>& out);

    // Helpers for each AST node type
    void compileNumber(const NumberNode* num, std::vector<Instruction>& out);
    void compileIdentifier(const IdentifierNode* id, std::vector<Instruction>& out);
    void compileBinary(const BinaryOpNode* bin, std::vector<Instruction>& out);
    void compileUnary(const UnaryOpNode* un, std::vector<Instruction>& out);
    void compileAssignment(const AssignmentNode* assign, std::vector<Instruction>& out);
    void compilePrint(const PrintNode* print, std::vector<Instruction>& out);

    // NEW: control-flow helpers (declarations only)
    void compileIf(const IfNode* iff, std::vector<Instruction>& out);
    void compileWhile(const WhileNode* wh, std::vector<Instruction>& out);
};
