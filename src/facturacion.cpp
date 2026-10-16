/* Modulo de Facturacion y Ventas */

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>   // setprecision (para los decimales)
#include <cstdlib>   // atoi, atof, system
#include <cstdio>    // remove, rename
#include <ctime>     // time, localtime (para la fecha)

using namespace std;

// Datos de un producto
struct Producto {
    string codigo;
    string nombre;
    float precio;
    int stock;
};

// Datos de una factura
struct Factura {
    int numero;
    string nit;
    float total;
    string fecha;
};

void registrarVenta();
void verHistorial();

// Saca el campo numero n de una linea con comas (n empieza en 0)
string campo(string linea, int n) {
    int i;
    int inicio = 0;

    for (i = 0; i < n; i++) {
        inicio = linea.find(',', inicio) + 1;
    }

    int fin = linea.find(',', inicio);
    if (fin < 0) {
        fin = linea.length();
    }

    return linea.substr(inicio, fin - inicio);
}

// Resta la cantidad vendida al stock del producto
void bajarStock(string codigo, int cant) {
    ifstream original("../data/productos.txt");
    ofstream temp("../data/temp.txt");
    string linea;
    int stock;

    while (getline(original, linea)) {
        if (linea != "") {
            if (campo(linea, 0) == codigo) {
                stock = atoi(campo(linea, 3).c_str());
                temp << campo(linea, 0) << "," << campo(linea, 1) << ","
                     << campo(linea, 2) << "," << stock - cant << endl;
            } else {
                temp << linea << endl;
            }
        }
    }

    original.close();
    temp.close();

    remove("../data/productos.txt");
    rename("../data/temp.txt", "../data/productos.txt");
}

// Menu principal del modulo de facturacion
void facturacion() {
    int op;

    do {
        system("clear");

        cout << "===================================================\n";
        cout << "        MÓDULO DE FACTURACIÓN Y VENTAS\n";
        cout << "===================================================\n";
        cout << "[1] Registrar Nueva Venta (Facturar)\n";
        cout << "[2] Ver Historial de Facturas\n";
        cout << "[3] Volver al Menú Principal\n";
        cout << "---------------------------------------------------\n";
        cout << "Seleccione una opción [1-3]: ";
        cin >> op;

        if (op == 1) {
            registrarVenta();
        } else if (op == 2) {
            verHistorial();
        } else if (op == 3) {
            cout << "\nRegresando al menú principal...\n";
        } else {
            cout << "\n[!] Opción inválida.\n";
            cout << "Presione Enter para continuar...";
            cin.ignore();
            cin.get();
        }

    } while (op != 3);
}

// Paso a paso para registrar una venta (NIT, producto y factura)
void registrarVenta() {
    Producto p;
    Factura f;
    string linea, nit, cod, nombre, tel, num;
    float subtotal, iva;
    int cant, ultimo;
    bool encontrado;
    time_t reloj;
    tm *hoy;

    // ---------- PASO 1: cliente ----------
    cout << "---------------------------------------------------\n";
    cout << "REGISTRAR NUEVA VENTA (1/3)\n";
    cout << "---------------------------------------------------\n";
    cout << "Ingrese el NIT del cliente: ";
    cin >> nit;
    f.nit = nit;

    // buscamos el cliente en el archivo
    encontrado = false;
    nombre = "";
    ifstream cli("../data/clientes.txt");
    while (getline(cli, linea)) {
        if (linea != "" && campo(linea, 0) == nit) {
            encontrado = true;
            nombre = campo(linea, 1);
        }
    }
    cli.close();

    if (encontrado) {
        cout << "[✓] Cliente encontrado: " << nombre << "\n";
    } else {
        cout << "[!] El NIT ingresado no se encuentra registrado.\n";
        cout << "--- REGISTRO RÁPIDO DE NUEVO CLIENTE ---\n";
        cout << "Ingrese Nombre completo: ";
        cin.ignore();
        getline(cin, nombre);
        cout << "Ingrese Teléfono: ";
        cin >> tel;

        ofstream nuevo("../data/clientes.txt", ios::app);
        nuevo << nit << "," << nombre << "," << tel << endl;
        nuevo.close();

        cout << "[✓] Cliente registrado y guardado en clientes.txt\n";
        cout << "[✓] Cliente asociado a la venta actual con éxito.\n";
    }

    cout << "---------------------------------------------------\n";
    cout << "Presione Enter para continuar con el producto...";
    cin.ignore();
    cin.get();

    // ---------- PASO 2: producto ----------
    cout << "\n---------------------------------------------------\n";
    cout << "REGISTRAR NUEVA VENTA (2/3)\n";
    cout << "---------------------------------------------------\n";
    cout << "Ingrese el Código del Producto: ";
    cin >> cod;

    // buscamos el producto en el archivo
    encontrado = false;
    ifstream pro("../data/productos.txt");
    while (getline(pro, linea)) {
        if (linea != "" && campo(linea, 0) == cod) {
            p.codigo = campo(linea, 0);
            p.nombre = campo(linea, 1);
            p.precio = atof(campo(linea, 2).c_str());
            p.stock  = atoi(campo(linea, 3).c_str());
            encontrado = true;
        }
    }
    pro.close();

    if (encontrado == false) {
        cout << "[!] El producto no existe.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore();
        cin.get();
        return;
    }

    cout << fixed << setprecision(2);
    cout << "[✓] Producto encontrado: " << p.nombre << "\n";
    cout << "Precio Unitario: Q" << p.precio << "\n";
    cout << "Stock Disponible: " << p.stock << " unidades\n";
    cout << "Ingrese la cantidad a comprar: ";
    cin >> cant;

    if (cant <= 0) {
        cout << "[!] Cantidad inválida.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore();
        cin.get();
        return;
    }
    if (cant > p.stock) {
        cout << "[!] No hay suficiente stock. Solo hay: " << p.stock << "\n";
        cout << "Presione Enter para continuar...";
        cin.ignore();
        cin.get();
        return;
    }

    // ---------- PASO 3: factura ----------
    // numero de factura (el mayor + 1)
    ultimo = 0;
    ifstream fac("../data/facturas.txt");
    while (getline(fac, linea)) {
        if (linea != "" && atoi(campo(linea, 0).c_str()) > ultimo) {
            ultimo = atoi(campo(linea, 0).c_str());
        }
    }
    fac.close();
    f.numero = ultimo + 1;

    // numero con ceros a la izquierda (ej: 0001)
    num = to_string(f.numero);
    while (num.length() < 4) {
        num = "0" + num;
    }

    // totales
    subtotal = p.precio * cant;
    iva = subtotal * 0.12;
    f.total = subtotal + iva;

    // fecha de hoy
    reloj = time(0);
    hoy = localtime(&reloj);
    f.fecha = to_string(hoy->tm_mday) + "/" + to_string(hoy->tm_mon + 1)
              + "/" + to_string(hoy->tm_year + 1900);

    // mostrar la factura
    cout << "\n---------------------------------------------------\n";
    cout << "FACTURA ELECTRÓNICA GENERADA\n";
    cout << "---------------------------------------------------\n";
    cout << "No. Factura: " << num << "\n";
    cout << "Cliente: " << nombre << " (NIT: " << f.nit << ")\n";
    cout << "--------------------------------------------------\n";
    cout << "DESCRIPCIÓN | CANT | P. UNIT | SUBTOTAL\n";
    cout << "--------------------------------------------------\n";
    cout << p.nombre << " | " << cant << " | Q" << p.precio << " | Q" << subtotal << "\n";
    cout << "--------------------------------------------------\n";
    cout << "Subtotal: Q" << subtotal << "\n";
    cout << "IVA (12%): Q" << iva << "\n";
    cout << "TOTAL A PAGAR: Q" << f.total << "\n";
    cout << "--------------------------------------------------\n";

    // guardar la factura en el archivo
    ofstream guardar("../data/facturas.txt", ios::app);
    guardar << num << "," << f.nit << ","
            << fixed << setprecision(2) << f.total << "," << f.fecha << endl;
    guardar.close();

    // descontar del inventario
    bajarStock(cod, cant);

    cout << "[✓] Venta procesada con éxito.\n";
    cout << "[✓] Stock actualizado en inventario.\n";
    cout << "[✓] Factura guardada en disco (facturas.txt).\n";
    cout << "---------------------------------------------------\n";
    cout << "Presione Enter para regresar al menú de ventas...";
    cin.ignore();
    cin.get();
}

// Muestra todas las facturas guardadas
void verHistorial() {
    ifstream archivo("../data/facturas.txt");
    string linea;
    int contador = 0;

    cout << "---------------------------------------------------------------------\n";
    cout << "HISTORIAL DE FACTURAS\n";
    cout << "---------------------------------------------------------------------\n";
    cout << "NO. | NIT CLIENTE | TOTAL PAGADO | FECHA/REGISTRO\n";
    cout << "---------------------------------------------------------------------\n";

    while (getline(archivo, linea)) {
        if (linea != "") {
            cout << campo(linea, 0) << " | " << campo(linea, 1) << " | Q"
                 << campo(linea, 2) << " | " << campo(linea, 3) << "\n";
            contador = contador + 1;
        }
    }
    archivo.close();

    cout << "---------------------------------------------------------------------\n";
    cout << "Total de ventas registradas: " << contador << "\n";
    cout << "Presione Enter para continuar...";
    cin.ignore();
    cin.get();
}


