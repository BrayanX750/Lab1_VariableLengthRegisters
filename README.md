# Lab 1 - Registros de Tamaño Variable e Índice Simple

Laboratorio 1 de Estructura de Datos II - UNITEC Q2 2026

## Descripción

Esta aplicación gestiona registros de alumnos usando un archivo binario con campos de longitud variable y un índice primario simple que se carga en memoria. Todo se maneja desde la línea de comandos.

## Compilar

```bash
g++ -std=c++11 src/main.cpp src/record.cpp src/index.cpp src/filemanager.cpp -o alumnos
```

## Comandos

### Agregar un alumno
```bash
./alumnos agregar <archivo.json>
```

### Buscar un alumno
```bash
./alumnos buscar <no_cuenta>
```

### Actualizar un alumno
```bash
./alumnos actualizar <archivo.json>
```

### Eliminar un alumno
```bash
./alumnos eliminar <no_cuenta>
```

### Limpiar el archivo de datos
```bash
./alumnos clean-up
```

## Ejemplos de uso

```bash
./alumnos agregar data/alumno1.json
./alumnos agregar data/alumno2.json
./alumnos agregar data/alumno3.json

./alumnos buscar 2020-10001

./alumnos actualizar data/alumno1.json

./alumnos eliminar 2022-30099

./alumnos clean-up
```

## Archivos JSON de prueba

**data/alumno1.json**
```json
{
    "no_cuenta": "2020-10001",
    "nombre": "Carlos Andres Mejia",
    "telefono": "9999-1234",
    "edad": 22,
    "fecha_ingreso": "20200115"
}
```

**data/alumno2.json**
```json
{
    "no_cuenta": "2021-20045",
    "nombre": "Maria Jose Hernandez Lopez",
    "telefono": "8888-5678",
    "edad": 21,
    "fecha_ingreso": "20210301"
}
```

**data/alumno3.json**
```json
{
    "no_cuenta": "2022-30099",
    "nombre": "Luis",
    "telefono": "7777-9012",
    "edad": 19,
    "fecha_ingreso": "20220801"
}
```

## Archivos generados

| Archivo | Descripción |
|---|---|
| `alumnos.dat` | Archivo binario donde se guardan los registros |
| `alumnos.idx` | Archivo de texto donde se guarda el índice |

## Estrategia de eliminación

Se usó **Soft Delete (Opción A)**. Cuando se elimina un alumno, el registro no se borra físicamente del archivo `alumnos.dat`, sino que se marca como inactivo en el índice. Esto significa que el registro sigue existiendo en el archivo pero ya no aparece en búsquedas.

Para borrar físicamente los registros eliminados se usa el comando `clean-up`, que lee todos los registros activos, reescribe el archivo `alumnos.dat` solo con ellos y reconstruye el índice desde cero con los nuevos offsets.
