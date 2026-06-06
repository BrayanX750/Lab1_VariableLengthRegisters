#include "record.h"
#include <iostream>
#include <fstream>

Record::Record() {
    noCuenta = "";
    nombre = "";
    telefono = "";
    edad = 0;
    fechaIngreso = "";
    activo = true;
}

std::string serializeRecord(Record r) {
    std::string resultado = "";

    std::string cuenta = r.noCuenta;
    while (cuenta.size() < 10) cuenta += ' ';
    resultado += cuenta;

    int largoNombre = r.nombre.size();
    char* bytesLargo = (char*)&largoNombre;
    for (int i = 0; i < 4; i++) resultado += bytesLargo[i];
    resultado += r.nombre;

    std::string tel = r.telefono;
    while (tel.size() < 12) tel += ' ';
    resultado += tel;

    char* bytesEdad = (char*)&r.edad;
    for (int i = 0; i < 4; i++) resultado += bytesEdad[i];

    std::string fecha = r.fechaIngreso;
    while (fecha.size() < 8) fecha += ' ';
    resultado += fecha;

    if (r.activo) {
        resultado += (char)1;
    } else {
        resultado += (char)0;
    }

    return resultado;
}

Record deserializeRecord(std::string data) {
    Record r;
    int pos = 0;

    r.noCuenta = data.substr(pos, 10);
    pos += 10;

    int largoNombre = 0;
    char* bytesLargo = (char*)&largoNombre;
    for (int i = 0; i < 4; i++) bytesLargo[i] = data[pos + i];
    pos += 4;

    r.nombre = data.substr(pos, largoNombre);
    pos += largoNombre;

    r.telefono = data.substr(pos, 12);
    pos += 12;

    int edad = 0;
    char* bytesEdad = (char*)&edad;
    for (int i = 0; i < 4; i++) bytesEdad[i] = data[pos + i];
    r.edad = edad;
    pos += 4;

    r.fechaIngreso = data.substr(pos, 8);
    pos += 8;

    r.activo = (data[pos] == 1);

    return r;
}

std::string getValorJSON(std::string json, std::string campo) {
    std::string buscar = "\"" + campo + "\"";
    int pos = json.find(buscar);
    if (pos == -1) return "";

    pos = json.find(":", pos) + 1;

    while (json[pos] == ' ') pos++;

    if (json[pos] == '"') {
        pos++;
        int fin = json.find('"', pos);
        return json.substr(pos, fin - pos);
    }

    int fin = pos;
    while (fin < json.size() && json[fin] != ',' && json[fin] != '}') fin++;

    std::string valor = json.substr(pos, fin - pos);

    while (valor.size() > 0 && valor[valor.size() - 1] == ' ') {
        valor = valor.substr(0, valor.size() - 1);
    }

    return valor;
}

Record readFromJSON(std::string filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "No se pudo abrir el archivo: " << filename << std::endl;
        return Record();
    }

    std::string json = "";
    std::string linea;
    while (getline(file, linea)) {
        json += linea;
    }
    file.close();

    Record r;
    r.noCuenta     = getValorJSON(json, "no_cuenta");
    r.nombre       = getValorJSON(json, "nombre");
    r.telefono     = getValorJSON(json, "telefono");
    r.fechaIngreso = getValorJSON(json, "fecha_ingreso");
    r.activo       = true;

    std::string edadTexto = getValorJSON(json, "edad");
    if (edadTexto == "") {
        std::cout << "Error: el campo edad no se encontro en el JSON." << std::endl;
        return Record();
    }

    try {
        r.edad = stoi(edadTexto);
    } catch (...) {
        std::cout << "Error: el campo edad tiene un valor invalido: " << edadTexto << std::endl;
        return Record();
    }

    return r;
}

void printRecord(Record r) {
    std::cout << "No. Cuenta    : " << r.noCuenta << std::endl;
    std::cout << "Nombre        : " << r.nombre << std::endl;
    std::cout << "Telefono      : " << r.telefono << std::endl;
    std::cout << "Edad          : " << r.edad << std::endl;
    std::cout << "Fecha Ingreso : " << r.fechaIngreso << std::endl;
    std::cout << "Estado        : " << (r.activo ? "Activo" : "Eliminado") << std::endl;
}
