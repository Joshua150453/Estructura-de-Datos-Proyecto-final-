#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <functional> // Para std::hash
#include <algorithm> // Para std::transform

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

    // Generar un hash único basado en el nombre de la canción
    size_t hash() const {
        const size_t prime1 = 0x9e3779b97f4a7c15; // Número primo grande
        const size_t prime2 = 0x85ebca6b;
        const size_t prime3 = 0xc2b2ae35;
        const size_t seed = 0x27d4eb2f165667c5;

        auto combine = [](size_t hash, const std::string &str) {
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

        // Mezcla final para mayor dispersión
        result ^= (result >> 33);
        result *= prime3;
        result ^= (result >> 29);
        result *= prime1;
        result ^= (result >> 32);

        return result;
    }
};

// Clase para representar un nodo del árbol binario
class NodoBST {
public:
    pair<size_t, Cancion> clave;  // Clave (hash y canción)
    NodoBST* izquierdo;  // Puntero al hijo izquierdo
    NodoBST* derecho;    // Puntero al hijo derecho

    NodoBST(const pair<size_t, Cancion>& _clave) : clave(_clave), izquierdo(nullptr), derecho(nullptr) {}
};

// Clase para el árbol binario de búsqueda
class BST {
private:
    NodoBST* raiz;

    // Función auxiliar para insertar en el árbol binario de búsqueda
    void insertarRecursivo(NodoBST*& nodo, const pair<size_t, Cancion>& clave) {
        if (nodo == nullptr) {
            nodo = new NodoBST(clave);
        } else if (clave.first < nodo->clave.first) {
            insertarRecursivo(nodo->izquierdo, clave);
        } else {
            insertarRecursivo(nodo->derecho, clave);
        }
    }

    // Función auxiliar para recorrer el árbol en orden
    void recorrerRecursivo(NodoBST* nodo) const {
        
        if (nodo != nullptr) {
            recorrerRecursivo(nodo->izquierdo);
            cout << "Canción: " << nodo->clave.second.track_name
                 << " - Artista: " << nodo->clave.second.artist_name << endl;
            recorrerRecursivo(nodo->derecho);
        }
    }

    // Función auxiliar para verificar si el árbol está vacío
    bool estaVacio(NodoBST* nodo) const {
        return nodo == nullptr;
    }
    NodoBST* encontrarMin(NodoBST* nodo) {
        while (nodo && nodo->izquierdo != nullptr) {
            nodo = nodo->izquierdo;
        }
        return nodo;
    }

    // Función auxiliar para eliminar un nodo en el árbol
    NodoBST* eliminarRecursivo(NodoBST* nodo, const string& nombreCancion) {
        if (nodo == nullptr) {
            return nodo; // Nodo no encontrado
        }

        // Comparar el nombre de la canción para decidir el camino
        if (nombreCancion < nodo->clave.second.track_name) {
            nodo->izquierdo = eliminarRecursivo(nodo->izquierdo, nombreCancion);
        } else if (nombreCancion > nodo->clave.second.track_name) {
            nodo->derecho = eliminarRecursivo(nodo->derecho, nombreCancion);
        } else {
            // Nodo encontrado
            if (nodo->izquierdo == nullptr && nodo->derecho == nullptr) {
                // Caso 1: Nodo sin hijos
                delete nodo;
                return nullptr;
            } else if (nodo->izquierdo == nullptr) {
                // Caso 2: Nodo con un solo hijo (derecho)
                NodoBST* temp = nodo->derecho;
                delete nodo;
                return temp;
            } else if (nodo->derecho == nullptr) {
                // Caso 2: Nodo con un solo hijo (izquierdo)
                NodoBST* temp = nodo->izquierdo;
                delete nodo;
                return temp;
            } else {
                // Caso 3: Nodo con dos hijos
                NodoBST* temp = encontrarMin(nodo->derecho); // Encontrar el menor en el subárbol derecho
                nodo->clave = temp->clave; // Reemplazar datos
                nodo->derecho = eliminarRecursivo(nodo->derecho, temp->clave.second.track_name); // Eliminar sucesor
            }
        }
        return nodo;
    }

public:
    BST() : raiz(nullptr) {}

    // Insertar una nueva canción en el árbol
    void insertar(const Cancion& c) {
        size_t hashClave = c.hash(); // Calcular el hash de la canción
        insertarRecursivo(raiz, {hashClave, c});
    }

    // Recorrer el árbol en orden y devolver true si hay elementos
    bool recorrer() const {
        if (estaVacio(raiz)) {
            return false; // Árbol vacío
        }
        recorrerRecursivo(raiz);
        return true; // Árbol no vacío
    }
    
    bool auxiliarRecorrer() const{
        
        if (estaVacio(raiz)) {
            return false; // Árbol vacío
        }else{
            return true;
        }
    }
    bool eliminar(const string& nombreCancion) {
        raiz = eliminarRecursivo(raiz, nombreCancion);
        if(estaVacio(raiz)){
            return false;
        }else{
            
            return true;
        }   
    }
    
    void buscarPorPrefijo(const string& prefijo, vector<string>& resultados) const {
    // Convertir el prefijo a mayúsculas para comparar
    string prefijoMayus = prefijo;
    transform(prefijoMayus.begin(), prefijoMayus.end(), prefijoMayus.begin(), ::toupper);

    buscarPorPrefijoRec(raiz, prefijoMayus, resultados);
}

private:
void buscarPorPrefijoRec(NodoBST* nodo, const string& prefijo, vector<string>& resultados) const {
    if (nodo == nullptr) {
        return; // Base de la recursión: nodo vacío
    }

    // Convertir el nombre de la canción actual a mayúsculas para comparación
    string nombreMayus = nodo->clave.second.track_name;
    transform(nombreMayus.begin(), nombreMayus.end(), nombreMayus.begin(), ::toupper);

    // Si el nombre de la canción comienza con el prefijo, agregar a los resultados
    if (nombreMayus.rfind(prefijo, 0) == 0) { // `rfind` con `0` verifica si comienza con el prefijo
        resultados.push_back(nodo->clave.second.track_name + " - " + nodo->clave.second.artist_name);
    }

    // Recorrer el subárbol izquierdo y derecho
    buscarPorPrefijoRec(nodo->izquierdo, prefijo, resultados);
    buscarPorPrefijoRec(nodo->derecho, prefijo, resultados);
}
};

// Función para leer y cargar canciones desde un archivo CSV
void cargar_canciones_desde_csv(const string& nombre_archivo, BST arboles[26]) {
    ifstream archivo(nombre_archivo);
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
            string campo;
            Cancion cancion;

            // Leer cada campo y asignarlo a la estructura de canción
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

            // Guardar la canción en un vector
            canciones.push_back(cancion);
        }
        archivo.close();

        // Insertar las canciones en el BST correspondiente
        for (size_t i = 0; i < canciones.size(); ++i) {
            char letra_inicial = toupper(canciones[i].track_name[0]);
            if (letra_inicial >= 'A' && letra_inicial <= 'Z') {
                // Insertar en el árbol correspondiente
                arboles[letra_inicial - 'A'].insertar(canciones[i]);
            }
        }

    } else {
        cerr << "No se pudo abrir el archivo " << nombre_archivo << endl;
    }
}
void agregarCancion(BST arboles[]) {
    Cancion nuevaCancion;

    // Solicitar datos de la canción al usuario
    cout << "Ingrese los datos de la canción:\n";
    cout << "Nombre del artista: ";
    cin.ignore(); // Limpiar el buffer de entrada
    getline(cin, nuevaCancion.artist_name);
    cout << "Nombre de la canción: ";
    getline(cin, nuevaCancion.track_name);
    cout << "ID de la canción: ";
    getline(cin, nuevaCancion.track_id);
    cout << "Popularidad: ";
    getline(cin, nuevaCancion.popularity);
    cout << "Año: ";
    getline(cin, nuevaCancion.year);
    cout << "Género: ";
    getline(cin, nuevaCancion.genre);
    cout << "Danceabilidad: ";
    getline(cin, nuevaCancion.danceability);
    cout << "Energía: ";
    getline(cin, nuevaCancion.energy);
    cout << "Clave: ";
    getline(cin, nuevaCancion.key);
    cout << "Sonoridad: ";
    getline(cin, nuevaCancion.loudness);
    cout << "Modo: ";
    getline(cin, nuevaCancion.mode);
    cout << "Habla: ";
    getline(cin, nuevaCancion.speechiness);
    cout << "Acústica: ";
    getline(cin, nuevaCancion.acousticness);
    cout << "Instrumentalidad: ";
    getline(cin, nuevaCancion.instrumentalness);
    cout << "Vivacidad: ";
    getline(cin, nuevaCancion.liveness);
    cout << "Valencia: ";
    getline(cin, nuevaCancion.valence);
    cout << "Tempo: ";
    getline(cin, nuevaCancion.tempo);
    cout << "Duración (ms): ";
    getline(cin, nuevaCancion.duration_ms);
    cout << "Compás: ";
    getline(cin, nuevaCancion.time_signature);

    // Determinar el bucket en función de la letra inicial de la canción
    char letra_inicial = toupper(nuevaCancion.track_name[0]);
    if (letra_inicial >= 'A' && letra_inicial <= 'Z') {
        int indice = letra_inicial - 'A';
        arboles[indice].insertar(nuevaCancion); // Insertar la canción en el árbol correspondiente
        cout << "La canción se ha agregado correctamente al bucket " << letra_inicial << "." << endl;
    } else {
        cerr << "Error: El nombre de la canción no comienza con una letra válida." << endl;
    }
}

void eliminarCancion(BST arboles[26]) {
    string nombreCancion;
    cout << "Ingrese el nombre de la canción a eliminar: ";
    cin.ignore(); // Limpiar el buffer de entrada
    getline(cin, nombreCancion);

    // Determinar el bucket en función de la letra inicial
    char letra_inicial = toupper(nombreCancion[0]);
    if (letra_inicial >= 'A' && letra_inicial <= 'Z') {
        int indice = letra_inicial - 'A';

        // Intentar eliminar la canción del BST correspondiente
        
        if(arboles[indice].eliminar(nombreCancion)){
        
        cout << "Se eliminó la canción: \"" << nombreCancion << "\" en el bucket " << letra_inicial << "." << endl;
        }else{
            
            cout<<"Nombre inválido"<<endl;
        }
            
    }else {
        cout << "Error: El nombre de la canción no comienza con una letra válida." << endl;
    }
    
}


void buscarCanciones(const BST arboles[], const string& consulta) {
    if (consulta.empty()) {
        cout << "Error: La consulta está vacía." << endl;
        return;
    }

    // Convertir consulta a mayúsculas para comparaciones consistentes
    string consultaMayus = consulta;
    transform(consultaMayus.begin(), consultaMayus.end(), consultaMayus.begin(), ::toupper);

    char letraInicial = toupper(consulta[0]);
    if (letraInicial >= 'A' && letraInicial <= 'Z') {
        int indice = letraInicial - 'A';
        cout << "Resultados de búsqueda para \"" << consulta << "\":" << endl<<endl;

        // Buscar en el BST correspondiente
        vector<string> resultados;
        arboles[indice].buscarPorPrefijo(consultaMayus, resultados);
        
         if (!resultados.empty()) {
            for (const string& resultado : resultados) {
                cout << resultado << endl;
            }
            cout<<endl;
        } else {
            cout << "No se encontraron resultados para \"" << consulta << "\"." << endl<<endl;
        }

    } else {
        cout << "Error: La consulta no comienza con una letra válida." << endl;
    }
}




int main() {
    BST arboles[26]; // Crear un arreglo de 26 árboles binarios de búsqueda (uno por cada letra del abecedario)

    cout << "Cargando archivo..." << endl;
    cargar_canciones_desde_csv("spotify_data.csv", arboles);
    cout << "Archivo cargado." << endl;
    
    int confirmacion= 1;
    
    while(confirmacion!=0){

        cout<<"Menú:"<<endl;
        cout<<"digite 1 para agregar una cancion"<<endl;
        cout<<"digite 2 para eliminar una canción"<<endl;
        cout<<"digite 3 para buscar una canción"<<endl;
        cout<<"digite 4 para imprimir las canciones"<<endl;
        cout<<"digite 0 para salir"<<endl;
        int entrada;
        cin>>entrada;
        if(entrada ==1){
            
            agregarCancion(arboles);
        }
        
        if(entrada==2){
            
            eliminarCancion(arboles);
            
        }
        
        if(entrada==3){
            
            cout << "Ingrese la letra, palabra o frase para buscar: ";
                cin.ignore();
                string consulta;
                getline(cin, consulta);
                buscarCanciones(arboles, consulta);
                
        }
    
        if (entrada == 4) {
            for (int i = 0; i < 26; ++i) {
                // Solo imprimir si el árbol tiene canciones
                
                if (arboles[i].auxiliarRecorrer()) {
                    cout << "Canciones que comienzan con la letra: " << char('A' + i) << endl<<endl;
                    arboles[i].recorrer();
                    cout<<endl;
                    // Ya se imprime dentro de `recorrer()`, no es necesario volver a imprimir aquí
                }
            }
            cout<<endl;
        }
        
        if(entrada == 0){
            
            return 0;
        }
    }

    return 0;
}

