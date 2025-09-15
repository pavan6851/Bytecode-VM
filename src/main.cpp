#include <iostream>
#include <string>
#include "lexer.h"
#include "parser.h"
#include "bytecode.h"
#include "compiler.h"
#include "vm.h"

// Helper: pretty-print AST
static void printAST(const ASTNode* node, int indent = 0) {
    std::string space(indent, ' ');
    if (auto num = dynamic_cast<const NumberNode*>(node)) {
        std::cout << space << "Number(" << num->value << ")\n";
    } else if (auto id = dynamic_cast<const IdentifierNode*>(node)) {
        std::cout << space << "Identifier(" << id->name << ")\n";
    } else if (auto bin = dynamic_cast<const BinaryOpNode*>(node)) {
        std::cout << space << "BinaryOp(" << bin->op << ")\n";
        printAST(bin->left.get(), indent + 2);
        printAST(bin->right.get(), indent + 2);
    } else if (auto un = dynamic_cast<const UnaryOpNode*>(node)) {
        std::cout << space << "UnaryOp(" << un->op << ")\n";
        printAST(un->expr.get(), indent + 2);
    } else if (auto assign = dynamic_cast<const AssignmentNode*>(node)) {
        std::cout << space << "Assignment(" << assign->varName << ")\n";
        printAST(assign->expr.get(), indent + 2);
    } else if (auto printNode = dynamic_cast<const PrintNode*>(node)) {
        std::cout << space << "Print\n";
        printAST(printNode->expr.get(), indent + 2);
    }
}

int main() {
    VM vm;
    std::string line;
    std::cout << "Bytecode REPL (Parser + Bytecode Test). Type 'exit' to quit.\n";

    while (true) {
        std::cout << ">>> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "exit") break;

        // 1) Lex
        Lexer lexer(line);
        auto tokens = lexer.tokenize();

        // 2) Parse
        Parser parser(tokens);
        try {
            auto stmts = parser.parse();

            // Print AST (all statements for this line)
            for (auto& stmt : stmts) {
                std::cout << "[AST]\n";
                printAST(stmt.get());
            }

            // 3) Compile using Compiler (handles &&, ||, !, comparisons, etc.)
            Compiler compiler;
            auto bytecode = compiler.compile(stmts);

            // 4) Disassemble/print bytecode
            std::cout << "[Bytecode]\n";
            for (const auto& instr : bytecode) {
                std::cout << opcodeToString(instr.op);
                if (!instr.arg.empty()) std::cout << " " << instr.arg;
                std::cout << "\n";
            }

            // 5) Run on VM
            vm.run(bytecode);

        } catch (std::runtime_error& e) {
            std::cerr << "Parser error: " << e.what() << "\n";
        }
    }

    return 0;
}
