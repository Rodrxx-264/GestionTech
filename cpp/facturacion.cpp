// Este modulo registra ventas y muestra las facturas guardadas.
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib> // atoi y atof convierten numeros guardados como texto
#include <ctime>   // fecha actual de la venta
#include <sstream>
#include <limits>
#include <cstdio>  // rename cambia el archivo temporal por el inventario
using namespace std;

// Funciones compartidas que estan en main.cpp.
void limpiarPantalla();
void pausa();

const string ARCHIVO_FACTURAS = "../txt/facturas.txt";

// Guardamos juntos los datos que se necesitan al registrar una venta.
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

void facturacion() {
    int opcion;

    do {
        limpiarPantalla();
        cout << "===================================================\n";
        cout << "          MÓDULO DE FACTURACIÓN Y VENTAS\n";
        cout << "===================================================\n";
        cout << "1. Registrar Nueva Venta\n";
        cout << "2. Ver Historial de Facturas\n";
        cout << "3. Resumen de ventas por mes\n";
        cout << "4. Volver al Menú Principal\n";
        cout << "Seleccione una opción [1-4]: ";
        if (!(cin >> opcion)) {
            if (cin.eof()) return;
            cin.clear();
            opcion = -1;
        }
        // Descartamos el Enter para que getline no se salte el NIT.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        limpiarPantalla();
        switch (opcion) {
            case 1: {
                // Venta: cliente -> producto -> calculos -> inventario -> factura.
                Factura factura;
                Cliente cliente;
                string codigo, nombreProducto, linea;
                float precio;
                int stock;

                // Primero buscamos al cliente o lo registramos.
                cout << "REGISTRAR NUEVA VENTA (1/3)\n";
                cout << "Ingrese NIT o CUI del cliente: ";
                getline(cin, cliente.nit);
                if (cliente.nit.empty() || cliente.nit.find(',') != string::npos) {
                    cout << "Escriba un NIT o CUI sin comas.\n";
                    break;
                }
                factura.nitCliente = cliente.nit;
                // Buscamos el NIT al principio de cada registro del archivo.
                ifstream consultaClientes("../txt/clientes.txt");
                while (getline(consultaClientes, linea)) {
                    if (linea.substr(0, linea.find(',')) == cliente.nit) {
                        stringstream datos(linea);
                        datos.ignore(numeric_limits<streamsize>::max(), ',');
                        getline(datos, cliente.nombre, ',');
                        break;
                    }
                }
                consultaClientes.close();
                if (!cliente.nombre.empty()) {
                    cout << "Cliente encontrado: " << cliente.nombre << '\n';
                } else {
                    cout << "El cliente no está registrado. Vamos a ingresar sus datos.\n";
                    cout << "Nombre completo: ";
                    getline(cin, cliente.nombre);
                    cout << "Teléfono: ";
                    getline(cin, cliente.telefono);
                    if (cliente.nombre.empty() || cliente.telefono.empty() ||
                        cliente.nombre.find(',') != string::npos ||
                        cliente.telefono.find(',') != string::npos) {
                        cout << "Complete los datos sin usar comas.\n";
                        break;
                    }
                    ofstream nuevosClientes("../txt/clientes.txt", ios::app);
                    if (nuevosClientes.is_open()) {
                        nuevosClientes << cliente.nit << ',' << cliente.nombre << ','
                                      << cliente.telefono << '\n';
                        nuevosClientes.close();
                    }
                    if (nuevosClientes.fail()) {
                        cout << "No se pudo guardar el cliente. Se cancela la venta.\n";
                        break;
                    }
                    cout << "Cliente registrado y guardado.\n";
                }

                pausa();
                limpiarPantalla();
                // Buscamos el producto por codigo y comprobamos el stock.
                cout << "REGISTRAR NUEVA VENTA (2/3)\n";
                cout << "Ingrese el código del producto: ";
                getline(cin, codigo);
                ifstream consultaProductos("../txt/productos.txt");
                bool existeProducto = false;
                while (getline(consultaProductos, linea)) {
                    if (linea.substr(0, linea.find(',')) == codigo) {
                        existeProducto = true;
                        break;
                    }
                }
                consultaProductos.close();
                if (!existeProducto) {
                    cout << "El producto no existe.\n";
                    break;
                }
                factura.codigoProducto = codigo;
                string precioTexto, stockTexto;
                stringstream datosProducto(linea);
                datosProducto.ignore(numeric_limits<streamsize>::max(), ',');
                getline(datosProducto, nombreProducto, ',');
                datosProducto.ignore(numeric_limits<streamsize>::max(), ',');
                getline(datosProducto, precioTexto, ',');
                getline(datosProducto, stockTexto);
                precio = atof(precioTexto.c_str());
                stock = atoi(stockTexto.c_str());
                cout << fixed << setprecision(2);
                cout << "Producto encontrado: " << nombreProducto << '\n';
                cout << "Precio unitario: Q" << precio << '\n';
                cout << "Stock disponible: " << stock << '\n';
                cout << "Ingrese la cantidad a comprar: ";
                if (!(cin >> factura.cantidad)) {
                    if (cin.eof()) return;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Cantidad inválida.\n";
                    break;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (factura.cantidad <= 0) {
                    cout << "La cantidad debe ser mayor que cero.\n";
                    break;
                }
                if (factura.cantidad > stock) {
                    cout << "No hay suficiente stock. Solo hay " << stock << " unidades.\n";
                    break;
                }

                pausa();
                if (cin.eof()) {
                    break;
                }
                limpiarPantalla();
                // Buscamos el número más grande para no repetir una factura.
                int ultimoNumero = 0;
                ifstream facturasAnteriores(ARCHIVO_FACTURAS);
                while (getline(facturasAnteriores, linea)) {
                    int numero = atoi(linea.c_str()); // el numero es el primer campo
                    if (numero > ultimoNumero) ultimoNumero = numero;
                }
                facturasAnteriores.close();
                factura.numeroFactura = ultimoNumero + 1;
                string numeroFactura = to_string(factura.numeroFactura);
                // El numero 3 se muestra como 0003.
                while (numeroFactura.length() < 4) numeroFactura = "0" + numeroFactura;
                // El IVA del 12 % se suma al precio de los productos.
                factura.subtotal = precio * factura.cantidad;
                factura.iva = factura.subtotal * 0.12f;
                factura.totalPagar = factura.subtotal + factura.iva;
                time_t reloj = time(0);
                tm* hoy = localtime(&reloj); // puntero al registro con la fecha actual
                factura.fecha = to_string(hoy->tm_mday) + "/" + to_string(hoy->tm_mon + 1)
                                + "/" + to_string(hoy->tm_year + 1900);

                cout << "FACTURA ELECTRÓNICA GENERADA (3/3)\n";
                cout << "No. Factura: " << numeroFactura << '\n';
                cout << "Cliente: " << cliente.nombre << " (NIT/CUI: " << cliente.nit << ")\n";
                cout << "Fecha: " << factura.fecha << '\n';
                cout << "DESCRIPCIÓN | CANTIDAD | PRECIO | SUBTOTAL\n";
                cout << nombreProducto << " | " << factura.cantidad << " | Q" << precio
                     << " | Q" << factura.subtotal << '\n';
                cout << "Subtotal: Q" << factura.subtotal << '\n';
                cout << "IVA (12%): Q" << factura.iva << '\n';
                cout << "TOTAL A PAGAR: Q" << factura.totalPagar << '\n';

                // Preparamos una nueva version del inventario con el stock menor.
                // Conservamos el texto original por si no se puede guardar la factura.
                string inventarioAnterior, inventarioNuevo;
                ifstream productosActuales("../txt/productos.txt");
                if (!productosActuales.is_open()) {
                    cout << "No se pudo abrir el inventario.\n";
                    break;
                }
                while (getline(productosActuales, linea)) {
                    inventarioAnterior += linea + '\n';
                    if (linea.substr(0, linea.find(',')) == codigo) {
                        size_t ultimaComa = linea.rfind(',');
                        inventarioNuevo += linea.substr(0, ultimaComa + 1)
                                         + to_string(stock - factura.cantidad) + '\n';
                    } else inventarioNuevo += linea + '\n';
                }
                bool lecturaCorrecta = !productosActuales.bad();
                productosActuales.close();
                const string temporal = "../txt/productos_temporal.txt";
                ifstream temporalExistente(temporal);
                // No sobrescribimos un temporal anterior, que puede contener datos.
                if (!lecturaCorrecta || temporalExistente.is_open()) {
                    cout << "No se pudo preparar el inventario (revise el archivo temporal).\n";
                    break;
                }
                ofstream nuevoInventario(temporal);
                if (nuevoInventario.is_open()) {
                    nuevoInventario << inventarioNuevo;
                    nuevoInventario.close();
                }
                if (nuevoInventario.fail()) {
                    cout << "No se pudo escribir el inventario.\n";
                    remove(temporal.c_str());
                    break;
                }
                #ifdef _WIN32
                    // Windows no permite renombrar encima del archivo original.
                    const string respaldo = "../txt/productos.txt.respaldo";
                    ifstream respaldoExistente(respaldo);
                    if (respaldoExistente.is_open() || rename("../txt/productos.txt", respaldo.c_str()) != 0) {
                        remove(temporal.c_str());
                        cout << "No se pudo respaldar el inventario.\n";
                        break;
                    }
                #endif
                // Cambiamos el inventario solo despues de preparar todo el texto.
                if (rename(temporal.c_str(), "../txt/productos.txt") != 0) {
                    #ifdef _WIN32
                        rename(respaldo.c_str(), "../txt/productos.txt");
                    #endif
                    remove(temporal.c_str());
                    cout << "No se pudo descontar el stock.\n";
                    break;
                }
                // La factura se guarda solo si ya se pudo descontar el stock.
                ofstream archivoFactura(ARCHIVO_FACTURAS, ios::app);
                if (archivoFactura.is_open()) {
                    archivoFactura << numeroFactura << ',' << factura.nitCliente << ','
                                   << factura.codigoProducto << ',' << factura.cantidad << ','
                                   << fixed << setprecision(2) << factura.subtotal << ','
                                   << factura.iva << ',' << factura.totalPagar << ','
                                   << factura.fecha << '\n';
                    archivoFactura.close();
                }
                if (archivoFactura.fail()) {
                    // Si falla el guardado, restauramos el inventario anterior.
                    bool restaurado = false;
                    #ifdef _WIN32
                        // Windows ya tiene una copia del inventario como respaldo.
                        if (remove("../txt/productos.txt") == 0) {
                            restaurado = rename(respaldo.c_str(), "../txt/productos.txt") == 0;
                        }
                    #else
                        ofstream restaurar(temporal);
                        if (restaurar.is_open()) {
                            restaurar << inventarioAnterior;
                            restaurar.close();
                        }
                        if (!restaurar.fail()) {
                            restaurado = rename(temporal.c_str(), "../txt/productos.txt") == 0;
                        }
                    #endif
                    if (restaurado) cout << "No se pudo guardar la factura. Se restauró el stock.\n";
                    else cout << "No se pudo guardar la factura ni restaurar el stock. Revise el inventario.\n";
                    break;
                }
                #ifdef _WIN32
                    remove(respaldo.c_str());
                #endif
                cout << "Venta procesada con éxito.\n";
                cout << "Factura guardada y stock actualizado.\n";
                break;
            }
            case 2: {
                // Historial: mostramos las facturas guardadas, sin modificarlas.
                ifstream archivo(ARCHIVO_FACTURAS);
                string linea;
                int total = 0;
                cout << "HISTORIAL DE FACTURAS\n";
                if (!archivo.is_open()) {
                    cout << "No se pudo abrir el archivo de facturas.\n";
                    break;
                }
                cout << "NO. | NIT CLIENTE | TOTAL PAGADO | FECHA\n";
                while (getline(archivo, linea)) {
                    if (!linea.empty()) {
                        string numero, nit, ignorado, importe, fecha;
                        stringstream datos(linea);
                        getline(datos, numero, ',');
                        getline(datos, nit, ',');
                        // Saltamos codigo, cantidad, subtotal e IVA.
                        for (int i = 0; i < 4; i++) getline(datos, ignorado, ',');
                        getline(datos, importe, ',');
                        getline(datos, fecha);
                        cout << numero << " | " << nit << " | Q" << importe
                             << " | " << fecha << '\n';
                        total++;
                    }
                }
                cout << "Total de ventas registradas: " << total << '\n';
                break;
            }
            case 3: {
                // Resumen: contamos facturas y unidades de cada mes.
                // Arreglo: los nombres de los 12 meses.
                string meses[12] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
                                    "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
                // Matriz: cada fila es un mes; columna 0 = ventas, columna 1 = unidades.
                int resumen[12][2] = {};
                ifstream archivo(ARCHIVO_FACTURAS);
                string linea;
                int total = 0;

                cout << "RESUMEN DE VENTAS POR MES (TODOS LOS AÑOS)\n";
                if (!archivo.is_open()) {
                    cout << "No se pudo abrir el archivo de facturas.\n";
                    break;
                }
                while (getline(archivo, linea)) {
                    string ignorado, cantidadTexto, fecha;
                    stringstream datos(linea);
                    // Los primeros tres datos son numero, NIT y codigo.
                    for (int i = 0; i < 3; i++) getline(datos, ignorado, ',');
                    getline(datos, cantidadTexto, ',');
                    // Despues vienen subtotal, IVA y total.
                    for (int i = 0; i < 3; i++) getline(datos, ignorado, ',');
                    getline(datos, fecha); // formato: dia/mes/año
                    size_t primeraBarra = fecha.find('/');
                    size_t segundaBarra = fecha.find('/', primeraBarra == string::npos ? 0 : primeraBarra + 1);
                    if (primeraBarra == string::npos || segundaBarra == string::npos) {
                        continue; // ignoramos filas sin una fecha completa
                    }
                    int mes = atoi(fecha.substr(primeraBarra + 1, segundaBarra - primeraBarra - 1).c_str());
                    int cantidad = atoi(cantidadTexto.c_str());
                    if (mes >= 1 && mes <= 12 && cantidad > 0) {
                        // El mes del archivo empieza en 1, el arreglo empieza en 0.
                        resumen[mes - 1][0]++;
                        resumen[mes - 1][1] += cantidad;
                        total++;
                    }
                }
                for (int i = 0; i < 12; i++) {
                    if (resumen[i][0] > 0) {
                        cout << meses[i] << ": " << resumen[i][0] << " ventas, "
                             << resumen[i][1] << " unidades\n";
                    }
                }
                if (total == 0) {
                    cout << "Aún no hay ventas con fecha válida.\n";
                }
                break;
            }
            case 4:
                cout << "Regresando al menú principal...\n";
                break;
            default:
                cout << "Opción inválida. Intente de nuevo.\n";
                break;
        }

        // La pausa permite ver la factura o el historial antes de regresar.
        if (opcion != 4) {
            pausa();
        }
    } while (opcion != 4);
}
