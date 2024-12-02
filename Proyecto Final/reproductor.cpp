#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <functional> // Para std::hash
#include <algorithm> // Para std::transform
#include <ctime> 
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

    NodoBST* eliminarRecursivo(NodoBST* nodo, const string& nombreCancion) {
        if (nodo == nullptr) {
            return nodo; // Nodo no encontrado
        }

        // Normalizar nombres a mayusculas
        string nombreActual = aMayuscula(nodo->clave.second.track_name);
        string nombreBuscado = aMayuscula(nombreCancion);

        if (nombreBuscado < nombreActual) {
            nodo->izquierdo = eliminarRecursivo(nodo->izquierdo, nombreCancion);
        }
        else if (nombreBuscado > nombreActual) {
            nodo->derecho = eliminarRecursivo(nodo->derecho, nombreCancion);
        }
        else {
            // Nodo encontrado: Proceder con eliminacion
            if (nodo->izquierdo == nullptr && nodo->derecho == nullptr) {
                delete nodo;
                return nullptr;
            }
            else if (nodo->izquierdo == nullptr) {
                NodoBST* temp = nodo->derecho;
                delete nodo;
                return temp;
            }
            else if (nodo->derecho == nullptr) {
                NodoBST* temp = nodo->izquierdo;
                delete nodo;
                return temp;
            }
            else {
                NodoBST* temp = encontrarMin(nodo->derecho);
                nodo->clave = temp->clave;
                nodo->derecho = eliminarRecursivo(nodo->derecho, temp->clave.second.track_name);
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
    bool eliminar(const string& nombreCancion) {
        raiz = eliminarRecursivo(raiz, nombreCancion);
        if (estaVacio(raiz)) {
            return false;
        }
        else {

            return true;
        }
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
            canciones.push_back(cancion);
        }
        archivo.close();

        // Insertar las canciones en el BST correspondiente
        for (size_t i = 0; i < canciones.size(); ++i) {
            char letra_inicial = toupper(canciones[i].track_name[0]);
            if (letra_inicial >= 'A' && letra_inicial <= 'Z') {
                // Insertar en el arbol correspondiente
                arboles[letra_inicial - 'A'].insertar(canciones[i]);
            }
        }

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

void eliminarCancion(BST arboles[26]) {
    string nombreCancion;
    cout << "Ingrese el nombre de la cancion a eliminar: ";
    cin.ignore(); // Limpiar el buffer de entrada
    getline(cin, nombreCancion);

    // Determinar el bucket en funcion de la letra inicial
    char letra_inicial = toupper(nombreCancion[0]);
    if (letra_inicial >= 'A' && letra_inicial <= 'Z') {
        int indice = letra_inicial - 'A';

        // Intentar eliminar la cancion del BST correspondiente

        if (arboles[indice].eliminar(nombreCancion)) {

            cout << "Se elimino la cancion: \"" << nombreCancion << "\" en el bucket " << letra_inicial << "." << endl;
        }
        else {

            cout << "Nombre invalido" << endl;
        }

    }
    else {
        cout << "Error: El nombre de la cancion no comienza con una letra valida." << endl;
    }

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

void insertarPopular(BSTaux& Ordenado, vector<Cancion>& canciones, int i, const string& orden) {
    // Verificar si la popularidad es un número válido antes de insertar
    if (!esNumeroValido(canciones[i].popularity)) {
        return;  // Ignorar canciones con popularidad no válida
    }

    // Crear un nodo nuevo para la canción actual
    Nodo* nuevoNodo = new Nodo;
    nuevoNodo->data = canciones[i]; // Asignar la canción actual al nodo
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
        float pop_nueva = stof(canciones[i].popularity);

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
        if (stof(canciones[i].popularity) < stof(padre->data.popularity)) {
            padre->left = nuevoNodo;
        }
        else {
            padre->right = nuevoNodo;
        }
    }
    else { // descendente
        if (stof(canciones[i].popularity) > stof(padre->data.popularity)) {
            padre->left = nuevoNodo;
        }
        else {
            padre->right = nuevoNodo;
        }
    }
}

void insertarDuracion(BSTaux& Ordenado, vector<Cancion>& canciones, int i, const string& orden) {
    // Verificar si la duración es un número válido antes de insertar
    if (!esNumeroValido(canciones[i].duration_ms)) {
        return;  // Ignorar canciones con duración no válida
    }

    // Crear un nodo nuevo para la canción actual
    Nodo* nuevoNodo = new Nodo;
    nuevoNodo->data = canciones[i]; // Asignar la canción actual al nodo
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
        float dur_nueva = stof(canciones[i].duration_ms);

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
        if (stof(canciones[i].duration_ms) < stof(padre->data.duration_ms)) {
            padre->left = nuevoNodo;
        }
        else {
            padre->right = nuevoNodo;
        }
    }
    else { // descendente
        if (stof(canciones[i].duration_ms) > stof(padre->data.duration_ms)) {
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

                canciones.push_back(cancion);
            }
            archivo.close();

            // Insertar las canciones en el BST correspondiente
            for (size_t i = 0; i < canciones.size(); ++i) {
                if (criterio == "popularidad") {
                    insertarPopular(Ordenado, canciones, i, orden);
                }
                else if (criterio == "duracion") {
                    insertarDuracion(Ordenado, canciones, i, orden);
                }
            }

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
