#pragma once
#include <string>
#include <vector>

struct Record {
    std::string noCuenta;
    std::string nombre;
    std::string telefono;
    int edad;
    std::string fechaIngreso;
    bool activo;

    Record();
};

std::string serializeRecord(Record r);
Record deserializeRecord(std::string data);
Record readFromJSON(std::string filename);
std::vector<Record> readManyFromJSON(std::string filename);
void printRecord(Record r);
