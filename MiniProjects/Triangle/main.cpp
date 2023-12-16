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
    //idk why you need to * 2
    glViewport(0, 0, width * 2, height * 2);

    //generate parts of the shader pipe line we will be working with
    //allows us to pass in verticies of a shape, and primitive
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //allows us to color our triangle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //our shader program we will be using to generate our shape
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //deallocate shaders after linking everything to final shader program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //coordinates inserted into fragment shader
    GLfloat verticies[] = {
        -0.5f, (-0.5f * float(sqrt(3)) / 3), 0.0f, // Lower left corner
		0.5f, (-0.5f * float(sqrt(3)) / 3), 0.0f, // Lower right corner
		0.0f, (0.5f * float(sqrt(3)) * 2 / 3), 0.0f, // Upper corner

        -0.5 / 2, ((0.5f * float(sqrt(3)) * 2 / 3) + (-0.5f * float(sqrt(3)) / 3)) / 2, 0, // inner left 3
        .5 / 2, ((-0.5f * float(sqrt(3)) / 3) + (0.5f * float(sqrt(3)) * 2 / 3)) / 2, 0, // inner right 4
        0, ((-0.5f * float(sqrt(3)) / 3) + (-0.5f * float(sqrt(3)) / 3)) / 2, 0, //inner down 5
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
    // Vertex Array Object
    // Vertex Buffer Object
    // Element Buffer Object
    GLuint VAO, VBO, EBO;

    //create our buffers and vertex array
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //select the appropriate array and buffer to use
    //https://stackoverflow.com/questions/21652546/what-is-the-role-of-glbindvertexarrays-vs-glbindbuffer-and-what-is-their-relatio
    // ^ really good explanation
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //bind our element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    //VBO takes the verticies
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
    //EBO takes the incidies
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    //set array and buffer to 0 to prevent potential modifications after this point in the code
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //will contiune to loop until window is close
    while(!glfwWindowShouldClose(window)) {
        //specify color of background
        glClearColor(.07, .13, .17, 1);
        //fill back buffer with color
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        //move back buffer to the front by swapping
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //upon window close, delete the window and GLFW API
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}