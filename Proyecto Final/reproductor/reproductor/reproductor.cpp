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
    int tamano;

public:
    ListaDobleEnlazada() : cabeza(nullptr), cola(nullptr), tamano(0) {}

    void agregar_cancion(const Cancion& c) {
        NodoCancion* nuevo = new NodoCancion(c);
        if (!cabeza) {
            cabeza = cola = nuevo;
        }
        else {
            cola->siguiente = nuevo;
            nuevo->anterior = cola;
            cola = nuevo;
        }
        tamano++;
    }

    void imprimir_lista() {
        NodoCancion* actual = cabeza;
        while (actual) {
            cout << "Cancion: " << actual->cancion.track_name << " - Artista: " << actual->cancion.artist_name << endl;
            actual = actual->siguiente;
        }
    }
};

// Función para leer y cargar canciones desde un archivo CSV
void cargar_canciones_desde_csv(ifstream& archivo, ListaDobleEnlazada& lista) {

    string linea;
    bool primera_linea = true;


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

}

int main() {

    ListaDobleEnlazada playlist;
    ifstream archivo("spotify_data.csv");

    if (archivo.is_open()) {
        cargar_canciones_desde_csv(archivo, playlist);
    }
    else {
        cerr << "No se pudo abrir el archivo " << endl;
    }

    // Imprimir la lista de canciones para verificar la carga
    playlist.imprimir_lista();

    return 0;
}