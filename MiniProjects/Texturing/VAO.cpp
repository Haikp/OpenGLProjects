#include "VAO.h"

/*
Generate a Vertex Array Object
*/
VAO::VAO()
{
    glGenVertexArrays(1, &ID);
}

/*
LinkAttrib Parameters;
    VBO - current buffer that we are working with
    layout - refer to default.vert to see the value of the layouts (location)
    numComponents - number of values to read in
    type - data type of values being read in
    stride - the amount of data before a cycle of data is completed
    offset - the amount of data to skip initially to begin reading the desired data
    FOR A VISUAL FOR STRIDE AND OFFSET, VIEW VAOAttribVisual.png IN LOCAL DIRECTORY
*/
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::Bind()
{
    glBindVertexArray(ID);
}

void VAO::Unbind()
{
    glBindVertexArray(0);
}

void VAO::Delete()
{
    glDeleteVertexArrays(1, &ID);
}