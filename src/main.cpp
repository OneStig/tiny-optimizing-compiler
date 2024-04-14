#include <iostream>
#include <fstream>

int main(const int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <tiny source file>" << std::endl;
        return 1;
    }

    std::ifstream sourceFile(argv[1]);

    if (!sourceFile.is_open()) {
        std::cerr << "Failed to open tiny source file" << std::endl;
        return 1;
    }

    return 0;
}
