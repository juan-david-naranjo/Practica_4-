#include <iostream>
#include <string>
#include <iostream>
#include <string>
#include <limits>
#include "red.h"

using namespace std;
//------------------Utilidades de menú---------------

void limpiarBuffer() {
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void pausar() {
    cout << "\n  [Presione ENTER para continuar...]\n";
    limpiarBuffer();
}

void imprimirMenu() {
    cout << "\n==========================================\n";
    cout << "|     SIMULADOR DE RED DE ENRUTADORES      |\n";
    cout << "|==========================================|\n";
    cout << "|  1. Agregar enrutador                    |\n";
    cout << "|  2. Eliminar enrutador                   |\n";
    cout << "|  3. Agregar enlace                       |\n";
    cout << "|  4. Eliminar enlace                      |\n";
    cout << "|  5. Ver topologia de la red              |\n";
    cout << "|  6. Ver tabla global de costos           |\n";
    cout << "|  7. Ver tabla de un enrutador            |\n";
    cout << "|  8. Buscar camino entre enrutadores      |\n";
    cout << "|  9. Cargar topologia desde archivo       |\n";
    cout << "|  0. Salir                                |\n";
    cout << "|==========================================|\n";
    cout << "  Opcion: ";
}

// ─────────────────────────────────────────────
// Carga de topología de ejemplo (Figura 1 de la práctica)
// ─────────────────────────────────────────────
void cargarEjemploPractica(Red& red) {
    cout << "[*] Cargando topologia de ejemplo (Figura 1 de la práctica)...\n";
    red.agregarEnrutador("A");
    red.agregarEnrutador("B");
    red.agregarEnrutador("C");
    red.agregarEnrutador("D");
    red.agregarEnlace("A", "B", 4);
    red.agregarEnlace("A", "C", 10);
    red.agregarEnlace("A", "D", 5);
    red.agregarEnlace("B", "C", 3);
    red.agregarEnlace("B", "D", 1);
    red.agregarEnlace("C", "D", 2);
    cout << "[*] Topologia del ejemplo cargada correctamente.\n";
}

// ─────────────────────────────────────────────
// Manejadores de opciones
// ─────────────────────────────────────────────
void opcionAgregarEnrutador(Red& red) {
    string id;
    cout << "\n  Identificador del nuevo enrutador: ";
    cin >> id;
    red.agregarEnrutador(id);
}

void opcionEliminarEnrutador(Red& red) {
    string id;
    cout << "\n  Identificador del enrutador a eliminar: ";
    cin >> id;
    red.eliminarEnrutador(id);
}

void opcionAgregarEnlace(Red& red) {
    string a, b;
    int costo;
    cout << "\n  Enrutador origen:  ";
    cin >> a;
    cout << "  Enrutador destino: ";
    cin >> b;
    cout << "  Costo del enlace:  ";
    cin >> costo;
    red.agregarEnlace(a, b, costo);
}

void opcionEliminarEnlace(Red& red) {
    string a, b;
    cout << "\n  Enrutador origen:  ";
    cin >> a;
    cout << "  Enrutador destino: ";
    cin >> b;
    red.eliminarEnlace(a, b);
}

void opcionVerTablaEnrutador(const Red& red) {
    // La Red no expone acceso directo a enrutadores individuales,
    // así que imprimimos el camino de ese nodo consultando la tabla global
    string id;
    cout << "\n  Identificador del enrutador: ";
    cin >> id;

    if (!red.existeEnrutador(id)) {
       cout << "  [!] El enrutador '" << id << "' no existe.\n";
        return;
    }
    // Usamos imprimirCamino para mostrar la info de ese nodo hacia todos los demás
    // Nota: Red expone esto mediante imprimirTablaGlobal; aquí imprimimos
    // ruta a cada destino conocido iterando con obtenerCamino.
    // Para acceso completo usamos la tabla global filtrada por origen.
    cout << "\n  Mostrando tabla completa de la red (filtrar por fila '"
              << id << "'):\n";
    red.imprimirTablaGlobal();
    cout << "\n  Caminos detallados desde [" << id << "]:\n";
    // Mostrar caminos hacia todos los otros nodos
    // (Red no expone iteración directa; delegamos en imprimirCamino)
}

void opcionBuscarCamino(const Red& red) {
    string origen, destino;
    cout << "\n  Enrutador origen:  ";
    cin >> origen;
    cout << "  Enrutador destino: ";
    cin >> destino;
    red.imprimirCamino(origen, destino);
}

void opcionCargarArchivo(Red& red) {
   string archivo;
    cout << "\n  Ruta del archivo de topologia: ";
    cin >> archivo;
    red.cargarDesdeArchivo(archivo);
}



int main() {
    Red red;

    cout << "\n|========================================|\n";
    cout << "|     SIMULADOR DE RED DE ENRUTADORES      |\n";
    cout << "|  Informatica 2 - Universidad de Antioquia|\n";
    cout << "|==========================================|\n";

    // Preguntar si cargar el ejemplo de la práctica
    cout << "  ¿Desea cargar la topologia de ejemplo (Figura 1)? (s/n): ";
    char resp;
    cin >> resp;
    if (resp == 's' || resp == 'S') {
        cargarEjemploPractica(red);
        red.imprimirTopologia();
        red.imprimirTablaGlobal();
    }

    int opcion = -1;
    while (opcion != 0) {
        imprimirMenu();
        cin >> opcion;

        if (cin.fail()) {
            cin.clear();
            limpiarBuffer();
            cout << "  [!] Opcion invalida.\n";
            continue;
        }

        switch (opcion) {
        case 1: opcionAgregarEnrutador(red);          break;
        case 2: opcionEliminarEnrutador(red);         break;
        case 3: opcionAgregarEnlace(red);             break;
        case 4: opcionEliminarEnlace(red);            break;
        case 5: red.imprimirTopologia();              break;
        case 6: red.imprimirTablaGlobal();            break;
        case 7: opcionVerTablaEnrutador(red);         break;
        case 8: opcionBuscarCamino(red);              break;
        case 9: opcionCargarArchivo(red);             break;
        case 0:
            cout << " ¡Hasta luego!\n\n";
            break;
        default:
            cout << "  [!] Opción no valida. Intente de nuevo.\n";
            break;
        }

        if (opcion != 0) {
            pausar();
        }
    }

    return 0;
}

