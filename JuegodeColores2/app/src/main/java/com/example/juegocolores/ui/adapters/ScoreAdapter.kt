package com.example.juegocolores.ui.fragments

import android.animation.ObjectAnimator
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.view.animation.DecelerateInterpolator
import androidx.fragment.app.Fragment
import androidx.fragment.app.viewModels
import androidx.lifecycle.lifecycleScope
import androidx.navigation.fragment.findNavController
import com.example.juegocolores.R
import com.example.juegocolores.databinding.FragmentGameBinding
import com.example.juegocolores.model.Score
import com.example.juegocolores.util.ColorUtils
import com.example.juegocolores.util.PrefsHelper
import com.example.juegocolores.viewmodel.GameViewModel
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import java.util.*

class GameFragment : Fragment() {
    private var _binding: FragmentGameBinding? = null
    private val binding get() = _binding!!
    private val viewModel: GameViewModel by viewModels()
    private lateinit var prefs: PrefsHelper

    // Historial de puntajes de la sesión actual (memoria volátil)
    companion object { val sessionScores = mutableListOf<Score>() }

    // Color objetivo actual (el que el usuario debe adivinar)
    private var currentTarget = ColorUtils.randomColor()

    // Infla el layout del fragmento
    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentGameBinding.inflate(inflater, container, false)
        return binding.root
    }

    // Se ejecuta al crearse la vista: inicializa todo el juego
    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        prefs = PrefsHelper(requireContext())
        setupButtons()      // Configura botones de colores
        observeViewModel()  // Observa puntaje y tiempo en pantalla

        // Reinicia el juego al empezar
        viewModel.resetScore()
        viewModel.resetTimer()
        viewModel.startTimer { finishGame() } // Inicia temporizador y define acción al terminar
        pickNewTarget() // Muestra el primer color objetivo
    }

    // Asocia cada botón de color con su función al hacer clic
    private fun setupButtons() {
        binding.btnRed.setOnClickListener { onColorPressed("Rojo") }
        binding.btnGreen.setOnClickListener { onColorPressed("Verde") }
        binding.btnBlue.setOnClickListener { onColorPressed("Azul") }
        binding.btnYellow.setOnClickListener { onColorPressed("Amarillo") }
        binding.btnPurple.setOnClickListener { onColorPressed("Morado") }
    }

    // Lógica que evalúa si el usuario acertó o falló al presionar un color
    private fun onColorPressed(selectedName: String) {
        val hit = selectedName == currentTarget.name
        if (hit) {
            viewModel.incrementScore() // Suma puntaje
            animateSuccess() // Efecto visual
            pickNewTarget()  // Cambia a otro color
        } else {
            animateFailure() // Animación de vibración
        }
    }

    // Animación al acertar (ligero aumento de tamaño)
    private fun animateSuccess() {
        val anim = ObjectAnimator.ofFloat(binding.viewColorBox, "scaleX", 1f, 1.08f, 1f)
        anim.duration = 200
        anim.interpolator = DecelerateInterpolator()
        anim.start()
    }

    // Animación al fallar (vibración lateral)
    private fun animateFailure() {
        lifecycleScope.launch {
            val anim = ObjectAnimator.ofFloat(binding.viewColorBox, "translationX", 0f, 20f, -20f, 0f)
            anim.duration = 300
            anim.start()
            delay(300)
        }
    }

    // Cambia aleatoriamente el color mostrado
    private fun pickNewTarget() {
        currentTarget = ColorUtils.randomColor()
        binding.viewColorBox.setBackgroundColor(currentTarget.colorInt)
        binding.tvColorName.text = currentTarget.name // (Opcional: mostrar nombre del color)
    }

    // Actualiza los elementos visuales cuando cambian los datos del ViewModel
    private fun observeViewModel() {
        viewModel.score.observe(viewLifecycleOwner) { binding.tvScore.text = "Puntaje: $it" }
        viewModel.timeLeft.observe(viewLifecycleOwner) {
            val seconds = (it / 1000).toInt()
            binding.tvTime.text = "Tiempo: ${seconds}s"
        }
    }

    // Acciones al finalizar el juego
    private fun finishGame() {
        val finalScore = viewModel.score.value ?: 0
        prefs.saveHighScore(finalScore) // Guarda mejor puntaje
        sessionScores.add(Score(finalScore, Date().time)) // Agrega al historial de sesión

        // Navega al fragmento de resultados con el puntaje actual
        val action = GameFragmentDirections.actionGameFragmentToResultFragment(finalScore)
        findNavController().navigate(action)
    }

    // Limpieza de recursos cuando se destruye la vista
    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
        viewModel.cancelTimer()
    }
}
