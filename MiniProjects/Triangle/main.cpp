#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

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
    glViewport(0, 0, width * 2, height * 2);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLfloat verticies[] = {
        -0.5f, (-0.5f * float(sqrt(3)) / 3), 0.0f, // Lower left corner
		0.5f, (-0.5f * float(sqrt(3)) / 3), 0.0f, // Lower right corner
		0.0f, (0.5f * float(sqrt(3)) * 2 / 3), 0.0f // Upper corner
        // 1, 1, 0,
        // 1, -1, 0,
        // -1, 1, 0
    };

    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //will contiune to loop until window is close
    while(!glfwWindowShouldClose(window)) {
        //specify color of background
        glClearColor(.07, .13, .17, 1);
        //fill back buffer with color
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //move back buffer to the front by swapping
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    //upon window close, delete the window and GLFW API
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}