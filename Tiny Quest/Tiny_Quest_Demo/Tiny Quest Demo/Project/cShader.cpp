#include "cShader.h"

cShader::cShader(){}
cShader::~cShader(){}

std::string get_file_contents(const char *filename)
{
  std::ifstream ifs(filename);
  std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );
  return content;
}

void glcppShaderSource(GLuint shader, std::string const &shader_string)
{
    GLchar const *shader_source = shader_string.c_str();
    GLint const shader_length = shader_string.size();

    glShaderSource(shader, 1, &shader_source, &shader_length);
}

void load_shader(GLuint shaderobject, char * const shadersourcefilename)
{
    glcppShaderSource(shaderobject,get_file_contents(shadersourcefilename));
}

void cShader::Load()
{
	prog = glCreateProgram();
	
	shaders[SHADER_SIMPLE_V] = glCreateShader(GL_VERTEX_SHADER);
	load_shader(shaders[SHADER_SIMPLE_V],"Shaders/simple.vert");
	glCompileShader(shaders[SHADER_SIMPLE_V]);

	shaders[SHADER_SIMPLE_G] = glCreateShader(GL_GEOMETRY_SHADER);
	load_shader(shaders[SHADER_SIMPLE_G],"Shaders/simple.geom");
	glCompileShader(shaders[SHADER_SIMPLE_G]);

	shaders[SHADER_SIMPLE_F] = glCreateShader(GL_FRAGMENT_SHADER);
	load_shader(shaders[SHADER_SIMPLE_F],"Shaders/simple.frag");
	glCompileShader(shaders[SHADER_SIMPLE_F]);

	shaders[SHADER_RED_F] = glCreateShader(GL_FRAGMENT_SHADER);
	load_shader(shaders[SHADER_RED_F],"Shaders/red.frag");
	glCompileShader(shaders[SHADER_RED_F]);

	shaders[SHADER_CTR_F] = glCreateShader(GL_FRAGMENT_SHADER);
	load_shader(shaders[SHADER_CTR_F],"Shaders/ctr.frag");
	glCompileShader(shaders[SHADER_CTR_F]);

	shaders[SHADER_FADE_F] = glCreateShader(GL_FRAGMENT_SHADER);
	load_shader(shaders[SHADER_FADE_F],"Shaders/fade.frag");
	glCompileShader(shaders[SHADER_FADE_F]);

	shaders[SHADER_GREYSCALEFADE_F] = glCreateShader(GL_FRAGMENT_SHADER);
	load_shader(shaders[SHADER_GREYSCALEFADE_F],"Shaders/greyscalefade.frag");
	glCompileShader(shaders[SHADER_GREYSCALEFADE_F]);

	shaders[SHADER_EPILEPTIC_F] = glCreateShader(GL_FRAGMENT_SHADER);
	load_shader(shaders[SHADER_EPILEPTIC_F],"Shaders/epileptic.frag");
	glCompileShader(shaders[SHADER_EPILEPTIC_F]);


	/*
	//carga el source del shader de forma directa:
	static const char *fragment_source = {
	"uniform float time;"
	"void main()"
	"{"
	"     gl_FragColor = mix(gl_Color,vec4(1,0,0,1),time);"
	"}"
	};

	glShaderSource(shaders[SHADER_RED_F], 1, &fragment_source, NULL);
	*/
}

void cShader::Attach(int shader_id)
{
	glAttachShader(prog, shaders[shader_id]);
}

void cShader::Detach(int shader_id)
{
	glDetachShader(prog, shaders[shader_id]);
}

void cShader::Activate()
{
	glLinkProgram(prog);
	glUseProgram(prog);
}

void cShader::Deactivate()
{
	glUseProgram(0);
}