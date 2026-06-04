#include <iostream>
#include <string>
#include "record.h"
#include "index.h"
#include "filemanager.h"

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

    FileManager fm("alumnos.dat");

    std::string cmd = argv[1];

    if (cmd == "agregar" && argc == 3) {
        Record r = readFromJSON(argv[2]);

        if (r.noCuenta == "") {
            std::cout << "Error leyendo el archivo JSON." << std::endl;
            return 1;
        }

        if (index.find(r.noCuenta) != nullptr) {
            std::cout << "Ya existe un alumno con ese numero de cuenta." << std::endl;
            return 1;
        }

        int size = fm.getRecordSize(r);
        long offset = fm.appendRecord(r);

        IndexEntry entry;
        entry.noCuenta = r.noCuenta;
        entry.offset = offset;
        entry.size = size;
        entry.activo = true;

        index.insert(entry);
        std::cout << "Alumno agregado correctamente." << std::endl;

    } else if (cmd == "eliminar" && argc == 3) {
        std::string noCuenta = argv[2];

        if (index.find(noCuenta) == nullptr) {
            std::cout << "No se encontro un alumno con ese numero de cuenta." << std::endl;
            return 1;
        }

        index.remove(noCuenta);
        std::cout << "Alumno eliminado correctamente." << std::endl;

    } else if (cmd == "buscar" && argc == 3) {
        std::string noCuenta = argv[2];
        IndexEntry* entry = index.find(noCuenta);

        if (entry == nullptr) {
            std::cout << "No se encontro un alumno con ese numero de cuenta." << std::endl;
            return 1;
        }

        Record r = fm.readRecord(entry->offset, entry->size);
        printRecord(r);

    } else if (cmd == "actualizar" && argc == 3) {
        Record r = readFromJSON(argv[2]);

        if (r.noCuenta == "") {
            std::cout << "Error leyendo el archivo JSON." << std::endl;
            return 1;
        }

        if (index.find(r.noCuenta) == nullptr) {
            std::cout << "No se encontro un alumno con ese numero de cuenta." << std::endl;
            return 1;
        }

        int size = fm.getRecordSize(r);
        long offset = fm.appendRecord(r);

        index.update(r.noCuenta, offset, size);
        std::cout << "Alumno actualizado correctamente." << std::endl;

    } else if (cmd == "clean-up") {
        std::vector<Record> activos;

        for (int i = 0; i < index.entries.size(); i++) {
            if (index.entries[i].activo) {
                Record r = fm.readRecord(index.entries[i].offset, index.entries[i].size);
                activos.push_back(r);
            }
        }

        fm.rewriteFile(activos);
        index.compact();

        long offset = 0;
        for (int i = 0; i < activos.size(); i++) {
            int size = fm.getRecordSize(activos[i]);
            index.update(activos[i].noCuenta, offset, size);
            offset += size;
        }

        std::cout << "Clean-up completado. Registros activos: " << activos.size() << std::endl;

    } else {
        std::cout << "Comando no reconocido." << std::endl;
        return 1;
    }

    index.save("alumnos.idx");
    return 0;
}
