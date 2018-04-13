#ifndef C_TEXTURE_H
#define C_TEXTURE_H

#include "../client_src/Globals.h"

class cTexture
{
public:
	cTexture(void);
	~cTexture(void);

	bool Load(char *filename,int type = GL_RGBA,int wraps = GL_REPEAT,int wrapt = GL_REPEAT,
			  int magf = GL_NEAREST,int minf = GL_NEAREST,bool mipmap = false);
	int  GetID();
	void GetSize(int *w,int *h);

private:
	GLuint id;
	int width,height;
};

#endif
