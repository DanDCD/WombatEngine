
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>



// A callback function to be called whenever the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height); // tell opengl the new window size (if changed)
}

int main()
{
    glfwInit(); // Initialize the GLFW library
    // Set the major and minor version of OpenGL to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Set the OpenGL profile to use
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "ThreeDimSim", NULL, NULL);
    if(window == NULL){
        std::cout << "Failed to create a GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialise GLAD
    bool loaded_GLAD = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); // tell GLAD to load the address of the OpenGL function pointers
    if(!loaded_GLAD){
        std::cout << "Failed to load GLAD" << std::endl;
        return -1;
    }

    // tell OpenGL the size of the rendering Window
    glViewport(0, 0, 800, 600);

    // assign our resizing function as the resizing window callback for our window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // keep doing this loop until user wants to close
    while(!glfwWindowShouldClose(window)){
        glfwSwapBuffers(window); // swap the buffer we have been drawing to into the front
        glfwPollEvents(); // poll for events
    }

    glfwTerminate();
    return 0;
}