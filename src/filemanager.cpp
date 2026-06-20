#include "filemanager.h"
#include <fstream>
#include <iostream>

FileManager::FileManager(std::string filename) {
    this->filename = filename;
}

Page FileManager::leerPagina(int numPagina) {
    Page pagina;

    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) return pagina;

    long posicion = numPagina * TAM_PAGINA;
    file.seekg(posicion);

    char* ptrUsados = (char*)&pagina.bytesUsados;
    for (int i = 0; i < 4; i++) {
        file.get(ptrUsados[i]);
    }

    for (int i = 0; i < TAM_DATOS; i++) {
        file.get(pagina.datos[i]);
    }

    int crcGuardado = 0;
    char* ptrCRC = (char*)&crcGuardado;
    for (int i = 0; i < 4; i++) {
        file.get(ptrCRC[i]);
    }

    file.close();

    pagina.verificarCRC(crcGuardado);

    return pagina;
}

void FileManager::guardarPagina(int numPagina, Page pagina) {
    std::ifstream verificar(filename);
    if (!verificar.is_open()) {
        std::ofstream crear(filename, std::ios::binary);
        crear.close();
    }
    verificar.close();

    std::fstream file(filename, std::ios::in | std::ios::out | std::ios::binary);

    long posicion = numPagina * TAM_PAGINA;
    file.seekp(posicion);

    char* ptrUsados = (char*)&pagina.bytesUsados;
    for (int i = 0; i < 4; i++) {
        file.put(ptrUsados[i]);
    }

    for (int i = 0; i < TAM_DATOS; i++) {
        file.put(pagina.datos[i]);
    }

    int crc = pagina.calcularCRC();
    char* ptrCRC = (char*)&crc;
    for (int i = 0; i < 4; i++) {
        file.put(ptrCRC[i]);
    }

    file.close();
}

int FileManager::totalPaginas() {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return 0;

    long tamano = file.tellg();
    file.close();

    return tamano / TAM_PAGINA;
}

long FileManager::appendRecord(Record r) {
    std::string data = serializeRecord(r);
    int tamano = data.size();

    int numPaginas = totalPaginas();
    int paginaDestino = -1;
    Page pagina;

    for (int i = 0; i < numPaginas; i++) {
        Page p = leerPagina(i);
        if (p.tieneEspacio(tamano)) {
            paginaDestino = i;
            pagina = p;
            break;
        }
    }

    if (paginaDestino == -1) {
        paginaDestino = numPaginas;
        pagina = Page();
    }

    int posEnPagina = pagina.agregarDatos(data);
    guardarPagina(paginaDestino, pagina);

    long offset = (long)paginaDestino * TAM_DATOS + posEnPagina;
    return offset;
}

Record FileManager::readRecord(long offset, int size) {
    int numPagina   = offset / TAM_DATOS;
    int posEnPagina = offset % TAM_DATOS;

    Page pagina = leerPagina(numPagina);
    std::string data = pagina.leerDatos(posEnPagina, size);

    return deserializeRecord(data);
}

void FileManager::marcarInactivo(long offset, int size) {
    int numPagina   = offset / TAM_DATOS;
    int posEnPagina = offset % TAM_DATOS;

    Page pagina = leerPagina(numPagina);
    pagina.datos[posEnPagina + size - 1] = 0;
    guardarPagina(numPagina, pagina);
}

void FileManager::rewriteFile(std::vector<Record> records) {
    std::ofstream borrar(filename, std::ios::binary | std::ios::trunc);
    borrar.close();

    for (int i = 0; i < records.size(); i++) {
        appendRecord(records[i]);
    }
}

int FileManager::getRecordSize(Record r) {
    std::string data = serializeRecord(r);
    return data.size();
}
