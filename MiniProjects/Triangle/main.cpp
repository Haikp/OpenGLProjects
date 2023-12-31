#include <glad/glad.h>
#include <GLFW/glfw3.h>
// TODO: put header files in to a folder on their own
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include <iostream>

//coordinates inserted into fragment shader
GLfloat verticies[] = 
{ //               COORDINATES                  /     COLORS           //
	-0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f, // Lower left corner 0
	 0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f, // Lower right corner 1
	 0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,     1.0f, 0.6f,  0.32f, // Upper corner 2
	-0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f, // Inner left 3
	 0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f, // Inner right 4
	 0.0f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f  // Inner down 5
};

/* shape of verticies inserted
        2
        /\
       /  \
      /    \
     3------4
    / \   /  \
   /   \/     \
  0----5-------1

*/

//order for shader to draw the tranges
GLuint indicies[] {
    0, 3, 5, // Lower left triangle
    3, 2, 4, // Upper triangle
    5, 4, 1 // Lower right triangle
    // 0, 1, 2
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
    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    //Obtain the data for the colors for the object from the buffer and link it to the VAO
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();

    //will contiune to loop until window is close
    while(!glfwWindowShouldClose(window)) {
        //specify color of background
        glClearColor(.07, .13, .17, 1);
        //fill back buffer with color
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.Activate();
        VAO.Bind();
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        //move back buffer to the front by swapping
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //upon window close, delete the window and GLFW API
    VAO.Delete();
    VBO.Delete();
    EBO.Delete();
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}