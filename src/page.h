#pragma once
#include <string>
#include "CRC.h"

#define TAM_PAGINA 512
#define TAM_CRC    4
#define TAM_HEADER 4
#define TAM_DATOS  (TAM_PAGINA - TAM_CRC - TAM_HEADER)

class Page {
public:
    int  bytesUsados;
    char datos[TAM_DATOS];

    Page();

    bool tieneEspacio(int size);
    int  agregarDatos(std::string bytes);
    std::string leerDatos(int pos, int size);
    int  calcularCRC();
    bool verificarCRC(int crcGuardado);
};
