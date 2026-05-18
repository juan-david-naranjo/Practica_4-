#include <iostream>
#include <string>
#include <limits>
#include "red.h"

int main() {
    Red red;

    cout << "\n|========================================|\n";
    cout << "|     SIMULADOR DE RED DE ENRUTADORES      |\n";
    cout << "|  Informática 2 - Universidad de Antioquia|\n";
    cout << "|==========================================|\n";

    // Preguntar si cargar el ejemplo de la práctica
    cout << "\n  ¿Desea cargar la topología de ejemplo (Figura 1)? (s/n): ";
    char resp;
    cin >> resp;
    if (resp == 's' || resp == 'S') {
        cargarEjemploPractica(red);
        red.imprimirTopologia();
        red.imprimirTablaGlobal();
    }

    int opcion = -1;
    while (opcion != 0) {
        break;
    }

    return 0;
}

