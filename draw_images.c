#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include "draw_images.h"
#include "calc.h"
#include "constants.h"

void load_gui_images(int frame_height) {
	fuente = cvFont(1, 2);
	play = cvLoadImage(MENU_PLAY_IMAGE, CV_LOAD_IMAGE_COLOR);
	nivel = cvLoadImage(MENU_LEVEL_IMAGE, CV_LOAD_IMAGE_COLOR);
	end = cvLoadImage(MENU_EXIT_IMAGE, CV_LOAD_IMAGE_COLOR);
	title = cvLoadImage(MENU_TITLE_IMAGE, CV_LOAD_IMAGE_COLOR);
	title2 = cvLoadImage(MENU_SUBTITLE_IMAGE, CV_LOAD_IMAGE_COLOR);
	bubble = cvLoadImage(BUBBLE_IMAGE, CV_LOAD_IMAGE_COLOR);
	life_rem_bar_orig = cvLoadImage(LIFE_REM_BAR_IMAGE, CV_LOAD_IMAGE_COLOR);
	game_over = cvLoadImage(GAME_OVER_IMAGE, CV_LOAD_IMAGE_COLOR);
	back = cvLoadImage(BACK_IMAGE, CV_LOAD_IMAGE_COLOR);
	easy = cvLoadImage(EASY_IMAGE, CV_LOAD_IMAGE_COLOR);
	medium = cvLoadImage(MEDIUM_IMAGE, CV_LOAD_IMAGE_COLOR);
	hard = cvLoadImage(HARD_IMAGE, CV_LOAD_IMAGE_COLOR);
	arrow = cvLoadImage(RIGHT_ARROW_IMAGE, CV_LOAD_IMAGE_COLOR);
	pop = cvLoadImage(POP_IMAGE, CV_LOAD_IMAGE_COLOR);

	IplImage* life_text_orig =
			cvLoadImage(LIFE_TEXT_IMAGE, CV_LOAD_IMAGE_COLOR);
	life_text = cvCreateImage(cvSize(55, 100), life_text_orig->depth,
			life_text_orig->nChannels);
	cvResize(life_text_orig, life_text, CV_INTER_LINEAR);
	cvReleaseImage(&life_text_orig);

	IplImage* life_bar_orig = cvLoadImage(LIFE_BAR_IMAGE, CV_LOAD_IMAGE_COLOR);
	life_bar = cvCreateImage(cvSize(life_bar_orig->width, frame_height
			- life_text->height - PADDING), life_bar_orig->depth,
			life_bar_orig->nChannels);
	cvResize(life_bar_orig, life_bar, CV_INTER_LINEAR);
	cvReleaseImage(&life_bar_orig);

	// Secuencia para las ROIs (regions of interest) de las burbujas
	storage = cvCreateMemStorage(0);
	rois = cvCreateSeq(0, sizeof(CvSeq), sizeof(int) * 5, storage);
}

void draw_gui_play(IplImage* frame, int life_remaining, int points) {

	// Ponemos las letras
	cvSetImageROI(frame, cvRect(0, frame->height - life_text->height,
			life_text->width, life_text->height));
	cvAdd(life_text, frame, frame, NULL);

	// Ponemos el marco de la vida
	cvSetImageROI(frame, cvRect((life_text->width - life_bar->width) / 2,
			PADDING, life_bar->width, life_bar->height));
	cvAdd(life_bar, frame, frame, NULL);

	// Ponemos la barra de vida restante
	if (life_remaining > 0) {
		IplImage* life_rem_bar = cvCreateImage(cvSize(life_rem_bar_orig->width,
				(frame->height - life_text->height - PADDING) * life_remaining
						/ MAX_LIFE), life_rem_bar_orig->depth,
				life_rem_bar_orig->nChannels);
		cvResize(life_rem_bar_orig, life_rem_bar, CV_INTER_LINEAR);
		int init_height = (frame->height - life_text->height - PADDING);
		cvSetImageROI(frame, cvRect((life_text->width - life_rem_bar->width)
				/ 2, PADDING + (init_height - life_rem_bar->height),
				life_rem_bar->width, life_rem_bar->height));
		cvAdd(life_rem_bar, frame, frame, NULL);
		cvReleaseImage(&life_rem_bar);
	}

	// Ponemos el contador de los puntos
	cvResetImageROI(frame);
	char points_text_char[20];
	sprintf(points_text_char, "Puntuacion: %d", points);
	CvSize font_size;
	cvGetTextSize(points_text_char, &fuente, &font_size, 0);
	cvPutText(frame, points_text_char, cvPoint(frame->width - font_size.width
			- PADDING, PADDING), &fuente, CV_RGB(0,0,255));
}

void draw_gui_menu(IplImage* frame, int altura) {
	int x_play = 0, y_play = 0;
	cvSetImageROI(frame, cvRect(x_play, y_play, play->width, play->height));
	cvAdd(play, frame, frame, NULL);

	int x_level = play->width + (((frame->width - play->width - end->width)
			- nivel->width) / 2), y_level = 0;
	cvSetImageROI(frame, cvRect(x_level, y_level, nivel->width, nivel->height));
	cvAdd(nivel, frame, frame, NULL);

	int x_exit = frame->width - end->width, y_exit = 0;
	cvSetImageROI(frame, cvRect(x_exit, y_exit, end->width, end->height));
	cvAdd(end, frame, frame, NULL);

	// Y Ahora el titulo
	cvSetImageROI(frame, cvRect((frame->width - title->width) / 2, altura,
			title->width, title->height));
	cvAdd(title, frame, frame, NULL);

	if (altura < (frame->height - title->height) / 2) {
		int alt = altura + title->height + 20;
		cvSetImageROI(frame, cvRect((frame->width - title2->width) / 2, alt,
				title2->width, title2->height));
		cvAdd(title2, frame, frame, NULL);
	}
}

void draw_gui_config(IplImage* frame, int difficulty) {
	int x_back = frame->width - back->width, y_back = frame->height
			- back->height;
	cvSetImageROI(frame, cvRect(x_back, y_back, back->width, back->height));
	cvAdd(back, frame, frame, NULL);

	IplImage* imagen;
	switch (difficulty) {
	case DIFF_EASY:
		imagen = easy;
		break;
	case DIFF_MEDIUM:
		imagen = medium;
		break;
	case DIFF_HARD:
		imagen = hard;
		break;
	}

	int x_diff = (frame->width - imagen->width) / 2, y_diff = PADDING;
	cvSetImageROI(frame, cvRect(x_diff, y_diff, imagen->width, imagen->height));
	cvAdd(imagen, frame, frame, NULL);

	// Y Ahora las flechas
	int x_arrow = x_diff - arrow->width, y_arrow = y_diff;
	cvSetImageROI(frame, cvRect(x_arrow, y_arrow, arrow->width, arrow->height));
	cvFlip(arrow, arrow, 1);
	cvAdd(arrow, frame, frame, NULL);

	x_arrow = x_diff + imagen->width;
	cvSetImageROI(frame, cvRect(x_arrow, y_arrow, arrow->width, arrow->height));
	cvFlip(arrow, arrow, 1);
	cvAdd(arrow, frame, frame, NULL);
}

void draw_new_ball(IplImage* frame) {
	int radio = rand() % BALL_MAX_RADIUS + BALL_MIN_RADIUS;
	int pos_x = rand() % (frame->width - radio * 2) + radio;
	int pos_y = 0;
	int explot = 0;
	int durac = POP_TIME;
	int roi[5] = { pos_x, pos_y, radio, explot, durac };
	cvSeqPush(rois, roi); // Meto el elemento en la secuencia
	cvSetImageROI(frame, cvRect(pos_x, pos_y, 2 * radio, 2 * radio));
	IplImage* bubble_esc = cvCreateImage(cvSize(2 * radio, 2 * radio),
			bubble->depth, bubble->nChannels);
	cvResize(bubble, bubble_esc, CV_INTER_LINEAR);
	cvAdd(bubble_esc, frame, frame, NULL);
	cvReleaseImage(&bubble_esc);
}

/**
 * Mueve las bolas por la pantalla.
 * Si llegan al final de esta resta vida.
 * Devuelve 1 si la vida pasa de 0
 */
int draw_move_balls(IplImage* frame, int *life_remaining, int velocity) {
	int i;
	for (i = 0; i < rois->total; i++) {
		int* p = (int*) cvGetSeqElem(rois, i);

		// Comprobamos si la burbuja esta explotada
		if (p[3]) {
			// Si es así entonces la pintamos
			cvSetImageROI(frame, cvRect(p[0], p[1], 2 * p[2], 2 * p[2]));
			IplImage* pop_esc = cvCreateImage(cvSize(2 * p[2], 2 * p[2]),
					pop->depth, pop->nChannels);
			cvResize(pop, pop_esc, CV_INTER_LINEAR);
			cvAdd(pop_esc, frame, frame, NULL);
			cvReleaseImage(&pop_esc);
			// La decrementamos
			p[4]--;
			// Y si su tiempo ha terminado, la eliminamos
			if (p[4] <= 0)
				cvSeqRemove(rois, i);
		} else { // Si la burbuja aun no se ha explotado

			p[1] += velocity; // La movemos

			// Si el circulo ha llegado al final de la
			// pantalla lo eliminamos de la lista
			if (p[1] + 2 * p[2] >= frame->height) {
				// Quitamos vida
				(*life_remaining) -= calc_damage(p[2]);
				if ((*life_remaining) <= 0)
					return 1;
				cvSeqRemove(rois, i);
				continue;
			}

			cvSetImageROI(frame, cvRect(p[0], p[1], 2 * p[2], 2 * p[2]));
			IplImage* bubble_esc = cvCreateImage(cvSize(2 * p[2], 2 * p[2]),
					bubble->depth, bubble->nChannels);
			cvResize(bubble, bubble_esc, CV_INTER_LINEAR);
			cvAdd(bubble_esc, frame, frame, NULL);
			cvReleaseImage(&bubble_esc);
		}
	}
	return 0;
}

void free_gui_images() {
	cvReleaseImage(&play);
	cvReleaseImage(&nivel);
	cvReleaseImage(&end);
	cvReleaseImage(&title);
	cvReleaseImage(&title2);
	cvReleaseImage(&bubble);
	cvReleaseImage(&life_text);
	cvReleaseImage(&life_rem_bar_orig);
	cvReleaseImage(&game_over);
	cvReleaseImage(&life_bar);
	cvReleaseImage(&back);
	cvReleaseImage(&easy);
	cvReleaseImage(&medium);
	cvReleaseImage(&hard);
	cvReleaseImage(&arrow);
	cvReleaseImage(&pop);
	cvReleaseMemStorage(&storage);
}
