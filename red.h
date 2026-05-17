#ifndef RED_H
#define RED_H

#include <string>
#include <map>
#include <list>
#include <vector>
#include <utility>
#include "Enrutador.h"
using namespace std;


class Red {
public:
    // --- Gestión de enrutadores ---
    void agregarEnrutador(const string& id);
    void eliminarEnrutador(const string& id);
    bool existeEnrutador(const string& id) const;

    // --- Gestión de enlaces (bidireccionales) ---
    bool agregarEnlace(const string& origen, const string& destino, int costo);
    bool eliminarEnlace(const string& origen, const string& destino);

    // --- Cálculo de rutas ---
    // Recalcula todas las tablas de enrutamiento usando Dijkstra para cada nodo
    void calcularRutas();

    // Devuelve el camino entre origen y destino como list<string>
    // Retorna lista vacía si no existe ruta
    list<string> obtenerCamino(const string& origen,
                                         const string& destino) const;

    // Devuelve el costo del camino entre origen y destino (-1 si no existe)
    int obtenerCosto(const string& origen, const string& destino) const;


    bool cargarDesdeArchivo(const string& archivo);

    // --- Impresión ---
    void imprimirTopologia() const;
    void imprimirTablaGlobal() const;          // Tabla completa de costos (todos vs todos)
    void imprimirCamino(const string& origen,
                        const string& destino) const;
    void listarEnrutadores() const;

    // Número de enrutadores en la red
    int numEnrutadores() const;

private:
    // Contenedor principal: nombre → Enrutador
    map<string, Enrutador> enrutadores_;

    // Ejecuta Dijkstra desde un nodo origen y actualiza su tabla
    void dijkstra(const string& origen);
};

#endif // RED_H
