// Este menu solo decide a que modulo entrar.
#include <iostream>
#include <limits>
using namespace std;

// Estas funciones estan hechas en main.cpp. Aqui solo las anunciamos.
void limpiarPantalla();
void pausa();

// Cada modulo tiene su propio archivo.
void clientes();
void productos();
void facturacion();

void menu() {
    int opcion;

    do {
        limpiarPantalla();
        cout << "===================================================\n";
        cout << "   SISTEMA DE GESTIÓN - TIENDA ELECTRÓNICA\n";
        cout << "===================================================\n";
        cout << "1. Gestión de Clientes\n";
        cout << "2. Gestión de Productos\n";
        cout << "3. Módulo de Facturación y Ventas\n";
        cout << "4. Salir\n";
        cout << "Seleccione una opción [1-4]: ";
        // Si escriben letras, limpiamos el error y mostramos opcion invalida.
        if (!(cin >> opcion)) {
            if (cin.eof()) return;
            cin.clear();
            opcion = -1;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Cada opcion entra a un modulo, que tiene su propio switch.
        switch (opcion) {
            case 1:
                clientes();
                break;
            case 2:
                productos();
                break;
            case 3:
                facturacion();
                break;
            case 4:
                cout << "\nSaliendo del sistema...\n";
                break;
            default:
                cout << "\nOpción inválida. Intente de nuevo.\n";
                pausa();
                break;
        }
    } while (opcion != 4);
}
