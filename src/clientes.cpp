// Registro y listado de clientes.
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Funciones compartidas que estan en main.cpp.
void limpiarPantalla();
void pausa();
int leerEntero();
string campo(const string& linea, int n);
string buscarRegistro(const string& ruta, const string& clave);

const string ARCHIVO_CLIENTES = "../data/clientes.txt";

struct Cliente {
    string nit;
    string nombre;
    string telefono;
};

// static deja estas funciones solo para este archivo.
static void registrarCliente();
static void listarClientes();

// Esta no es static porque facturacion tambien la usa para el registro rapido.
bool guardarCliente(const string& nit, const string& nombre, const string& telefono) {
    // app agrega al final sin borrar los clientes anteriores.
    ofstream archivo(ARCHIVO_CLIENTES, ios::app);
    if (!archivo.is_open()) {
        return false;
    }

    archivo << nit << ',' << nombre << ',' << telefono << '\n';
    archivo.close();
    return !archivo.fail(); // comprobamos que se pudo terminar de guardar
}

void clientes() {
    int opcion;

    do {
        limpiarPantalla();
        cout << "===================================================\n";
        cout << "                 MÓDULO DE CLIENTES\n";
        cout << "===================================================\n";
        cout << "1. Registrar Nuevo Cliente\n";
        cout << "2. Listar Clientes\n";
        cout << "3. Volver al Menú Principal\n";
        cout << "Seleccione una opción [1-3]: ";
        opcion = leerEntero();
        if (cin.eof()) {
            return;
        }

        limpiarPantalla();
        switch (opcion) {
            case 1:
                registrarCliente();
                break;
            case 2:
                listarClientes();
                break;
            case 3:
                cout << "Regresando al menú principal...\n";
                break;
            default:
                cout << "Opción inválida. Intente de nuevo.\n";
                break;
        }

        // Dejamos ver el resultado. Al dar Enter, el bucle limpia y muestra el menu.
        if (opcion != 3) {
            pausa();
        }
    } while (opcion != 3);
}

static void registrarCliente() {
    Cliente cliente;

    cout << "REGISTRAR CLIENTE\n";
    cout << "---------------------------------------------------\n";
    cout << "Ingrese el NIT: ";
    getline(cin, cliente.nit);

    if (cliente.nit.empty()) {
        cout << "El NIT no puede estar vacío.\n";
        return; // vuelve al menu sin pedir los demas datos
    }
    if (!buscarRegistro(ARCHIVO_CLIENTES, cliente.nit).empty()) {
        cout << "Este NIT ya está registrado.\n";
        return;
    }

    // getline permite nombres y telefonos con espacios.
    cout << "Ingrese el nombre completo: ";
    getline(cin, cliente.nombre);
    cout << "Ingrese el teléfono: ";
    getline(cin, cliente.telefono);

    if (cliente.nombre.empty() || cliente.telefono.empty()) {
        cout << "El nombre y el teléfono no pueden quedar vacíos.\n";
        return;
    }
    // Las comas se usan para separar los campos del archivo, no para los datos.
    if (cliente.nit.find(',') != string::npos ||
        cliente.nombre.find(',') != string::npos ||
        cliente.telefono.find(',') != string::npos) {
        cout << "No use comas en los datos del cliente.\n";
        return;
    }

    if (guardarCliente(cliente.nit, cliente.nombre, cliente.telefono)) {
        cout << "\n¡Cliente registrado y guardado exitosamente!\n";
    } else {
        cout << "No se pudo guardar el cliente. Revise la carpeta data.\n";
    }
}

static void listarClientes() {
    ifstream archivo(ARCHIVO_CLIENTES);
    string linea;
    int total = 0;

    cout << "LISTADO DE CLIENTES\n";
    cout << "---------------------------------------------------\n";
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de clientes. Puede que aún no exista.\n";
        return;
    }

    cout << "NIT | NOMBRE | TELÉFONO\n";
    while (getline(archivo, linea)) {
        if (!linea.empty()) {
            cout << campo(linea, 0) << " | " << campo(linea, 1)
                 << " | " << campo(linea, 2) << '\n';
            total++;
        }
    }

    if (total == 0) {
        cout << "Aún no hay clientes registrados.\n";
    } else {
        cout << "---------------------------------------------------\n";
        cout << "Total: " << total << " cliente(s).\n";
    }
    // Los archivos tambien se cierran solos al terminar la funcion.
}
