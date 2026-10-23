# GestionTech

Proyecto final de Algoritmos. Sistema de gestión para una tienda de
electrónica escrito en C++.

## Integrantes

- Luis Antonio García Morales — 5190-26-5373
- Rodrigo Andree Imeri Raymundo — 5190-26-9636
- Marlon Michael Pérez Méndez — 5190-26-9781
- Alvaro Emilio Cortez López — 5190-26-9911
- Brian Emanuel López Franco — 5190-26-11745

Universidad Mariano Gálvez de Guatemala, sede Villa Nueva. Ingeniería en
Sistemas y Ciencias de la Computación, plan sábado; Algoritmos, sección B.

## Qué hace el programa

1. Solicita usuario y contraseña (`admin` / `sistema`).
2. Permite registrar y listar clientes por NIT o CUI.
3. Permite agregar, consultar, actualizar y eliminar productos del inventario.
4. Registra ventas, calcula IVA (12 %), descuenta stock y muestra el historial.

Cada módulo tiene un menú `do-while` y un `switch`. El trabajo de cada opción
está dentro de su `case`, con comentarios que explican cómo se leen y escriben
los archivos. `main.cpp` contiene el inicio de sesión, la limpieza de pantalla
y la pausa; `funciones.cpp` contiene solo ayudas para leer números y separar los
campos del texto. Ver `TEMAS_DEL_PROYECTO.md` para ubicar los temas de clase.

## Archivos y formatos

El programa debe ejecutarse **desde `cpp`** para encontrar la carpeta `../archivos txt`:

```text
GestionTech/
├── archivos txt/
│   ├── clientes.txt   NIT,NombreCompleto,Telefono
│   ├── productos.txt  Codigo,Nombre,Categoria,Precio,Stock
│   └── facturas.txt   NoFactura,NitCliente,CodigoProducto,Cantidad,Subtotal,Iva,TotalPagar,Fecha
└── cpp/
    ├── main.cpp
    ├── funciones.cpp
    ├── menu.cpp
    ├── clientes.cpp
    ├── productos.cpp
    └── facturacion.cpp
```

Los `.txt` usan UTF-8 y comas como separadores; no escriba comas dentro de los
datos.

## Compilar

En macOS, desde la carpeta `src`:

```bash
clang++ -std=c++17 main.cpp funciones.cpp menu.cpp clientes.cpp productos.cpp facturacion.cpp -o programa
./programa
```

En Windows con `g++` (MSYS2 UCRT64), desde `src`:

```bash
g++ -std=c++17 -finput-charset=UTF-8 -fexec-charset=UTF-8 main.cpp funciones.cpp menu.cpp clientes.cpp productos.cpp facturacion.cpp -o programa.exe
./programa.exe
```

En Windows, `main.cpp` configura la consola para mostrar tildes y ñ usando
UTF-8. Guardar los `.cpp` y `.txt` con esa misma codificación.
