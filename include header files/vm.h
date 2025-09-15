#pragma once
#include "bytecode.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>

class VM {
public:
    void run(const std::vector<Instruction>& bytecode);

private:
    std::vector<int> stack;
    std::unordered_map<std::string, int> variables;

    void push(int value) { stack.push_back(value); }
    int pop();
};
