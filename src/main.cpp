#include <iostream>
#include <locale.h>
#include <string>
#include <cstdlib>

using namespace std;

void menu(); 

int main()
{
      setlocale(LC_ALL, "spanish");
    string usuario;
    string password;
    bool accesoConcedido = false;

    do
    {
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

        if (usuario == "admin" && password == "sistema")
        {
            cout << " ¡Acceso concedido! Bienvenido al sistema.\n";
            accesoConcedido = true;
        } else
        {
            cout << " ¡Error! Usuario o contraseña incorrectos.\n";
            cout << " Intente de nuevo.\n";
            cout << "---------------------------------------------------\n";
            cout << " Presione Enter para continuar...";
            cin.ignore();
            cin.get(); 
        }

    }
    while (!accesoConcedido);

    cout << "\n---------------------------------------------------\n";
    cout << " [✓] Entrando al sistema principal...\n";
    menu();
    return 0;
}