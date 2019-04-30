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

const int width = 450, height = 450;
const int RacketWidth = 10, RacketHeight = 100;
const int BallWidth = 10, BallHeight = 10;

double gameTime;

int LeftY = 0;
int RightY = 0;
int Racketspeed = 5;

int ballX, ballY;
int ballVelocityX = 1;
float ballVelocityY = 0.0f;
float StoredVelocity = 0.0f;

// ball velocity
double TimeSpeed=0.2;

int LeftScore = 0;
int RightScore = 0;

void Reset();
void Collisions();
bool Between(int target, int min, int max);

class gl_helper {
    public:
        static void DrawRect(int x, int y, int width, int height);
        static void DrawNumber(int x , int y, int n);
        static void DebugPoint(float x , float y);
};

void gl_helper::DebugPoint(float x, float y) {
    
    float SavedColor[4];
    glGetFloatv(GL_CURRENT_COLOR, SavedColor);
    glColor4f(1.0f, 0, 0, 1.0f);
    
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
    
    glColor4f(SavedColor[0],SavedColor[1],SavedColor[2],SavedColor[3]);
}

void gl_helper::DrawRect(int x, int y, int width, int height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void gl_helper::DrawNumber(int x, int y, int n) {
    switch(n) {
        case 0:
            DrawRect(x + 5, y, 20, 5);
            DrawRect(x, y + 5, 5, 20);
            DrawRect(x + 25, y + 5, 5, 20);
            DrawRect(x, y + 30, 5, 20);
            DrawRect(x + 25, y + 30, 5, 20);
            DrawRect(x + 5, y + 50, 20, 5 );
            break;
            
        case 1:
            DrawRect(x + 25, y + 5, 5, 20);
            DrawRect(x + 25, y + 30, 5, 20);
            break;
            
        case 2:
            DrawRect(x + 5, y , 20, 5);
            DrawRect(x + 25, y + 5, 5, 20);
            DrawRect(x + 5, y + 25, 20, 5);
            DrawRect(x, y + 30, 5, 20);
            DrawRect(x + 5, y + 50, 20, 5);
            break;
            
        case 3:
            DrawRect(x + 5, y, 20, 5);
            DrawRect(x + 25, y + 5, 5, 20);
            DrawRect(x + 5, y + 25, 20, 5);
            DrawRect(x + 25, y + 30, 5, 20);
            DrawRect(x + 5, y + 50, 20, 5);
            break;
            
        case 4:
            DrawRect(x, y + 5, 5, 20);
            DrawRect(x + 25, y + 5, 5, 20);
            DrawRect(x + 5, y + 25, 20, 5);
            DrawRect(x + 25, y + 30, 5, 20);
            break;
            
        case 5:
            DrawRect(x + 5, y, 20, 5);
            DrawRect(x, y + 5, 5, 20);
            DrawRect(x + 5, y + 25, 20, 5);
            DrawRect(x + 25, y + 30, 5, 20);
            DrawRect(x + 5, y + 50, 20, 5);
            break;
            
        case 6:
            DrawRect(x + 5, y, 20, 5);
            DrawRect(x, y + 5, 5, 20);
            DrawRect(x + 5, y + 25, 20, 5);
            DrawRect(x, y + 30, 5, 20);
            DrawRect(x + 25, y + 30, 5, 20);
            DrawRect(x + 5, y + 50, 20, 5);
            break;
            
        case 7:
            DrawRect(x + 5, y, 20, 5);
            DrawRect(x, y + 5, 5, 20);
            DrawRect(x + 25, y + 5, 5, 20);
            DrawRect(x + 25, y + 30, 5, 20);
            break;
            
        case 8:
            DrawRect(x + 5, y, 20, 5);
            DrawRect(x, y + 5, 5, 20);
            DrawRect(x + 25, y + 5, 5, 20);
            DrawRect(x + 5, y + 25, 20, 5);
            DrawRect(x, y + 30, 5, 20);
            DrawRect(x + 25, y + 30, 5, 20);
            DrawRect(x + 5, y + 50, 20, 5);
            break;
            
        case 9:
            DrawRect(x + 5, y, 20, 5);
            DrawRect(x, y + 5, 5, 20);
            DrawRect(x + 25, y + 5, 5, 20);
            DrawRect(x + 5, y + 25, 20, 5);
            DrawRect(x + 25, y + 30, 5, 20);
            DrawRect(x + 5, y + 50, 20, 5);
            break;
    }
}

void mykey(GLFWwindow* window, int key, int scancode, int action, int mods){
//    if (action == GLFW_KEY_PRESS) {
//        switch(key) {
//            default:
//                break;
//        }
//    }
    //Left Control UP
    if(key == GLFW_KEY_UP) {
        RightY-=Racketspeed;
    }
    //Left Control Down
    if(key == GLFW_KEY_DOWN) {
        RightY+=Racketspeed;
    }
    //Right Control UP
    if(key == GLFW_KEY_W) {
        LeftY-=Racketspeed;
    }
    //Right Control Down
    if(key == GLFW_KEY_S) {
        LeftY += Racketspeed;
    }
}


int main() {
    GLFWwindow* window;
    
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    window = glfwCreateWindow(width, height, "Pong", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    glfwSetKeyCallback(window, mykey);
    
    //Ball Starting position
    ballX = width/2 - 5 ;
    ballY = height/2 - 5;
    
    //Racket Starting Y Position
    LeftY = height/2 - 50;
    RightY = height/2 - 50;
    
    gameTime = glfwGetTime() * TimeSpeed;
    
    
    while(!glfwWindowShouldClose(window)){
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0 , width , height , 0 , 0 , 1);
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glClearColor(0.0f, 0.0f, 0.0f, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //Collision Detection
        Collisions();
        
        //Left Racket
        gl_helper::DrawRect( 0 , LeftY, RacketWidth , RacketHeight  );
        //Right Racket
        gl_helper::DrawRect( width - 10 , RightY , RacketWidth , RacketHeight  );
        //Ball
        gl_helper::DrawRect( ballX , ballY , BallWidth , BallHeight  );
        
        //Left Score
        gl_helper::DrawNumber( width/2 - 40 , 10 , LeftScore );
        //Right Score
        gl_helper::DrawNumber( width/2 + 10 , 10 , RightScore );
        //Colon
        gl_helper::DrawRect( width/2 - 2 , 25 , 5 , 5 );
        gl_helper::DrawRect( width/2 - 2 , 45 , 5 , 5 );
        
        glfwSwapBuffers(window);
        glfwSwapInterval(1);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}

void Collisions(){
    
    if( glfwGetTime() - gameTime > 1.0 ){
        
        gameTime = glfwGetTime()*TimeSpeed;
        
        ballX += ballVelocityX;
        
        StoredVelocity += ballVelocityY;
        
        if(StoredVelocity > 1.0f){
            ballY+=1;
            StoredVelocity-=1;
        }else if(StoredVelocity < -1.0f){
            ballY-=1;
            StoredVelocity+=1;
            
        }
    }
    
    
    //Ball at Right Edge
    if( ballX > width - RacketWidth*2 ){
        
        if( Between(ballY, RightY + RacketHeight, RightY )  ){
            
            ballVelocityX= -ballVelocityX;
            
            if( Between(ballY, RightY, RightY + RacketHeight*0.3 ) ){
                
                ballVelocityY-=0.3;
                
            }else if( Between(ballY,  RightY + RacketHeight - RacketHeight*0.3 , RightY + RacketHeight ) ){
                
                ballVelocityY+=0.3;
                
            }
            
            
        }else{
            
            LeftScore++;
            Reset();
            
        }
        
        //Ball at Left Edge
    }else if( ballX < RacketWidth ){
        
        if( Between(ballY, LeftY + RacketHeight, LeftY )  ){
            
            ballVelocityX= -ballVelocityX;
            
            if( Between(ballY, LeftY, LeftY + RacketHeight*0.3 ) ){
                
                ballVelocityY-=0.3;
                
            }else if( Between(ballY,  LeftY + RacketHeight - RacketHeight*0.3 , RightY + RacketHeight ) ){
                
                ballVelocityY+=0.3;
            }
            
        }else{
            
            RightScore++;
            Reset();
            
        }
        
        //Ball at Top Edge
    }else if( ballY < 0 ){
        
        ballY = 0;
        ballVelocityY= -ballVelocityY;
        
        //Ball at Buttom Edge
    }else if( ballY > height - BallHeight ){
        
        ballY = height - BallHeight;
        ballVelocityY= -ballVelocityY;
        
    }
    
}


bool Between(int target, int a , int b ){
    
    int max;
    int min;
    
    if( a > b){
        
        max = a;
        min = b;
        
    }else{
        max=b;
        min=a;
    }
    
    if( target < max && target > min)
        return true;
    
    return false;
    
}

void Reset(){
    
    ballX = width/2 - BallWidth/2 ;
    glfwSetTime(0.0);
    gameTime=0.0;
    ballVelocityX=1;
    ballVelocityY=0;
    StoredVelocity=0;
    
    if(LeftScore > 9 || RightScore > 9){
        
        LeftScore = 0;
        RightScore = 0;
    }
}
