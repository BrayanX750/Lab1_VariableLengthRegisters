#include <iostream>
#include <string>
#include "index.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Uso: alumnos <comando> [argumentos]" << std::endl;
        std::cout << "  agregar    <archivo.json>" << std::endl;
        std::cout << "  eliminar   <no_cuenta>" << std::endl;
        std::cout << "  buscar     <no_cuenta>" << std::endl;
        std::cout << "  actualizar <archivo.json>" << std::endl;
        std::cout << "  clean-up" << std::endl;
        return 1;
    }

    Index index;
    index.load("alumnos.idx");

    std::string cmd = argv[1];

    if (cmd == "agregar" && argc == 3) {

    } else if (cmd == "eliminar" && argc == 3) {

    } else if (cmd == "buscar" && argc == 3) {

    } else if (cmd == "actualizar" && argc == 3) {

    } else if (cmd == "clean-up") {

    } else {
        std::cout << "Comando no reconocido." << std::endl;
        return 1;
    }

    index.save("alumnos.idx");
    return 0;
}
