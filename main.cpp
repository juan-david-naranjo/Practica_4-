#include <iostream>
#include <string>
#include <limits>
#include "red.h"

// ─────────────────────────────────────────────
// Utilidades de menú
// ─────────────────────────────────────────────
void limpiarBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void pausar() {
    std::cout << "\n  [Presione ENTER para continuar...]\n";
    limpiarBuffer();
}

void imprimirMenu() {
    std::cout << "\n==========================================\n";
    std::cout << "|     SIMULADOR DE RED DE ENRUTADORES      |\n";
    std::cout << "|==========================================|\n";
    std::cout << "|  1. Agregar enrutador                    |\n";
    std::cout << "|  2. Eliminar enrutador                   |\n";
    std::cout << "|  3. Agregar enlace                       |\n";
    std::cout << "|  4. Eliminar enlace                      |\n";
    std::cout << "|  5. Ver topología de la red              |\n";
    std::cout << "|  6. Ver tabla global de costos           |\n";
    std::cout << "|  7. Ver tabla de un enrutador            |\n";
    std::cout << "|  8. Buscar camino entre enrutadores      |\n";
    std::cout << "|  9. Cargar topología desde archivo       |\n";
    std::cout << "|  0. Salir                                |\n";
    std::cout << "|==========================================|\n";
    std::cout << "  Opción: ";
}

// ─────────────────────────────────────────────
// Carga de topología de ejemplo (Figura 1 de la práctica)
// ─────────────────────────────────────────────
void cargarEjemploPractica(Red& red) {
    std::cout << "\n[*] Cargando topología de ejemplo (Figura 1 de la práctica)...\n";
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
    std::cout << "[*] Topología del ejemplo cargada correctamente.\n";
}

// ─────────────────────────────────────────────
// Manejadores de opciones
// ─────────────────────────────────────────────
void opcionAgregarEnrutador(Red& red) {
    std::string id;
    std::cout << "\n  Identificador del nuevo enrutador: ";
    std::cin >> id;
    red.agregarEnrutador(id);
}

void opcionEliminarEnrutador(Red& red) {
    std::string id;
    std::cout << "\n  Identificador del enrutador a eliminar: ";
    std::cin >> id;
    red.eliminarEnrutador(id);
}

void opcionAgregarEnlace(Red& red) {
    std::string a, b;
    int costo;
    std::cout << "\n  Enrutador origen:  ";
    std::cin >> a;
    std::cout << "  Enrutador destino: ";
    std::cin >> b;
    std::cout << "  Costo del enlace:  ";
    std::cin >> costo;
    red.agregarEnlace(a, b, costo);
}

void opcionEliminarEnlace(Red& red) {
    std::string a, b;
    std::cout << "\n  Enrutador origen:  ";
    std::cin >> a;
    std::cout << "  Enrutador destino: ";
    std::cin >> b;
    red.eliminarEnlace(a, b);
}

void opcionVerTablaEnrutador(const Red& red) {
    // La Red no expone acceso directo a enrutadores individuales,
    // así que imprimimos el camino de ese nodo consultando la tabla global
    std::string id;
    std::cout << "\n  Identificador del enrutador: ";
    std::cin >> id;

    if (!red.existeEnrutador(id)) {
        std::cout << "  [!] El enrutador '" << id << "' no existe.\n";
        return;
    }
    // Usamos imprimirCamino para mostrar la info de ese nodo hacia todos los demás
    // Nota: Red expone esto mediante imprimirTablaGlobal; aquí imprimimos
    // ruta a cada destino conocido iterando con obtenerCamino.
    // Para acceso completo usamos la tabla global filtrada por origen.
    std::cout << "\n  Mostrando tabla completa de la red (filtrar por fila '"
              << id << "'):\n";
    red.imprimirTablaGlobal();
    std::cout << "\n  Caminos detallados desde [" << id << "]:\n";
    // Mostrar caminos hacia todos los otros nodos
    // (Red no expone iteración directa; delegamos en imprimirCamino)
}

void opcionBuscarCamino(const Red& red) {
    std::string origen, destino;
    std::cout << "\n  Enrutador origen:  ";
    std::cin >> origen;
    std::cout << "  Enrutador destino: ";
    std::cin >> destino;
    red.imprimirCamino(origen, destino);
}

void opcionCargarArchivo(Red& red) {
    std::string archivo;
    std::cout << "\n  Ruta del archivo de topología: ";
    std::cin >> archivo;
    red.cargarDesdeArchivo(archivo);
}

// ─────────────────────────────────────────────
// Main
// ─────────────────────────────────────────────
int main() {
    Red red;

    std::cout << "\n|========================================|\n";
    std::cout << "|     SIMULADOR DE RED DE ENRUTADORES      |\n";
    std::cout << "|  Informática 2 - Universidad de Antioquia|\n";
    std::cout << "|==========================================|\n";

    // Preguntar si cargar el ejemplo de la práctica
    std::cout << "\n  ¿Desea cargar la topología de ejemplo (Figura 1)? (s/n): ";
    char resp;
    std::cin >> resp;
    if (resp == 's' || resp == 'S') {
        cargarEjemploPractica(red);
        red.imprimirTopologia();
        red.imprimirTablaGlobal();
    }

    int opcion = -1;
    while (opcion != 0) {
        imprimirMenu();
        std::cin >> opcion;

        if (std::cin.fail()) {
            std::cin.clear();
            limpiarBuffer();
            std::cout << "  [!] Opción inválida.\n";
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
            std::cout << "\n  ¡Hasta luego!\n\n";
            break;
        default:
            std::cout << "  [!] Opción no válida. Intente de nuevo.\n";
            break;
        }

        if (opcion != 0) {
            pausar();
        }
    }

    return 0;
}

