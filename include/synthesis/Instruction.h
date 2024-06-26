//
// Created by Steven He on 4/16/24.
//

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

enum class InsType {
    READ, WRITE, WRITENL,
    CONST,
    ADD, SUB, MUL, DIV, CMP,
    PHI,
    BRA, BNE, BEQ, BLE, BLT, BGE, BGT,
    JSR, RET, GETPAR, SETPAR,
    MT,
};

inline bool isCommutable(const InsType type) {
    return type == InsType::ADD || type == InsType::MUL;
}

using InstrSig = std::tuple<InsType, int, int>;

namespace std {
    template<>
    struct equal_to<InstrSig> {
        bool operator()(const InstrSig& lhs, const InstrSig& rhs) const {
            const auto& [type1, x1, y1] = lhs;
            const auto& [type2, x2, y2] = rhs;

            if (type1 != type2) return false;

            if (isCommutable(type1)) {
                return (x1 == x2 && y1 == y2) || (x1 == y2 && y1 == x2);
            }

            return x1 == x2 && y1 == y2;
        }
    };

    template<>
    struct hash<InstrSig> {
        size_t operator()(const InstrSig& sig) const {
            const auto& [type, first, second] = sig;
            size_t h1 = std::hash<InsType>()(type);
            size_t h2, h3;

            if (isCommutable(type)) {
                h2 = std::hash<int>()(std::min(first, second));
                h3 = std::hash<int>()(std::max(first, second));
            } else {
                h2 = std::hash<int>()(first);
                h3 = std::hash<int>()(second);
            }

            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}

struct Instruction {
    int id;
    InsType type;
    int x;
    int y;

    // reserve 0 for no instruction
    explicit Instruction(const int id, const InsType type, const int x = 0, const int y = 0)
        : id{id}, type{type}, x{x}, y{y} {}

    [[nodiscard]] std::string toString() const {
        if (type == InsType::CONST) {
            return std::to_string(id) + ": const #" + std::to_string(x);
        }
        if (type == InsType::SETPAR || type == InsType::GETPAR) {
            return std::to_string(id) + ": " + insTypeToString(type)
                + std::to_string(x)
                + " " + (y ? " (" + std::to_string(y) + ")" : "");
        }

        return std::to_string(id) + ": " + insTypeToString(type)
            + (x ? " (" + std::to_string(x) + ")" : "")
            + (y ? " (" + std::to_string(y) + ")" : "");
    }

private:
    static std::string insTypeToString(const InsType& type) {
        switch (type) {
            case InsType::READ: return "read";
            case InsType::WRITE: return "write";
            case InsType::WRITENL: return "writeNL";
            case InsType::CONST: return "const";
            case InsType::ADD: return "add";
            case InsType::SUB: return "sub";
            case InsType::MUL: return "mul";
            case InsType::DIV: return "div";
            case InsType::CMP: return "cmp";
            case InsType::PHI: return "phi";
            case InsType::BRA: return "bra";
            case InsType::BNE: return "bne";
            case InsType::BEQ: return "beq";
            case InsType::BLE: return "ble";
            case InsType::BLT: return "blt";
            case InsType::BGE: return "bge";
            case InsType::BGT: return "bgt";
            case InsType::JSR: return "jsr";
            case InsType::RET: return "ret";
            case InsType::GETPAR: return "getpar";
            case InsType::SETPAR: return "setpar";
            case InsType::MT: return "\\<empty\\>";
            default: return "UNKNOWN_INS";
        }
    }
};

#endif //INSTRUCTION_H
