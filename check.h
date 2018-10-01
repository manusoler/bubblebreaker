#ifndef CHECK_H_
#define CHECK_H_

#define DRAW_RECTANGLE 0

/**
 * Detecta la cabeza. Devuelve una secuencia de CvRec* que contiene la localizacion de las caras encontradas
 * en la imagen img.
 */
CvSeq* detect_head(IplImage* img, CvMemStorage* storage_head, CvHaarClassifierCascade* cascade);

/**
 * Detecta las manos con guantes negros.
 * Devuelve una imagen binaria donde lo blanco corresponde a objetos negros.
 */
IplImage* detect_hand(IplImage *image);

/**
 * Comprueba si la imagen binaria image posee suficientes puntos blancos en el rectangulo pasado.
 * En caso de poseer mas de 1/4 del area del rectangulo devuelve 1, 0 en caso contrario.
 */
int check_touched(IplImage* image,int x, int y, int width, int height);

#endif /* CHECK_H_ */
