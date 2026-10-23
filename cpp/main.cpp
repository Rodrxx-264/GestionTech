// Inicio de sesion y dos acciones usadas por los menus.
// Credenciales: admin / sistema.
#include <iostream>
#include <clocale>
#include <string>
#include <cstdlib>

#ifdef _WIN32
    #include <windows.h>
#endif

using namespace std;

void menu();

void limpiarPantalla() {
    // Cada sistema usa un comando distinto para limpiar la consola.
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pausa() {
    // El Enter permite leer el resultado antes de volver a mostrar un menu.
    cout << "\nPresione Enter para continuar...";
    cin.get();
}

int main() {
    setlocale(LC_ALL, "");

    #ifdef _WIN32
        // En Windows, mostramos y leemos los textos usando UTF-8.
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        setlocale(LC_CTYPE, ".UTF8");
    #endif

    string usuario, password;
    bool accesoConcedido = false;

    do {
        limpiarPantalla();
        cout << "===================================================\n";
        cout << "        SISTEMA DE GESTIÓN - ACCESO ADMIN\n";
        cout << "===================================================\n";
        cout << "Usuario: ";
        getline(cin, usuario);
        cout << "Contraseña: ";
        getline(cin, password);

        if (cin.eof()) {
            return 0;
        }

        // Solo entramos al menu cuando coinciden los dos datos.
        if (usuario == "admin" && password == "sistema") {
            cout << "\n¡Acceso concedido! Bienvenido al sistema.\n";
            accesoConcedido = true;
            pausa();
        } else {
            cout << "\nUsuario o contraseña incorrectos. Intente de nuevo.\n";
            pausa();
        }
    } while (!accesoConcedido);

    menu();
    return 0;
}
