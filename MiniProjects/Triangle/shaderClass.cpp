#include "shaderClass.h"

/*
parses given file into a string
*/
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

/*
generates the shader program given the vertex shader and fragment shader
*/
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    //generate parts of the shader pipe line we will be working with
    //allows us to pass in verticies of a shape, and primitive
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    //allows us to color our triangle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    //our shader program we will be using to generate our shape
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    //deallocate shaders after linking everything to final shader program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
    glUseProgram(ID);
}

void Shader::Delete()
{
    glDeleteProgram(ID);
}