#include <iostream>
#include <locale.h>
#include <string>
#include <cstdlib>

using namespace std;

void menu(); 

int main() {
      setlocale(LC_ALL, "spanish");
    string usuario;
    string password;
    bool accesoConcedido = false;

    do {
        // Limpiamos pantalla (en Mac/Linux usa "clear", en Windows "cls")
        system("clear"); 

        cout << "===================================================\n";
        cout << "        SISTEMA DE GESTIÓN - ACCESO ADMIN           \n";
        cout << "===================================================\n";
        cout << " Por favor, ingrese sus credenciales de acceso.\n\n";
        cout << " Usuario: ";
        cin >> usuario;
        cout << " Contraseña: ";
        cin >> password;
        cout << "---------------------------------------------------\n";

        // Validamos credenciales fijas de administrador
        if (usuario == "admin" && password == "sistema") {
            cout << " ¡Acceso concedido! Bienvenido al sistema.\n";
            accesoConcedido = true;
        } else {
            cout << " ¡Error! Usuario o contraseña incorrectos.\n";
            cout << " Intente de nuevo.\n";
            cout << "---------------------------------------------------\n";
            cout << " Presione Enter para continuar...";
            cin.ignore(); // Limpiamos el buffer
            cin.get();    // Esperamos a que el usuario presione Enter
        }

    } while (!accesoConcedido); // Se repite si fallas, avanza si aciertas

    cout << "\n---------------------------------------------------\n";
    cout << " [✓] Entrando al sistema principal...\n";
    menu();
    return 0;
}