#ifndef C_SHADER_H
#define C_SHADER_H

#include "../client_src/Globals.h"

#define NUM_SHADERS 8

#define SHADER_SIMPLE_V			0
#define SHADER_SIMPLE_G			1
#define SHADER_SIMPLE_F			2
#define SHADER_RED_F			3
#define SHADER_CTR_F			4
#define SHADER_FADE_F			5
#define SHADER_GREYSCALEFADE_F  6
#define SHADER_EPILEPTIC_F      7

class cShader
{
public:
	cShader();
	~cShader();

	void Load();
	void Attach(int shader_id);
	void Detach(int shader_id);
	void Activate();
	void Deactivate();

	template<class T>
	void SetUniform(std::string uniform, T value) //el acceso a uniforms se hace siempre despues de linkar (metodo Activate())
	{
		char *cstr = new char[uniform.length() + 1];
		strcpy(cstr, uniform.c_str());
		GLint loc = glGetUniformLocation(prog, cstr);
		if (loc != -1)
		{
			if (typeid(T) == typeid(int))
				glUniform1i(loc, value);
			else
				glUniform1f(loc, value);
		}
	}

private:
	GLuint prog;
	GLuint shaders[NUM_SHADERS];
};

#endif
