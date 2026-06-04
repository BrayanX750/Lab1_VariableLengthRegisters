#include "filemanager.h"
#include <fstream>
#include <iostream>

FileManager::FileManager(std::string filename) {
    this->filename = filename;
}

long FileManager::appendRecord(Record r) {
    std::ifstream verificar(filename);
    if (!verificar.is_open()) {
        std::ofstream crear(filename, std::ios::binary);
        crear.close();
    }
    verificar.close();

    std::ofstream file(filename, std::ios::binary | std::ios::app);

    std::ifstream calcOffset(filename, std::ios::binary | std::ios::ate);
    long offset = calcOffset.tellg();
    calcOffset.close();

    std::string data = serializeRecord(r);
    file.write(data.c_str(), data.size());
    file.close();

    return offset;
}

Record FileManager::readRecord(long offset, int size) {
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cout << "No se pudo abrir el archivo de datos." << std::endl;
        return Record();
    }

    file.seekg(offset);

    std::string data = "";
    for (int i = 0; i < size; i++) {
        char c;
        file.get(c);
        data += c;
    }

    file.close();
    return deserializeRecord(data);
}

void FileManager::rewriteFile(std::vector<Record> records) {
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);

    for (int i = 0; i < records.size(); i++) {
        std::string data = serializeRecord(records[i]);
        file.write(data.c_str(), data.size());
    }

    file.close();
}

int FileManager::getRecordSize(Record r) {
    std::string data = serializeRecord(r);
    return data.size();
}
