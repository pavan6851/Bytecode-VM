#include "vm.h"

int VM::pop() {
    if (stack.empty()) throw std::runtime_error("Stack underflow");
    int val = stack.back();
    stack.pop_back();
    return val;
}

void VM::run(const std::vector<Instruction>& bytecode) {
    for (size_t pc = 0; pc < bytecode.size(); /* ++pc below */) {
        const auto& instr = bytecode[pc];
        switch (instr.op) {
            case OpCode::LOAD_CONST:
                push(std::stoi(instr.arg));
                break;
            case OpCode::LOAD_VAR:
                if (variables.find(instr.arg) == variables.end())
                    throw std::runtime_error("Undefined variable: " + instr.arg);
                push(variables[instr.arg]);
                break;
            case OpCode::STORE_VAR: {
                int val = pop();
                variables[instr.arg] = val;
                break;
            }
            case OpCode::ADD: {
                int b = pop(), a = pop();
                push(a + b);
                break;
            }
            case OpCode::SUB: {
                int b = pop(), a = pop();
                push(a - b);
                break;
            }
            case OpCode::MUL: {
                int b = pop(), a = pop();
                push(a * b);
                break;
            }
            case OpCode::DIV: {
                int b = pop(), a = pop();
                if (b == 0) throw std::runtime_error("Division by zero");
                push(a / b);
                break;
            }
            case OpCode::MOD: {
                int b = pop(), a = pop();
                push(a % b);
                break;
            }
            case OpCode::PRINT: {
                int val = pop();
                std::cout << val << std::endl;
                break;
            }
            case OpCode::CMP_EQ: {
                int b = pop(), a = pop();
                push(a == b ? 1 : 0);
                break;
            }
            case OpCode::CMP_NEQ: {
                int b = pop(), a = pop();
                push(a != b ? 1 : 0);
                break;
            }
            case OpCode::CMP_LT: {
                int b = pop(), a = pop();
                push(a < b ? 1 : 0);
                break;
            }
            case OpCode::CMP_LTE: {
                int b = pop(), a = pop();
                push(a <= b ? 1 : 0);
                break;
            }
            case OpCode::CMP_GT: {
                int b = pop(), a = pop();
                push(a > b ? 1 : 0);
                break;
            }
            case OpCode::CMP_GTE: {
                int b = pop(), a = pop();
                push(a >= b ? 1 : 0);
                break;
            }
            case OpCode::LOGICAL_AND: {
                int b = pop(), a = pop();
                push((a && b) ? 1 : 0);
                break;
            }
            case OpCode::LOGICAL_OR: {
                int b = pop(), a = pop();
                push((a || b) ? 1 : 0);
                break;
            }
            case OpCode::LOGICAL_NOT: {
                int a = pop();
                push(!a ? 1 : 0);
                break;
            }

            // Only these three are new:
            case OpCode::JMP: {
                pc = static_cast<size_t>(std::stoul(instr.arg));
                continue;
            }
            case OpCode::JMP_IF_TRUE: {
                int c = pop();
                if (c) { pc = static_cast<size_t>(std::stoul(instr.arg)); continue; }
                break;
            }
            case OpCode::JMP_IF_FALSE: {
                int c = pop();
                if (!c) { pc = static_cast<size_t>(std::stoul(instr.arg)); continue; }
                break;
            }
        }
        ++pc;
    }
}
