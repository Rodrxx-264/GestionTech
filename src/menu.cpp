#include <iostream>
#include <cstdlib>

using namespace std;

// Prototipos de los otros módulos (para que el menú sepa a qué funciones llamar)
void clientes();
void productos();
void facturacion();

void menu() {
    int opcion;
    
    do {
        cout << "\n=======================================" << endl;
        cout << "   SISTEMA DE GESTIÓN - TIENDA ELECTRÓNICA" << endl;
        cout << "=======================================" << endl;
        cout << "1. Gestión de Clientes \n";
        cout << "2. Gestión de Productos (Inventario) \n";
        cout << "3. Facturación \n";
        cout << "4. Salir \n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        
        switch(opcion) {
            case 1:
                system("cls");
                clientes(); // Llama al código del archivo de clientes
                break;
            case 2:
                system("cls");
                productos(); // Llama al código del archivo de productos
                break;
            case 3:
                system("cls");
                facturacion(); // Llama al código del archivo de facturación
                break;
            case 4:
                system("cls");
                cout << "Saliendo del sistema...\n";
                break;
            default:
                system("cls");
                cout << "¡Opción no válida!\n";
                break;
        }
        
        if (opcion != 4) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore();
            cin.get();
            system("cls");
        }
        
    } while(opcion != 4);
}