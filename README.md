# Proyecto Reproductor

## Integrantes:
- Abimael Ernesto Frontado Fajardo
- Frederick Dicarlo Mares Graos
- Joshua Diad Ortiz Rosas

## Primer Avance

## Descripción
Este proyecto es una aplicación en C++ para gestionar una lista de reproducción de canciones, utilizando una lista doblemente enlazada para almacenar y manipular la información de las canciones. Permite cargar canciones desde un archivo CSV, agregar y eliminar canciones, cambiar su orden en la lista y reproducirlas de manera aleatoria.

## Características
- Estructura de datos: Implementa una lista doblemente enlazada para gestionar las canciones.
- Carga de datos: Carga canciones desde un archivo CSV y las agrega a la lista de reproducción.
- Agregar y eliminar canciones: Permite añadir nuevas canciones y eliminar canciones existentes de la lista.
- Cambiar el orden de las canciones: Mueve una canción de una posición actual a una nueva posición en la lista.
- Reproducción aleatoria: Reproduce todas las canciones de la lista en un orden aleatorio.
- Impresión de la lista: Imprime la lista de reproducción en la consola.
- Estructura de Datos

## Cancion
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

## ListaDobleEnlazada
Clase que representa la lista doblemente enlazada de canciones y contiene métodos para:

- agregar_cancion: Añade una canción al final de la lista.
- eliminar_cancion: Elimina una canción específica de la lista.
- cambiar_orden: Cambia la posición de una canción dentro de la lista.
- reproduccion_aleatoria: Imprime la lista de canciones en un orden aleatorio.
- imprimir_lista: Muestra todas las canciones de la lista en la consola.
## Archivos en el Proyecto
- main.cpp: Contiene el código fuente con la lógica para cargar y gestionar la lista de reproducción.
- spotify_data.csv: Archivo CSV con los datos de las canciones.

## Ejemplo de Uso
- Al ejecutar el programa, se cargará la lista de reproducción desde el archivo CSV y se imprimirá en la consola.
- Se puede observar la función de cambio de orden de canciones, y la lista se reproducirá de manera aleatoria.
## Notas Importantes
- Este código asume que los datos en el CSV están correctamente formateados.
- Las posiciones de la función cambiar_orden deben ser válidas y estar dentro del rango de la lista actual.
