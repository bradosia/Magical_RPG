#include "cActorFront.h"

cActorFront::cActorFront() {
}
cActorFront::~cActorFront() {
}

void cActorFront::setTarget(cActor* targetPtr_) {
	targetPtr = targetPtr_;
}

void cActorFront::setSound(cSound* soundPtr_) {
	soundPtr = soundPtr_;
}

void cActorFront::front(std::vector<std::string>* frontList) {
	int i, n;
	n = frontList->size();
	for (i = 0; i < n; i++) {
		cmd((*frontList)[i]);
	}
	frontList->clear();
}

void cActorFront::cmd(std::string cmd_) {
	if (cmd_ == "give_powerup") {
		soundPtr->Play(SOUND_TOKEN);
	}
}

void cActorFront::drawPlayer1(int tex_id, int tex_w, int tex_h, bool run) {
	float xo, yo, xf, yf;
	bool reverse = false;
	float tex_offset_x, tex_offset_y;

	tex_offset_x = 1.0f
			/ float(tex_w / (targetPtr->GetFrameTexTiles() * TEXTURE_TILE_SIZE));
	tex_offset_y = 1.0f
			/ float(tex_h / (targetPtr->GetFrameTexTiles() * TEXTURE_TILE_SIZE));

	xo = yo = 0;

	if (targetPtr->animationStateId == STATE_STANDING) {
		if (targetPtr->faceXY == 90) {
			xo = tex_offset_x * 1;
			yo = tex_offset_y + tex_offset_y * 0;
		} else if (targetPtr->faceXY == 0) {
			xo = tex_offset_x * 2;
			yo = tex_offset_y + tex_offset_y * 0;
		} else if (targetPtr->faceXY == 270) {
			xo = tex_offset_x * 0;
			yo = tex_offset_y + tex_offset_y * 0;
		} else if (targetPtr->faceXY == 180) {
			xo = tex_offset_x * 2;
			yo = tex_offset_y + tex_offset_y * 0;
			reverse = true;
		}
	} else if (targetPtr->animationStateId == STATE_SKILL) {
		if (targetPtr->faceXY == 90) {
			xo = tex_offset_x * 1;
			yo = tex_offset_y + tex_offset_y * 9 + (targetPtr->GetFrame() * tex_offset_y);
			if (run)
				targetPtr->NextFrame(2);
		} else if (targetPtr->faceXY == 0) {
			xo = tex_offset_x * 2;
			yo = tex_offset_y + tex_offset_y * 9 + (targetPtr->GetFrame() * tex_offset_y);
			if (run)
				targetPtr->NextFrame(2);
		} else if (targetPtr->faceXY == 270) {
			xo = tex_offset_x * 0;
			yo = tex_offset_y + tex_offset_y * 9 + (targetPtr->GetFrame() * tex_offset_y);
			if (run)
				targetPtr->NextFrame(2);
		} else if (targetPtr->faceXY == 180) {
			xo = tex_offset_x * 2;
			yo = tex_offset_y + tex_offset_y * 9 + (targetPtr->GetFrame() * tex_offset_y);
			if (run)
				targetPtr->NextFrame(2);
			reverse = true;
		}
	} else if (targetPtr->animationStateId == STATE_WALK) {
		if (targetPtr->faceXY == 90) {
			xo = tex_offset_x * 1;
			yo = tex_offset_y + tex_offset_y * 1 + (targetPtr->GetFrame() * tex_offset_y);
			if (run)
				targetPtr->NextFrame(4);
		} else if (targetPtr->faceXY == 0) {
			xo = tex_offset_x * 2;
			yo = tex_offset_y + tex_offset_y * 1 + (targetPtr->GetFrame() * tex_offset_y);
			if (run)
				targetPtr->NextFrame(4);
		} else if (targetPtr->faceXY == 270) {
			xo = tex_offset_x * 0;
			yo = tex_offset_y + tex_offset_y * 1 + (targetPtr->GetFrame() * tex_offset_y);
			if (run)
				targetPtr->NextFrame(4);
		} else if (targetPtr->faceXY == 180) {
			xo = tex_offset_x * 2;
			yo = tex_offset_y + tex_offset_y * 1 + (targetPtr->GetFrame() * tex_offset_y);
			if (run)
				targetPtr->NextFrame(4);
			reverse = true;
		}
	} else if (targetPtr->animationStateId == STATE_SKILLWALK) {
		if (targetPtr->faceXY == 90) {
			xo = tex_offset_x * 1;
			yo = tex_offset_y + tex_offset_y * 10 + (targetPtr->GetFrame() * tex_offset_y);
			if (run)
				targetPtr->NextFrame(2);
		} else if (targetPtr->faceXY == 0) {
			xo = tex_offset_x * 2;
			yo = tex_offset_y + tex_offset_y * 10 + (targetPtr->GetFrame() * tex_offset_y);
			if (run)
				targetPtr->NextFrame(2);
		} else if (targetPtr->faceXY == 270) {
			xo = tex_offset_x * 0;
			yo = tex_offset_y + tex_offset_y * 10 + (targetPtr->GetFrame() * tex_offset_y);
			if (run)
				targetPtr->NextFrame(2);
		} else if (targetPtr->faceXY == 180) {
			xo = tex_offset_x * 2;
			yo = tex_offset_y + tex_offset_y * 10 + (targetPtr->GetFrame() * tex_offset_y);
			if (run)
				targetPtr->NextFrame(2);
			reverse = true;
		}
	} else if (targetPtr->animationStateId == STATE_DAMAGE) {
		if (targetPtr->faceXY == 90) {
			xo = tex_offset_x * 1;
			yo = tex_offset_y + tex_offset_y * 12 + (targetPtr->GetFrame() * tex_offset_y);
			if (run)
				targetPtr->NextFrame(2);
		} else if (targetPtr->faceXY == 0) {
			xo = tex_offset_x * 2;
			yo = tex_offset_y + tex_offset_y * 12 + (targetPtr->GetFrame() * tex_offset_y);
			if (run)
				targetPtr->NextFrame(2);
		} else if (targetPtr->faceXY == 270) {
			xo = tex_offset_x * 0;
			yo = tex_offset_y + tex_offset_y * 12 + (targetPtr->GetFrame() * tex_offset_y);
			if (run)
				targetPtr->NextFrame(2);
		} else if (targetPtr->faceXY == 180) {
			xo = tex_offset_x * 2;
			yo = tex_offset_y + tex_offset_y * 12 + (targetPtr->GetFrame() * tex_offset_y);
			if (run)
				targetPtr->NextFrame(2);
			reverse = true;
		}
	} else if (targetPtr->animationStateId == STATE_ATTACK) {
		if (targetPtr->faceXY == 90) {
			xo = tex_offset_x * 1;
			yo = tex_offset_y + tex_offset_y * 5 + (targetPtr->GetFrame() * tex_offset_y);
			if (run)
				targetPtr->NextFrame(4);
		} else if (targetPtr->faceXY == 0) {
			xo = tex_offset_x * 2;
			yo = tex_offset_y + tex_offset_y * 5 + (targetPtr->GetFrame() * tex_offset_y);
			if (run)
				targetPtr->NextFrame(4);
		} else if (targetPtr->faceXY == 270) {
			xo = tex_offset_x * 0;
			yo = tex_offset_y + tex_offset_y * 5 + (targetPtr->GetFrame() * tex_offset_y);
			if (run)
				targetPtr->NextFrame(4);
		} else if (targetPtr->faceXY == 180) {
			xo = tex_offset_x * 2;
			yo = tex_offset_y + tex_offset_y * 5 + (targetPtr->GetFrame() * tex_offset_y);
			reverse = true;
			if (run)
				targetPtr->NextFrame(4);
		}
	} else if (targetPtr->animationStateId == STATE_SUPERATTACK) {
		xo = tex_offset_x * 3;
		yo = tex_offset_y + tex_offset_y * 1 + (targetPtr->GetFrame() * tex_offset_y);
		if (run)
			targetPtr->NextFrame(7);
	} else if (targetPtr->animationStateId == STATE_DYING) {
		xo = tex_offset_x * 3;
		yo = tex_offset_y + tex_offset_y * 0;
	}

	xf = xo + tex_offset_x;
	yf = yo - tex_offset_y;
	if (reverse) {
		float aux = xo;
		xo = xf;
		xf = aux;
	}
	drawRect(tex_id, xo, yo, xf, yf);
}

void cActorFront::drawRect(int tex_id, float xo, float yo, float xf, float yf) {
	int screen_x, screen_y;
	cRect hitbox = targetPtr->GetHitBox();
	int hitbox_w, hitbox_h;

	screen_x = targetPtr->GetX() + SCENE_Xo;
	screen_y = targetPtr->GetY() + SCENE_Yo;
	hitbox_w = hitbox.right - hitbox.left;
	hitbox_h = hitbox.top - hitbox.bottom;

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, tex_id);
	glBegin(GL_QUADS);
	/*std::cout << "xo=" << xo << " yo=" << yo << " xf=" << xf << " yf=" << yf << std::endl;
	 std::cout << "screen_x=" << screen_x << " screen_y=" << screen_y << " GetX()=" << GetX() << " GetY()=" << GetY() << std::endl;*/
	glTexCoord2f(xo, yo);
	glVertex3i(screen_x - (TILE_SIZE * targetPtr->frame_texture_tiles - hitbox_w) / 2,
			screen_y - (TILE_SIZE * targetPtr->frame_texture_tiles - hitbox_h) / 2,
			screen_y);
	glTexCoord2f(xf, yo);
	glVertex3i(screen_x + (TILE_SIZE * targetPtr->frame_texture_tiles + hitbox_w) / 2,
			screen_y - (TILE_SIZE * targetPtr->frame_texture_tiles - hitbox_h) / 2,
			screen_y);
	glTexCoord2f(xf, yf);
	glVertex3i(screen_x + (TILE_SIZE * targetPtr->frame_texture_tiles + hitbox_w) / 2,
			screen_y + (TILE_SIZE * targetPtr->frame_texture_tiles + hitbox_h) / 2,
			screen_y);
	glTexCoord2f(xo, yf);
	glVertex3i(screen_x - (TILE_SIZE * targetPtr->frame_texture_tiles - hitbox_w) / 2,
			screen_y + (TILE_SIZE * targetPtr->frame_texture_tiles + hitbox_h) / 2,
			screen_y);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}
