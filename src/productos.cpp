// Aqui manejamos el catalogo y los cambios en el inventario.
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip> // precios con dos decimales
#include <cstdlib> // atoi y atof convierten el texto del archivo a numeros
#include <cstdio>  // rename y remove para reemplazar el archivo
using namespace std;

// Funciones compartidas que estan en main.cpp.
void limpiarPantalla();
void pausa();
int leerEntero();
float leerDecimal();
string campo(const string& linea, int n);
string buscarRegistro(const string& ruta, const string& clave);

const string ARCHIVO_PRODUCTOS = "../data/productos.txt";
const string TEMPORAL_PRODUCTOS = "../data/productos_temporal.txt";

struct Producto {
    string codigo;
    string nombre;
    string categoria;
    float precio;
    int stock;
};

// Estas funciones solo se usan en este archivo, por eso llevan static.
static void agregarProducto();
static void verCatalogo();
static void actualizarProducto();
static void eliminarProducto();

static Producto leerProducto(const string& linea) {
    // El archivo guarda los cinco campos que se planearon en la fase 2.
    Producto producto;
    producto.codigo = campo(linea, 0);
    producto.nombre = campo(linea, 1);
    producto.categoria = campo(linea, 2);
    producto.precio = atof(campo(linea, 3).c_str());
    producto.stock = atoi(campo(linea, 4).c_str());
    return producto;
}

static void escribirProducto(ofstream& archivo, const Producto& producto) {
    // La categoria va entre el nombre y el precio, igual que en el diseño.
    archivo << producto.codigo << ',' << producto.nombre << ','
            << producto.categoria << ',' << fixed << setprecision(2) << producto.precio << ','
            << producto.stock << '\n';
}

static bool buscarProducto(const string& codigo, Producto& producto) {
    string linea = buscarRegistro(ARCHIVO_PRODUCTOS, codigo);
    if (linea.empty()) {
        return false;
    }
    // El & permite dejar los datos encontrados en el producto que nos pasaron.
    producto = leerProducto(linea);
    return true;
}

static bool guardarProducto(const Producto& producto) {
    ofstream archivo(ARCHIVO_PRODUCTOS, ios::app);
    if (!archivo.is_open()) {
        return false;
    }
    escribirProducto(archivo, producto);
    archivo.close();
    return !archivo.fail();
}

static bool cambiarProducto(const Producto& producto, bool eliminar) {
    // No podemos cambiar una sola linea directamente: copiamos a otro archivo.
    ifstream original(ARCHIVO_PRODUCTOS);
    if (!original.is_open()) {
        return false;
    }
    ofstream temporal(TEMPORAL_PRODUCTOS);
    if (!temporal.is_open()) {
        return false;
    }

    string linea;
    while (getline(original, linea)) {
        if (campo(linea, 0) == producto.codigo) {
            // Al eliminar, no copiamos la linea. Al actualizar, escribimos la nueva.
            if (!eliminar) {
                escribirProducto(temporal, producto);
            }
        } else {
            temporal << linea << '\n'; // los demas productos quedan iguales
        }
    }

    bool lecturaCorrecta = !original.bad();
    original.close();
    temporal.close();
    if (!lecturaCorrecta || temporal.fail()) {
        return false;
    }

    // Windows no permite renombrar sobre un archivo que ya existe.
    // Primero apartamos el original; si algo falla, lo devolvemos a su lugar.
    #ifdef _WIN32
        string respaldo = ARCHIVO_PRODUCTOS + ".respaldo";
        ifstream copiaAnterior(respaldo);
        if (copiaAnterior.is_open()) {
            return false; // no borramos un respaldo que ya estaba ahi
        }
        if (rename(ARCHIVO_PRODUCTOS.c_str(), respaldo.c_str()) != 0) {
            return false;
        }
        if (rename(TEMPORAL_PRODUCTOS.c_str(), ARCHIVO_PRODUCTOS.c_str()) != 0) {
            rename(respaldo.c_str(), ARCHIVO_PRODUCTOS.c_str());
            return false;
        }
        remove(respaldo.c_str());
        return true;
    #else
    // En macOS/Linux rename reemplaza el original sin tener que borrarlo antes.
    return rename(TEMPORAL_PRODUCTOS.c_str(), ARCHIVO_PRODUCTOS.c_str()) == 0;
    #endif
}

// Facturacion llama esta funcion. El cambio de inventario queda en este modulo.
bool bajarStock(const string& codigo, int cantidad) {
    Producto producto;
    if (!buscarProducto(codigo, producto)) {
        return false;
    }
    if (cantidad <= 0 || cantidad > producto.stock) {
        return false;
    }
    producto.stock = producto.stock - cantidad;
    return cambiarProducto(producto, false);
}

void productos() {
    int opcion;

    do {
        limpiarPantalla();
        cout << "===================================================\n";
        cout << "                 MÓDULO DE INVENTARIO\n";
        cout << "===================================================\n";
        cout << "1. Agregar Nuevo Producto\n";
        cout << "2. Ver Catálogo de Productos\n";
        cout << "3. Actualizar Producto (Precio/Stock)\n";
        cout << "4. Eliminar Producto del Inventario\n";
        cout << "5. Volver al Menú Principal\n";
        cout << "Seleccione una opción [1-5]: ";
        opcion = leerEntero();
        if (cin.eof()) {
            return;
        }

        limpiarPantalla();
        switch (opcion) {
            case 1:
                agregarProducto();
                break;
            case 2:
                verCatalogo();
                break;
            case 3:
                actualizarProducto();
                break;
            case 4:
                eliminarProducto();
                break;
            case 5:
                cout << "Regresando al menú principal...\n";
                break;
            default:
                cout << "Opción inválida. Intente de nuevo.\n";
                break;
        }

        // Igual que en clientes: Enter para ver el menu limpio otra vez.
        if (opcion != 5) {
            pausa();
        }
    } while (opcion != 5);
}

static void agregarProducto() {
    Producto producto, existente;

    cout << "AGREGAR PRODUCTO\n";
    cout << "---------------------------------------------------\n";
    cout << "Ingrese el código: ";
    getline(cin, producto.codigo);

    if (producto.codigo.empty()) {
        cout << "El código no puede estar vacío.\n";
        return;
    }
    if (buscarProducto(producto.codigo, existente)) {
        cout << "Ya existe un producto con ese código.\n";
        return;
    }

    cout << "Ingrese el nombre: ";
    getline(cin, producto.nombre);
    cout << "Ingrese la categoría: ";
    getline(cin, producto.categoria);
    if (producto.nombre.empty() || producto.categoria.empty() ||
        producto.codigo.find(',') != string::npos ||
        producto.nombre.find(',') != string::npos ||
        producto.categoria.find(',') != string::npos) {
        cout << "Complete el nombre y la categoría sin usar comas.\n";
        return;
    }
    cout << "Ingrese el precio: Q";
    producto.precio = leerDecimal();
    cout << "Ingrese el stock inicial: ";
    producto.stock = leerEntero();
    if (cin.eof()) {
        return;
    }

    if (producto.precio < 0 || producto.stock < 0) {
        cout << "El precio y el stock no pueden ser negativos.\n";
        return;
    }

    if (guardarProducto(producto)) {
        cout << "\nProducto agregado con éxito.\n";
    } else {
        cout << "No se pudo guardar el producto.\n";
    }
}

static void verCatalogo() {
    int total = 0;
    string linea;
    ifstream archivo(ARCHIVO_PRODUCTOS);

    cout << "CATÁLOGO DE PRODUCTOS\n";
    cout << "---------------------------------------------------\n";
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de productos. Puede que aún no exista.\n";
        return;
    }

    cout << "CÓDIGO | NOMBRE | CATEGORÍA | PRECIO | STOCK\n";
    cout << fixed << setprecision(2);
    // Igual que en clientes: leemos y mostramos cada linea, sin limite de 100.
    while (getline(archivo, linea)) {
        if (!linea.empty()) {
            Producto producto = leerProducto(linea);
            cout << producto.codigo << " | " << producto.nombre << " | "
                 << producto.categoria << " | Q" << producto.precio
                 << " | " << producto.stock << '\n';
            total++;
        }
    }

    if (total == 0) {
        cout << "Aún no hay productos registrados.\n";
    } else {
        cout << "Total de productos en catálogo: " << total << '\n';
    }
}

static void actualizarProducto() {
    Producto producto;
    string codigo;
    int opcion;

    cout << "ACTUALIZAR PRODUCTO\n";
    cout << "---------------------------------------------------\n";
    cout << "Ingrese el código: ";
    getline(cin, codigo);
    if (!buscarProducto(codigo, producto)) {
        cout << "No se encontró un producto con ese código.\n";
        return;
    }

    cout << fixed << setprecision(2);
    cout << "Producto: " << producto.nombre << '\n';
    cout << "Precio: Q" << producto.precio << " | Stock: " << producto.stock << '\n';
    cout << "1. Actualizar precio\n";
    cout << "2. Actualizar stock\n";
    cout << "Seleccione una opción [1-2]: ";
    opcion = leerEntero();

    switch (opcion) {
        case 1:
            cout << "Ingrese el nuevo precio: Q";
            producto.precio = leerDecimal();
            break;
        case 2:
            cout << "Ingrese el nuevo stock: ";
            producto.stock = leerEntero();
            break;
        default:
            cout << "Opción inválida.\n";
            return;
    }
    if (cin.eof()) {
        return;
    }
    if (producto.precio < 0 || producto.stock < 0) {
        cout << "El precio y el stock no pueden ser negativos.\n";
        return;
    }

    if (cambiarProducto(producto, false)) {
        cout << "\nProducto actualizado con éxito.\n";
    } else {
        cout << "No se pudo actualizar el archivo de productos.\n";
    }
}

static void eliminarProducto() {
    Producto producto;
    string codigo, respuesta;

    cout << "ELIMINAR PRODUCTO\n";
    cout << "---------------------------------------------------\n";
    cout << "Ingrese el código: ";
    getline(cin, codigo);
    if (!buscarProducto(codigo, producto)) {
        cout << "No se encontró un producto con ese código.\n";
        return;
    }

    cout << "Producto: " << producto.nombre << '\n';
    cout << "¿Desea eliminarlo? (S/N): ";
    getline(cin, respuesta);
    if (respuesta != "S" && respuesta != "s") {
        cout << "Operación cancelada.\n";
        return;
    }

    if (cambiarProducto(producto, true)) {
        cout << "\nProducto eliminado con éxito.\n";
    } else {
        cout << "No se pudo eliminar el producto.\n";
    }
}
