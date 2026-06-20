#pragma once
#include <string>
#include <vector>
#include "record.h"
#include "page.h"

class FileManager {
public:
    std::string filename;

    FileManager(std::string filename);

    long appendRecord(Record r);
    Record readRecord(long offset, int size);
    void rewriteFile(std::vector<Record> records);
    int getRecordSize(Record r);
    void marcarInactivo(long offset, int size);

private:
    Page leerPagina(int numPagina);
    void guardarPagina(int numPagina, Page pagina);
    int totalPaginas();
};
