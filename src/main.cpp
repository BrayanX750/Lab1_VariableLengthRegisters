#include <iostream>
#include <string>
#include "record.h"
#include "index.h"
#include "filemanager.h"

bool esSoloNumeros(std::string texto) {
    for (int i = 0; i < texto.size(); i++) {
        if (texto[i] < '0' || texto[i] > '9') return false;
    }
    return true;
}

bool validarRecord(Record r) {
    if (r.noCuenta == "") {
        std::cout << "Error: el numero de cuenta no puede estar vacio." << std::endl;
        return false;
    }

    if (r.noCuenta.size() > 10) {
        std::cout << "Error: el numero de cuenta no puede tener mas de 10 caracteres." << std::endl;
        return false;
    }

    if (r.nombre == "") {
        std::cout << "Error: el nombre no puede estar vacio." << std::endl;
        return false;
    }

    if (r.telefono == "") {
        std::cout << "Error: el telefono no puede estar vacio." << std::endl;
        return false;
    }

    if (r.telefono.size() > 12) {
        std::cout << "Error: el telefono no puede tener mas de 12 caracteres." << std::endl;
        return false;
    }

    if (r.edad <= 0 || r.edad > 120) {
        std::cout << "Error: la edad debe estar entre 1 y 120." << std::endl;
        return false;
    }

    if (r.fechaIngreso.size() != 8) {
        std::cout << "Error: la fecha de ingreso debe tener exactamente 8 digitos (AAAAMMDD)." << std::endl;
        return false;
    }

    if (!esSoloNumeros(r.fechaIngreso)) {
        std::cout << "Error: la fecha de ingreso solo debe contener numeros." << std::endl;
        return false;
    }

    return true;
}

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

    Index indice;
    indice.load("alumnos.idx");

    FileManager fm("alumnos.dat");

    std::string cmd = argv[1];

    if (cmd == "agregar" && argc == 3) {
        Record alumno = readFromJSON(argv[2]);

        if (alumno.noCuenta == "") {
            std::cout << "Error: no se pudo leer el archivo JSON." << std::endl;
            return 1;
        }

        if (!validarRecord(alumno)) return 1;

        if (indice.find(alumno.noCuenta) != nullptr) {
            std::cout << "Error: ya existe un alumno con el numero de cuenta " << alumno.noCuenta << std::endl;
            return 1;
        }

        long offset = fm.appendRecord(alumno);
        int  tamano = fm.getRecordSize(alumno);

        IndexEntry entrada;
        entrada.noCuenta = alumno.noCuenta;
        entrada.offset   = offset;
        entrada.size     = tamano;
        entrada.activo   = true;

        indice.insert(entrada);
        std::cout << "Alumno agregado correctamente." << std::endl;

    } else if (cmd == "eliminar" && argc == 3) {
        std::string cuenta = argv[2];

        if (cuenta.size() > 10) {
            std::cout << "Error: numero de cuenta invalido." << std::endl;
            return 1;
        }

        if (indice.find(cuenta) == nullptr) {
            std::cout << "Error: no se encontro ningun alumno con el numero de cuenta " << cuenta << std::endl;
            return 1;
        }

        indice.remove(cuenta);
        std::cout << "Alumno eliminado correctamente." << std::endl;

    } else if (cmd == "buscar" && argc == 3) {
        std::string cuenta = argv[2];

        if (cuenta.size() > 10) {
            std::cout << "Error: numero de cuenta invalido." << std::endl;
            return 1;
        }

        IndexEntry* entrada = indice.find(cuenta);

        if (entrada == nullptr) {
            std::cout << "Error: no se encontro ningun alumno con el numero de cuenta " << cuenta << std::endl;
            return 1;
        }

        Record alumno = fm.readRecord(entrada->offset, entrada->size);
        printRecord(alumno);

    } else if (cmd == "actualizar" && argc == 3) {
        Record alumno = readFromJSON(argv[2]);

        if (alumno.noCuenta == "") {
            std::cout << "Error: no se pudo leer el archivo JSON." << std::endl;
            return 1;
        }

        if (!validarRecord(alumno)) return 1;

        if (indice.find(alumno.noCuenta) == nullptr) {
            std::cout << "Error: no se encontro ningun alumno con el numero de cuenta " << alumno.noCuenta << std::endl;
            return 1;
        }

        long nuevoOffset = fm.appendRecord(alumno);
        int  nuevoTamano = fm.getRecordSize(alumno);

        indice.update(alumno.noCuenta, nuevoOffset, nuevoTamano);
        std::cout << "Alumno actualizado correctamente." << std::endl;

    } else if (cmd == "clean-up") {
        std::vector<Record> activos;

        for (int i = 0; i < indice.entries.size(); i++) {
            if (indice.entries[i].activo == true) {
                Record alumno = fm.readRecord(indice.entries[i].offset, indice.entries[i].size);
                activos.push_back(alumno);
            }
        }

        if (activos.size() == 0) {
            std::cout << "No hay registros activos, el archivo queda vacio." << std::endl;
            fm.rewriteFile(activos);
            indice.compact();
            indice.save("alumnos.idx");
            return 0;
        }

        fm.rewriteFile(activos);
        indice.compact();

        long offsetActual = 0;
        for (int i = 0; i < activos.size(); i++) {
            int tamano = fm.getRecordSize(activos[i]);
            indice.update(activos[i].noCuenta, offsetActual, tamano);
            offsetActual += tamano;
        }

        std::cout << "Clean-up completado. Registros activos: " << activos.size() << std::endl;

    } else {
        std::cout << "Comando no reconocido: " << cmd << std::endl;
        std::cout << "Comandos disponibles: agregar, eliminar, buscar, actualizar, clean-up" << std::endl;
        return 1;
    }

    indice.save("alumnos.idx");
    return 0;
}
