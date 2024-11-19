#include <iostream> 
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

// Estructura para almacenar informacion de una cancion
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

    // Anadir una cancion a la lista de reproduccion
    void agregar_cancion(const Cancion& c) {
        NodoCancion* nuevo = new NodoCancion(c);
        if (!cabeza) {
            cabeza = cola = nuevo;
        } else {
            cola->siguiente = nuevo;
            nuevo->anterior = cola;
            cola = nuevo;
        }
        tamano++;
    }

    // Eliminar una cancion de la lista de reproduccion
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
                tamano--;
                cout << "Cancion eliminada: " << c.track_name << endl;
                return;
            }
            actual = actual->siguiente;
        }
        cout << "Cancion no encontrada: " << c.track_name << endl;
    }

    // Cambiar la cancion de una posicion actual a una nueva posicion
    void cambiar_orden(int posicion_actual, int nueva_posicion) {
    if (posicion_actual < 0 || nueva_posicion < 0 || posicion_actual >= tamano || nueva_posicion >= tamano) {
        cout << "Posiciones invalidas." << endl;
        return;
    }
    if (posicion_actual == nueva_posicion) return;

    NodoCancion* actual = cabeza;

    // Mover al nodo en la posicion actual
    for (int i = 0; i < posicion_actual; i++) {
        actual = actual->siguiente;
    }

    // Desvincular el nodo de su posicion actual
    if (actual->anterior) actual->anterior->siguiente = actual->siguiente;
    if (actual->siguiente) actual->siguiente->anterior = actual->anterior;
    if (actual == cabeza) cabeza = actual->siguiente;
    if (actual == cola) cola = actual->anterior;

    // Insertar el nodo en la nueva posicion
    NodoCancion* destino = cabeza;
    for (int i = 0; i < nueva_posicion; i++) {
        destino = destino->siguiente;
    }

    if (nueva_posicion == 0) { // Nueva posicion es la cabeza
        actual->siguiente = cabeza;
        if (cabeza) cabeza->anterior = actual;
        cabeza = actual;
        actual->anterior = nullptr;
    } else if (nueva_posicion == tamano - 1) { // Nueva posicion es la cola
        actual->anterior = cola;
        if (cola) cola->siguiente = actual;
        cola = actual;
        actual->siguiente = nullptr;
    } else { // Nueva posicion en medio
        actual->anterior = destino->anterior;
        actual->siguiente = destino;
        if (destino->anterior) destino->anterior->siguiente = actual;
        destino->anterior = actual;
    }

    cout << "Cancion movida de la posicion " << posicion_actual << " a " << nueva_posicion << endl;
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

        cout << "Reproduccion aleatoria de canciones:" << endl;
        for (auto* cancion : canciones) {
            cout << "Cancion: " << cancion->cancion.track_name << " - Artista: " << cancion->cancion.artist_name << endl;
        }
    }

    // Imprimir lista de reproduccion
    void imprimir_lista() {
        NodoCancion* actual = cabeza;
        int cont = 1;
        while (actual) {
            cout << "Cancion " <<cont<<" : "<< actual->cancion.track_name << " - Artista: " << actual->cancion.artist_name << endl;
            actual = actual->siguiente;
            cont++;
        }
    }
};

// Funcion para leer y cargar canciones desde un archivo CSV
void cargar_canciones_desde_csv(const string& nombre_archivo, ListaDobleEnlazada& lista) {
    ifstream archivo(nombre_archivo);
    string linea;
    bool primera_linea = true;

    if (archivo.is_open()) {
        while (getline(archivo, linea)) {
            if (primera_linea) { // Omitir la primera linea si es el encabezado
                primera_linea = false;
                continue;
            }

            stringstream ss(linea);
            string campo;
            Cancion cancion;

            // Leer cada campo y asignarlo a la estructura de cancion
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

            // Agregar la cancion a la lista
            lista.agregar_cancion(cancion);
        }
        archivo.close();
    } else {
        cerr << "No se pudo abrir el archivo " << nombre_archivo << endl;
    }
}

int main() {
    ListaDobleEnlazada playlist;

    cout<<"Cargando archivo"<<endl;

    cargar_canciones_desde_csv("spotify_data.csv", playlist);
    cout<<"Archivo cargado"<<endl;

    string confirmacion;
    cout<<"Desea imprimir la lista?"<<endl;

    cin >>confirmacion;

    if(confirmacion =="si"){

        playlist.imprimir_lista();

    }
    

    cin>>confirmacion;
    

 
   

    return 0;
}
