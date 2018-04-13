#include "cProjectileFront.h"

cProjectileFront::cProjectileFront() {
}
cProjectileFront::~cProjectileFront() {
}

void cProjectileFront::setTarget(cProjectile* targetPtr_) {
	targetPtr = targetPtr_;
}

void cProjectileFront::setSound(cSound* soundPtr_) {
	soundPtr = soundPtr_;
}

void cProjectileFront::front(std::vector<std::string>* frontList) {
	int i, n;
	n = frontList->size();
	for (i = 0; i < n; i++) {
		cmd((*frontList)[i]);
	}
	frontList->clear();
}

void cProjectileFront::cmd(std::string cmd_) {
	if (cmd_ == "give_powerup") {
		soundPtr->Play(SOUND_TOKEN);
	}
}

void cProjectileFront::Draw(int tex_id, int tex_w, int tex_h, bool run) {
	if (targetPtr->getType() == 0) {
		//fireball
		drawFireball(tex_id, tex_w, tex_h, run);
	} else if (targetPtr->getType() == 1) {
		//arrow
		drawArrow(tex_id, tex_w, tex_h, run);
	}
}

void cProjectileFront::DrawRect(int tex_id, float xo, float yo, float xf,
		float yf) {
	int screen_x, screen_y;

	screen_x = targetPtr->x + SCENE_Xo;
	screen_y = targetPtr->y + SCENE_Yo;

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, tex_id);
	glBegin(GL_QUADS);
	glTexCoord2f(xo, yo);
	glVertex3i(
			screen_x
					- (TILE_SIZE * targetPtr->frame_texture_tiles
							- targetPtr->hit_w) / 2,
			screen_y
					- (TILE_SIZE * targetPtr->frame_texture_tiles
							- targetPtr->hit_h) / 2, screen_y); //z ligeramente superior para dar sensacion de que esta por encima del nivel del suelo
	glTexCoord2f(xf, yo);
	glVertex3i(
			screen_x
					+ (TILE_SIZE * targetPtr->frame_texture_tiles
							+ targetPtr->hit_w) / 2,
			screen_y
					- (TILE_SIZE * targetPtr->frame_texture_tiles
							- targetPtr->hit_h) / 2, screen_y);
	glTexCoord2f(xf, yf);
	glVertex3i(
			screen_x
					+ (TILE_SIZE * targetPtr->frame_texture_tiles
							+ targetPtr->hit_w) / 2,
			screen_y
					+ (TILE_SIZE * targetPtr->frame_texture_tiles
							+ targetPtr->hit_h) / 2, screen_y);
	glTexCoord2f(xo, yf);
	glVertex3i(
			screen_x
					- (TILE_SIZE * targetPtr->frame_texture_tiles
							- targetPtr->hit_w) / 2,
			screen_y
					+ (TILE_SIZE * targetPtr->frame_texture_tiles
							+ targetPtr->hit_h) / 2, screen_y);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cProjectileFront::drawFireball(int tex_id, int tex_w, int tex_h,
		bool run) {
	float xo, yo, xf, yf;
	bool reverse = false;
	float tex_offset_x, tex_offset_y;

	tex_offset_x =
			1.0f
					/ float(
							tex_w
									/ (targetPtr->GetFrameTexTiles()
											* TEXTURE_TILE_SIZE));
	tex_offset_y =
			1.0f
					/ float(
							tex_h
									/ (targetPtr->GetFrameTexTiles()
											* TEXTURE_TILE_SIZE));

	switch (targetPtr->GetState()) {
	case STATE_LEVITATEUP:
		xo = tex_offset_x * 0 + (GetFrame() * tex_offset_x);
		yo = tex_offset_y + tex_offset_y * 1;
		if (run)
			NextFrame(3);
		break;
	case STATE_LEVITATERIGHT:
		xo = tex_offset_x * 0 + (GetFrame() * tex_offset_x);
		yo = tex_offset_y + tex_offset_y * 2;
		if (run)
			NextFrame(3);
		break;
	case STATE_LEVITATEDOWN:
		xo = tex_offset_x * 0 + (GetFrame() * tex_offset_x);
		yo = tex_offset_y + tex_offset_y * 0;
		if (run)
			NextFrame(3);
		break;
	case STATE_LEVITATELEFT:
		xo = tex_offset_x * 0 + (GetFrame() * tex_offset_x);
		yo = tex_offset_y + tex_offset_y * 2;
		reverse = true;
		if (run)
			NextFrame(3);
		break;

	case STATE_ENDINGUP:
		if (GetFrame() <= 3) {
			xo = tex_offset_x * 0 + (GetFrame() * tex_offset_x);
			yo = tex_offset_y + tex_offset_y * 3;
		} else {
			xo = tex_offset_x * 0 + (GetFrame() * tex_offset_x);
			yo = tex_offset_y + tex_offset_y * 4;
		}
		if (run)
			NextFrame(6);
		break;
	case STATE_ENDINGRIGHT:
		if (GetFrame() <= 3) {
			xo = tex_offset_x * 0 + (GetFrame() * tex_offset_x);
			yo = tex_offset_y + tex_offset_y * 3;
		} else {
			xo = tex_offset_x * 0 + (GetFrame() * tex_offset_x);
			yo = tex_offset_y + tex_offset_y * 4;
		}
		if (run)
			NextFrame(6);
		break;
	case STATE_ENDINGDOWN:
		if (GetFrame() <= 3) {
			xo = tex_offset_x * 0 + (GetFrame() * tex_offset_x);
			yo = tex_offset_y + tex_offset_y * 3;
		} else {
			xo = tex_offset_x * 0 + (GetFrame() * tex_offset_x);
			yo = tex_offset_y + tex_offset_y * 4;
		}
		if (run)
			NextFrame(6);
		break;
	case STATE_ENDINGLEFT:
		if (GetFrame() <= 3) {
			xo = tex_offset_x * 0 + (GetFrame() * tex_offset_x);
			yo = tex_offset_y + tex_offset_y * 3;
		} else {
			xo = tex_offset_x * 0 + (GetFrame() * tex_offset_x);
			yo = tex_offset_y + tex_offset_y * 4;
		}
		if (run)
			NextFrame(6);
		break;
	}
	xf = xo + tex_offset_x;
	yf = yo - tex_offset_y;
	if (reverse) {
		float aux = xo;
		xo = xf;
		xf = aux;
	}
	DrawRect(tex_id, xo, yo, xf, yf);
}

void cProjectileFront::drawArrow(int tex_id,int tex_w,int tex_h,bool run)
{
	float xo,yo,xf,yf;
	bool reverse=false;
	float tex_offset_x,tex_offset_y;

	tex_offset_x = 1.0f/float(tex_w/(targetPtr->GetFrameTexTiles()*TEXTURE_TILE_SIZE));
	tex_offset_y = 1.0f/float(tex_h/(targetPtr->GetFrameTexTiles()*TEXTURE_TILE_SIZE));

	switch(targetPtr->GetState())
	{
		case STATE_LEVITATEUP:		xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*0;
									break;
		case STATE_LEVITATERIGHT:	xo = tex_offset_x*2 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*0;
									break;
		case STATE_LEVITATEDOWN:	xo = tex_offset_x*1 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*0;
									break;
		case STATE_LEVITATELEFT:	xo = tex_offset_x*2 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*0;
									reverse = true;
									break;

		case STATE_ENDINGUP:		xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*0;
									NextFrame(1);
									break;
		case STATE_ENDINGRIGHT:		xo = tex_offset_x*2 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*0;
									NextFrame(1);
									break;
		case STATE_ENDINGDOWN:		xo = tex_offset_x*1 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*0;
									NextFrame(1);
									break;
		case STATE_ENDINGLEFT:		xo = tex_offset_x*2 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*0;
									NextFrame(1);
									reverse = true;
									break;
	}
	xf = xo + tex_offset_x;
	yf = yo - tex_offset_y;
	if(reverse)
	{
		float aux = xo;
		xo = xf;  xf = aux;
	}
	DrawRect(tex_id,xo,yo,xf,yf);
}

void cProjectileFront::NextFrame(int max) {
	targetPtr->delay++;
	if (targetPtr->delay == targetPtr->frame_delay) {
		targetPtr->seq++;
		targetPtr->seq %= max;
		targetPtr->delay = 0;
	}
}
int cProjectileFront::GetFrame() {
	return targetPtr->seq;
}
