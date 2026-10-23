// Inventario: cada opción del menú hace su trabajo en su propio case.
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>
#include <cstdlib> // atoi y atof convierten numeros leidos como texto
#include <cstdio>  // rename cambia el archivo original por el temporal
using namespace std;

void limpiarPantalla();
void pausa();

const string ARCHIVO_PRODUCTOS = "../txt/productos.txt";
const string TEMPORAL_PRODUCTOS = "../txt/productos_temporal.txt";

struct Producto {
    string codigo;
    string nombre;
    string categoria;
    float precio;
    int stock;
};

void productos() {
    int opcion;
    do {
        limpiarPantalla();
        cout << "MÓDULO DE INVENTARIO\n";
        cout << "1. Agregar producto\n2. Ver catálogo\n";
        cout << "3. Actualizar precio o stock\n4. Eliminar producto\n";
        cout << "5. Volver al menú principal\nOpción: ";
        // Una entrada que no sea numero pasa por el default del switch.
        if (!(cin >> opcion)) {
            if (cin.eof()) return;
            cin.clear();
            opcion = -1;
        }
        // Descartamos el Enter antes de pedir textos con getline.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        limpiarPantalla();

        switch (opcion) {
            case 1: {
                // Alta: revisamos el codigo, pedimos los datos y agregamos una linea.
                Producto producto;
                bool existe = false;
                string linea;
                cout << "AGREGAR PRODUCTO\nCódigo: ";
                getline(cin, producto.codigo);
                if (producto.codigo.empty() || producto.codigo.find(',') != string::npos) {
                    cout << "Código inválido.\n";
                    break;
                }
                ifstream consulta(ARCHIVO_PRODUCTOS);
                // El codigo es el primer dato de cada producto guardado.
                while (getline(consulta, linea)) {
                    if (linea.substr(0, linea.find(',')) == producto.codigo) {
                        existe = true;
                        break;
                    }
                }
                if (existe) {
                    cout << "Ya existe un producto con ese código.\n";
                    break;
                }
                cout << "Nombre: ";
                getline(cin, producto.nombre);
                cout << "Categoría: ";
                getline(cin, producto.categoria);
                if (producto.nombre.empty() || producto.categoria.empty() ||
                    producto.nombre.find(',') != string::npos ||
                    producto.categoria.find(',') != string::npos) {
                    cout << "Complete los datos sin comas.\n";
                    break;
                }
                cout << "Precio: Q";
                if (!(cin >> producto.precio)) {
                    if (cin.eof()) return;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Precio inválido.\n";
                    break;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Stock inicial: ";
                if (!(cin >> producto.stock)) {
                    if (cin.eof()) return;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Stock inválido.\n";
                    break;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (producto.precio < 0 || producto.stock < 0) {
                    cout << "El precio y el stock no pueden ser negativos.\n";
                    break;
                }
                ofstream archivo(ARCHIVO_PRODUCTOS, ios::app);
                if (archivo.is_open()) {
                    // app agrega una linea sin borrar los productos anteriores.
                    archivo << producto.codigo << ',' << producto.nombre << ','
                            << producto.categoria << ',' << fixed << setprecision(2)
                            << producto.precio << ',' << producto.stock << '\n';
                    archivo.close();
                }
                if (archivo.fail()) cout << "No se pudo guardar el producto.\n";
                else cout << "Producto agregado con éxito.\n";
                break;
            }
            case 2: {
                // Consulta: leemos el archivo completo sin cambiar el inventario.
                ifstream archivo(ARCHIVO_PRODUCTOS);
                string linea;
                int total = 0;
                cout << "CATÁLOGO DE PRODUCTOS\n";
                if (!archivo.is_open()) {
                    cout << "No se pudo abrir el archivo.\n";
                    break;
                }
                cout << "CÓDIGO | NOMBRE | CATEGORÍA | PRECIO | STOCK\n";
                // Cada coma separa un dato del registro.
                while (getline(archivo, linea)) {
                    if (linea.empty()) continue;
                    Producto producto;
                    string precioTexto, stockTexto;
                    stringstream datos(linea);
                    getline(datos, producto.codigo, ',');
                    getline(datos, producto.nombre, ',');
                    getline(datos, producto.categoria, ',');
                    getline(datos, precioTexto, ',');
                    getline(datos, stockTexto);
                    producto.precio = atof(precioTexto.c_str());
                    producto.stock = atoi(stockTexto.c_str());
                    cout << producto.codigo << " | " << producto.nombre << " | "
                         << producto.categoria << " | Q" << fixed << setprecision(2)
                         << producto.precio << " | " << producto.stock << '\n';
                    total++;
                }
                cout << "Total de productos en catálogo: " << total << '\n';
                break;
            }
            case 3: {
                // Cambio: buscamos el producto y pedimos solo el nuevo valor.
                Producto producto;
                string linea, precioTexto, stockTexto;
                int cambio;
                bool encontrado = false;
                cout << "ACTUALIZAR PRODUCTO\nCódigo: ";
                getline(cin, producto.codigo);
                ifstream consulta(ARCHIVO_PRODUCTOS);
                // Primero leemos los valores actuales, antes de pedir el cambio.
                while (getline(consulta, linea)) {
                    if (linea.substr(0, linea.find(',')) == producto.codigo) {
                        encontrado = true;
                        stringstream datos(linea);
                        datos.ignore(numeric_limits<streamsize>::max(), ',');
                        getline(datos, producto.nombre, ',');
                        getline(datos, producto.categoria, ',');
                        getline(datos, precioTexto, ',');
                        getline(datos, stockTexto);
                        producto.precio = atof(precioTexto.c_str());
                        producto.stock = atoi(stockTexto.c_str());
                        break;
                    }
                }
                consulta.close();
                if (!encontrado) {
                    cout << "No se encontró el producto.\n";
                    break;
                }
                cout << producto.nombre << " | Q" << fixed << setprecision(2)
                     << producto.precio << " | Stock: " << producto.stock << '\n';
                cout << "1. Cambiar precio\n2. Cambiar stock\nOpción: ";
                if (!(cin >> cambio)) {
                    if (cin.eof()) return;
                    cin.clear();
                    cambio = -1;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (cambio == 1) {
                    cout << "Nuevo precio: Q";
                    if (!(cin >> producto.precio)) {
                        if (cin.eof()) return;
                        cin.clear();
                        cout << "Precio inválido.\n";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    }
                } else if (cambio == 2) {
                    cout << "Nuevo stock: ";
                    if (!(cin >> producto.stock)) {
                        if (cin.eof()) return;
                        cin.clear();
                        cout << "Stock inválido.\n";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    }
                } else {
                    cout << "Opción inválida.\n";
                    break;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (producto.precio < 0 || producto.stock < 0) {
                    cout << "No se aceptan valores negativos.\n";
                    break;
                }
                // Un archivo de texto no se modifica en medio: copiamos sus lineas
                // a un archivo temporal y reemplazamos solo el producto buscado.
                ifstream original(ARCHIVO_PRODUCTOS);
                if (!original.is_open()) {
                    cout << "No se pudo abrir el archivo de productos.\n";
                    break;
                }
                ifstream anterior(TEMPORAL_PRODUCTOS);
                if (anterior.is_open()) {
                    cout << "Ya existe un archivo temporal; no se sobrescribirá.\n";
                    break;
                }
                ofstream temporal(TEMPORAL_PRODUCTOS);
                if (!temporal.is_open()) {
                    cout << "No se pudo actualizar el archivo.\n";
                    break;
                }
                while (getline(original, linea)) {
                    if (linea.substr(0, linea.find(',')) == producto.codigo) {
                        temporal << producto.codigo << ',' << producto.nombre << ','
                                 << producto.categoria << ',' << fixed << setprecision(2)
                                 << producto.precio << ',' << producto.stock << '\n';
                    } else temporal << linea << '\n';
                }
                bool correcto = !original.bad();
                original.close();
                temporal.close();
                if (!correcto || temporal.fail()) {
                    cout << "No se pudo escribir el archivo temporal.\n";
                    remove(TEMPORAL_PRODUCTOS.c_str());
                    break;
                }
                // En Windows se aparta el original; en Linux rename lo reemplaza.
                #ifdef _WIN32
                    string respaldo = ARCHIVO_PRODUCTOS + ".respaldo";
                    ifstream existeRespaldo(respaldo);
                    if (existeRespaldo.is_open() || rename(ARCHIVO_PRODUCTOS.c_str(), respaldo.c_str()) != 0) {
                        remove(TEMPORAL_PRODUCTOS.c_str());
                        cout << "No se pudo respaldar el inventario.\n";
                        break;
                    }
                #endif
                if (rename(TEMPORAL_PRODUCTOS.c_str(), ARCHIVO_PRODUCTOS.c_str()) != 0) {
                    #ifdef _WIN32
                        rename(respaldo.c_str(), ARCHIVO_PRODUCTOS.c_str());
                    #endif
                    remove(TEMPORAL_PRODUCTOS.c_str());
                    cout << "No se pudo actualizar el inventario.\n";
                    break;
                }
                #ifdef _WIN32
                    remove(respaldo.c_str());
                #endif
                cout << "Producto actualizado con éxito.\n";
                break;
            }
            case 4: {
                // Baja: confirmamos antes de copiar los demas productos al temporal.
                string codigo, linea, nombre, respuesta;
                bool encontrado = false;
                cout << "ELIMINAR PRODUCTO\nCódigo: ";
                getline(cin, codigo);
                ifstream consulta(ARCHIVO_PRODUCTOS);
                // Buscamos el codigo para mostrar el nombre antes de confirmar.
                while (getline(consulta, linea)) {
                    if (linea.substr(0, linea.find(',')) == codigo) {
                        encontrado = true;
                        stringstream datos(linea);
                        datos.ignore(numeric_limits<streamsize>::max(), ',');
                        getline(datos, nombre, ',');
                        break;
                    }
                }
                consulta.close();
                if (!encontrado) {
                    cout << "No se encontró el producto.\n";
                    break;
                }
                cout << "Producto: " << nombre << "\n¿Desea eliminarlo? (S/N): ";
                getline(cin, respuesta);
                if (respuesta != "S" && respuesta != "s") {
                    cout << "Operación cancelada.\n";
                    break;
                }
                ifstream original(ARCHIVO_PRODUCTOS);
                if (!original.is_open()) {
                    cout << "No se pudo abrir el archivo de productos.\n";
                    break;
                }
                ifstream anterior(TEMPORAL_PRODUCTOS);
                if (anterior.is_open()) {
                    cout << "Ya existe un archivo temporal; no se sobrescribirá.\n";
                    break;
                }
                ofstream temporal(TEMPORAL_PRODUCTOS);
                if (!temporal.is_open()) {
                    cout << "No se pudo eliminar el producto.\n";
                    break;
                }
                // Al copiar omitimos la línea del producto eliminado.
                while (getline(original, linea)) {
                    if (linea.substr(0, linea.find(',')) != codigo) temporal << linea << '\n';
                }
                bool correcto = !original.bad();
                original.close();
                temporal.close();
                if (!correcto || temporal.fail()) {
                    cout << "No se pudo escribir el archivo temporal.\n";
                    remove(TEMPORAL_PRODUCTOS.c_str());
                    break;
                }
                // Solo reemplazamos el archivo real despues de copiarlo completo.
                #ifdef _WIN32
                    string respaldo = ARCHIVO_PRODUCTOS + ".respaldo";
                    ifstream existeRespaldo(respaldo);
                    if (existeRespaldo.is_open() || rename(ARCHIVO_PRODUCTOS.c_str(), respaldo.c_str()) != 0) {
                        remove(TEMPORAL_PRODUCTOS.c_str());
                        cout << "No se pudo respaldar el inventario.\n";
                        break;
                    }
                #endif
                if (rename(TEMPORAL_PRODUCTOS.c_str(), ARCHIVO_PRODUCTOS.c_str()) != 0) {
                    #ifdef _WIN32
                        rename(respaldo.c_str(), ARCHIVO_PRODUCTOS.c_str());
                    #endif
                    remove(TEMPORAL_PRODUCTOS.c_str());
                    cout << "No se pudo eliminar el producto.\n";
                    break;
                }
                #ifdef _WIN32
                    remove(respaldo.c_str());
                #endif
                cout << "Producto eliminado con éxito.\n";
                break;
            }
            case 5:
                cout << "Regresando al menú principal...\n";
                break;
            default:
                cout << "Opción inválida.\n";
        }
        if (opcion != 5) pausa();
    } while (opcion != 5);
}
