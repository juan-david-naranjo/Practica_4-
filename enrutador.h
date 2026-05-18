#ifndef ENRUTADOR_H
#define ENRUTADOR_H

#include <string>
#include <map>
#include <list>
#include <vector>
#include <limits>
#include "Enlace.h"


using namespace std;
// Constante para representar "infinito" (sin ruta conocida)
const int INF = numeric_limits<int>::max();

/*
 * Clase Enrutador
 * ---------------
 * Modela un nodo de red (router). Almacena:
 *  - Su identificador (nombre)
 *  - Sus vecinos directos y el costo de cada enlace  →  map<string, int>
 *    (map elegido porque permite O(log n) en búsqueda por nombre y mantiene
 *     orden; útil para imprimir tablas ordenadas alfabéticamente)
 *  - Su tabla de costos mínimos hacia todos los destinos  →  map<string, int>
 *  - La ruta completa hacia cada destino  →  map<string, list<string>>
 *    (list elegida para la ruta porque las inserciones al frente son O(1)
 *     durante la reconstrucción del camino en Dijkstra)
 */
class Enrutador {
public:
    // --- Constructor ---
    explicit Enrutador(const string& id);

    // --- Getters ---
    const string& getId() const;

    // Vecinos directamente conectados: nombre → costo
    const map<string, int>& getVecinos() const;

    // Tabla de costos mínimos hacia cada destino
    const map<string, int>& getTablaCostos() const;

    // Camino completo hacia un destino (vacío si no existe ruta)
    const list<string>& getCamino(const string& destino) const;

    // Mapa completo de caminos (para iterar)
    const map<string, list<string>>& getTablaCaminos() const;



private:
    string id_;                                        // Identificador único
    map<string, int> vecinos_;                    // Vecinos directos y sus costos
    map<string, int> tablaCostos_;                // Costo mínimo a cada destino
    map<string, list<string>> tablaCaminos_; // Camino completo a cada destino

    // Camino vacío para retornar por referencia cuando no existe ruta
    static const list<string> CAMINO_VACIO;
};

#endif // ENRUTADOR_H

