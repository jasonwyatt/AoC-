#include <fstream>
#include <ostream>
#include <sstream>
#include <vector>

#include "absl/strings/str_split.h"
#include "tools/log.h"
#include "tools/main.h"

struct Machine {
    long a;
    long b;
    long c;
    int pc;
    std::vector<int> program;

    bool execute(std::ostringstream& output) {
        if (pc >= program.size()) return false;
        switch (program[pc]) {
            case 0: return adv();
            case 1: return bxl();
            case 2: return bst();
            case 3: return jnz();
            case 4: return bxc();
            case 5: return out(output);
            case 6: return bdv();
            case 7: return cdv();
        }
        return false;
    }

    /**
     * The adv instruction (opcode 0) performs division. The numerator is the value in the A
     * register. The denominator is found by raising 2 to the power of the instruction's combo
     * operand. (So, an operand of 2 would divide A by 4 (2^2); an operand of 5 would divide A by
     * 2^B.) The result of the division operation is truncated to an integer and then written to
     * the A register.
     */
    bool adv() {
        if (pc > program.size() - 2) return false;
        long comboOpValue;
        if (!getComboOperand(comboOpValue)) return false;
        long result = a / std::pow<long>(2, comboOpValue);
        a = result;
        pc += 2;
        return true;
    }

    /**
     * The bxl instruction (opcode 1) calculates the bitwise XOR of register B and the instruction's
     * literal operand, then stores the result in register B.
     */
    bool bxl() {
        if (pc > program.size() - 2) return false;
        int operand = program[pc + 1];
        b = b ^ operand;
        pc += 2;
        return true;
    }

    /**
     * The bst instruction (opcode 2) calculates the value of its combo operand modulo 8 (thereby
     * keeping only its lowest 3 bits), then writes that value to the B register.
     */
    bool bst() {
        if (pc > program.size() - 2) return false;
        long operand;
        if (!getComboOperand(operand)) return false;
        b = (operand % 8 + 8) % 8;
        pc += 2;
        return true;
    }

    /**
     * The jnz instruction (opcode 3) does nothing if the A register is 0. However, if the A
     * register is not zero, it jumps by setting the instruction pointer to the value of its literal
     * operand; if this instruction jumps, the instruction pointer is not increased by 2 after this
     * instruction.
     */
    bool jnz() {
        if (pc > program.size() - 2) return false;
        int operand = program[pc + 1];
        if (a == 0) {
            pc += 2;
            return true;
        }
        pc = operand;
        return true;
    }

    /**
     * The bxc instruction (opcode 4) calculates the bitwise XOR of register B and register C, then
     * stores the result in register B. (For legacy reasons, this instruction reads an operand but
     * ignores it.)
     */
    bool bxc() {
        if (pc > program.size() - 2) return false;
        b = b ^ c;
        pc += 2;
        return true;
    }

    /**
     * The out instruction (opcode 5) calculates the value of its combo operand modulo 8, then
     * outputs that value. (If a program outputs multiple values, they are separated by commas.)
     */
    bool out(std::ostringstream& output) {
        if (pc > program.size() - 2) return false;
        long operandValue;
        if (!getComboOperand(operandValue)) {
            ERROR("Could not get combo operand %d", program[pc + 1]);
            return false;
        }
        if (output.str().size() > 0) {
            output << ",";
        }
        output << (operandValue % 8);
        pc += 2;
        return true;
    }

    /**
     * The bdv instruction (opcode 6) works exactly like the adv instruction except that the result
     * is stored in the B register. (The numerator is still read from the A register.)
     */
    bool bdv() {
        if (pc > program.size() - 2) return false;
        long operandValue;
        if (!getComboOperand(operandValue)) return false;
        long result = a / std::pow<long>(2, operandValue);
        b = result;
        pc += 2;
        return true;
    }

    /**
     * The cdv instruction (opcode 7) works exactly like the adv instruction except that the
     * result is stored in the C register. (The numerator is still read from the A register.)
     */
    bool cdv() {
        if (pc > program.size() - 2) return false;
        long operandValue;
        if (!getComboOperand(operandValue)) return false;
        unsigned long result = a >> operandValue;
        c = result;
        pc += 2;
        return true;
    }

    /**
     * Combo operands 0 through 3 represent literal values 0 through 3.
     * Combo operand 4 represents the value of register A.
     * Combo operand 5 represents the value of register B.
     * Combo operand 6 represents the value of register C.
     * Combo operand 7 is reserved and will not appear in valid programs.
     */
    bool getComboOperand(long& out) {
        switch (program[pc + 1]) {
            case 0:
            case 1:
            case 2:
            case 3:
                out = program[pc + 1];
                break;
            case 4:
                out = a;
                break;
            case 5:
                out = b;
                break;
            case 6:
                out = c;
                break;
            case 7:
                return false;
        }
        return true;
    }

    void log() {
        std::ostringstream prog;
        bool first = true;
        for (int op : program) {
            if (first) {
                first = false;
            } else {
                prog << ",";
            }
            prog << op;
        }
        DEBUG("Machine { a: %ld, b: %ld, c: %ld, pc: %d, program: [%s] }",
              a, b, c, pc, prog.str().c_str());
    }
};

Machine parse(std::ifstream& in) {
    Machine result;
    result.pc = 0;

    std::string line;
    std::vector<std::string> split;

    std::getline(in, line);
    split = absl::StrSplit(line, " ");
    result.a = std::stol(split[2]);

    std::getline(in, line);
    split = absl::StrSplit(line, " ");
    result.b = std::stol(split[2]);

    std::getline(in, line);
    split = absl::StrSplit(line, " ");
    result.c = std::stol(split[2]);

    std::getline(in, line);
    std::getline(in, line);
    split = absl::StrSplit(line, " ");
    std::vector<std::string> commandStrs = absl::StrSplit(split[1], ",");
    for (auto commandStr : commandStrs) {
        result.program.push_back(std::stoi(commandStr));
    }

    return result;
}

void part1(std::ifstream& in) {
    Machine machine = parse(in);
    machine.log();

    std::ostringstream output;
    while (machine.execute(output)) {
        //..
    }
    INFO("Part 1: %s", output.str().c_str());
}

bool find(Machine& machine, unsigned long aValue, int startIndex, unsigned long& result) {
    if (startIndex == -1) {
        result = aValue;
        return true;
    }
    for (int i = 0; i < 8; i++) {
        machine.a = (aValue << 3) | i;
        machine.b = 0;
        machine.c = 0;
        machine.pc = 0;
        std::ostringstream out;
        while (machine.execute(out)) {
            // ...
        }
        std::vector<std::string> split = absl::StrSplit(out.str(), ",");
        if (split.size() > 0) {
            bool okay = true;
            for (int j = 0; j < machine.program.size() - startIndex; j++) {
                if (std::stoi(split[j]) != machine.program[startIndex + j]) {
                    okay = false;
                    break;
                }
            }
            if (okay) {
                long nextAValue = (aValue << 3) | i;
                if (find(machine, nextAValue, startIndex - 1, result)) {
                    return true;
                }
            }
        }
    }
    return false;
}

void part2(std::ifstream& in) {
    Machine machine = parse(in);
    int startIndex = machine.program.size() - 1;
    unsigned long answer;

    if (find(machine, 0, startIndex, answer)) {
        INFO("Part 2: %ld", answer);
    } else {
        INFO("Part 2: not found");
    }
}

MAIN(part1, part2);