// Aqui empieza el programa y estan las funciones que comparten los modulos.
// Usuario de prueba: admin / contraseña: sistema
#include <iostream>
#include <clocale>
#include <string>
#include <cstdlib>
#include <fstream>
#include <limits> // para descartar toda la entrada que queda antes del Enter
using namespace std;

void menu();

void limpiarPantalla() {
    // El comando cambia segun el sistema donde se ejecute el programa.
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pausa() {
    cout << "\nPresione Enter para continuar...";
    cin.get();
}

int leerEntero() {
    int numero;

    // Si escriben letras, quitamos el error y pedimos el numero otra vez.
    while (!(cin >> numero)) {
        if (cin.eof()) {
            return 0; // ya no hay entrada; los menus revisan esto para salir
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Escriba un numero entero: ";
    }

    // Quitamos el Enter pendiente para que el siguiente getline no se salte.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return numero;
}

float leerDecimal() {
    float numero;

    // Es lo mismo que leerEntero, pero permite precios con decimales.
    while (!(cin >> numero)) {
        if (cin.eof()) {
            return 0;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Escriba un numero (ejemplo: 25.50): ";
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return numero;
}

string campo(const string& linea, int n) {
    // Por ejemplo: campo("123,Ana,5555", 1) devuelve "Ana".
    size_t inicio = 0;

    // Saltamos las comas anteriores al campo que nos interesa.
    for (int i = 0; i < n; i++) {
        inicio = linea.find(',', inicio);
        if (inicio == string::npos) {
            return ""; // no hay mas campos en esta linea
        }
        inicio++;
    }

    size_t fin = linea.find(',', inicio);
    if (fin == string::npos) {
        fin = linea.size(); // si no hay otra coma, llegamos al final
    }

    return linea.substr(inicio, fin - inicio);
}

string buscarRegistro(const string& ruta, const string& clave) {
    // Clientes y productos se buscan igual: el primer campo es el NIT o codigo.
    ifstream archivo(ruta);
    string linea;

    while (getline(archivo, linea)) {
        if (!linea.empty() && campo(linea, 0) == clave) {
            return linea; // devolvemos la linea completa para leer sus datos
        }
    }
    return ""; // no se encontro, o no se pudo abrir el archivo
}

int main() {
    setlocale(LC_ALL, "");
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
        } else {
            cout << "\nUsuario o contraseña incorrectos. Intente de nuevo.\n";
            pausa();
        }
    } while (!accesoConcedido);

    menu();
    return 0;
}
