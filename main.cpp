#define GL_GLEXT_PROTOTYPES
#include "includes/shader.h"
#include "includes/camera.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"
#include "includes/glm/gtc/type_ptr.hpp"
#include "includes/stb_image.h"
#include <filesystem>
namespace fs = std::filesystem;

const unsigned int screenWidth = 1024;
const unsigned int screenHeight = 768;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool disco = false;
float lightHeight = 3.0f;
bool showFPS = false;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int loadTexture(char const * path);

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glfwGetFramebufferSize(window, &width, &height);
    std::cout << "Width:\t" << width << "\t|\tHeight:\t" << height << "\n";
    glfwSetWindowSize(window, width, height);
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    /*========Handling Controls========*/
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime * 2.0f);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime * 2.0f);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime * 2.0);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime * 2.0f);
    /*=================================*/
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //Window close
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) //Lines of Triangles
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) //Fill the Triangles
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
        showFPS = !showFPS;
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        lightHeight += .03f;
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        lightHeight -= .03f;
    if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
        disco = true;
    if (glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS)
        disco = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window;

    window = glfwCreateWindow(screenWidth, screenHeight, "Solo Project", NULL, NULL);

    if (window == nullptr) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    std::cout << glGetString(GL_VERSION) << std::endl;

float Cube[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, //Βack Side
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //Front Side
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, //Left Side
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, //Right Side
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f, //Bottom Side
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, //Top Side
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

    float Back[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    };

    float Front[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
    };

    float Left[] = {
    // positions          // normals           // texture coords
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    };

    float Right[] = {
    // positions          // normals           // texture coords
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    };

    float Bottom[] = {
    // positions          // normals           // texture coords        
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    };

    float Top[] = {
    // positions          // normals           // texture coords
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glm::vec3 FloorPositions[] = {
        glm::vec3(-6.0f, -1.0f, -5.0f), 
        glm::vec3(-5.0f, -1.0f,  6.0f), 
        glm::vec3( 5.0f, -1.0f, -5.0f), 
        glm::vec3( 5.0f, -1.0f,  6.0f)  
    };

    glm::vec3 TopWall[] = {
        glm::vec3(-6.0f,  4.0f, -5.0f), 
        glm::vec3(-5.0f,  4.0f,  6.0f), 
        glm::vec3( 5.0f,  4.0f, -5.0f), 
        glm::vec3( 5.0f,  4.0f,  6.0f) 
    };

    glm::vec3 LeftWall[] = {
        glm::vec3(-7.0f,  0.0f, -5.0f), 
        glm::vec3(-7.0f,  3.0f, -5.0f), 
        glm::vec3(-7.0f,  0.0f,  6.0f), 
        glm::vec3(-7.0f,  3.0f,  6.0f)  
    };

    glm::vec3 RightWall[] = {
        glm::vec3(6.0f,  0.0f, -5.0f), 
        glm::vec3(6.0f,  3.0f, -5.0f), 
        glm::vec3(6.0f,  0.0f,  6.0f), 
        glm::vec3(6.0f,  3.0f,  6.0f)
    };

    glm::vec3 FrontWall[] = {
        glm::vec3(-6.0f, 0.0f, -6.0f),
        glm::vec3(-6.0f, 3.0f, -6.0f),
        glm::vec3( 5.0f, 0.0f, -6.0f),
        glm::vec3( 5.0f, 3.0f, -6.0f)
    };

    glm::vec3 BackWall[] = {
        glm::vec3(-6.0f, 0.0f, 7.0f),
        glm::vec3(-6.0f, 3.0f, 7.0f),
        glm::vec3( 5.0f, 0.0f, 7.0f),
        glm::vec3( 5.0f, 3.0f, 7.0f)
    };


    ShaderProgramSource source = ParseShader("res/Basic.shader");
    ShaderProgramSource light  = ParseShader("res/LightSource.shader");

    unsigned int lampShader     = CreateShader(light.VertexShader, light.FragmentShader);
    unsigned int shader         = CreateShader(source.VertexShader, source.FragmentShader);

    unsigned int vbo, cube;
    glGenVertexArrays(1, &cube);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Cube), Cube, GL_STATIC_DRAW);

    glBindVertexArray(cube);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

/*=============================================================================================================*/
    unsigned int sidesvbo[6], sides[6]; //1 = back, 2 = front, 3 = left, 4 = right, 5 = bottom, 6 = top
    glGenVertexArrays(6, sides);
    glGenBuffers(6, sidesvbo);

    glBindBuffer(GL_ARRAY_BUFFER, sidesvbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Back), Back, GL_STATIC_DRAW);
    glBindVertexArray(sides[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    //FRONT
    glBindBuffer(GL_ARRAY_BUFFER, sidesvbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Front), Front, GL_STATIC_DRAW);
    glBindVertexArray(sides[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //LEFT
    glBindBuffer(GL_ARRAY_BUFFER, sidesvbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Left), Left, GL_STATIC_DRAW);
    glBindVertexArray(sides[2]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    //RIGHT
    glBindBuffer(GL_ARRAY_BUFFER, sidesvbo[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Right), Right, GL_STATIC_DRAW);
    glBindVertexArray(sides[3]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    //BOTTOM
    glBindBuffer(GL_ARRAY_BUFFER, sidesvbo[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Bottom), Bottom, GL_STATIC_DRAW);
    glBindVertexArray(sides[4]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    //TOP
    glBindBuffer(GL_ARRAY_BUFFER, sidesvbo[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Top), Top, GL_STATIC_DRAW);
    glBindVertexArray(sides[5]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    

    unsigned int wall = loadTexture(fs::path("res/wall.jpg").c_str());
    unsigned int floor = loadTexture(fs::path("res/tiles.png").c_str());
    
    
    useShader(shader);


    glEnable(GL_DEPTH_TEST);


    double lastTime = 0.0;
    unsigned int counter = 0;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        useShader(shader);
        setVec3(shader, "viewPos", camera.Position);

        glm::vec3 PointLightPositions[] = {
            glm::vec3( 5.0f, lightHeight,  6.0f),
            glm::vec3( 5.0f, lightHeight, -5.0f),
            glm::vec3(-6.0f, lightHeight, -5.0f),
            glm::vec3(-6.0f, lightHeight,  6.0f)
        };

        //Point Lights
        setVec3(shader, "pointLights[0].position", PointLightPositions[0]);
        setVec3(shader, "pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        setVec3(shader, "pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        setVec3(shader, "pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        setFloat(shader, "pointLights[0].constant", 1.0f);
        setFloat(shader, "pointLights[0].linear", 0.09);
        setFloat(shader, "pointLights[0].quadratic", 0.032);

        setVec3(shader, "pointLights[1].position", PointLightPositions[1]);
        setVec3(shader, "pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        setVec3(shader, "pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        setVec3(shader, "pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        setFloat(shader, "pointLights[1].constant", 1.0f);
        setFloat(shader, "pointLights[1].linear", 0.09);
        setFloat(shader, "pointLights[1].quadratic", 0.032);

        setVec3(shader, "pointLights[2].position", PointLightPositions[2]);
        setVec3(shader, "pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        setVec3(shader, "pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        setVec3(shader, "pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        setFloat(shader, "pointLights[2].constant", 1.0f);
        setFloat(shader, "pointLights[2].linear", 0.09);
        setFloat(shader, "pointLights[2].quadratic", 0.032);

        setVec3(shader, "pointLights[3].position", PointLightPositions[3]);
        setVec3(shader, "pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        setVec3(shader, "pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        setVec3(shader, "pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        setFloat(shader, "pointLights[3].constant", 1.0f);
        setFloat(shader, "pointLights[3].linear", 0.09);
        setFloat(shader, "pointLights[3].quadratic", 0.032);


        
        setFloat(shader, "material.shininess", 64.0f); 
             
        
        glm::mat4 proj  = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth /(float)screenHeight, 0.1f, 100.0f);
        glm::mat4 view =  camera.GetViewMatrix();
        setMat4(shader, "projection", proj);
        setMat4(shader, "view", view);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floor);
        setInt(shader, "material.texture", 0);


        glBindVertexArray(sides[5]);
        //Floor
        float begX, endX = FloorPositions[3].x;
        float begZ, endZ = FloorPositions[3].z;
        for (int i = 0, begX = FloorPositions[i].x; begX <= endX; i++, begX++)
        {
            for (int j = 0, begZ = FloorPositions[j].z; begZ <= endZ; j++, begZ++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(begX, FloorPositions[0].y, begZ));
                setMat4(shader, "model", model);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }


        glBindVertexArray(sides[4]);
        //TopWall
        endX = TopWall[3].x;
        endZ = TopWall[3].z;
        for (int i = 0, begX = TopWall[i].x; begX <= endX; i++, begX++)
        {
            for (int j = 0, begZ = TopWall[j].z; begZ <= endZ; j++, begZ++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(begX, TopWall[0].y, begZ));
                setMat4(shader, "model", model);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }

        setVec3(shader, "pointLights[0].specular", 0.0f, 0.0f, 0.0f);
        setVec3(shader, "pointLights[1].specular", 0.0f, 0.0f, 0.0f);
        setVec3(shader, "pointLights[2].specular", 0.0f, 0.0f, 0.0f);
        setVec3(shader, "pointLights[3].specular", 0.0f, 0.0f, 0.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wall);
        setInt(shader, "material.texture", 0);
        
        glBindVertexArray(sides[3]);
        //LeftWall
        endZ = LeftWall[3].z;
        float begY, endY = LeftWall[3].y;
        for (int i = 0, begZ = LeftWall[i].z; begZ <= endZ; i++, begZ++)
        {
            for (int j = 0, begY = LeftWall[j].y; begY <= endY; j++, begY++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(LeftWall[0].x, begY, begZ));
                setMat4(shader, "model", model);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }
        glBindVertexArray(sides[2]);
        //RightWall
        endZ = RightWall[3].z;
        endY = RightWall[3].y;
        for (int i = 0, begZ = RightWall[i].z; begZ <= endZ; i++, begZ++)
        {
            for (int j = 0, begY = RightWall[j].y; begY <= endY; j++, begY++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(RightWall[0].x, begY, begZ));
                setMat4(shader, "model", model);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }
        glBindVertexArray(sides[1]);
        //FrontWall
        endX = FrontWall[3].x;
        endY = FrontWall[3].y;
        for (int i = 0, begX = FrontWall[i].x; begX <= endX; i++, begX++)
        {
            for (int j = 0, begY = FrontWall[j].y; begY <= endY; j++, begY++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(begX, begY, FrontWall[0].z));
                setMat4(shader, "model", model);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }
        glBindVertexArray(sides[0]);
        //BackWall
        endX = BackWall[3].x;
        endY = BackWall[3].y;
        for (int i = 0, begX = BackWall[i].x; begX <= endX; i++, begX++)
        {
            for (int j = 0, begY = BackWall[j].y; begY <= endY; j++, begY++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(begX, begY, BackWall[0].z));
                setMat4(shader, "model", model);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }

        float red   = sin((float)glfwGetTime()) / 2 + ((sin((float)glfwGetTime()) / 2 < 0.05 ? 0.55 : 0.5));
        float green = cos((float)glfwGetTime()) / 2 + ((cos((float)glfwGetTime()) / 2 < 0.05 ? 0.55 : 0.5));
        float blue  = sin((float)glfwGetTime()) / 2 + ((sin((float)glfwGetTime()) / 2 < 0.05 ? 0.55 : 0.5));
        // std::cout << "red\t" << red << std::endl;
        // std::cout << "green\t" << green << std::endl;
        
        if (disco)
        {
            useShader(shader);
            setBool(shader, "disco", disco);
            setFloat(shader, "green", green);
            setFloat(shader, "red", red);
            setFloat(shader, "blue", blue);
            useShader(lampShader);
            setBool(lampShader, "disco", disco);
            setFloat(lampShader, "green", green);
            setFloat(lampShader, "red", red);
            setFloat(lampShader, "blue", blue);
        }
        else
        {
            useShader(shader);
            setBool(shader, "disco", 0);
            useShader(lampShader);
            setBool(lampShader, "disco", 0);
        }
        
        glBindVertexArray(cube);
        useShader(lampShader);
        setMat4(lampShader, "projection", proj);
        setMat4(lampShader, "view", view);
        glm::mat4 model = glm::mat4(1.0f);
        for (int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, PointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.5f));
            setMat4(lampShader, "model", model);
            glDrawArrays(GL_TRIANGLES, 0 , 36);
        }

        ++counter;
        double currentTime = glfwGetTime();
        
        if (currentTime - lastTime >= 1.0)
        {
                if (showFPS)
                    std::cout << "FPS:\t" << counter << std::endl;
                ++lastTime;
                counter = 0;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &cube);
    glDeleteVertexArrays(6, sides);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(6, sidesvbo);

    glfwTerminate();
    return 0;
}

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    
    return textureID;
}

//g++ main.cpp -std=c++17 -lstdc++fs -lGL -lglfw && ./a.out