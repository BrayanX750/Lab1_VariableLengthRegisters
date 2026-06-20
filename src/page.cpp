#include "page.h"
#include <iostream>

Page::Page() {
    bytesUsados = 0;
    for (int i = 0; i < TAM_DATOS; i++) {
        datos[i] = 0;
    }
}

bool Page::tieneEspacio(int size) {
    return (bytesUsados + size) <= TAM_DATOS;
}

int Page::agregarDatos(std::string bytes) {
    int posicion = bytesUsados;

    for (int i = 0; i < bytes.size(); i++) {
        datos[bytesUsados] = bytes[i];
        bytesUsados++;
    }

    return posicion;
}

std::string Page::leerDatos(int pos, int size) {
    std::string resultado = "";

    for (int i = 0; i < size; i++) {
        resultado += datos[pos + i];
    }

    return resultado;
}

int Page::calcularCRC() {
    std::uint32_t crc = CRC::Calculate(datos, bytesUsados, CRC::CRC_32());
    return (int)crc;
}

bool Page::verificarCRC(int crcGuardado) {
    int crcCalculado = calcularCRC();

    if (crcCalculado != crcGuardado) {
        std::cout << "Advertencia: la pagina tiene datos corruptos." << std::endl;
        return false;
    }

    return true;
}
