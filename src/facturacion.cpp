// Este modulo registra ventas y muestra las facturas guardadas.
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib> // atoi y atof para leer numeros guardados como texto
#include <ctime>   // para obtener la fecha de la venta
using namespace std;

// Funciones compartidas que estan en main.cpp.
void limpiarPantalla();
void pausa();
int leerEntero();
string campo(const string& linea, int n);
string buscarRegistro(const string& ruta, const string& clave);

// Reutilizamos el guardado de clientes y el cambio de stock de productos.
bool guardarCliente(const string& nit, const string& nombre, const string& telefono);
bool bajarStock(const string& codigo, int cantidad);

const string ARCHIVO_FACTURAS = "../data/facturas.txt";

struct Factura {
    int numero;
    string nit;
    float total;
    string fecha;
};

// Estas funciones solo son para las opciones de este modulo.
static void registrarVenta();
static void verHistorial();

static int siguienteNumero() {
    ifstream archivo(ARCHIVO_FACTURAS);
    string linea;
    int ultimo = 0;

    // Buscamos el numero mas grande para no repetir una factura.
    while (getline(archivo, linea)) {
        int numero = atoi(campo(linea, 0).c_str());
        if (numero > ultimo) {
            ultimo = numero;
        }
    }
    return ultimo + 1;
}

static string numeroConCeros(int numero) {
    string texto = to_string(numero);
    while (texto.length() < 4) {
        texto = "0" + texto; // por ejemplo: 2 se convierte en 0002
    }
    return texto;
}

static bool guardarFactura(const Factura& factura) {
    ofstream archivo(ARCHIVO_FACTURAS, ios::app);
    if (!archivo.is_open()) {
        return false;
    }
    archivo << numeroConCeros(factura.numero) << ',' << factura.nit << ','
            << fixed << setprecision(2) << factura.total << ',' << factura.fecha << '\n';
    archivo.close();
    return !archivo.fail();
}

void facturacion() {
    int opcion;

    do {
        limpiarPantalla();
        cout << "===================================================\n";
        cout << "          MÓDULO DE FACTURACIÓN Y VENTAS\n";
        cout << "===================================================\n";
        cout << "1. Registrar Nueva Venta\n";
        cout << "2. Ver Historial de Facturas\n";
        cout << "3. Volver al Menú Principal\n";
        cout << "Seleccione una opción [1-3]: ";
        opcion = leerEntero();
        if (cin.eof()) {
            return;
        }

        limpiarPantalla();
        switch (opcion) {
            case 1:
                registrarVenta();
                break;
            case 2:
                verHistorial();
                break;
            case 3:
                cout << "Regresando al menú principal...\n";
                break;
            default:
                cout << "Opción inválida. Intente de nuevo.\n";
                break;
        }

        // El mismo comportamiento que clientes y productos.
        if (opcion != 3) {
            pausa();
        }
    } while (opcion != 3);
}

static void registrarVenta() {
    Factura factura;
    string nit, nombreCliente, telefono;
    string codigo, nombreProducto, linea;
    float precio, subtotal, iva;
    int stock, cantidad;

    // Paso 1: buscar al cliente o registrarlo si es nuevo.
    cout << "REGISTRAR VENTA - CLIENTE\n";
    cout << "---------------------------------------------------\n";
    cout << "Ingrese el NIT: ";
    getline(cin, nit);
    if (nit.empty() || nit.find(',') != string::npos) {
        cout << "Escriba un NIT que no esté vacío ni tenga comas.\n";
        return;
    }
    factura.nit = nit;

    linea = buscarRegistro("../data/clientes.txt", nit);
    if (!linea.empty()) {
        nombreCliente = campo(linea, 1);
        cout << "Cliente encontrado: " << nombreCliente << '\n';
    } else {
        cout << "El cliente no está registrado. Vamos a ingresar sus datos.\n";
        cout << "Nombre completo: ";
        getline(cin, nombreCliente);
        cout << "Teléfono: ";
        getline(cin, telefono);

        if (nombreCliente.empty() || telefono.empty() ||
            nombreCliente.find(',') != string::npos || telefono.find(',') != string::npos) {
            cout << "Complete los datos sin usar comas.\n";
            return;
        }
        if (!guardarCliente(nit, nombreCliente, telefono)) {
            cout << "No se pudo guardar el cliente. Se cancela la venta.\n";
            return;
        }
        cout << "Cliente registrado y guardado.\n";
    }

    pausa();
    limpiarPantalla();

    // Paso 2: buscar el producto y revisar que alcance el stock.
    cout << "REGISTRAR VENTA - PRODUCTO\n";
    cout << "---------------------------------------------------\n";
    cout << "Ingrese el código del producto: ";
    getline(cin, codigo);
    linea = buscarRegistro("../data/productos.txt", codigo);
    if (linea.empty()) {
        cout << "El producto no existe.\n";
        return;
    }

    // Usamos los mismos cuatro campos que guarda productos.cpp.
    nombreProducto = campo(linea, 1);
    precio = atof(campo(linea, 2).c_str());
    stock = atoi(campo(linea, 3).c_str());
    cout << fixed << setprecision(2);
    cout << "Producto encontrado: " << nombreProducto << '\n';
    cout << "Precio unitario: Q" << precio << '\n';
    cout << "Stock disponible: " << stock << '\n';
    cout << "Ingrese la cantidad a comprar: ";
    cantidad = leerEntero();
    if (cin.eof()) {
        return;
    }

    if (cantidad <= 0) {
        cout << "La cantidad debe ser mayor que cero.\n";
        return;
    }
    if (cantidad > stock) {
        cout << "No hay suficiente stock. Solo hay " << stock << " unidades.\n";
        return;
    }

    // Paso 3: calcular el total y preparar la factura.
    factura.numero = siguienteNumero();
    subtotal = precio * cantidad;
    iva = subtotal * 0.12f;
    factura.total = subtotal + iva;

    // localtime nos da el dia, mes y año de la computadora.
    time_t reloj = time(0);
    tm* hoy = localtime(&reloj);
    factura.fecha = to_string(hoy->tm_mday) + "/" + to_string(hoy->tm_mon + 1)
                    + "/" + to_string(hoy->tm_year + 1900);

    cout << "\nFACTURA ELECTRÓNICA\n";
    cout << "---------------------------------------------------\n";
    cout << "No. Factura: " << numeroConCeros(factura.numero) << '\n';
    cout << "Cliente: " << nombreCliente << " (NIT: " << nit << ")\n";
    cout << "Fecha: " << factura.fecha << '\n';
    cout << "DESCRIPCIÓN | CANTIDAD | PRECIO | SUBTOTAL\n";
    cout << nombreProducto << " | " << cantidad << " | Q" << precio
         << " | Q" << subtotal << '\n';
    cout << "Subtotal: Q" << subtotal << '\n';
    cout << "IVA (12%): Q" << iva << '\n';
    cout << "TOTAL A PAGAR: Q" << factura.total << '\n';

    // No anunciamos exito si falla alguno de los archivos.
    if (!guardarFactura(factura)) {
        cout << "No se pudo guardar la factura. No se descontó stock.\n";
        return;
    }
    if (!bajarStock(codigo, cantidad)) {
        cout << "La factura se guardó, pero NO se pudo descontar el stock.\n";
        cout << "Revise el inventario antes de registrar otra venta.\n";
        return;
    }

    cout << "\nVenta procesada con éxito.\n";
    cout << "Factura guardada y stock actualizado.\n";
}

static void verHistorial() {
    ifstream archivo(ARCHIVO_FACTURAS);
    string linea;
    int total = 0;

    cout << "HISTORIAL DE FACTURAS\n";
    cout << "---------------------------------------------------\n";
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de facturas. Puede que aún no exista.\n";
        return;
    }

    cout << "NO. | NIT CLIENTE | TOTAL PAGADO | FECHA\n";
    while (getline(archivo, linea)) {
        if (!linea.empty()) {
            cout << campo(linea, 0) << " | " << campo(linea, 1) << " | Q"
                 << campo(linea, 2) << " | " << campo(linea, 3) << '\n';
            total++;
        }
    }

    if (total == 0) {
        cout << "Aún no hay facturas registradas.\n";
    } else {
        cout << "Total: " << total << " factura(s).\n";
    }
}
