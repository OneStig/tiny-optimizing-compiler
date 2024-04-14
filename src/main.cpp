#include <iostream>
#include "FileReader.h"

int main(const int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <tiny source file>" << std::endl;
        return 1;
    }

    FileReader fr(argv[1]);

    return 0;
}
