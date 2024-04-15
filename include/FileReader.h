#ifndef FILEREADER_H
#define FILEREADER_H

#include <fstream>
#include <iostream>
#include <string>

class FileReader {
private:
    std::ifstream input;

    static void error(const std::string& errMsg) {
        std::cerr << errMsg << std::endl;
    }

public:
    explicit FileReader(const char* file) : input(file) {
        if (!input.is_open()) {
            error("Failed to open tiny source file");
        }
    }

    ~FileReader() {
        if (input.is_open()) {
            input.close();
        }
    }

    char nextChar() {
        if (char c; input.get(c)) {
            return c;
        }

        if (input.eof()) {
            return EOF;
        }

        error("Error advancing next character");
        return EOF;
    }
};

#endif //FILEREADER_H
