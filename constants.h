#ifndef CONSTANTS_H_
#define CONSTANTS_H_


#define BALL_MAX_PUNT 50				// Puntuacion maxima de una burbuja
#define BALL_MIN_PUNT 15				// Puntuacion minima de una burbuja

#define BALL_MAX_DAMAGE 20				// Daño maximo de una burbuja

#define BALL_MAX_RADIUS 40				// Radio maxima de una burbuja
#define BALL_MIN_RADIUS 10				// Radio minimo de una burbuja

#define BALL_MOVEMENT_EASY 5			// Velocidad de movimiento de la burbuja
#define BALL_MOVEMENT_MEDIUM 10			// Velocidad de movimiento de la burbuja
#define BALL_MOVEMENT_HARD 15			// Velocidad de movimiento de la burbuja

#define FRAME_T_EASY 30					// Indica cada cuantos frames se crean nuevas bolas
#define FRAME_T_MEDIUM 20				// Indica cada cuantos frames se crean nuevas bolas
#define FRAME_T_HARD 10					// Indica cada cuantos frames se crean nuevas bolas

#define POP_TIME 5						// Duracion de la explosion de la burbuja

#define PADDING 20						// Espacio de algunos objetos con respecto al borde de la ventana

#define MAX_LIFE 100					// Maximo de vida del jugador

#define CASCADE_NAME "haars/haarcascade_frontalface_alt_tree.xml"//"aGest.xml"

#define TOCADO 100 						//TODO: Valor que indica cuando se considera una bola tocada (pixels)


/**************
 * DIFICULTAD *
 **************/
#define DIFF_EASY 0			// Dificultad facil
#define DIFF_MEDIUM 1		// Dificultad media
#define DIFF_HARD 2			// Dificultad dificil

/********************
 * ESTADO DEL JUEGO *
 ********************/
#define STATE_MENU 0			// Menu del juego
#define STATE_CONFIG 1			// Configuracion
#define STATE_PLAY 2			// Juego
#define STATE_PAUSE 3			// Pause
#define STATE_GAME_OVER 4		// Juego terminado
#define STATE_END 5				// Salir

/************
 * VENTANAS *
 ************/
#define WINDOW_NAME "Videojuego"	// Ventana en la que se ve el videojuego
#define WINDOW_NAME2 "Movimiento"	// Ventana en la que se ve el movimiento de las manos

/************
 * IMAGENES *
 ************/
#define MENU_PLAY_IMAGE "imgs/jugar.png"					// Imagen del boton jugar del menu
#define MENU_LEVEL_IMAGE "imgs/nivel.png"					// Imagen del boton nivel del menu
#define MENU_EXIT_IMAGE "imgs/salir.png"					// Imagen del boton salir del menu
#define MENU_TITLE_IMAGE "imgs/titulo.png"					// Imagen del titulo del juego
#define MENU_SUBTITLE_IMAGE "imgs/titulo2.png"				// Imagen del subtitulo del juego
#define BUBBLE_IMAGE "imgs/bubble.jpg"						// Imagen de las burbujas
#define LIFE_TEXT_IMAGE "imgs/texto_vida.jpg"				// Imagen del texto VIDA
#define LIFE_BAR_IMAGE "imgs/barra_vida.jpg"				// Imagen del borde de la barra de vida
#define LIFE_REM_BAR_IMAGE "imgs/barra_vida_restante.jpg"	// Imagen de la vida restante
#define GAME_OVER_IMAGE "imgs/gameover.jpg"					// Imagen del fin de juego
#define EASY_IMAGE "imgs/facil.png"							// Imagen del nivel facil
#define MEDIUM_IMAGE "imgs/medio.png"						// Imagen del nivel medio
#define HARD_IMAGE "imgs/dificil.png"						// Imagen del nivel dificil
#define BACK_IMAGE "imgs/atras.png"							// Imagen de atras
#define RIGHT_ARROW_IMAGE "imgs/red_arrow.png"				// Imagen de la flecha
#define POP_IMAGE "imgs/pop.png"							// Imagen de la burbuja al explotar

#endif /* CONSTANTS_H_ */
