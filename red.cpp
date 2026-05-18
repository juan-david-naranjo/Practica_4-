#include "red.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <queue>
#include <functional>
#include <algorithm>

// --------------------------------------------
// Gestión de enrutadores
// --------------------------------------------
void Red::agregarEnrutador(const string& id) {
    if (enrutadores_.find(id) == enrutadores_.end()) {
        enrutadores_.emplace(id, Enrutador(id));
        cout << "[+] Enrutador '" << id << "' agregado.\n";
        calcularRutas();
    } else {
        cout << "[!] El enrutador '" << id << "' ya existe.\n";
    }
}

void Red::eliminarEnrutador(const string& id) {
    auto it = enrutadores_.find(id);
    if (it == enrutadores_.end()) {
        cout << "[!] El enrutador '" << id << "' no existe.\n";
        return;
    }
    // Eliminar todos los enlaces hacia este enrutador en los demás nodos
    for (auto& par : enrutadores_) {
        par.second.eliminarVecino(id);
    }
    enrutadores_.erase(it);
    cout << "[-] Enrutador '" << id << "' eliminado.\n";
    calcularRutas();
}

bool Red::existeEnrutador(const string& id) const {
    return enrutadores_.find(id) != enrutadores_.end();
}

int Red::numEnrutadores() const {
    return static_cast<int>(enrutadores_.size());
}

// --------------------------------------------
// Gestión de enlaces
// ---------------------------------------------
bool Red::agregarEnlace(const string& origen, const string& destino, int costo) {
    if (!existeEnrutador(origen)) {
        cout << "[!] Enrutador origen '" << origen << "' no existe.\n";
        return false;
    }
    if (!existeEnrutador(destino)) {
        cout << "[!] Enrutador destino '" << destino << "' no existe.\n";
        return false;
    }
    if (costo <= 0) {
        cout << "[!] El costo debe ser mayor que 0.\n";
        return false;
    }
    // Enlace bidireccional
    enrutadores_.at(origen).agregarVecino(destino, costo);
    enrutadores_.at(destino).agregarVecino(origen, costo);
    cout << "[+] Enlace " << origen << " <-> " << destino
              << " (costo: " << costo << ") agregado.\n";
    calcularRutas();
    return true;
}

bool Red::eliminarEnlace(const string& origen, const string& destino) {
    if (!existeEnrutador(origen) || !existeEnrutador(destino)) {
        cout << "[!] Uno o ambos enrutadores no existen.\n";
        return false;
    }
    enrutadores_.at(origen).eliminarVecino(destino);
    enrutadores_.at(destino).eliminarVecino(origen);
    cout << "[-] Enlace " << origen << " <-> " << destino << " eliminado.\n";
    calcularRutas();
    return true;
}

// --------------------------------------------
// Algoritmo de Dijkstra (para hallar el costo)
//----------------------------------------------

void Red::dijkstra(const string& origen) {
    // dist[v] = costo mínimo conocido desde origen hasta v
    map<std::string, int> dist;
    // previo[v] = nodo anterior en el camino mínimo hacia v
    map<string, string> previo;

    // Inicializar distancias a INF
    for (const auto& par : enrutadores_) {
        dist[par.first] = INF;
        previo[par.first] = "";
    }
    dist[origen] = 0;

    // Min-heap: (distancia, nodo)
    using ParDistNodo = pair<int, string>;
    priority_queue<ParDistNodo,
                        vector<ParDistNodo>,
                        greater<ParDistNodo>> cola;
    cola.push({0, origen});

    while (!cola.empty()) {
        auto [d, u] = cola.top();
        cola.pop();

        // Si ya encontramos una ruta mejor, ignorar
        if (d > dist[u]) continue;

        // Relajar vecinos
        for (const auto& vecPar : enrutadores_.at(u).getVecinos()) {
            const string& v = vecPar.first;
            int peso = vecPar.second;

            if (dist[u] != INF && dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                previo[v] = u;
                cola.push({dist[v], v});
            }
        }
    }

    // Actualizar tabla del enrutador origen
    Enrutador& enr = enrutadores_.at(origen);
    enr.limpiarTabla();

    for (const auto& par : enrutadores_) {
        const string& dest = par.first;

        // Reconstruir camino usando list (inserciones al frente O(1))
        list<string> camino;

        if (dest == origen) {
            // Ruta hacia sí mismo
            camino.push_back(origen);
            enr.setCostoYCamino(dest, 0, camino);
            continue;
        }

        if (dist[dest] == INF) {
            // No hay ruta
            enr.setCostoYCamino(dest, INF, camino);
            continue;
        }

        // Reconstruir: ir hacia atrás desde dest hasta origen
        string actual = dest;
        while (!actual.empty()) {
            camino.push_front(actual);
            actual = previo[actual];
        }

        enr.setCostoYCamino(dest, dist[dest], camino);
    }
}

//----------------------------------------------
// Calcular rutas para todos los enrutadores
//----------------------------------------------
void Red::calcularRutas() {
    for (const auto& par : enrutadores_) {
        dijkstra(par.first);
    }
}

// ---------------------------------------------
// Consulta de camino y costo
// ---------------------------------------------
list<string> Red::obtenerCamino(const string& origen,
                                          const string& destino) const {
    auto it = enrutadores_.find(origen);
    if (it == enrutadores_.end()) return {};
    return it->second.getCamino(destino);
}

int Red::obtenerCosto(const string& origen, const string& destino) const {
    auto it = enrutadores_.find(origen);
    if (it == enrutadores_.end()) return -1;
    const auto& tabla = it->second.getTablaCostos();
    auto jt = tabla.find(destino);
    if (jt == tabla.end()) return -1;
    return (jt->second == INF) ? -1 : jt->second;
}

// ---------------------------------------------
// Carga desde archivo
// ---------------------------------------------

bool Red::cargarDesdeArchivo(const string& archivo) {
    ifstream fin(archivo);
    if (!fin.is_open()) {
        cout << "[!] No se pudo abrir el archivo: " << archivo << "\n";
        return false;
    }

    string linea;
    int nLinea = 0;
    while (getline(fin, linea)) {
        ++nLinea;
        // Eliminar espacios al inicio
        size_t inicio = linea.find_first_not_of(" \t\r\n");
        if (inicio == string::npos) continue;
        linea = linea.substr(inicio);

        // Ignorar comentarios y líneas vacías
        if (linea.empty() || linea[0] == '#') continue;

        istringstream ss(linea);
        string comando;
        ss >> comando;

        if (comando == "ROUTER") {
            string id;
            if (ss >> id) {
                if (enrutadores_.find(id) == enrutadores_.end()) {
                    enrutadores_.emplace(id, Enrutador(id));
                }
            } else {
                cout << "[!] Línea " << nLinea << ": ROUTER sin identificador.\n";
            }
        } else if (comando == "LINK") {
            string a, b;
            int costo;
            if (ss >> a >> b >> costo) {
                if (!existeEnrutador(a)) {
                    enrutadores_.emplace(a, Enrutador(a));
                }
                if (!existeEnrutador(b)) {
                    enrutadores_.emplace(b, Enrutador(b));
                }
                enrutadores_.at(a).agregarVecino(b, costo);
                enrutadores_.at(b).agregarVecino(a, costo);
            } else {
                cout << "[!] Línea " << nLinea << ": formato LINK inválido.\n";
            }
        } else {
            cout << "[!] Línea " << nLinea << ": comando desconocido '"
                      << comando << "'.\n";
        }
    }

    fin.close();
    calcularRutas();
    cout << "[+] Topología cargada desde '" << archivo << "'.\n";
    return true;
}

// ---------------------------------------------
// Impresión
// ---------------------------------------------
void Red::listarEnrutadores() const {
    cout << "\n  Enrutadores en la red (" << enrutadores_.size() << "):\n";
    for (const auto& par : enrutadores_) {
        cout << "    - " << par.first << "\n";
    }
}

void Red::imprimirTopologia() const {
    cout << "\n========== TOPOLOGÍA DE LA RED ==========\n";
    listarEnrutadores();
    cout << "\n  Conexiones:\n";
    for (const auto& par : enrutadores_) {
        par.second.imprimirVecinos();
    }
    cout << "=========================================\n";
}

void Red::imprimirTablaGlobal() const {
    if (enrutadores_.empty()) {
        cout << "  La red no tiene enrutadores.\n";
        return;
    }

    // Recopilar lista de nodos ordenada (map ya los mantiene ordenados)
   vector<string> nodos;
    for (const auto& par : enrutadores_) {
        nodos.push_back(par.first);
    }

    const int COL = 8;
    cout << "\n========== TABLA GLOBAL DE COSTOS MÍNIMOS ==========\n";

    // Encabezado
    cout << "  " << setw(COL) << " ";
    for (const auto& n : nodos) {
        cout << setw(COL) << n;
    }
    cout << "\n  " << string(COL * (nodos.size() + 1) + 2, '-') << "\n";

    // Filas
    for (const auto& fila : nodos) {
        cout << "  " << setw(COL) << fila;
        const auto& tabla = enrutadores_.at(fila).getTablaCostos();
        for (const auto& col : nodos) {
            auto it = tabla.find(col);
            if (it == tabla.end() || it->second == INF) {
                cout << setw(COL) << "INF";
            } else {
                cout << setw(COL) << it->second;
            }
        }
        cout << "\n";
    }
    cout << "=====================================================\n";
}

void Red::imprimirCamino(const string& origen, const string& destino) const {
    cout << "\n  Ruta de [" << origen << "] a [" << destino << "]:\n";

    if (!existeEnrutador(origen)) {
        cout << "  [!] El enrutador '" << origen << "' no existe.\n";
        return;
    }
    if (!existeEnrutador(destino)) {
        cout << "  [!] El enrutador '" << destino << "' no existe.\n";
        return;
    }

    const list<string>& camino =
        enrutadores_.at(origen).getCamino(destino);

    if (camino.empty()) {
        cout << "  Sin ruta disponible.\n";
        return;
    }

    int costo = obtenerCosto(origen, destino);

    cout << "  Camino: ";
    bool primero = true;
    for (const auto& nodo : camino) {
        if (!primero) out << " -> ";
        cout << nodo;
        primero = false;
    }
    cout << "\n  Costo total: " << costo << "\n";
}
