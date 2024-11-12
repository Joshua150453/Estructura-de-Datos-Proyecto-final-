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
    int popularity;
    int year;
    string genre;
    float danceability;
    float energy;
    int key;
    float loudness;
    int mode;
    float speechiness;
    float acousticness;
    float instrumentalness;
    float liveness;
    float valence;
    float tempo;
    int duration_ms;
    int time_signature;
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
    ifstream archivo("spotify_data.csv");
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
            cancion.popularity = stoi(campo);

            getline(ss, campo, ','); // Year
            cancion.year = stoi(campo);

            getline(ss, cancion.genre, ',');

            getline(ss, campo, ','); // Danceability
            cancion.danceability = stof(campo);

            getline(ss, campo, ','); // Energy
            cancion.energy = stof(campo);

            getline(ss, campo, ','); // Key
            cancion.key = stoi(campo);

            getline(ss, campo, ','); // Loudness
            cancion.loudness = stof(campo);

            getline(ss, campo, ','); // Mode
            cancion.mode = stoi(campo);

            getline(ss, campo, ','); // Speechiness
            cancion.speechiness = stof(campo);

            getline(ss, campo, ','); // Acousticness
            cancion.acousticness = stof(campo);

            getline(ss, campo, ','); // Instrumentalness
            cancion.instrumentalness = stof(campo);

            getline(ss, campo, ','); // Liveness
            cancion.liveness = stof(campo);

            getline(ss, campo, ','); // Valence
            cancion.valence = stof(campo);

            getline(ss, campo, ','); // Tempo
            cancion.tempo = stof(campo);

            getline(ss, campo, ','); // Duration_ms
            cancion.duration_ms = stoi(campo);

            getline(ss, campo, ','); // Time_signature
            cancion.time_signature = stoi(campo);

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

    // Imprimir la lista de canciones para verificar la carga
    playlist.imprimir_lista();

    return 0;
}