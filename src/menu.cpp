// Este menu solo decide a que modulo entrar.
#include <iostream>
using namespace std;

// Estas funciones estan hechas en main.cpp. Aqui solo las anunciamos.
void limpiarPantalla();
void pausa();
int leerEntero();

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
        opcion = leerEntero();
        if (cin.eof()) {
            return;
        }

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
