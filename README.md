**Bytecode Virtual Machine**

A lightweight compiler and stack-based virtual machine implemented in C++. The project demonstrates the complete language pipeline — lexical analysis, parsing, AST construction, bytecode generation, and execution.

**Features**

• Lexer for tokenizing identifiers, numbers, operators, and keywords.

• Recursive descent parser for AST (Abstract Syntax Tree) construction.

• Bytecode compiler converting AST nodes into stack-based instructions.

• Virtual Machine supporting:

    • Arithmetic operators: +, -, *, /, %
    • Comparison operators: ==, !=, <, <=, >, >=
    • Logical operators: &&, ||, !
    • Variables and assignment
    • Print statements
    • Interactive REPL for testing programs and expressions.

**File Structure**

| File           | Description                                  |
| -------------- | -------------------------------------------- |
| `lexer.cpp`    | Implementation of the lexical analyzer       |
| `parser.cpp`   | Recursive descent parser + AST builder       |
| `compiler.cpp` | AST → Bytecode compiler                      |
| `vm.cpp`       | Stack-based virtual machine executor         |
| `main.cpp`     | Entry point, runs REPL and program execution |
| `README.md`    | Project documentation                        |


**Compilation**

    mkdir build
    cd build
    cmake ..
    make

**Run the REPL**

    ./bytecode_vm

**EXAMPLE**

**INPUT**

    x = 3 * (2 + 4);
    print x;
        
    print 7 >= 7 && 6 <= 6;
    print 1 || 0;
    print !(0);

**OUTPUT**

    18
    1
    1
    1

SAMPLE REPL SESSION IN TERMINAL:

    >>> x = 5 + 10;
    >>> print x;
    15
    >>> print x > 7 && x < 20;
    1
    >>> exit




