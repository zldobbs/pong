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
#include "Angel.h"

typedef vec4 point4;
typedef vec4 color4;

GLuint vao;
int Index = 0;
const int NumVertices = 36;
point4 points[NumVertices];
color4 colors[NumVertices];

// Translation will hold the current translation modifier
// { x, y, _, z }
GLfloat  Translation[4] = { 0.0, 0.0, 0.0, 0.0 };

GLuint  translation; // The location of the "translation" shader uniform shader variable

// define vertices of ball
point4 ballVertices[8] = {
    point4(-0.1, -0.1, 0.1, 1.0),
    point4(-0.1, 0.1, 0.1, 1.0),
    point4(0.1, 0.1, 0.1, 1.0),
    point4(0.1, -0.1, 0.1, 1.0),
    point4(-0.1, -0.1, -0.1, 1.0),
    point4(-0.1, 0.1, -0.1, 1.0),
    point4(0.1, 0.1, -0.1, 1.0),
    point4(0.1, -0.1, -0.1, 1.0)
};

// vertex colors.. default to white for now
color4 vertex_colors[8] = {
    color4(1.0, 1.0, 1.0, 1.0),  // white
    color4(1.0, 1.0, 1.0, 1.0),  // white
    color4(1.0, 1.0, 1.0, 1.0),  // white
    color4(1.0, 1.0, 1.0, 1.0),  // white
    color4(1.0, 1.0, 1.0, 1.0),  // white
    color4(1.0, 1.0, 1.0, 1.0),  // white
    color4(1.0, 1.0, 1.0, 1.0),  // white
    color4(1.0, 1.0, 1.0, 1.0)   // white
};

// coloring cube
void quad(int a, int b, int c, int d, point4* vertices) {
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}

// color the ball using quad
void colorBall() {
    quad(1, 0, 3, 2, ballVertices);
    quad(2, 3, 7, 6, ballVertices);
    quad(3, 0, 4, 7, ballVertices);
    quad(6, 5, 1, 2, ballVertices);
    quad(4, 5, 6, 7, ballVertices);
    quad(5, 4, 8, 1, ballVertices);
}

void init(void) {
    colorBall();
    
    // create a vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // create and initialize buffer object
    // NOTE how can this be reused to also generate the bumpers?
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
    
    // load shaders
    // NOTE for right now make this a relative path
    // TODO fix this to work without making a specific path
    GLuint program = InitShader("/Users/zachdobbs/Documents/Computer Graphics/FinalProject/GraphicsFinalProject/GraphicsFinalProject/vshader36.glsl", "/Users/zachdobbs/Documents/Computer Graphics/FinalProject/GraphicsFinalProject/GraphicsFinalProject/fshader36.glsl");
    glUseProgram(program);
    
    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0));
    
    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(sizeof(points)));
    
    translation = glGetUniformLocation(program, "translation");
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

// key callback handler
void mykey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch(key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_W:
                Translation[0] += 1.0;
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
    init();
    
    // setup the callbacks
    glfwSetKeyCallback(window, mykey);
    
    do {
        // clear window
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        // TODO do the rest of stuff here..
        glUniform4fv(translation, 1, Translation);
        
        glDrawArrays(GL_TRIANGLES, 0, NumVertices);
        
        glfwPollEvents();
        glfwSwapBuffers(window);
    } while(!glfwWindowShouldClose(window));
    
    glfwTerminate();
    return 0;
}
