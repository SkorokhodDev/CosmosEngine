#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "app.hpp"

// TODO: write a macros to create default copy constructors.

int main() {
    Cosmos::Application app;
    
    try{
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}