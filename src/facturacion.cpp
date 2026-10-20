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

// Es la misma estructura que usamos en clientes.cpp.
// Como no usamos headers, los tres campos deben mantenerse iguales en ambos.
struct Cliente {
    string nit;
    string nombre;
    string telefono;
};

struct Factura {
    int numeroFactura;
    string nitCliente;
    string codigoProducto;
    int cantidad;
    float subtotal;
    float iva;
    float totalPagar;
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
    // Guardamos el detalle completo, no solo el total de la venta.
    archivo << numeroConCeros(factura.numeroFactura) << ',' << factura.nitCliente << ','
            << factura.codigoProducto << ',' << factura.cantidad << ','
            << fixed << setprecision(2) << factura.subtotal << ',' << factura.iva << ','
            << factura.totalPagar << ',' << factura.fecha << '\n';
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
    Cliente cliente;
    string codigo, nombreProducto, linea;
    float precio;
    int stock;

    // Paso 1: buscar al cliente o registrarlo si es nuevo.
    cout << "REGISTRAR NUEVA VENTA (1/3)\n";
    cout << "---------------------------------------------------\n";
    cout << "Ingrese NIT o CUI del cliente: ";
    getline(cin, cliente.nit);
    if (cliente.nit.empty() || cliente.nit.find(',') != string::npos) {
        cout << "Escriba un NIT o CUI que no esté vacío ni tenga comas.\n";
        return;
    }
    factura.nitCliente = cliente.nit;

    linea = buscarRegistro("../data/clientes.txt", cliente.nit);
    if (!linea.empty()) {
        cliente.nombre = campo(linea, 1);
        cliente.telefono = campo(linea, 2);
        cout << "Cliente encontrado: " << cliente.nombre << '\n';
    } else {
        cout << "El cliente no está registrado. Vamos a ingresar sus datos.\n";
        cout << "Nombre completo: ";
        getline(cin, cliente.nombre);
        cout << "Teléfono: ";
        getline(cin, cliente.telefono);

        if (cliente.nombre.empty() || cliente.telefono.empty() ||
            cliente.nombre.find(',') != string::npos || cliente.telefono.find(',') != string::npos) {
            cout << "Complete los datos sin usar comas.\n";
            return;
        }
        if (!guardarCliente(cliente.nit, cliente.nombre, cliente.telefono)) {
            cout << "No se pudo guardar el cliente. Se cancela la venta.\n";
            return;
        }
        cout << "Cliente registrado y guardado.\n";
    }

    pausa();
    limpiarPantalla();

    // Paso 2: buscar el producto y revisar que alcance el stock.
    cout << "REGISTRAR NUEVA VENTA (2/3)\n";
    cout << "---------------------------------------------------\n";
    cout << "Ingrese el código del producto: ";
    getline(cin, codigo);
    linea = buscarRegistro("../data/productos.txt", codigo);
    if (linea.empty()) {
        cout << "El producto no existe.\n";
        return;
    }

    // La categoria ocupa el campo 2: precio y stock ahora son los campos 3 y 4.
    factura.codigoProducto = codigo;
    nombreProducto = campo(linea, 1);
    precio = atof(campo(linea, 3).c_str());
    stock = atoi(campo(linea, 4).c_str());
    cout << fixed << setprecision(2);
    cout << "Producto encontrado: " << nombreProducto << '\n';
    cout << "Precio unitario: Q" << precio << '\n';
    cout << "Stock disponible: " << stock << '\n';
    cout << "Ingrese la cantidad a comprar: ";
    factura.cantidad = leerEntero();
    if (cin.eof()) {
        return;
    }

    if (factura.cantidad <= 0) {
        cout << "La cantidad debe ser mayor que cero.\n";
        return;
    }
    if (factura.cantidad > stock) {
        cout << "No hay suficiente stock. Solo hay " << stock << " unidades.\n";
        return;
    }

    pausa(); // el diseño deja revisar el producto antes de mostrar la factura
    if (cin.eof()) {
        return; // no terminamos la venta si se cierra la entrada
    }
    limpiarPantalla();

    // Paso 3: los calculos quedan dentro de Factura para mostrarlos y guardarlos.
    factura.numeroFactura = siguienteNumero();
    factura.subtotal = precio * factura.cantidad;
    factura.iva = factura.subtotal * 0.12f;
    factura.totalPagar = factura.subtotal + factura.iva;

    // localtime nos da el dia, mes y año de la computadora.
    time_t reloj = time(0);
    tm* hoy = localtime(&reloj);
    factura.fecha = to_string(hoy->tm_mday) + "/" + to_string(hoy->tm_mon + 1)
                    + "/" + to_string(hoy->tm_year + 1900);

    cout << "\nFACTURA ELECTRÓNICA GENERADA (3/3)\n";
    cout << "---------------------------------------------------\n";
    cout << "No. Factura: " << numeroConCeros(factura.numeroFactura) << '\n';
    cout << "Cliente: " << cliente.nombre << " (NIT/CUI: " << cliente.nit << ")\n";
    cout << "Fecha: " << factura.fecha << '\n';
    cout << "DESCRIPCIÓN | CANTIDAD | PRECIO | SUBTOTAL\n";
    cout << nombreProducto << " | " << factura.cantidad << " | Q" << precio
         << " | Q" << factura.subtotal << '\n';
    cout << "Subtotal: Q" << factura.subtotal << '\n';
    cout << "IVA (12%): Q" << factura.iva << '\n';
    cout << "TOTAL A PAGAR: Q" << factura.totalPagar << '\n';

    // No anunciamos exito si falla alguno de los archivos.
    if (!guardarFactura(factura)) {
        cout << "No se pudo guardar la factura. No se descontó stock.\n";
        return;
    }
    if (!bajarStock(factura.codigoProducto, factura.cantidad)) {
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
                 << campo(linea, 6) << " | " << campo(linea, 7) << '\n';
            total++;
        }
    }

    if (total == 0) {
        cout << "Aún no hay facturas registradas.\n";
    } else {
        cout << "Total de ventas registradas: " << total << '\n';
    }
}
