#include <iostream>
#include <cstdlib>

using namespace std;

void clientes();
void productos();
void facturacion();
void menu() {
    int opcion;
    
    do
    {
        system("clear");
        
        cout << "===================================================\n";
        cout << "   SISTEMA DE GESTIÓN - TIENDA ELECTRÓNICA     \n";
        cout << "===================================================\n";
        cout << " [1] Gestión de Clientes\n";
        cout << " [2] Gestión de Productos\n";
        cout << " [3] Módulo de Facturación y Ventas\n";
        cout << " [4] Salir\n";
        cout << "---------------------------------------------------\n";
        cout << " Seleccione una opción [1-4]: ";
        cin >> opcion;
        
        switch (opcion)
        {
        case 1:
            clientes();
            break;
        case 2:
            productos();
            break;
        case 3:
            facturacion();
            break;
        default:
            cout << "\n¡Opción no válida! Intente de nuevo.\n";
            break;
        }
        
        if (opcion != 4)
        {
            cout << "\nPresione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
        
    } while(opcion != 4);
}