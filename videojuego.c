#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include "constants.h"
#include "draw_images.h"
#include "check.h"
#include "calc.h"

#define PIEL 1

int estado_videojuego = STATE_MENU;
int dificultad = DIFF_EASY;
int velocidad = BALL_MOVEMENT_EASY, tasa = FRAME_T_EASY;
int erode = 10, dilate = 20;

int main() {

	// Preparamos la captura de video por la webcam
	CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);
	if (!capture) {
		fprintf(stderr, "ERROR: capture is NULL \n");
		return -1;
	}

	int frame_num = 0, gameover = 0, life_remaining = MAX_LIFE, points = 0;

	CvMemStorage* storage_head = cvCreateMemStorage(0);
	CvHaarClassifierCascade* cascade = cvLoadHaarClassifierCascade(
			CASCADE_NAME, cvSize(0, 0));

	// Obtenemos la primera captura
	IplImage* frame = cvQueryFrame(capture);
	if (!frame) {
		printf("Error al capturar de la camara. Revisela por favor.\n");
		return -1;
	}
#if PIEL == 1
	IplImage* hsv_image = cvCreateImage(cvGetSize(frame), 8, 3);
	IplImage* hsv = cvCreateImage(cvGetSize(frame), 8, 1);
	IplImage* mov_ant = 0;
	IplImage* movimiento = cvCreateImage(cvGetSize(frame), 8, 1);
#else
	IplImage* hsv;
#endif

	// Cargamos las imagenes de la interfaz
	load_gui_images(frame->height);

	// Creamos la ventana donde se mostrara la captura
	cvNamedWindow(WINDOW_NAME, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(WINDOW_NAME2, CV_WINDOW_AUTOSIZE);

	int altura = frame->height - title->height; // Altura para el titulo

	while (1) {
		// Obtenemos un frame y lo copiamos
		frame = cvQueryFrame(capture);
		if (!frame) {
			fprintf(stderr, "ERROR: frame is null...\n");
			break;
		}
		cvFlip(frame, frame, 1);

#if PIEL == 1
		cvCvtColor(frame, hsv_image, CV_BGR2HSV);
		cvInRangeS(hsv_image, cvScalar(0, 30, 80, 0), cvScalar(20, 150, 255, 0), hsv);
#else
		hsv = detect_hand(frame);
#endif

		// Erosionar y Dilatar , para la eliminacion de pixeles perdidos
		cvErode(hsv, hsv, 0, erode);
		cvDilate(hsv, hsv, 0, dilate);

		//		if (!mov_ant) {
		//			mov_ant = cvCreateImage(cvGetSize(frame), 8, 1);
		//		} else {
		//			cvSub(hsv, mov_ant, movimiento, NULL);
		//			cvShowImage(WINDOW_NAME2, movimiento);
		//		}
		//
		//		mov_ant = cvCloneImage(hsv);

		if (estado_videojuego == STATE_MENU) {
			// Mostramos el menu en pantalla
			draw_gui_menu(frame, altura);
			if (altura >= (frame->height - title->height) / 2)
				altura -= 3;

			if (check_touched(hsv, 0, 0, play->width, play->height)) { // Comprobar si se ha tocado START
				estado_videojuego = STATE_PLAY;
				gameover = 0;
				life_remaining = MAX_LIFE;
				points = 0;
			} else if (check_touched(hsv, play->width + (((frame->width
					- play->width - end->width) - nivel->width) / 2), 0,
					play->width, play->height)) // Comprobar si se ha tocado NIVEL
				estado_videojuego = STATE_CONFIG;
			else if (check_touched(hsv, frame->width - end->width, 0,
					end->width, end->height)) // Comprobar si se ha tocado EXIT
				estado_videojuego = STATE_END;
		} else if (estado_videojuego == STATE_CONFIG) {

			// Dibujamos el menu de configuracion
			draw_gui_config(frame, dificultad);

			int x_diff = (frame->width - easy->width) / 2, y_diff = PADDING;

			if (check_touched(hsv, frame->width - back->width, frame->height
					- back->height, play->width, play->height)) // Comprobar si se ha tocado BACK
				estado_videojuego = STATE_MENU;
			else if (check_touched(hsv, x_diff - arrow->width, y_diff,
					play->width, play->height)) { // Comprobar si se ha tocado la flecha de la izquierda
				dificultad--;
				if (dificultad < DIFF_EASY) {
					dificultad = DIFF_HARD;
					velocidad = BALL_MOVEMENT_HARD;
					tasa = FRAME_T_HARD;
				} else {
					switch (dificultad) {
					case DIFF_EASY:
						velocidad = BALL_MOVEMENT_EASY;
						tasa = FRAME_T_EASY;
						break;
					case DIFF_MEDIUM:
						velocidad = BALL_MOVEMENT_MEDIUM;
						tasa = FRAME_T_MEDIUM;
						break;
					}
				}
			} else if (check_touched(hsv, x_diff + easy->width, y_diff,
					play->width, play->height)) { // Comprobar si se ha tocado la flecha de la derecha
				dificultad++;
				if (dificultad > DIFF_HARD) {
					dificultad = DIFF_EASY;
					velocidad = BALL_MOVEMENT_EASY;
					tasa = FRAME_T_EASY;
				} else {
					switch (dificultad) {
					case DIFF_HARD:
						velocidad = BALL_MOVEMENT_HARD;
						tasa = FRAME_T_HARD;
						break;
					case DIFF_MEDIUM:
						velocidad = BALL_MOVEMENT_MEDIUM;
						tasa = FRAME_T_MEDIUM;
						break;
					}
				}
			}

		} else if (estado_videojuego == STATE_PLAY) {

			// Si la dificultad no es facil
			if (dificultad > DIFF_EASY) {
				// Detectamos la cara
				CvSeq* faces = detect_head(frame, storage_head, cascade);
				// Comprobamos si hay alguna bola colisionando en la cara
				int i;
				for (i = 0; i < (faces ? faces->total : 0); i++) {
					// Create a new rectangle for drawing the face
					CvRect* r = (CvRect*) cvGetSeqElem(faces, i);
					int j = 0;
					for (; j < rois->total; j++) {
						int* p = (int*) cvGetSeqElem(rois, j);
						if (!p[3]) { // Si la bola no ha sido explotada
							int x1 = p[0] + p[2], y1 = p[1] + p[2];
							if (x1 > r->x && x1 < r->x + r->width && y1 > r->y
									&& y1 < r->y + r->height) {
								life_remaining -= calc_damage(p[2]);
								if (life_remaining <= 0) {
									gameover = 1;
									break;
								}
							}
						}
					}
				}
			}

			// Dibujamos todos los componentes de la interfaz
			draw_gui_play(frame, life_remaining, points);

			if (!gameover) {
				// Dibujamos nuevas peloticas
				if (!(frame_num++ % tasa))
					draw_new_ball(frame);

				// Movemos las bolas ya existentes
				gameover = draw_move_balls(frame, &life_remaining, velocidad);

				// Comprobamos si hemos tocado alguna bola
				int i;
				for (i = 0; i < rois->total; i++) {
					int* p = (int*) cvGetSeqElem(rois, i);
					int x = p[0], y = p[1], lado = 2 * p[2];
					if (check_touched(hsv, x, y, lado, lado)) {
						points += calc_punt(p[2]);
						//cvSeqRemove(rois, i);
						p[3] = 1; // La establecemos a explotada
					}
				}
			} else { // EL JUEGO HA ACABADO
				IplImage* game_over_res = cvCreateImage(cvSize(frame->width,
						frame->height), game_over->depth, game_over->nChannels);
				cvResize(game_over, game_over_res, CV_INTER_LINEAR);
				cvResetImageROI(frame);
				cvAdd(game_over_res, frame, frame, NULL);
				cvReleaseImage(&game_over_res);
			}
		} else if (estado_videojuego == STATE_PAUSE) {
			// TODO: Mostrar Algo
		} else
			// El usuario ha pulsado fin
			break;

		cvResetImageROI(frame);
		if (estado_videojuego != STATE_PAUSE)
			cvShowImage(WINDOW_NAME, frame);
		cvShowImage(WINDOW_NAME2, hsv);
		char tecla = cvWaitKey(10);
		if (tecla == 'm' && estado_videojuego == STATE_PLAY)
			estado_videojuego = STATE_MENU;
		else if (tecla == 'p') {
			if (estado_videojuego == STATE_PAUSE)
				estado_videojuego = STATE_PLAY;
			else if (estado_videojuego == STATE_PLAY)
				estado_videojuego = STATE_PAUSE;
		} else if (tecla == 'q')
			estado_videojuego = STATE_END;
		else if (tecla == 'b' && estado_videojuego == STATE_CONFIG) {
			estado_videojuego = STATE_MENU;
		}
		else if (tecla == 'j' && estado_videojuego == STATE_MENU)
			estado_videojuego = STATE_PLAY;
	}

	free_gui_images();
	cvReleaseCapture(&capture);
	cvReleaseImage(&frame);
	cvReleaseImage(&hsv);
#if PIEL == 1
	cvReleaseImage(&hsv_image);
	cvReleaseImage(&mov_ant);
	cvReleaseImage(&movimiento);
#endif
	cvDestroyAllWindows();
	return 0;
}
