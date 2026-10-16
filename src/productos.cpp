/* Modulo de Productos e Inventario */

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>   // setprecision (para los decimales)
#include <cstdlib>   // atoi, atof, system
#include <cstdio>    // remove, rename

using namespace std;

// Datos de un producto
struct Producto {
    string codigo;
    string nombre;
    string categoria;
    float precio;
    int stock;
};

static void agregarProducto();
static void verCatalogo();
static void actualizarProducto();
static void eliminarProducto();

// Saca el campo numero n de una linea con comas (n empieza en 0)
static string campo(string linea, int n) {
    int i;
    int inicio = 0;

    for (i = 0; i < n; i++) {
        inicio = linea.find(',', inicio) + 1;
    }

    int fin = linea.find(',', inicio);
    if (fin < 0) {
        fin = linea.length();
    }

    return linea.substr(inicio, fin - inicio);
}

// Busca un producto por su codigo; si lo encuentra lo guarda en *p y devuelve true
static bool buscarProducto(string codigo, Producto *p) {
    ifstream archivo("../data/productos.txt");
    string linea;
    bool encontrado = false;

    while (getline(archivo, linea)) {
        if (linea != "" && campo(linea, 0) == codigo) {
            p->codigo    = campo(linea, 0);
            p->nombre    = campo(linea, 1);
            p->categoria = campo(linea, 2);
            p->precio    = atof(campo(linea, 3).c_str());
            p->stock     = atoi(campo(linea, 4).c_str());
            encontrado = true;
        }
    }
    archivo.close();

    return encontrado;
}

// Menu principal del modulo de productos e inventario
void productos() {
    int op;

    do {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        cout << "===================================================\n";
        cout << "        MÓDULO DE PRODUCTOS E INVENTARIO\n";
        cout << "===================================================\n";
        cout << "[1] Agregar Nuevo Producto\n";
        cout << "[2] Ver Catálogo de Productos\n";
        cout << "[3] Actualizar Producto (Precio/Stock)\n";
        cout << "[4] Eliminar Producto del Inventario\n";
        cout << "[5] Volver al Menú Principal\n";
        cout << "---------------------------------------------------\n";
        cout << "Seleccione una opción [1-5]: ";
        cin >> op;

        if (op == 1) {
            agregarProducto();
        } else if (op == 2) {
            verCatalogo();
        } else if (op == 3) {
            actualizarProducto();
        } else if (op == 4) {
            eliminarProducto();
        } else if (op == 5) {
            cout << "\nRegresando al menú principal...\n";
        } else {
            cout << "\n[!] Opción inválida.\n";
            cout << "Presione Enter para continuar...";
            cin.ignore();
            cin.get();
        }

    } while (op != 5);
}

// Registra un producto nuevo en el inventario
static void agregarProducto() {
    string cod;
    Producto p, existente;

    cout << "---------------------------------------------------\n";
    cout << "AGREGAR NUEVO PRODUCTO\n";
    cout << "---------------------------------------------------\n";
    cout << "Ingrese el Código del producto: ";
    cin >> cod;

    if (buscarProducto(cod, &existente)) {
        cout << "[!] Ya existe un producto registrado con ese código.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore();
        cin.get();
        return;
    }

    p.codigo = cod;

    cin.ignore();
    cout << "Ingrese el Nombre: ";
    getline(cin, p.nombre);
    cout << "Ingrese la Categoría: ";
    getline(cin, p.categoria);
    cout << "Ingrese el Precio: Q";
    cin >> p.precio;
    cout << "Ingrese el Stock inicial: ";
    cin >> p.stock;

    ofstream archivo("../data/productos.txt", ios::app);
    archivo << p.codigo << "," << p.nombre << "," << p.categoria << ","
            << fixed << setprecision(2) << p.precio << "," << p.stock << endl;
    archivo.close();

    cout << "---------------------------------------------------\n";
    cout << "Producto agregado con éxito.\n";
    cout << "Guardado en productos.txt\n";
    cout << "---------------------------------------------------\n";
    cout << "Presione Enter para continuar...";
    cin.ignore();
    cin.get();
}

// Muestra el catalogo completo cargandolo primero a un arreglo
static void verCatalogo() {
    const int MAX_PRODUCTOS = 100;
    Producto lista[MAX_PRODUCTOS];
    int n = 0;

    ifstream archivo("../data/productos.txt");
    string linea;

    while (getline(archivo, linea) && n < MAX_PRODUCTOS) {
        if (linea != "") {
            lista[n].codigo    = campo(linea, 0);
            lista[n].nombre    = campo(linea, 1);
            lista[n].categoria = campo(linea, 2);
            lista[n].precio    = atof(campo(linea, 3).c_str());
            lista[n].stock     = atoi(campo(linea, 4).c_str());
            n++;
        }
    }
    archivo.close();

    cout << "---------------------------------------------------------------------\n";
    cout << "CATÁLOGO DE PRODUCTOS\n";
    cout << "---------------------------------------------------------------------\n";
    cout << "CÓDIGO | NOMBRE | CATEGORÍA | PRECIO | STOCK\n";
    cout << "---------------------------------------------------------------------\n";

    cout << fixed << setprecision(2);

    // recorremos el arreglo con un puntero
    for (Producto *p = lista; p < lista + n; p++) {
        cout << p->codigo << " | " << p->nombre << " | " << p->categoria
             << " | Q" << p->precio << " | " << p->stock << "\n";
    }

    cout << "---------------------------------------------------------------------\n";
    if (n == 0) {
        cout << "No hay productos registrados.\n";
    } else {
        cout << "Total de productos registrados: " << n << "\n";
    }
    cout << "Presione Enter para continuar...";
    cin.ignore();
    cin.get();
}

// Actualiza el precio o el stock de un producto existente
static void actualizarProducto() {
    string cod;
    Producto p;
    int opcion;

    cout << "---------------------------------------------------\n";
    cout << "ACTUALIZAR PRODUCTO\n";
    cout << "---------------------------------------------------\n";
    cout << "Ingrese el Código del producto: ";
    cin >> cod;

    if (!buscarProducto(cod, &p)) {
        cout << "[!] No se encontró un producto con ese código.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore();
        cin.get();
        return;
    }

    cout << fixed << setprecision(2);
    cout << "Producto encontrado: " << p.nombre << "\n";
    cout << "Precio actual: Q" << p.precio << " | Stock actual: " << p.stock << "\n";
    cout << "[1] Actualizar Precio\n";
    cout << "[2] Actualizar Stock\n";
    cout << "Seleccione una opción [1-2]: ";
    cin >> opcion;

    if (opcion == 1) {
        cout << "Ingrese el nuevo precio: Q";
        cin >> p.precio;
    } else if (opcion == 2) {
        cout << "Ingrese el nuevo stock: ";
        cin >> p.stock;
    } else {
        cout << "[!] Opción inválida.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore();
        cin.get();
        return;
    }

    ifstream original("../data/productos.txt");
    ofstream temp("../data/temp.txt");
    string linea;

    while (getline(original, linea)) {
        if (linea != "") {
            if (campo(linea, 0) == cod) {
                temp << p.codigo << "," << p.nombre << "," << p.categoria << ","
                     << fixed << setprecision(2) << p.precio << "," << p.stock << endl;
            } else {
                temp << linea << endl;
            }
        }
    }

    original.close();
    temp.close();

    remove("../data/productos.txt");
    rename("../data/temp.txt", "../data/productos.txt");

    cout << "---------------------------------------------------\n";
    cout << "Producto actualizado con éxito.\n";
    cout << "---------------------------------------------------\n";
    cout << "Presione Enter para continuar...";
    cin.ignore();
    cin.get();
}

// Elimina un producto del inventario
static void eliminarProducto() {
    string cod;
    Producto p;
    char confirmar;

    cout << "---------------------------------------------------\n";
    cout << "ELIMINAR PRODUCTO\n";
    cout << "---------------------------------------------------\n";
    cout << "Ingrese el Código del producto: ";
    cin >> cod;

    if (!buscarProducto(cod, &p)) {
        cout << "[!] No se encontró un producto con ese código.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore();
        cin.get();
        return;
    }

    cout << "Producto encontrado: " << p.nombre << "\n";
    cout << "¿Desea eliminarlo? (S/N): ";
    cin >> confirmar;

    if (confirmar != 'S' && confirmar != 's') {
        cout << "\nOperación cancelada.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore();
        cin.get();
        return;
    }

    ifstream original("../data/productos.txt");
    ofstream temp("../data/temp.txt");
    string linea;

    while (getline(original, linea)) {
        if (linea != "" && campo(linea, 0) != cod) {
            temp << linea << endl;
        }
    }

    original.close();
    temp.close();

    remove("../data/productos.txt");
    rename("../data/temp.txt", "../data/productos.txt");

    cout << "---------------------------------------------------\n";
    cout << "Producto eliminado con éxito.\n";
    cout << "---------------------------------------------------\n";
    cout << "Presione Enter para continuar...";
    cin.ignore();
    cin.get();
}