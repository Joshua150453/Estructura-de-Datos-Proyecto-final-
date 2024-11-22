# Proyecto Reproductor

## Integrantes:
- Abimael Ernesto Frontado Fajardo
- Frederick Dicarlo Mares Graos
- Joshua Diad Ortiz Rosas

## Primer Avance

### Código:
```cpp
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

```

### Descripción
Este proyecto es una aplicación en C++ para gestionar una lista de reproducción de canciones, utilizando una lista doblemente enlazada para almacenar y manipular la información de las canciones. Permite cargar canciones desde un archivo CSV, agregar y eliminar canciones, cambiar su orden en la lista y reproducirlas de manera aleatoria.

### Características
- Estructura de datos: Implementa una lista doblemente enlazada para gestionar las canciones.
- Carga de datos: Carga canciones desde un archivo CSV y las agrega a la lista de reproducción.
- Agregar y eliminar canciones: Permite añadir nuevas canciones y eliminar canciones existentes de la lista.
- Cambiar el orden de las canciones: Mueve una canción de una posición actual a una nueva posición en la lista.
- Reproducción aleatoria: Reproduce todas las canciones de la lista en un orden aleatorio.
- Impresión de la lista: Imprime la lista de reproducción en la consola.
- Estructura de Datos

### Canción
Estructura para almacenar los detalles de una canción:

- artist_name: Nombre del artista.
- track_name: Nombre de la canción.
- track_id: ID de la canción.
- popularity: Popularidad de la canción.
- year: Año de lanzamiento.
- genre: Género musical.
- danceability, energy, key, loudness, mode, speechiness, acousticness, instrumentalness, liveness, valence, tempo, duration_ms, time_signature: Otros atributos musicales.
- NodoCancion
- Clase que representa un nodo en la lista doblemente enlazada, que almacena una canción y punteros a los nodos anterior y siguiente.

### ListaDobleEnlazada
Clase que representa la lista doblemente enlazada de canciones y contiene métodos para:

- agregar_cancion: Añade una canción al final de la lista.
- eliminar_cancion: Elimina una canción específica de la lista.
- cambiar_orden: Cambia la posición de una canción dentro de la lista.
- reproduccion_aleatoria: Imprime la lista de canciones en un orden aleatorio.
- imprimir_lista: Muestra todas las canciones de la lista en la consola.
### Archivos en el Proyecto
- main.cpp: Contiene el código fuente con la lógica para cargar y gestionar la lista de reproducción.
- spotify_data.csv: Archivo CSV con los datos de las canciones.

### Ejemplo de Uso
- Al ejecutar el programa, se cargará la lista de reproducción desde el archivo CSV y se imprimirá en la consola.
- Se puede observar la función de cambio de orden de canciones, y la lista se reproducirá de manera aleatoria.
### Notas Importantes
- Este código asume que los datos en el CSV están correctamente formateados.
- Las posiciones de la función cambiar_orden deben ser válidas y estar dentro del rango de la lista actual.

## Segundo Avance

### Código:
```cpp
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
```

### Explicación Detallada del Código
#### 1. Inclusión de Bibliotecas:
El código incluye varias bibliotecas estándar de C++:

- #include <iostream>: Para manejar entrada y salida en consola.
- #include <fstream>: Para manejar operaciones de archivos.
- #include <sstream>: Para procesar cadenas de texto, útil al leer CSV.
- #include <string>: Para manejar cadenas de texto (std::string).
- #include <vector>: Para manejar vectores dinámicos.
- #include <algorithm>: Para usar funciones como std::shuffle.
- #include <random>: Para generar números aleatorios, utilizado en la reproducción aleatoria de canciones.

#### 2. Definición de la estructura Cancion
Se define una estructura Cancion que representa una canción con múltiples atributos, como:

- Nombre del artista (artist_name)
- Nombre de la pista (track_name)
- ID de la pista (track_id)
- Popularidad, género, y otros datos numéricos relevantes (por ejemplo, danceability, energy, etc.).

#### 3. Clase NodoCancion
Se define una clase que representa un nodo de una lista doblemente enlazada:

##### Atributos:
- cancion: Almacena un objeto Cancion.
- anterior y siguiente: Punteros a los nodos adyacentes.

##### Constructor:
Inicializa el nodo con una canción específica y establece los punteros como nullptr.

#### 4. Clase ListaDobleEnlazada
Esta clase implementa una lista doblemente enlazada para manejar una lista de reproducción:

##### Atributos:
- cabeza y cola: Apuntan al primer y último nodo de la lista.
- tamano: Lleva un conteo de los nodos en la lista.

##### Métodos principales:
##### Carga de Canciones desde un Archivo CSV:
El programa lee canciones desde un archivo CSV y las agrega a una lista doblemente enlazada.

##### Agregar Canción:
Las canciones se pueden añadir al final de la lista.

##### Eliminar Canción:
Busca una canción por su nombre y la elimina de la lista.

##### Cambiar Orden:
Permite mover una canción de una posición a otra dentro de la lista.

##### Reproducción Aleatoria:
Mezcla y reproduce las canciones en un orden aleatorio.

##### Impresión de Lista:
Muestra la lista completa de canciones con su orden actual.

#### 5. Función cargar_canciones_desde_csv
Esta función:

- Lee un archivo CSV línea por línea.
- Convierte cada línea en un objeto Cancion.
- Agrega cada canción a la lista usando el método agregar_cancion.
- Ignora la primera línea del archivo, asumiendo que es un encabezado.

#### 6. Función main
La función principal realiza lo siguiente:

- Crea una instancia de ListaDobleEnlazada para almacenar las canciones.
- Llama a cargar_canciones_desde_csv para leer canciones desde un archivo llamado spotify_data.csv.
- Pregunta al usuario si desea imprimir la lista de canciones y, si responde "si", llama a imprimir_lista para mostrarla.

### Futuras Mejoras:

Añadir función de búsqueda por nombre de canción; artista; etc.

