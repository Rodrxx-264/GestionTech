// Registro y listado de clientes.
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
using namespace std;

// Funciones compartidas que estan en main.cpp.
void limpiarPantalla();
void pausa();

const string ARCHIVO_CLIENTES = "../txt/clientes.txt";

struct Cliente {
    string nit;
    string nombre;
    string telefono;
};

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
        if (!(cin >> opcion)) {
            if (cin.eof()) return;
            cin.clear();
            opcion = -1;
        }
        // Quitamos el Enter para que el siguiente getline lea el dato completo.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        limpiarPantalla();
        switch (opcion) {
            case 1: {
                // Los datos del cliente solo se necesitan en esta opcion.
                Cliente cliente;
                cout << "REGISTRAR CLIENTE\n";
                cout << "Ingrese NIT o CUI: ";
                getline(cin, cliente.nit);

                if (cliente.nit.empty() || cliente.nit.find(',') != string::npos) {
                    cout << "Escriba un NIT o CUI sin comas.\n";
                    break;
                }
                ifstream consulta(ARCHIVO_CLIENTES);
                string linea;
                bool existe = false;
                // El NIT es el primer dato de cada linea; evitamos duplicados.
                while (getline(consulta, linea)) {
                    if (linea.substr(0, linea.find(',')) == cliente.nit) {
                        existe = true;
                        break;
                    }
                }
                if (existe) {
                    cout << "Este NIT ya está registrado.\n";
                    break;
                }
                cout << "Ingrese el nombre completo: ";
                getline(cin, cliente.nombre);
                cout << "Ingrese el teléfono: ";
                getline(cin, cliente.telefono);
                if (cliente.nombre.empty() || cliente.telefono.empty() ||
                    cliente.nombre.find(',') != string::npos || cliente.telefono.find(',') != string::npos) {
                    // Las comas separan columnas en el archivo de clientes.
                    cout << "Complete los datos sin usar comas.\n";
                    break;
                }
                ofstream archivo(ARCHIVO_CLIENTES, ios::app);
                if (archivo.is_open()) {
                    // app añade el cliente sin borrar los anteriores.
                    archivo << cliente.nit << ',' << cliente.nombre << ',' << cliente.telefono << '\n';
                    archivo.close();
                }
                if (!archivo.fail()) {
                    cout << "Cliente registrado y guardado.\n";
                } else {
                    cout << "No se pudo guardar el cliente. Revise la carpeta txt.\n";
                }
                break;
            }
            case 2: {
                ifstream archivo(ARCHIVO_CLIENTES);
                string linea;
                int total = 0;
                cout << "LISTADO DE CLIENTES\n";
                if (!archivo.is_open()) {
                    cout << "No se pudo abrir el archivo de clientes.\n";
                    break;
                }
                // Una linea del archivo representa un cliente.
                cout << "NIT/CUI | NOMBRE COMPLETO | TELÉFONO\n";
                while (getline(archivo, linea)) {
                    if (!linea.empty()) {
                        string nit, nombre, telefono;
                        stringstream datos(linea);
                        getline(datos, nit, ',');
                        getline(datos, nombre, ',');
                        getline(datos, telefono);
                        cout << nit << " | " << nombre << " | " << telefono << '\n';
                        total++;
                    }
                }
                cout << "Total de clientes: " << total << '\n';
                break;
            }
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
