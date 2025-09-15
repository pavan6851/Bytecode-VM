#pragma once
#include <string>
#include <vector>

enum class OpCode {
    LOAD_CONST,
    LOAD_VAR,
    STORE_VAR,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    PRINT,
    HALT,

    CMP_EQ,
    CMP_NEQ,
    CMP_LT,
    CMP_LTE,
    CMP_GT,
    CMP_GTE,

    LOGICAL_AND,
    LOGICAL_OR,
    LOGICAL_NOT,

    JMP,
    JMP_IF_TRUE,
    JMP_IF_FALSE
};

struct Instruction {
    OpCode op;
    std::string arg; // value, variable name, or target index (for jumps)
};

inline std::string opcodeToString(OpCode op) {
    switch (op) {
        case OpCode::LOAD_CONST:  return "LOAD_CONST";
        case OpCode::LOAD_VAR:    return "LOAD_VAR";
        case OpCode::STORE_VAR:   return "STORE_VAR";
        case OpCode::ADD:         return "ADD";
        case OpCode::SUB:         return "SUB";
        case OpCode::MUL:         return "MUL";
        case OpCode::DIV:         return "DIV";
        case OpCode::MOD:         return "MOD";
        case OpCode::PRINT:       return "PRINT";
        case OpCode::HALT:        return "HALT";

        case OpCode::CMP_EQ:      return "CMP_EQ";
        case OpCode::CMP_NEQ:     return "CMP_NEQ";
        case OpCode::CMP_LT:      return "CMP_LT";
        case OpCode::CMP_LTE:     return "CMP_LTE";
        case OpCode::CMP_GT:      return "CMP_GT";
        case OpCode::CMP_GTE:     return "CMP_GTE";

        case OpCode::LOGICAL_AND: return "LOGICAL_AND";
        case OpCode::LOGICAL_OR:  return "LOGICAL_OR";
        case OpCode::LOGICAL_NOT: return "LOGICAL_NOT";

        case OpCode::JMP:         return "JMP";
        case OpCode::JMP_IF_TRUE: return "JMP_IF_TRUE";
        case OpCode::JMP_IF_FALSE:return "JMP_IF_FALSE";
    }
    return "UNKNOWN";
}
