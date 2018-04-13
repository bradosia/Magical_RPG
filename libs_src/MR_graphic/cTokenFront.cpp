#include "cTokenFront.h"

cTokenFront::cTokenFront() {
}
cTokenFront::~cTokenFront() {
}

void cTokenFront::setTarget(cToken* targetPtr_) {
	targetPtr = targetPtr_;
}

void cTokenFront::setSound(cSound* soundPtr_) {
	soundPtr = soundPtr_;
}

void cTokenFront::front(std::vector<std::string>* frontList) {
	int i, n;
	n = frontList->size();
	for (i = 0; i < n; i++) {
		cmd((*frontList)[i]);
	}
	frontList->clear();
}

void cTokenFront::cmd(std::string cmd_) {
	if (cmd_ == "give_powerup") {
		soundPtr->Play(SOUND_TOKEN);
	}
}

void cTokenFront::Draw(int tex_id, int tex_w, int tex_h, bool run) {
	float xo, yo, xf, yf;
	float tex_offset_x, tex_offset_y;

	tex_offset_x = 1.0f / float(tex_w / TEXTURE_TILE_SIZE);
	tex_offset_y = 1.0f / float(tex_h / TEXTURE_TILE_SIZE);

	xo = tex_offset_x * 0 + (GetFrame() * tex_offset_x);
	yo = tex_offset_y + tex_offset_y * 0;
	if (run)
		NextFrame(4); //total de frames de la animacion

	xf = xo + tex_offset_x;
	yf = yo - tex_offset_y;

	int screen_x, screen_y;

	screen_x = targetPtr->x + SCENE_Xo;
	screen_y = targetPtr->y + SCENE_Yo;

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, tex_id);
	glBegin(GL_QUADS);
	glTexCoord2f(xo, yo);
	glVertex3i(screen_x, screen_y, targetPtr->y - targetPtr->height_overground);
	glTexCoord2f(xf, yo);
	glVertex3i(screen_x + TILE_SIZE, screen_y,
			targetPtr->y - targetPtr->height_overground);
	glTexCoord2f(xf, yf);
	glVertex3i(screen_x + TILE_SIZE, screen_y + TILE_SIZE,
			targetPtr->y - targetPtr->height_overground);
	glTexCoord2f(xo, yf);
	glVertex3i(screen_x, screen_y + TILE_SIZE,
			targetPtr->y - targetPtr->height_overground);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}
void cTokenFront::NextFrame(int max) {
	targetPtr->delay++;
	if (targetPtr->delay == targetPtr->frame_delay) {
		targetPtr->seq++;
		targetPtr->seq %= max;
		targetPtr->delay = 0;
	}
}
int cTokenFront::GetFrame() {
	return targetPtr->seq;
}
