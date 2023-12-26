#include <glad/glad.h>
#include <GLFW/glfw3.h>
// TODO: put header files in to a folder on their own
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include <iostream>
#include <stb/stb_image.h>

//coordinates inserted into fragment shader
GLfloat verticies[] = 
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner 0
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner 1
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner 2
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f,  // Lower right corner 3
};

//order for shader to draw the trangles
GLuint indicies[] {
    0, 2, 1,
    0, 3, 2,
};
    
int main() {
    //Mandatory Initialization of GLFW
    //API that helps simplifies creating windows
    glfwInit();

    //Explicitly state the current version, 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //using modern functions package/profile, core
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //MAC EXCLUSIVE LINE
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    //screen dimensions
    int width = 800;
    int height = 800;

    //intialize a window to open
    GLFWwindow* window = glfwCreateWindow(width, height, "example", NULL, NULL);
    //error checking if window was intialized correctly
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }   

    //MUST EXPLICITLY STATE CURRENT WINDOW
    glfwMakeContextCurrent(window);

    //manages GLFW function pointers, interacts with window
    gladLoadGL();
    //define range that glad can work with
    glViewport(0, 0, width, height);

    //constructor for shader program
    Shader shaderProgram("default.vert", "default.frag");

    //make a Vertex Array Object
    VAO VAO;
    VAO.Bind();

    //Vertex Buffer Object, object contains verticies of image to be made
    VBO VBO(verticies, sizeof(verticies));
    //Element Buffer Object, object contains the indicies, or instructions for the shapes to be generated
    EBO EBO(indicies, sizeof(indicies));

    //Obtain the data for the verticies for the object from the buffer and link it to the VAO
    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    //Obtain the data for the colors for the object from the buffer and link it to the VAO
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO.LinkAttrib(VBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();

    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
    
    //Texture
    int imgWid, imgHgh, numColCh;
    unsigned char* bytes = stbi_load("PogO.png", &imgWid, &imgHgh, &numColCh, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWid, imgHgh, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0); 

    GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
    shaderProgram.Activate();
    glUniform1i(tex0Uni, 0);

    //will contiune to loop until window is close
    while(!glfwWindowShouldClose(window)) {
        //specify color of background
        glClearColor(.07, .13, .17, 1);
        //fill back buffer with color
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.Activate();
        glUniform1f(uniID, 0.5f);
        glBindTexture(GL_TEXTURE_2D, texture);
        VAO.Bind();
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //move back buffer to the front by swapping
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //upon window close, delete the window and GLFW API
    VAO.Delete();
    VBO.Delete();
    EBO.Delete();
    glDeleteTextures(1, &texture);
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}