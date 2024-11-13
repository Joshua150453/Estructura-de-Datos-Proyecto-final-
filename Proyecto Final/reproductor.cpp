#include <iostream> 
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

// Estructura para almacenar información de una canción
struct Cancion {
    string artist_name;
    string track_name;
    string track_id;
    string popularity;
    string year;
    string genre;
    string danceability;
    string energy;
    string key;
    string loudness;
    string mode;
    string speechiness;
    string acousticness;
    string instrumentalness;
    string liveness;
    string valence;
    string tempo;
    string duration_ms;
    string time_signature;
};

// Nodo para la lista doblemente enlazada
class NodoCancion {
public:
    Cancion cancion;
    NodoCancion* anterior;
    NodoCancion* siguiente;

    NodoCancion(Cancion c) : cancion(c), anterior(nullptr), siguiente(nullptr) {}
};

// Lista doblemente enlazada para almacenar canciones
class ListaDobleEnlazada {
private:
    NodoCancion* cabeza;
    NodoCancion* cola;
    int tamaño;

public:
    ListaDobleEnlazada() : cabeza(nullptr), cola(nullptr), tamaño(0) {}

    // Añadir una canción a la lista de reproducción
    void agregar_cancion(const Cancion& c) {
        NodoCancion* nuevo = new NodoCancion(c);
        if (!cabeza) {
            cabeza = cola = nuevo;
        } else {
            cola->siguiente = nuevo;
            nuevo->anterior = cola;
            cola = nuevo;
        }
        tamaño++;
    }

    // Eliminar una canción de la lista de reproducción
    void eliminar_cancion(const Cancion& c) {
        NodoCancion* actual = cabeza;
        while (actual) {
            if (actual->cancion.track_name == c.track_name) {
                if (actual->anterior) {
                    actual->anterior->siguiente = actual->siguiente;
                } else {
                    cabeza = actual->siguiente;
                }
                if (actual->siguiente) {
                    actual->siguiente->anterior = actual->anterior;
                } else {
                    cola = actual->anterior;
                }
                delete actual;
                tamaño--;
                cout << "Canción eliminada: " << c.track_name << endl;
                return;
            }
            actual = actual->siguiente;
        }
        cout << "Canción no encontrada: " << c.track_name << endl;
    }

    // Cambiar la canción de una posición actual a una nueva posición
    void cambiar_orden(int posicion_actual, int nueva_posicion) {
    if (posicion_actual < 0 || nueva_posicion < 0 || posicion_actual >= tamaño || nueva_posicion >= tamaño) {
        cout << "Posiciones inválidas." << endl;
        return;
    }
    if (posicion_actual == nueva_posicion) return;

    NodoCancion* actual = cabeza;

    // Mover al nodo en la posición actual
    for (int i = 0; i < posicion_actual; i++) {
        actual = actual->siguiente;
    }

    // Desvincular el nodo de su posición actual
    if (actual->anterior) actual->anterior->siguiente = actual->siguiente;
    if (actual->siguiente) actual->siguiente->anterior = actual->anterior;
    if (actual == cabeza) cabeza = actual->siguiente;
    if (actual == cola) cola = actual->anterior;

    // Insertar el nodo en la nueva posición
    NodoCancion* destino = cabeza;
    for (int i = 0; i < nueva_posicion; i++) {
        destino = destino->siguiente;
    }

    if (nueva_posicion == 0) { // Nueva posición es la cabeza
        actual->siguiente = cabeza;
        if (cabeza) cabeza->anterior = actual;
        cabeza = actual;
        actual->anterior = nullptr;
    } else if (nueva_posicion == tamaño - 1) { // Nueva posición es la cola
        actual->anterior = cola;
        if (cola) cola->siguiente = actual;
        cola = actual;
        actual->siguiente = nullptr;
    } else { // Nueva posición en medio
        actual->anterior = destino->anterior;
        actual->siguiente = destino;
        if (destino->anterior) destino->anterior->siguiente = actual;
        destino->anterior = actual;
    }

    cout << "Canción movida de la posición " << posicion_actual << " a " << nueva_posicion << endl;
}

    // Reproducir canciones en orden aleatorio
    void reproduccion_aleatoria() {
        vector<NodoCancion*> canciones;
        NodoCancion* actual = cabeza;
        while (actual) {
            canciones.push_back(actual);
            actual = actual->siguiente;
        }

        // Mezclar el vector de canciones aleatoriamente
        random_device rd;
        mt19937 g(rd());
        shuffle(canciones.begin(), canciones.end(), g);

        cout << "Reproducción aleatoria de canciones:" << endl;
        for (auto* cancion : canciones) {
            cout << "Canción: " << cancion->cancion.track_name << " - Artista: " << cancion->cancion.artist_name << endl;
        }
    }

    // Imprimir lista de reproducción
    void imprimir_lista() {
        NodoCancion* actual = cabeza;
        while (actual) {
            cout << "Canción: " << actual->cancion.track_name << " - Artista: " << actual->cancion.artist_name << endl;
            actual = actual->siguiente;
        }
    }
};

// Función para leer y cargar canciones desde un archivo CSV
void cargar_canciones_desde_csv(const string& nombre_archivo, ListaDobleEnlazada& lista) {
    ifstream archivo(nombre_archivo);
    string linea;
    bool primera_linea = true;

    if (archivo.is_open()) {
        while (getline(archivo, linea)) {
            if (primera_linea) { // Omitir la primera línea si es el encabezado
                primera_linea = false;
                continue;
            }

            stringstream ss(linea);
            string campo;
            Cancion cancion;

            // Leer cada campo y asignarlo a la estructura de canción
            getline(ss, cancion.artist_name, ',');
            getline(ss, cancion.track_name, ',');
            getline(ss, cancion.track_id, ',');
            
            getline(ss, campo, ','); // Popularity
            cancion.popularity = campo;

            getline(ss, campo, ','); // Year
            cancion.year = campo;

            getline(ss, cancion.genre, ',');
            cancion.genre = campo;

            getline(ss, campo, ','); // Danceability
            cancion.danceability = campo;

            getline(ss, campo, ','); // Energy
            cancion.energy = campo;

            getline(ss, campo, ','); // Key
            cancion.key = campo;

            getline(ss, campo, ','); // Loudness
            cancion.loudness = campo;

            getline(ss, campo, ','); // Mode
            cancion.mode = campo;

            getline(ss, campo, ','); // Speechiness
            cancion.speechiness = campo;

            getline(ss, campo, ','); // Acousticness
            cancion.acousticness = campo;

            getline(ss, campo, ','); // Instrumentalness
            cancion.instrumentalness = campo;

            getline(ss, campo, ','); // Liveness
            cancion.liveness = campo;

            getline(ss, campo, ','); // Valence
            cancion.valence = campo;

            getline(ss, campo, ','); // Tempo
            cancion.tempo = campo;

            getline(ss, campo, ','); // Duration_ms
            cancion.duration_ms = campo;

            getline(ss, campo, ','); // Time_signature
            cancion.time_signature = campo;

            // Agregar la canción a la lista
            lista.agregar_cancion(cancion);
        }
        archivo.close();
    } else {
        cerr << "No se pudo abrir el archivo " << nombre_archivo << endl;
    }
}

int main() {
    ListaDobleEnlazada playlist;

    cargar_canciones_desde_csv("spotify_data.csv", playlist);

    playlist.imprimir_lista();

    cout << "\nCambiando el orden de las canciones...\n";
    playlist.cambiar_orden(1, 3);  // Cambia las posiciones según tu lista
    
    playlist.imprimir_lista();

    cout << "\nReproducción aleatoria de canciones...\n";
    playlist.reproduccion_aleatoria();

    return 0;
}
