#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.2f, 0.8f, 1.0f);\n"
                                   "}\n\0";

// VVI (README part) : Input to graphics pipeline here is  called vertex data, as
// it is collection of vertices, where one vertex is collection of data per 3d
// coordinate, where data is collection of vertex attributes ,where say in our
// triangle case say vertex attributes are 3d coordinate and color only

int main()
{

    // ========Intro=============
    // first, we need to be able to have window and opengl context to render things, which are os-specific and the opengl leaves that to us, that is we need to create a window, define a context, and handle user input all by ourselves.

    // luckily there are things like glfw to do that for us in plaform independent way.

    // glfw does - make window,
    // make context on window where we can draw, handle window parameters
    // , and makes us enable to take inputs platform independent way.

    // glad is about helping us retreive the location of functions needed and stores them in function pointer in platform independent way

    // ====== Intro finished ======

    // initalizing glfw
    glfwInit();
    // telling glfw that opengl 3.3 is what we want to use in core profile way.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // creating window object ,required by most other glfw functions.
    GLFWwindow *window = glfwCreateWindow(800, 600, "Learn-OpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // making the context of the window object as main context of current thread.
    glfwMakeContextCurrent(window);

    // glad part now

    // glad manages function pointers for opengl so we initialize glad before any opengl function is used.

    // initializing glad by loading all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // setting the viewport where we can draw stuff inside the window of glfw
    glViewport(0, 0, 800, 600);
    // setting callback to be called when window resized
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ===========start for triangle rendering =========

    // data for coordinates of triangle in normalized device coordinates , where z coordinate = 0 ,making it in depth =0,i.e. 2d.
    float vertices[] = {
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    // after vertex data is defined we would like to send as input to vertex shader  ,by creating memory on gpu storing vertex data, we manage this memory via VBO.

    // creating VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // binding VAO
    glBindVertexArray(VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // From this point on any buffer calls we make (on the GL_ARRAY_BUFFER target) will be used to configure the currently bound buffer, which is VBO

    // copy the above vertices data to VBO
    // glBufferData is a function specifically targeted to copy user-defined data into the currently bound buffer.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Till now we stored the vertex data in gpu memory by managing using VBO, now we will write vertex and fragment shader to use that data.
    // for vertex shader and fragment shader see source code above

    // the vertex shader is simple as we just did no processing in input data and just forwarded as shader output

    // VVI: opengl has to compile the source code at runtime to use that shader say vertex shader, so we create shader object

    unsigned int vertexSh;
    vertexSh = glCreateShader(GL_VERTEX_SHADER);

    // after creating , we now need to bind shader source code to shader object and compile
    glShaderSource(vertexSh, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexSh);

    // checking if compilation was successful or not
    int success;
    char info[512];
    glGetShaderiv(vertexSh, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexSh, 512, NULL, info);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << info << std::endl;
    }
    // now see above for source code of fragment shader, but below this
    // we do like vertex shader for upto compilation

    unsigned int fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragShader);

    // checking now for compilation error in fragShader
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragShader, 512, NULL, info);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << info << std::endl;
    }

    // now both shader should have been compiled , so now we need to link those
    // shaders objects into a shader program to use for rendering

    // creating a shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    // the activated shader program's shaders will be used ,when we issue render calls
    // now we attach previously compiled shaders to shader program and link them
    glAttachShader(shaderProgram, vertexSh);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    // checking if linking error occured.

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, info);
        std::cout << "ERROR::LINK_FAILED\n"
                  << info << std::endl;
    }

    // linking error check part finished
    glUseProgram(shaderProgram);
    // every shader and rendering call after this glUseProgram function, will
    // now use this shader Program and thus all those vertex,frag shaders

    // deleting vertex and frag shaders as we dont need them now
    glDeleteShader(vertexSh);
    glDeleteShader(fragShader);

    // till now we stored the vertex data in GPU and told GPU how it can use the
    // vertex data inside vertex and fragment shader ,but now we need to tell opengl
    // how it can understand/interpret the vertex data from memory and how it
    // can connect vertex data to vertex shader's attribute

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    // TODO: I didnot really get this line
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // TODO: This line too.
    glBindVertexArray(0);

    // windshouldclose checks if glfw has been instructed to close
    while (!glfwWindowShouldClose(window))
    {
        // always main parts are:
        // 1. process events
        // 2. update in window
        // 3. render the window
        processInput(window);
        glClearColor(0.8, 0.7, 0.9, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        // checks if any events are triggered and calls corresponding functions via callback methods.
        glfwPollEvents();
    }

    // to properly clean all glfw resources which are allocated
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}