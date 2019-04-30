//
//  main.cpp
//  GraphicsFinalProject
//
//  Created by Zachary Dobbs on 4/29/19.
//  Copyright © 2019 Zachary Dobbs. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// key callback handler
void mykey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch(key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
        }
    }
}

int main(int argc, const char * argv[]) {
    GLFWwindow *window = NULL;
    const GLubyte *renderer;
    const GLubyte *version;
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
    #ifdef __APPLE__
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif
    
    window = glfwCreateWindow(640, 640, "Pong", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    renderer = glGetString(GL_RENDERER); /* get renderer string */
    version = glGetString(GL_VERSION);     /* version as a string */
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    // TODO init();
    
    // setup the callbacks
    glfwSetKeyCallback(window, mykey);
    
    do {
        // clear window
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        // TODO do the rest of stuff here..
        
        glfwPollEvents();
        glfwSwapBuffers(window);
    } while(!glfwWindowShouldClose(window));
    
    glfwTerminate();
    return 0;
}
