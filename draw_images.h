#ifndef DRAW_IMAGES_H_
#define DRAW_IMAGES_H_

IplImage *play, *end, *title, *title2, *bubble, *life_text, *life_bar, *nivel;
IplImage *life_rem_bar_orig, *game_over, *arrow, *easy, *medium, *hard, *back, *pop;
CvMemStorage* storage;
CvSeq* rois;
CvFont fuente;

void load_gui_images(int frame_height);
void draw_gui_play(IplImage* frame, int life_remaining, int points);
void draw_gui_menu(IplImage* frame, int altura);
void draw_gui_config(IplImage* frame, int difficulty);
void draw_new_ball(IplImage* frame);
int draw_move_balls(IplImage* frame, int *life_remaining, int velocity);
void free_gui_images();

#endif /* DRAW_IMAGES_H_ */
