# Proyecto Reproductor

- Ingresar a Proyecto Final pare ver todos los archivo
  
## Integrantes:
- Abimael Ernesto Frontado Fajardo (100%)
- Frederick Dicarlo Mares Graos (100%)
- Joshua Diad Ortiz Rosas (100%)

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

## Código Final:
```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <functional> // Para std::hash
#include <algorithm> // Para std::transform
#include <ctime> 
#include <random>
#include <limits> 
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

    // Generar un hash unico basado en el nombre de la cancion
    size_t hash() const {
        const size_t prime1 = 0x9e3779b97f4a7c15; // Numero primo grande
        const size_t prime2 = 0x85ebca6b;
        const size_t prime3 = 0xc2b2ae35;
        const size_t seed = 0x27d4eb2f165667c5;

        auto combine = [](size_t hash, const std::string& str) {
            std::hash<std::string> hasher;
            size_t value = hasher(str);
            value ^= (value >> 30);
            value *= prime1;
            value ^= (value >> 27);
            value *= prime2;
            value ^= (value >> 31);
            return hash ^ value;
            };

        size_t result = seed;
        result = combine(result, track_name);
        result = combine(result, artist_name);
        result = combine(result, track_id);
        result = combine(result, genre);

        // Mezcla final para mayor dispersion
        result ^= (result >> 33);
        result *= prime3;
        result ^= (result >> 29);
        result *= prime1;
        result ^= (result >> 32);

        return result;
    }
};

// Clase para representar un nodo del arbol binario
class NodoBST {
public:
    pair<size_t, Cancion> clave;  // Clave (hash y cancion)
    NodoBST* izquierdo;  // Puntero al hijo izquierdo
    NodoBST* derecho;    // Puntero al hijo derecho

    NodoBST(const pair<size_t, Cancion>& _clave) : clave(_clave), izquierdo(nullptr), derecho(nullptr) {}
};

// Clase para el arbol binario de busqueda
class BST {
private:


    // Funcion auxiliar para insertar en el arbol binario de busqueda
    void insertarRecursivo(NodoBST*& nodo, const pair<size_t, Cancion>& clave) {
        if (nodo == nullptr) {
            nodo = new NodoBST(clave);
        }
        else if (clave.first < nodo->clave.first) {
            insertarRecursivo(nodo->izquierdo, clave);
        }
        else {
            insertarRecursivo(nodo->derecho, clave);
        }
    }

    // Funcion auxiliar para recorrer el arbol en orden
    void recorrerRecursivo(NodoBST* nodo) const {

        if (nodo != nullptr) {
            recorrerRecursivo(nodo->izquierdo);
            cout << "Cancion: " << nodo->clave.second.track_name
                << " - Artista: " << nodo->clave.second.artist_name << endl;
            recorrerRecursivo(nodo->derecho);
        }
    }

    // Funcion auxiliar para verificar si el arbol esta vacio
    bool estaVacio(NodoBST* nodo) const {
        return nodo == nullptr;
    }
    NodoBST* encontrarMin(NodoBST* nodo) {
        while (nodo && nodo->izquierdo != nullptr) {
            nodo = nodo->izquierdo;
        }
        return nodo;
    }

    // Funcion auxiliar para eliminar un nodo en el arbol
    string aMayuscula(const string& cadena) {
        string resultado = cadena;
        transform(resultado.begin(), resultado.end(), resultado.begin(), ::toupper);
        return resultado;
    }

    NodoBST* eliminarRecursivo(NodoBST* nodo, const string& nombreCancion, const string& nombreArtista) {
        if (nodo == nullptr) {
            return nodo; // Nodo no encontrado
        }

        // Normalizar nombres a mayúsculas
        string nombreActual = aMayuscula(nodo->clave.second.track_name);
        string artistaActual = aMayuscula(nodo->clave.second.artist_name);
        string nombreBuscado = aMayuscula(nombreCancion);
        string artistaBuscado = aMayuscula(nombreArtista);

        if (nombreBuscado < nombreActual || (nombreBuscado == nombreActual && artistaBuscado < artistaActual)) {
            nodo->izquierdo = eliminarRecursivo(nodo->izquierdo, nombreCancion, nombreArtista);
        } else if (nombreBuscado > nombreActual || (nombreBuscado == nombreActual && artistaBuscado > artistaActual)) {
            nodo->derecho = eliminarRecursivo(nodo->derecho, nombreCancion, nombreArtista);
        } else {
            // Nodo encontrado: Proceder con la eliminación
            if (nodo->izquierdo == nullptr && nodo->derecho == nullptr) {
                delete nodo;
                return nullptr;
            } else if (nodo->izquierdo == nullptr) {
                NodoBST* temp = nodo->derecho;
                delete nodo;
                return temp;
            } else if (nodo->derecho == nullptr) {
                NodoBST* temp = nodo->izquierdo;
                delete nodo;
                return temp;
            } else {
                NodoBST* temp = encontrarMin(nodo->derecho);
                nodo->clave = temp->clave;
                nodo->derecho = eliminarRecursivo(nodo->derecho, temp->clave.second.track_name, temp->clave.second.artist_name);
            }
        }
        return nodo;
    }


public:

    NodoBST* raiz;
    BST() : raiz(nullptr) {}

    // Insertar una nueva cancion en el arbol
    void insertar(const Cancion& c) {
        size_t hashClave = c.hash(); // Calcular el hash de la cancion
        insertarRecursivo(raiz, { hashClave, c });
    }

    // Recorrer el arbol en orden y devolver true si hay elementos
    bool recorrer() const {
        if (estaVacio(raiz)) {
            return false; // arbol vacio
        }
        recorrerRecursivo(raiz);
        return true; // arbol no vacio
    }

    bool auxiliarRecorrer() const {

        if (estaVacio(raiz)) {
            return false; // arbol vacio
        }
        else {
            return true;
        }
    }
    bool eliminar(const string& nombreCancion, const string& nombreArtista) {
        raiz = eliminarRecursivo(raiz, nombreCancion, nombreArtista);
        return !estaVacio(raiz);
    }


    void buscarPorPrefijo(const string& prefijo, vector<string>& resultados) const {
        // Convertir el prefijo a mayusculas para comparar
        string prefijoMayus = prefijo;
        transform(prefijoMayus.begin(), prefijoMayus.end(), prefijoMayus.begin(), ::toupper);

        buscarPorPrefijoRec(raiz, prefijoMayus, resultados);
    }

private:
    void buscarPorPrefijoRec(NodoBST* nodo, const string& prefijo, vector<string>& resultados) const {
        if (nodo == nullptr) {
            return; // Base de la recursion: nodo vacio
        }

        // Convertir el nombre de la cancion actual a mayusculas para comparacion
        string nombreMayus = nodo->clave.second.track_name;
        transform(nombreMayus.begin(), nombreMayus.end(), nombreMayus.begin(), ::toupper);

        // Si el nombre de la cancion comienza con el prefijo, agregar a los resultados
        if (nombreMayus.rfind(prefijo, 0) == 0) { // rfind con 0 verifica si comienza con el prefijo
            resultados.push_back(nodo->clave.second.track_name + " - " + nodo->clave.second.artist_name);
        }

        // Recorrer el subarbol izquierdo y derecho
        buscarPorPrefijoRec(nodo->izquierdo, prefijo, resultados);
        buscarPorPrefijoRec(nodo->derecho, prefijo, resultados);
    }
};

// Funcion para leer y cargar canciones desde un archivo CSV
void cargar_canciones_desde_csv(const string& nombre_archivo, BST arboles[26]) {
    ifstream archivo(nombre_archivo);
    string linea;
    bool primera_linea = true;
    vector<Cancion> canciones;

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
            getline(ss, cancion.artist_name, ','); //saltar el primer numero
            getline(ss, cancion.artist_name, ',');
            getline(ss, cancion.track_name, ',');
            getline(ss, cancion.track_id, ',');
            getline(ss, cancion.popularity, ',');
            getline(ss, cancion.year, ',');
            getline(ss, cancion.genre, ',');
            getline(ss, cancion.danceability, ',');
            getline(ss, cancion.energy, ',');
            getline(ss, cancion.key, ',');
            getline(ss, cancion.loudness, ',');
            getline(ss, cancion.mode, ',');
            getline(ss, cancion.speechiness, ',');
            getline(ss, cancion.acousticness, ',');
            getline(ss, cancion.instrumentalness, ',');
            getline(ss, cancion.liveness, ',');
            getline(ss, cancion.valence, ',');
            getline(ss, cancion.tempo, ',');
            getline(ss, cancion.duration_ms, ',');
            getline(ss, cancion.time_signature, ',');

            // Guardar la cancion en un vector
            char letra_inicial = toupper(cancion.track_name[0]);
            if (letra_inicial >= 'A' && letra_inicial <= 'Z') {
                // Insertar en el arbol correspondiente
                arboles[letra_inicial - 'A'].insertar(cancion);
            }
        }
        archivo.close();

        // Insertar las canciones en el BST correspondiente

    }
    else {
        cerr << "No se pudo abrir el archivo " << nombre_archivo << endl;
    }
}

void agregarCancion(BST arboles[]) {
    Cancion nuevaCancion;

    // Solicitar datos de la cancion al usuario
    cout << "Ingrese los datos de la cancion:\n";
    cout << "Nombre del artista: ";
    cin.ignore(); // Limpiar el buffer de entrada
    getline(cin, nuevaCancion.artist_name);
    cout << "Nombre de la cancion: ";
    getline(cin, nuevaCancion.track_name);
    cout << "ID de la cancion: ";
    getline(cin, nuevaCancion.track_id);
    cout << "Popularidad: ";
    getline(cin, nuevaCancion.popularity);
    cout << "Año: ";
    getline(cin, nuevaCancion.year);
    cout << "Género: ";
    getline(cin, nuevaCancion.genre);
    cout << "Danceabilidad: ";
    getline(cin, nuevaCancion.danceability);
    cout << "Energia: ";
    getline(cin, nuevaCancion.energy);
    cout << "Clave: ";
    getline(cin, nuevaCancion.key);
    cout << "Sonoridad: ";
    getline(cin, nuevaCancion.loudness);
    cout << "Modo: ";
    getline(cin, nuevaCancion.mode);
    cout << "Habla: ";
    getline(cin, nuevaCancion.speechiness);
    cout << "Acustica: ";
    getline(cin, nuevaCancion.acousticness);
    cout << "Instrumentalidad: ";
    getline(cin, nuevaCancion.instrumentalness);
    cout << "Vivacidad: ";
    getline(cin, nuevaCancion.liveness);
    cout << "Valencia: ";
    getline(cin, nuevaCancion.valence);
    cout << "Tempo: ";
    getline(cin, nuevaCancion.tempo);
    cout << "Duracion (ms): ";
    getline(cin, nuevaCancion.duration_ms);
    cout << "Compas: ";
    getline(cin, nuevaCancion.time_signature);

    // Determinar el bucket en funcion de la letra inicial de la cancion
    char letra_inicial = toupper(nuevaCancion.track_name[0]);
    if (letra_inicial >= 'A' && letra_inicial <= 'Z') {
        int indice = letra_inicial - 'A';
        arboles[indice].insertar(nuevaCancion); // Insertar la cancion en el arbol correspondiente
        cout << "La cancion se ha agregado correctamente al bucket " << letra_inicial << "." << endl;
    }
    else {
        cerr << "Error: El nombre de la cancion no comienza con una letra valida." << endl;
    }
}

void eliminarCancion( BST Aux[26]) {
    string nombreCancion;
    ifstream archivo("spotify_data.txt");
    string nombreArtista;
    cout << "Ingrese el nombre de la canción: " << endl;
    getline(cin, nombreCancion);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "Ingrese el nombre del artista: " << endl;
    getline(cin, nombreArtista);

    string linea;
    bool primera_linea = true;

    while (getline(archivo, linea)) {
        if (primera_linea) {
            primera_linea = false; // Saltar la primera línea (cabecera del archivo).
            continue;
        }

        stringstream ss(linea);
        Cancion cancion;

        // Leer todos los campos de la canción.
        getline(ss, cancion.artist_name, ',');
        getline(ss, cancion.track_name, ',');
        getline(ss, cancion.track_id, ',');
        getline(ss, cancion.popularity, ',');
        getline(ss, cancion.year, ',');
        getline(ss, cancion.genre, ',');
        getline(ss, cancion.danceability, ',');
        getline(ss, cancion.energy, ',');
        getline(ss, cancion.key, ',');
        getline(ss, cancion.loudness, ',');
        getline(ss, cancion.mode, ',');
        getline(ss, cancion.speechiness, ',');
        getline(ss, cancion.acousticness, ',');
        getline(ss, cancion.instrumentalness, ',');
        getline(ss, cancion.liveness, ',');
        getline(ss, cancion.valence, ',');
        getline(ss, cancion.tempo, ',');
        getline(ss, cancion.duration_ms, ',');
        getline(ss, cancion.time_signature, ',');

        // Comparar y omitir la canción a eliminar.
        if (cancion.track_name != nombreCancion && cancion.artist_name != nombreArtista) {
            char letra_inicial = toupper(cancion.track_name[0]);
        if (letra_inicial >= 'A' && letra_inicial <= 'Z') {
            Aux[letra_inicial - 'A'].insertar(cancion);
        }
        }

        // Insertar la canción directamente en el árbol correspondiente.
        
    }
    archivo.close();

    
    
}


void buscarCanciones(const BST arboles[], const string& consulta) {
    if (consulta.empty()) {
        cout << "Error: La consulta esta vacia." << endl;
        return;
    }

    // Convertir consulta a mayusculas para comparaciones consistentes
    string consultaMayus = consulta;
    transform(consultaMayus.begin(), consultaMayus.end(), consultaMayus.begin(), ::toupper);

    char letraInicial = toupper(consulta[0]);
    if (letraInicial >= 'A' && letraInicial <= 'Z') {
        int indice = letraInicial - 'A';
        cout << "Resultados de busqueda para \"" << consulta << "\":" << endl << endl;

        // Buscar en el BST correspondiente
        vector<string> resultados;
        arboles[indice].buscarPorPrefijo(consultaMayus, resultados);

        if (!resultados.empty()) {
            for (const string& resultado : resultados) {
                cout << resultado << endl;
            }
            cout << endl;
        }
        else {
            cout << "No se encontraron resultados para \"" << consulta << "\"." << endl << endl;
        }

    }
    else {
        cout << "Error: La consulta no comienza con una letra valida." << endl;
    }
}

void recopilarCanciones(NodoBST* nodo, vector<Cancion>& canciones) {
    if (!nodo) return;

    // Recorrido en orden (in-order traversal)
    recopilarCanciones(nodo->izquierdo, canciones);
    canciones.push_back(nodo->clave.second); // Suponiendo que las canciones estan en el nodo
    recopilarCanciones(nodo->derecho, canciones);
}

void imprimirCancionesAleatorias(BST arboles[26]) {
    vector<Cancion> todasLasCanciones;

    // Recopila canciones de todos los buckets
    for (int i = 0; i < 26; ++i) {
        recopilarCanciones(arboles[i].raiz, todasLasCanciones); // Suponiendo que la raiz es accesible
    }

    // Mezclar canciones de forma aleatoria
    random_device rd;
    mt19937 gen(rd()); // Generador de numeros aleatorios
    shuffle(todasLasCanciones.begin(), todasLasCanciones.end(), gen);

    // Imprimir las canciones mezcladas
    cout << "Canciones en orden aleatorio:\n";
    for (const auto& cancion : todasLasCanciones) {
        cout << "Cancion: " << cancion.track_name << " - Artista: " << cancion.artist_name << endl;
    }
    cout << endl;
}

void cambiar_orden(BST arboles[26], int pos1, int pos2) {
    vector<Cancion> todasLasCanciones;

    cout << "3 RAYAS DE COCAINA" << endl;
    // Recopila canciones de todos los buckets
    for (int i = 0; i < 26; ++i) {
        recopilarCanciones(arboles[i].raiz, todasLasCanciones);
    }

    // Validar posiciones
    if (pos1 < 1 || pos2 < 1 || pos1 > todasLasCanciones.size() || pos2 > todasLasCanciones.size()) {
        cerr << "Error: Las posiciones ingresadas estan fuera de rango." << endl;
        return;
    }

    // Convertir a indices (0-based)
    int index1 = pos1 - 1;
    int index2 = pos2 - 1;

    // Intercambiar las canciones en las posiciones especificadas
    swap(todasLasCanciones[index1], todasLasCanciones[index2]);



    cout << "Las posiciones " << pos1 << " y " << pos2 << " han sido intercambiadas." << endl;

    for (int i = 0; i < todasLasCanciones.size(); i++) {
        cout << "Cancion: " << todasLasCanciones.at(i).track_name << " - Artista: " << todasLasCanciones.at(i).artist_name << endl;
    }

    cout << endl;

    return;
}

struct Nodo {
    Cancion data;
    Nodo* left;
    Nodo* right;
};

struct BSTaux {
    Nodo* root = nullptr;
    bool insertarIzquierda = true; // Alternador para manejar popularidades iguales
};

bool esNumeroValido(const string& str) {
    try {
        stof(str);  // Convertir a float
        return true;  // Si no lanza excepción, es un número válido
    }
    catch (...) {
        return false;  // Si lanza una excepción, no es un número válido
    }
}

void insertarPopular(BSTaux& Ordenado, Cancion cancion, const string& orden) {
    // Verificar si la popularidad es un número válido antes de insertar
    if (!esNumeroValido(cancion.popularity)) {
        return;  // Ignorar canciones con popularidad no válida
    }

    // Crear un nodo nuevo para la canción actual
    Nodo* nuevoNodo = new Nodo;
    nuevoNodo->data = cancion; // Asignar la canción actual al nodo
    nuevoNodo->left = nullptr;
    nuevoNodo->right = nullptr;

    // Si el árbol está vacío, el nuevo nodo es la raíz
    if (Ordenado.root == nullptr) {
        Ordenado.root = nuevoNodo;
        return;
    }

    // Insertar en la posición correcta en el árbol según la popularidad
    Nodo* actual = Ordenado.root;
    Nodo* padre = nullptr;

    while (actual != nullptr) {
        padre = actual;
        // Comparar la popularidad según el orden especificado
        float pop_actual = stof(actual->data.popularity);
        float pop_nueva = stof(cancion.popularity);

        if (orden == "ascendente") {
            if (pop_nueva < pop_actual) {
                actual = actual->left; // Ir hacia la izquierda
            }
            else {
                actual = actual->right; // Ir hacia la derecha
            }
        }
        else { // descendente
            if (pop_nueva > pop_actual) {
                actual = actual->left; // Ir hacia la izquierda
            }
            else {
                actual = actual->right; // Ir hacia la derecha
            }
        }
    }

    // Insertar el nodo como hijo izquierdo o derecho
    if (orden == "ascendente") {
        if (stof(cancion.popularity) < stof(padre->data.popularity)) {
            padre->left = nuevoNodo;
        }
        else {
            padre->right = nuevoNodo;
        }
    }
    else { // descendente
        if (stof(cancion.popularity) > stof(padre->data.popularity)) {
            padre->left = nuevoNodo;
        }
        else {
            padre->right = nuevoNodo;
        }
    }
}

void insertarDuracion(BSTaux& Ordenado, Cancion cancion, const string& orden) {
    // Verificar si la duración es un número válido antes de insertar
    if (!esNumeroValido(cancion.duration_ms)) {
        return;  // Ignorar canciones con duración no válida
    }

    // Crear un nodo nuevo para la canción actual
    Nodo* nuevoNodo = new Nodo;
    nuevoNodo->data = cancion; // Asignar la canción actual al nodo
    nuevoNodo->left = nullptr;
    nuevoNodo->right = nullptr;

    // Si el árbol está vacío, el nuevo nodo es la raíz
    if (Ordenado.root == nullptr) {
        Ordenado.root = nuevoNodo;
        return;
    }

    // Insertar en la posición correcta en el árbol según la duración
    Nodo* actual = Ordenado.root;
    Nodo* padre = nullptr;

    while (actual != nullptr) {
        padre = actual;

        // Comparar la duración según el orden especificado
        float dur_actual = stof(actual->data.duration_ms);
        float dur_nueva = stof(cancion.duration_ms);

        if (orden == "ascendente") {
            if (dur_nueva < dur_actual) {
                actual = actual->left; // Ir hacia la izquierda
            }
            else {
                actual = actual->right; // Ir hacia la derecha
            }
        }
        else { // descendente
            if (dur_nueva > dur_actual) {
                actual = actual->left; // Ir hacia la izquierda
            }
            else {
                actual = actual->right; // Ir hacia la derecha
            }
        }
    }

    // Insertar el nodo como hijo izquierdo o derecho
    if (orden == "ascendente") {
        if (stof(cancion.duration_ms) < stof(padre->data.duration_ms)) {
            padre->left = nuevoNodo;
        }
        else {
            padre->right = nuevoNodo;
        }
    }
    else { // descendente
        if (stof(cancion.duration_ms) > stof(padre->data.duration_ms)) {
            padre->left = nuevoNodo;
        }
        else {
            padre->right = nuevoNodo;
        }
    }
}

void imprimirInOrder(Nodo* nodo, const string& criterio) {
    if (nodo == nullptr) {
        return;
    }

    // Recorrer el subárbol izquierdo
    imprimirInOrder(nodo->left, criterio);

    // Imprimir los datos de la canción
    cout << "Artista: " << nodo->data.artist_name << ", "
        << "Canción: " << nodo->data.track_name << ", ";

    if (criterio == "popularidad") {
        cout << "Popularidad: " << nodo->data.popularity << endl;
    }
    else if (criterio == "duracion") {
        cout << "Duración (ms): " << nodo->data.duration_ms << endl;
    }

    // Recorrer el subárbol derecho
    imprimirInOrder(nodo->right, criterio);
}

void imprimirCancionesOrdenadas(const string& criterio, const string& orden, string filtroAnio) {
    ifstream archivo("spotify_data.csv"); // Asegúrate de usar el nombre correcto del archivo
    BSTaux Ordenado;

    if (criterio == "popularidad" || criterio == "duracion") {
        string linea;
        bool primera_linea = true;
        vector<Cancion> canciones;

        if (archivo.is_open()) {
            while (getline(archivo, linea)) {
                if (primera_linea) { // Omitir la primera línea si es el encabezado
                    primera_linea = false;
                    continue;
                }

                stringstream ss(linea);
                Cancion cancion;

                // Leer cada campo y asignarlo a la estructura de Canción
                getline(ss, cancion.artist_name, ','); // Saltar el primer número
                getline(ss, cancion.artist_name, ',');
                getline(ss, cancion.track_name, ',');
                getline(ss, cancion.track_id, ',');
                getline(ss, cancion.popularity, ',');
                getline(ss, cancion.year, ',');
                getline(ss, cancion.genre, ',');
                getline(ss, cancion.danceability, ',');
                getline(ss, cancion.energy, ',');
                getline(ss, cancion.key, ',');
                getline(ss, cancion.loudness, ',');
                getline(ss, cancion.mode, ',');
                getline(ss, cancion.speechiness, ',');
                getline(ss, cancion.acousticness, ',');
                getline(ss, cancion.instrumentalness, ',');
                getline(ss, cancion.liveness, ',');
                getline(ss, cancion.valence, ',');
                getline(ss, cancion.tempo, ',');
                getline(ss, cancion.duration_ms, ',');
                getline(ss, cancion.time_signature, ',');

                // Filtrar según el año, si filtroAnio no es "0"
                if (filtroAnio != "0") {
                    try {
                        int anioFiltro = stoi(filtroAnio);
                        if (stoi(cancion.year) != anioFiltro) {
                            continue; // Omitir canciones que no coincidan con el año
                        }
                    }
                    catch (invalid_argument&) {
                        // Manejar excepción si es necesario
                    }
                }

                
                 if (criterio == "popularidad") {
                    insertarPopular(Ordenado, cancion, orden);
                }
                else if (criterio == "duracion") {
                    insertarDuracion(Ordenado,cancion, orden);
                }
            }
            archivo.close();

            // Insertar las canciones en el BST correspondiente

            // Imprimir el árbol en orden
            imprimirInOrder(Ordenado.root, criterio);
        }
        else {
            cerr << "No se pudo abrir el archivo" << endl;
        }
    }
    else {
        cout << "Asegúrate de escribir según se indicó: 'popularidad' o 'duracion'" << endl;
    }
}

void limpiarMemoria(NodoBST* nodo) {
    if (nodo == nullptr) {
        return;
    }

    // Liberar memoria de los subárboles izquierdo y derecho
    limpiarMemoria(nodo->izquierdo);
    limpiarMemoria(nodo->derecho);

    // Liberar memoria del nodo actual
    delete nodo;
}

void limpiarTodosLosBST(BST arboles[26]) {
    for (int i = 0; i < 26; ++i) {
        limpiarMemoria(arboles[i].raiz);
        arboles[i].raiz = nullptr; // Asegurarse de que la raíz se establezca en nullptr después de limpiar
    }
}

int main() {

    BST arboles[26]; // Crear un arreglo de 26 arboles binarios de busqueda (uno por cada letra del abecedario)
    BST eliminar[26];
    cout << "Cargando archivo..." << endl;
    cargar_canciones_desde_csv("spotify_data.csv", arboles);
    cout << "Archivo cargado." << endl;
    int entrada = -1;

    while (entrada != 0) {

        cout << "Menu:" << endl;
        cout << "digite 1 para agregar una cancion" << endl;
        cout << "digite 2 para eliminar una cancion" << endl;
        cout << "digite 3 para buscar una cancion" << endl;
        cout << "digite 4 para imprimir las canciones" << endl;
        cout << "digite 5 para imprimir las canciones segun su criterio de busqueda" << endl;
        cout << "digite 6 para reproducir de manera aleatoria las canciones" << endl;
        cout << "digite 7 para cambiar el orden de las canciones" << endl;
        cout << "digite 0 para salir" << endl;


        cin >> entrada;
        if (entrada == 1) {

            agregarCancion(arboles);
        }

        if (entrada == 2) {

            
            eliminarCancion(arboles);
            
            
        }

        if (entrada == 3) {

            cout << "Ingrese la letra, palabra o frase para buscar: ";
            cin.ignore();
            string consulta;
            getline(cin, consulta);
            buscarCanciones(arboles, consulta);

        }

        if (entrada == 4) {
            for (int i = 0; i < 26; ++i) {
                // Solo imprimir si el arbol tiene canciones

                if (arboles[i].auxiliarRecorrer()) {
                    cout << "Canciones que comienzan con la letra: " << char('A' + i) << endl << endl;
                    arboles[i].recorrer();
                    cout << endl;
                    // Ya se imprime dentro de recorrer(), no es necesario volver a imprimir aqui
                }
            }
            cout << endl;
        }

        if (entrada == 5) {
            string criterio;
            string orden;
            string filtroAnio;

            cout << "Escriba 'popularidad' para ordenar según la popularidad o 'duracion' para ordenar según la duración de la canción" << endl;
            cin >> criterio;
            cout << "Escriba 'ascendente' o 'descendente' para el orden de la impresión" << endl;
            cin >> orden;
            cout << "Escriba un año para buscar en base a ese año, si quiere todas las canciones simplemente ingrese 0" << endl;
            cin >> filtroAnio;

            imprimirCancionesOrdenadas(criterio, orden, filtroAnio);
        }

        if (entrada == 6) {

            imprimirCancionesAleatorias(arboles);
        }

        if (entrada == 7) {

            int pos1;
            int pos2;

            cout << "Ingrese la posicion de la cancion" << endl;
            cin >> pos1;
            cout << "Ingrese la posicion a la que se desea cambiar" << endl;
            cin >> pos2;

            cambiar_orden(arboles, pos1, pos2);
        }
        if (entrada == 0) {

            limpiarTodosLosBST(arboles);
             cout << "Limpiao" << endl;
            return 0;
        }
        

    }

}
```
### Descripcion:
#### Estructura del Proyecto
##### Estructura Cancion
Almacena información detallada de una canción, incluyendo atributos como nombre del artista, título de la canción, popularidad, género, duración, entre otros. Además, implementa una función hash() para generar un identificador único basado en atributos clave de la canción.

##### Clase NodoBST
Representa un nodo en el árbol binario de búsqueda. Contiene una clave (hash y canción), un puntero al hijo izquierdo y un puntero al hijo derecho.

##### Clase BST
Implementa un árbol binario de búsqueda con las siguientes operaciones:

- Insertar: Agrega canciones al árbol con base en su hash.
- Eliminar: Busca y elimina una canción por su nombre.
- Recorrer: Imprime todas las canciones almacenadas en orden alfabético.
- Buscar por prefijo: Encuentra canciones cuyo título comienza con un texto específico.
- Limpieza: Libera la memoria de los nodos del árbol.

##### Funciones Globales
- Carga desde CSV: Lee canciones desde un archivo CSV y las organiza en 26 árboles BST, uno por cada letra del alfabeto.
- Agregar canción: Permite al usuario ingresar manualmente los datos de una canción y agregarla al árbol correspondiente.
- Eliminar canción: Elimina una canción específica según su título.
- Buscar canciones: Busca canciones por letra, palabra o frase en su título.
- Ordenar canciones: Ordena canciones por popularidad o duración en orden ascendente o descendente.
- Reproducción aleatoria: Mezcla e imprime las canciones en un orden aleatorio.
- Intercambiar canciones: Cambia el orden de dos canciones en la lista.

##### Archivo Principal (main)
El programa principal proporciona un menú interactivo con las siguientes opciones:

- Agregar una canción.
- Eliminar una canción.
- Buscar una canción.
- Imprimir todas las canciones.
- Imprimir canciones ordenadas por popularidad o duración.
- Reproducir canciones de forma aleatoria.
- Intercambiar el orden de dos canciones.
- Salir del programa.
- Al finalizar, limpia toda la memoria utilizada por los árboles.

##### Notas Importantes
- El código organiza las canciones en 26 árboles BST, facilitando búsquedas por letras iniciales.
- La funcionalidad de ordenamiento y reproducción aleatoria utiliza estructuras dinámicas y algoritmos eficientes para manejo de datos.
- Todas las operaciones están optimizadas para manejar archivos grandes, pero requieren que el archivo CSV esté correctamente formateado.
