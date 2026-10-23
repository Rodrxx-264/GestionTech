# GestionTech

Sistema de consola en C++ para una tienda de electrónica. Inicio de sesión:
`admin` / `sistema`. Se pueden registrar y listar clientes, administrar el
inventario, registrar ventas de un producto por factura, consultar el historial
y ver un resumen mensual de ventas. Cada venta suma un 12 % de IVA al subtotal.

## Integrantes

- Luis Antonio García Morales — 5190-26-5373
- Rodrigo Andree Imeri Raymundo — 5190-26-9636
- Marlon Michael Pérez Méndez — 5190-26-9781
- Alvaro Emilio Cortez López — 5190-26-9911
- Brian Emanuel López Franco — 5190-26-11745

Universidad Mariano Gálvez de Guatemala, sede Villa Nueva. Ingeniería en
Sistemas y Ciencias de la Computación, plan sábado; Algoritmos, sección B.

## Estructura

```text
Proyecto final/
├── cpp/
│   ├── main.cpp          Acceso, pantalla y pausa
│   ├── menu.cpp          Menú principal
│   ├── clientes.cpp      Registro y listado
│   ├── productos.cpp     Inventario
│   └── facturacion.cpp   Ventas, historial y resumen
└── txt/
    ├── clientes.txt      NIT,Nombre,Telefono
    ├── productos.txt     Codigo,Nombre,Categoria,Precio,Stock
    └── facturas.txt      Numero,NIT,Codigo,Cantidad,Subtotal,IVA,Total,Fecha
```

Los archivos `.txt` usan comas como separadores: no use comas dentro de un dato.
Los archivos ya incluyen ejemplos; puede trabajar con copias si no quiere
modificarlos. El programa usa rutas relativas, por eso **se ejecuta desde `cpp`**.

## Compilar y ejecutar (Linux, macOS o MSYS2)

```bash
cd cpp
g++ -std=c++17 main.cpp menu.cpp clientes.cpp productos.cpp facturacion.cpp -o programa
./programa
```

En Windows el ejecutable se llama `programa.exe`. Para los temas vistos en
clase y ejemplos concretos de dónde aparecen, consulte `TEMAS_DEL_PROYECTO.md`.
