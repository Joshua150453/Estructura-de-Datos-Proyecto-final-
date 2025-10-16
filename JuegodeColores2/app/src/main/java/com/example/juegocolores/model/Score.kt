package com.example.juegocolores.model

// Clase de datos para representar un puntaje con su valor y la fecha en que se obtuvo
data class Score(
    val value: Int,        // Puntaje obtenido
    val timestamp: Long    // Momento en que se registró el puntaje
)
