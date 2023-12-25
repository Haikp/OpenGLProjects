#version 330 core
// keyword layout helps the shader identify what the values were given in the vertex array
// the arrays can be understood as reading groups of 3 for instance for the vec3, which is stored in the variable aPos
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color;

void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
   color = aColor;
}