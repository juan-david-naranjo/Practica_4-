#ifndef ENLACE_H
#define ENLACE_H

#include <string>
using namespace std;

// Representa un enlace (edge) entre dos enrutadores con un costo asociado
struct Enlace {
    string destino; // Identificador del enrutador destino
    int costo;           // Costo del enlace

    Enlace(const string& destino, int costo)
        : destino(destino), costo(costo) {}
};

#endif // ENLACE_H
