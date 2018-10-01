#include <cv.h>
#include <highgui.h>
#include "calc.h"
#include "draw_images.h"
#include "constants.h"

int calc_damage(int radius) {
	return (int) (BALL_MAX_DAMAGE * radius / BALL_MAX_RADIUS);
}

int calc_punt(int radius) {
	return (int) (BALL_MAX_PUNT * radius / BALL_MAX_PUNT);
}
