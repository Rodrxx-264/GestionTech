#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Cliente {
    string nit;
    string nombre;
    string telefono;
};

void clientes() {
    int opcion;
    do {
        cout << "\n==========================" << endl;
        cout << "    MODULO DE CLIENTES" << endl;
        cout << "==========================" << endl;
        cout << "1. Registrar Nuevo Cliente" << endl;
        cout << "2. Listar Clientes" << endl;
        cout << "3. Volver al Menú Principal" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                string nuevoNit, nuevoNombre, nuevoTelefono;
                
                cout << "\n==========================" << endl;
                cout << "     REGISTRAR CLIENTE" << endl;
                cout << "==========================" << endl;
                cout << "Ingrese el NIT del cliente: ";
                cin >> nuevoNit;

                // Validar si el NIT ya existe antes de registrar
                ifstream archivoLectura("../data/clientes.txt");
                string linea;
                bool clienteExiste = false;

                if (archivoLectura.is_open()) {
                    while (getline(archivoLectura, linea)) {
                        if (linea.empty()) continue;
                        
                        size_t pos = linea.find(',');
                        string nitArchivo = (pos != string::npos) ? linea.substr(0, pos) : linea;
                        
                        if (nitArchivo == nuevoNit) {
                            clienteExiste = true;
                            break;
                        }
                    }
                    archivoLectura.close();
                }

                if (clienteExiste) {
                    cout << "\n[!] Error: El NIT '" << nuevoNit << "' ya se encuentra registrado en el sistema.\n";
                } else {
                    cout << "Ingrese el nombre completo: ";
                    cin.ignore(); // Limpiar el buffer de entrada
                    getline(cin, nuevoNombre);

                    cout << "Ingrese el teléfono de contacto: ";
                    cin >> nuevoTelefono;

                    // Guardar en el archivo de texto
                    ofstream archivoSalida("../data/clientes.txt", ios::app);
                    if (archivoSalida.is_open()) {
                        archivoSalida << nuevoNit << "," << nuevoNombre << "," << nuevoTelefono << endl;
                        archivoSalida.close();
                        cout << "\n[✓] ¡Cliente registrado y guardado exitosamente!\n";
                    } else {
                        cout << "\n[X] Error: No se pudo abrir el archivo para guardar el cliente.\n";
                    }
                }
                break;
            }
            case 2: {
                cout << "\n==========================" << endl;
                cout << "    LISTADO DE CLIENTES" << endl;
                cout << "==========================" << endl;

                ifstream archivoLectura("../data/clientes.txt");
                string linea;
                bool hayClientes = false;

                if (archivoLectura.is_open()) {
                    cout << "NIT\t\tNOMBRE\t\t\tTELÉFONO" << endl;
                    cout << "--------------------------------------------------" << endl;
                    
                    while (getline(archivoLectura, linea)) {
                        if (linea.empty()) continue;
                        hayClientes = true;
                        cout << linea << endl;
                    }
                    archivoLectura.close();

                    if (!hayClientes) {
                        cout << "[Aviso] Aún no hay clientes registrados en el sistema.\n";
                    }
                } else {
                    cout << "[Aviso] No se encontró el archivo de clientes o aún no ha sido creado.\n";
                }
                break;
            }
            case 3:
                cout << "Regresando al menú principal...\n";
                break;
            default:
                cout << "[!] Opción inválida. Intente de nuevo.\n";
                break;
        }

    } while (opcion != 3);
}