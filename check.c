#include <cv.h>
#include <highgui.h>
#include "check.h"
#include "constants.h"

IplImage* detect_hand(IplImage *image) {

	IplImage* hsv_image = cvCreateImage(cvGetSize(image), 8, 3);
	IplImage* hsv = cvCreateImage(cvGetSize(image), 8, 1);

	int step = hsv_image->widthStep / sizeof(uchar);
	int channels = hsv_image->nChannels;

	int step_mono = hsv->widthStep / sizeof(uchar);
	int channels_mono = hsv->nChannels;

	cvCvtColor(image, hsv_image, CV_BGR2HSV);

	uchar *data_hsv = (uchar *) hsv_image->imageData;
	uchar *data_mono = (uchar *) hsv->imageData;
	// Recorremos la imagen
	int i, j;
	int hlower = 0, hupper = 360, Saturation = 0, Brightness = 20;
	for (i = 0; i < image->height; i++) {
		for (j = 0; j < image->width; j++) {
			// Segmentamos la imagen mediante los angulos de Hue
			if (((data_hsv[i * step + j * channels]) >= hlower) && ((data_hsv[i	* step + j * channels]) <= hupper)) {
				if (data_hsv[i * step + j * channels + 1] >= Saturation) {
					if (data_hsv[i * step + j * channels + 2] >= Brightness)
						data_mono[i * step_mono + j * channels_mono] = 0; // Coloreamos el pixel en negro
					else
						data_mono[i * step_mono + j * channels_mono] = 255; // Coloreamos el pixel en blanco
				} else
					data_mono[i * step_mono + j * channels_mono] = 255; // Coloreamos el pixel en blanco
			} else
				data_mono[i * step_mono + j * channels_mono] = 0; // Coloreamos el pixel en blanco
		}
	}
	return hsv;
}

CvSeq* detect_head(IplImage* img, CvMemStorage* storage_head,
		CvHaarClassifierCascade* cascade) {

	// Clear the memory storage which was used before
	cvClearMemStorage(storage_head);

	// Find whether the cascade is loaded, to find the faces. If yes, then:
	if (cascade) {
		// There can be more than one face in an image. So create a growable sequence of faces.
		// Detect the objects and store them in the sequence
		CvSeq* faces = cvHaarDetectObjects(img, cascade, storage_head, 3, 3, 0,
				cvSize(0, 0));

#if DRAW_RECTANGLE==1
		// Loop the number of faces found.
		int i;
		for (i = 0; i < (faces ? faces->total : 0); i++) {
			// Create a new rectangle for drawing the face
			CvRect* r = (CvRect*) cvGetSeqElem(faces, i);

			// Draw the rectangle in the input image
			cvRectangle(img, cvPoint(r->x, r->y), cvPoint(r->x + r->width, r->y
					+ r->height), CV_RGB(255,0,0), 3, 8, 0);
		}
#endif

		return faces;
	}

	return NULL;
}

int check_touched(IplImage* image, int x, int y, int width, int height) {
	int col, fila, cont = 0;
	for (col = x; col < x + width; col++) {
		for (fila = y; fila < y + height; fila++) {
			CvScalar datos = cvGet2D(image, fila, col);
			if (datos.val[0] == 255)
				cont++;
		}
	}
	if (cont >= width * height / 4)
		return 1;
	return 0;
}
