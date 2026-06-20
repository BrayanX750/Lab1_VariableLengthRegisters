# Lab 1 - Registros de Tamaño Variable e Índice Simple

Laboratorio 1 - Estructura de Datos II - UNITEC Q2 2026

## Como compilar

Para compilar el proyecto hay que correr este comando en la terminal desde la carpeta raiz del proyecto:

```bash
g++ -std=c++11 src/main.cpp src/record.cpp src/index.cpp src/filemanager.cpp src/page.cpp -o alumnos
```

Eso genera el ejecutable `alumnos` que se usa para todos los comandos.

## Como usar cada comando

Para agregar un alumno se le pasa un archivo json con los datos:
```bash
./alumnos agregar data/alumno1.json
```

También se puede pasar un json con un arreglo de alumnos y los agrega todos de una sola vez:
```bash
./alumnos agregar data/alumnos_prueba.json
```

Para buscar un alumno se le pasa el numero de cuenta:
```bash
./alumnos buscar 2020-10001
```

Para actualizar los datos de un alumno se le pasa el json con los datos nuevos:
```bash
./alumnos actualizar data/alumno1.json
```

Para eliminar un alumno se le pasa el numero de cuenta:
```bash
./alumnos eliminar 2020-10001
```

Para limpiar el archivo de datos y quitar los registros eliminados:
```bash
./alumnos clean-up
```

## Ejemplo completo

```bash
./alumnos agregar data/alumno1.json
./alumnos agregar data/alumno2.json
./alumnos agregar data/alumno3.json
./alumnos buscar 2021-20045
./alumnos eliminar 2022-30099
./alumnos clean-up
```

## Archivos JSON de prueba

También hay un archivo con 20 alumnos para probar la paginación:
```bash
./alumnos agregar data/alumnos_prueba.json
```



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

## Archivos que genera el programa

Cuando se corre el programa se crean dos archivos automaticamente:

- `alumnos.dat` — aqui se guardan los registros de los alumnos en binario con paginas de 512 bytes
- `alumnos.idx` — aqui se guarda el indice con el numero de cuenta, el offset y el tamano de cada registro

## Como funciona la paginacion y el CRC

El archivo `alumnos.dat` guarda los datos en paginas de 512 bytes. Cada pagina tiene un encabezado de 4 bytes que dice cuantos bytes se han usado, luego 504 bytes de datos y al final 4 bytes con un CRC32 para verificar que los datos no esten corruptos.

Cuando se agrega un alumno el programa busca una pagina con espacio suficiente. Si no encuentra ninguna crea una pagina nueva. Si un registro no cabe en una pagina se pone en la siguiente, los registros nunca se parten entre dos paginas.

Cada vez que se lee una pagina se verifica el CRC para asegurarse de que los datos estan bien.

## Como funciona la eliminacion

Cuando se elimina un alumno con el comando `eliminar`, el registro no se borra del archivo `alumnos.dat` inmediatamente, sino que en el indice se marca como inactivo. Esto se llama soft delete.

El registro eliminado sigue en el archivo pero el programa lo ignora en las busquedas. Para borrarlo fisicamente hay que correr `clean-up`, que reescribe el archivo solo con los alumnos activos y reconstruye el indice desde cero.
