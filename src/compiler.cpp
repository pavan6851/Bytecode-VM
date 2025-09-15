#include "compiler.h"
#include <stdexcept>

// Compile a whole program (list of AST nodes/statements)
std::vector<Instruction> Compiler::compile(const std::vector<std::unique_ptr<ASTNode>>& program) {
    std::vector<Instruction> out;
    for (const auto& stmt : program) {
        compileNode(stmt.get(), out);
    }
    return out;
}

// Dispatcher: decide which compile* helper to call
void Compiler::compileNode(const ASTNode* node, std::vector<Instruction>& out) {
    if (auto num = dynamic_cast<const NumberNode*>(node)) {
        compileNumber(num, out);
    } 
    else if (auto id = dynamic_cast<const IdentifierNode*>(node)) {
        compileIdentifier(id, out);
    } 
    else if (auto bin = dynamic_cast<const BinaryOpNode*>(node)) {
        compileBinary(bin, out);
    } 
    else if (auto un = dynamic_cast<const UnaryOpNode*>(node)) {
        compileUnary(un, out);
    } 
    else if (auto assign = dynamic_cast<const AssignmentNode*>(node)) {
        compileAssignment(assign, out);
    } 
    else if (auto printNode = dynamic_cast<const PrintNode*>(node)) {
        compilePrint(printNode, out);
    }
        else if (auto iff = dynamic_cast<const IfNode*>(node)) {
        compileIf(iff, out);
    }
    else if (auto wh = dynamic_cast<const WhileNode*>(node)) {
        compileWhile(wh, out);
    } 
    else {
        throw std::runtime_error("Unknown AST node in compiler");
    }
}

// ---- Helpers ----
void Compiler::compileNumber(const NumberNode* num, std::vector<Instruction>& out) {
    out.push_back({OpCode::LOAD_CONST, std::to_string(num->value)});
}

void Compiler::compileIdentifier(const IdentifierNode* id, std::vector<Instruction>& out) {
    out.push_back({OpCode::LOAD_VAR, id->name});
}

void Compiler::compileAssignment(const AssignmentNode* assign, std::vector<Instruction>& out) {
    compileNode(assign->expr.get(), out);
    out.push_back({OpCode::STORE_VAR, assign->varName});
}

void Compiler::compilePrint(const PrintNode* print, std::vector<Instruction>& out) {
    compileNode(print->expr.get(), out);
    out.push_back({OpCode::PRINT, ""});
}

void Compiler::compileBinary(const BinaryOpNode* bin, std::vector<Instruction>& out) {
    compileNode(bin->left.get(), out);
    compileNode(bin->right.get(), out);

    if (bin->op == "+") out.push_back({OpCode::ADD, ""});
    else if (bin->op == "-") out.push_back({OpCode::SUB, ""});
    else if (bin->op == "*") out.push_back({OpCode::MUL, ""});
    else if (bin->op == "/") out.push_back({OpCode::DIV, ""});
    else if (bin->op == "%") out.push_back({OpCode::MOD, ""});
    else if (bin->op == "==") out.push_back({OpCode::CMP_EQ, ""});
    else if (bin->op == "!=") out.push_back({OpCode::CMP_NEQ, ""});
    else if (bin->op == "<") out.push_back({OpCode::CMP_LT, ""});
    else if (bin->op == "<=") out.push_back({OpCode::CMP_LTE, ""});
    else if (bin->op == ">") out.push_back({OpCode::CMP_GT, ""});
    else if (bin->op == ">=") out.push_back({OpCode::CMP_GTE, ""});
    else if (bin->op == "&&") out.push_back({OpCode::LOGICAL_AND, ""});
    else if (bin->op == "||") out.push_back({OpCode::LOGICAL_OR, ""});
    else {
        throw std::runtime_error("Unknown binary operator: " + bin->op);
    }
}

void Compiler::compileUnary(const UnaryOpNode* un, std::vector<Instruction>& out) {
    // FIX: UnaryOpNode uses 'expr' as its child expression.
    compileNode(un->expr.get(), out);

    if (un->op == "!") {
        out.push_back({OpCode::LOGICAL_NOT, ""});
    }
    else if (un->op == "-") {
        // emulate NEG: 0 - expr
        out.push_back({OpCode::LOAD_CONST, "0"});
        out.push_back({OpCode::SUB, ""});
    }
    else {
        throw std::runtime_error("Unknown unary operator: " + un->op);
    }
}

// --- Control-flow helpers ---

static size_t emit(std::vector<Instruction>& out, OpCode op, std::string arg = "") {
    out.push_back({op, std::move(arg)});
    return out.size() - 1;
}

static void patch(std::vector<Instruction>& out, size_t at, size_t target) {
    out[at].arg = std::to_string(target);
}

void Compiler::compileIf(const IfNode* iff, std::vector<Instruction>& out) {
    // condition
    compileNode(iff->condition.get(), out);
    // if false, jump to else/end (patch later)
    size_t jfalse = emit(out, OpCode::JMP_IF_FALSE, "0");

    // then-branch
    compileNode(iff->thenBranch.get(), out);

    if (iff->elseBranch) {
        // jump over else after then executes
        size_t jend = emit(out, OpCode::JMP, "0");
        // false -> start of else
        patch(out, jfalse, out.size());
        // else-branch
        compileNode(iff->elseBranch.get(), out);
        // end -> after else
        patch(out, jend, out.size());
    } else {
        // no else: false -> after then
        patch(out, jfalse, out.size());
    }
}

void Compiler::compileWhile(const WhileNode* wh, std::vector<Instruction>& out) {
    size_t loopStart = out.size();

    // condition
    compileNode(wh->condition.get(), out);
    // exit loop if condition false
    size_t jfalse = emit(out, OpCode::JMP_IF_FALSE, "0");

    // body
    compileNode(wh->body.get(), out);

    // back edge to loop start
    emit(out, OpCode::JMP, std::to_string(loopStart));

    // patch exit to right after body
    patch(out, jfalse, out.size());
}

