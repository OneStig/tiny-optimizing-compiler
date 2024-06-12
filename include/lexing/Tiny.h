#ifndef TINY_H
#define TINY_H

#include <iostream>
#include <stdexcept>

class TinySyntaxError final : public std::runtime_error {
public:
    explicit TinySyntaxError(const std::string& err)
        : std::runtime_error(err) {};
};

inline void unusedWarning(const std::string& varName) {
    std::cout << "Warning, variable '" << varName << "' potentially used before initializing."
                                                     " It has been initialized to 0 by default." << std::endl;
}

#endif //TINY_H
