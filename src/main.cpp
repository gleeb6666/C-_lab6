#include "config.h"
#include "demo.h"

#include <exception>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Використання: " << (argc > 0 ? argv[0] : "variant6")
                  << " <файл з вхідними даними>\n";
        return 1;
    }

    try {
        runDemo(readConfig(argv[1]), std::cout);
    } catch (const ConfigError& error) {
        std::cerr << "Помилка у вхідних даних: " << error.what() << '\n';
        return 1;
    } catch (const std::exception& error) {
        std::cerr << "Помилка: " << error.what() << '\n';
        return 2;
    }
    return 0;
}
