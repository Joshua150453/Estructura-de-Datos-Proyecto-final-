package com.example.juegocolores

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.navigation.fragment.NavHostFragment

// Actividad principal que contiene el NavHostFragment para manejar la navegación
class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        // Asocia la vista principal con el layout activity_main.xml
        setContentView(R.layout.activity_main)
    }
}
