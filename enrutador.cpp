#include "enrutador.h"
#include <iostream>
#include <iomanip>

// Inicialización del miembro estático
const list<string> Enrutador::CAMINO_VACIO = {};


// Constructor

Enrutador::Enrutador(const string& id) : id_(id) {}


// Getters

const string& Enrutador::getId() const {
    return id_;
}

const map<string, int>& Enrutador::getVecinos() const {
    return vecinos_;
}

const map<string, int>& Enrutador::getTablaCostos() const {
    return tablaCostos_;
}

const list<string>& Enrutador::getCamino(const string& destino) const {
    auto it = tablaCaminos_.find(destino);
    if (it != tablaCaminos_.end()) {
        return it->second;
    }
    return CAMINO_VACIO;
}

const map<string, list<string>>& Enrutador::getTablaCaminos() const {
    return tablaCaminos_;
}


// Modificadores de topología

void Enrutador::agregarVecino(const string& id, int costo) {
    vecinos_[id] = costo;
}

void Enrutador::eliminarVecino(const string& id) {
    vecinos_.erase(id);
}

bool Enrutador::tieneVecino(const string& id) const {
    return vecinos_.find(id) != vecinos_.end();
}

// -----------------Actualización de tabla de enrutamiento------------

void Enrutador::setCostoYCamino(const string& destino, int costo,
                                const list<string>& camino) {
    tablaCostos_[destino] = costo;
    tablaCaminos_[destino] = camino;
}

void Enrutador::limpiarTabla() {
    tablaCostos_.clear();
    tablaCaminos_.clear();
}

// ---------------Impresión------------------

void Enrutador::imprimirVecinos() const {
    cout << "  Vecinos directos de [" << id_ << "]:\n";
    if (vecinos_.empty()) {
        cout << "    (ninguno)\n";
        return;
    }
    for (const auto& par : vecinos_) {
        cout << "    -> " << par.first << "  (costo: " << par.second << ")\n";
    }
}

void Enrutador::imprimirTabla() const {
    cout << "\n  Tabla de enrutamiento de [" << id_ << "]:\n";
    cout << "  " << string(45, '-') << "\n";
    cout << "  " << left << setw(10) << "Destino"
         << setw(8)  << "Costo"
         << "Camino\n";
    cout << "  " << string(45, '-') << "\n";

    for (const auto& par : tablaCostos_) {
        const string& dest = par.first;
        int costo = par.second;

        cout << "  " << left << setw(10) << dest;
        if (costo == INF) {
            cout << setw(8) << "INF";
        } else {
            cout << setw(8) << costo;
        }

        // Imprimir camino
        auto it = tablaCaminos_.find(dest);
        if (it != tablaCaminos_.end() && !it->second.empty()) {
            bool primero = true;
            for (const auto& nodo : it->second) {
                if (!primero) cout << " -> ";
                cout << nodo;
                primero = false;
            }
        } else {
            cout << "(sin ruta)";
        }
        cout << "\n";
    }
    cout << "  " << string(45, '-') << "\n";
}


