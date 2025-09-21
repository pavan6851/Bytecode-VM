#include "bytecode.h"
#include "parser.h"   // for ASTNode, NumberNode, IdentifierNode, etc.
#include <stdexcept>

void generateBytecode(const ASTNode* node, std::vector<Instruction>& instructions) {
    if (auto num = dynamic_cast<const NumberNode*>(node)) {
        instructions.push_back({OpCode::LOAD_CONST, std::to_string(num->value)});
    }
    else if (auto id = dynamic_cast<const IdentifierNode*>(node)) {
        instructions.push_back({OpCode::LOAD_VAR, id->name});
    }
    else if (auto bin = dynamic_cast<const BinaryOpNode*>(node)) {
        generateBytecode(bin->left.get(), instructions);
        generateBytecode(bin->right.get(), instructions);

        if (bin->op == "+") instructions.push_back({OpCode::ADD, ""});
        else if (bin->op == "-") instructions.push_back({OpCode::SUB, ""});
        else if (bin->op == "*") instructions.push_back({OpCode::MUL, ""});
        else if (bin->op == "/") instructions.push_back({OpCode::DIV, ""});
        else if (bin->op == "%") instructions.emplace_back({OpCode::MOD, ""});

        // 🔹 New comparison operators
        else if (bin->op == "==") instructions.push_back({OpCode::CMP_EQ, ""});
        else if (bin->op == "!=") instructions.push_back({OpCode::CMP_NEQ, ""});
        else if (bin->op == "<")  instructions.push_back({OpCode::CMP_LT, ""});
        else if (bin->op == "<=") instructions.push_back({OpCode::CMP_LTE, ""});
        else if (bin->op == ">")  instructions.push_back({OpCode::CMP_GT, ""});
        else if (bin->op == ">=") instructions.push_back({OpCode::CMP_GTE, ""});

        else throw std::runtime_error("Unknown binary operator: " + bin->op);
    }

    else if (auto assign = dynamic_cast<const AssignmentNode*>(node)) {
        generateBytecode(assign->expr.get(), instructions);
        instructions.push_back({OpCode::STORE_VAR, assign->varName});
    }
    else if (auto printNode = dynamic_cast<const PrintNode*>(node)) {
        generateBytecode(printNode->expr.get(), instructions);
        instructions.push_back({OpCode::PRINT, ""});
    }
}
