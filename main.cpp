//
//  main.cpp
//  GraphicsFinalProject - Pong game
//  Zach Dobbs, Seth John, Brian Hillis
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLUT/glut.h>

#define GL_SILENCE_DEPRECATION

const int width = 450, height = 450;
const int bumperWidth = 10, bumperHeight = 100;
const int ballWidth = 10, ballHeight = 10;

double gameTime;

int leftBumperYPos = 0;
int rightBumperYPos = 0;
int bumperSpeed = 10;

int ballX, ballY;
int ballSpeed = 2;
int ballVelocityX = ballSpeed;
float ballVelocityY = 0.0f;
float storedVelocity = 0.0f;

int ball2X, ball2Y;
int ball2VelocityX = ballSpeed;
float ball2VelocityY = 0.0f;
float storedVelocity2 = 0.0f;
bool multipleBalls = false;

double gameTime2;

bool startGame = false;
int nameInput = 0;
int name1Length = 0;
int name2Length = 0;
float player1Color[3] = { 1.0f, 1.0f, 1.0f };
float player2Color[3] = { 1.0f, 1.0f, 1.0f };

double gameSpeed = 0;

int player1Score = 0;
int player2Score = 0;
char* scoreText = (char*)malloc(20*sizeof(char));
char player1NameText[20] = "";
char player2NameText[20] = "";
char charInput[1];
char gameModeText[50];
char ballSpeedText[50];

void setup();
void moveBall();
void moveBall2();
void addBall();
bool inbetween(int target, int min, int max);

class PongDisplay {
public:
	static void drawRect(int x, int y, int width, int height, float r, float g, float b);
	static void drawStrokedText(char* string, float scale, float x, float y, float z);
};

void PongDisplay::drawRect(int x, int y, int width, int height, float r, float g, float b) {
    glColor3f(r, g, b);
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();
}

void PongDisplay::drawStrokedText(char* string, float scale, float x, float y, float z) {
	char* c;
	glPushMatrix();
	glTranslatef(x, y+8,z);
	glScalef(scale, -scale, z);
    glColor3f(1.0f, 1.0f, 1.0f);
	c = string;
	while (*c != '\0') {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
		c++;
	}
	glPopMatrix();
}

// key callback
void mykey(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (action == GLFW_PRESS) {
        if (!startGame) {
            switch(nameInput) {
                case 1:
                    if (key >= 'A' && key <= 'Z' && name1Length <= 12) {
                        sprintf(charInput, "%c", key);
                        strcat(player1NameText, charInput);
                        name1Length++;
                    }
                    if (key == GLFW_KEY_SPACE && name1Length <= 12) {
                        strcat(player1NameText, " ");
                        name1Length++;
                    }
                    if (key == GLFW_KEY_BACKSPACE && name1Length > 0) {
                        player1NameText[strlen(player1NameText) - 1] = '\0';
                        name1Length--;
                    }
                    break;
                case 2:
                    if (key == GLFW_KEY_C) {
                        float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                        float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                        float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                        player1Color[0] = r1;
                        player1Color[1] = r2;
                        player1Color[2] = r3;
                    }
                    break;
                case 3:
                    if (key >= 'A' && key <= 'Z' && name2Length <= 12) {
                        sprintf(charInput, "%c", key);
                        strcat(player2NameText, charInput);
                        name2Length++;
                    }
                    if (key == GLFW_KEY_SPACE && name2Length <= 12) {
                        strcat(player2NameText, " ");
                        name2Length++;
                    }
                    if (key == GLFW_KEY_BACKSPACE && name2Length > 0) {
                        player2NameText[strlen(player2NameText) - 1] = '\0';
                        name2Length--;
                    }
                    break;
                case 4:
                    if (key == GLFW_KEY_C) {
                        float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                        float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                        float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                        player2Color[0] = r1;
                        player2Color[1] = r2;
                        player2Color[2] = r3;
                    }
                    break;
            }
            switch(key) {
                case GLFW_KEY_ENTER:
                    if (nameInput < 4) {
                        nameInput++;
                    }
                    else {
                        startGame = true;
                        setup();
                    }
                    break;
                case GLFW_KEY_B:
                    if (nameInput == 0) {
                        ballSpeed++;
                        if (ballSpeed > 4) {
                            ballSpeed = 1;
                        }
                        sprintf(ballSpeedText, "Ball Speed: %d", ballSpeed);
                    }
                    break;
                case GLFW_KEY_G:
                    if (nameInput == 0) {
                        if (multipleBalls) {
                            multipleBalls = false;
                            strcpy(gameModeText, "Game Mode: Regular");
                        }
                        else {
                            multipleBalls = true;
                            strcpy(gameModeText, "Game Mode: Two Ball Mayhem");
                        }
                    }
                    break;
                case GLFW_KEY_ESCAPE:
                    if (nameInput == 0) {
                        glfwSetWindowShouldClose(window, GLFW_TRUE);
                    }
                    else {
                        startGame = false;
                        nameInput = 0;
                        strcpy(player1NameText, "");
                        strcpy(player2NameText, "");
                        name1Length = 0;
                        name2Length = 0;
                        player1Color[0] = 1.0f;
                        player1Color[1] = 1.0f;
                        player1Color[2] = 1.0f;
                        player2Color[0] = 1.0f;
                        player2Color[1] = 1.0f;
                        player2Color[2] = 1.0f;
                    }
                    break;
                default:
                    break;
            }
        }
        else {
            switch(key) {
                case GLFW_KEY_ESCAPE:
                    startGame = false;
                    nameInput = 0;
                    strcpy(player1NameText, "");
                    strcpy(player2NameText, "");
                    name1Length = 0;
                    name2Length = 0;
                    player1Color[0] = 1.0f;
                    player1Color[1] = 1.0f;
                    player1Color[2] = 1.0f;
                    player2Color[0] = 1.0f;
                    player2Color[1] = 1.0f;
                    player2Color[2] = 1.0f;
                    player1Score = 0;
                    player2Score = 0; 
                    break;
                case GLFW_KEY_O:
                    if (rightBumperYPos > 0)
                        rightBumperYPos -= bumperSpeed;
                    break;
                case GLFW_KEY_L:
                    if (rightBumperYPos + bumperHeight < height)
                        rightBumperYPos += bumperSpeed;
                    break;
                case GLFW_KEY_W:
                    if (leftBumperYPos > 0)
                        leftBumperYPos -= bumperSpeed;
                    break;
                case GLFW_KEY_S:
                    if (leftBumperYPos + bumperHeight < height)
                        leftBumperYPos += bumperSpeed;
                    break;
                default:
                    break;
            }
        }
    }
}


int main() {
    GLFWwindow* window;
    
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    window = glfwCreateWindow(width, height, "Pong", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    glfwSetKeyCallback(window, mykey);
    
    // define the text needed in the game
    // TODO add ability for users to pick a name, show on game screen
    char titleText[20] = "Welcome to Pong!";
    char leftBumperCText[40] = "W and S control the left bumper";
    char rightBumperCText[40] = "O and L control the right bumper";
    strcpy(gameModeText, "Game Mode: Regular");
    char gameModeCText[40] = "Press G to change game mode";
    strcpy(ballSpeedText, "Ball Speed: 2");
    char ballSpeedCText[40] = "Pess B to change ball speed";
    char startGameText[30] = "Press enter to begin";
    
    char player1NameCText[20] = "Player 1 Name: ";
    char player2NameCText[20] = "Player 2 Name: ";
    char playerColorCText[40] = "Paddle color (Press C to change): ";
    
    gameTime = glfwGetTime() * gameSpeed;
    setup();
    
    while(!glfwWindowShouldClose(window)){
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width, height, 0, 0, 1);
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glClearColor(0.0f, 0.0f, 0.0f, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        if (startGame) {
            // move the ball and check for any moveBall
            moveBall();
            if(multipleBalls == true){
                PongDisplay::drawRect(ball2X, ball2Y, ballWidth, ballHeight, 1.0f, 1.0f, 1.0f);
                moveBall2();
            }
            // draw ball
            PongDisplay::drawRect(ballX, ballY, ballWidth, ballHeight, 1.0f, 1.0f, 1.0f);
            // draw left bumper
            PongDisplay::drawRect(0, leftBumperYPos, bumperWidth, bumperHeight, player1Color[0], player1Color[1], player1Color[2]);
            // draw right bumper
            PongDisplay::drawRect(width - 10, rightBumperYPos, bumperWidth, bumperHeight, player2Color[0], player2Color[1], player2Color[2]);
            // draw the current score
            PongDisplay::drawStrokedText(player1NameText, 0.1f, 145 - (name1Length * 3), 20, 0);
            PongDisplay::drawStrokedText(player2NameText, 0.1f, 300 - (name2Length * 3), 20, 0);
            PongDisplay::drawStrokedText(scoreText, 0.4f, 135, 80, 0);
        }
        else {
            switch (nameInput) {
                case 4:
                    PongDisplay::drawStrokedText(playerColorCText, 0.1f, 100, 170, 0);
                    PongDisplay::drawRect(350, 170, 25, 10, player2Color[0], player2Color[1], player2Color[2]);
                case 3:
                    PongDisplay::drawStrokedText(player2NameCText, 0.1f, 100, 150, 0);
                    PongDisplay::drawStrokedText(player2NameText, 0.1f, 250, 150, 0);
                case 2:
                    PongDisplay::drawStrokedText(playerColorCText, 0.1f, 100, 120, 0);
                    PongDisplay::drawRect(350, 120, 25, 10, player1Color[0], player1Color[1], player1Color[2]);
                case 1:
                    PongDisplay::drawStrokedText(player1NameCText, 0.1f, 100, 100, 0);
                    PongDisplay::drawStrokedText(player1NameText, 0.1f, 250, 100, 0);
                    break;
                default:
                    // display the menu
                    PongDisplay::drawStrokedText(titleText, 0.3f, 50, 70, 0);
                    PongDisplay::drawStrokedText(leftBumperCText, 0.1f, 120, 120, 0);
                    PongDisplay::drawStrokedText(rightBumperCText, 0.1f, 120, 150, 0);
                    PongDisplay::drawStrokedText(gameModeText, 0.1f, 120, 180, 0);
                    PongDisplay::drawStrokedText(gameModeCText, 0.09f, 120, 200, 0);
                    PongDisplay::drawStrokedText(ballSpeedText, 0.1f, 120, 230, 0);
                    PongDisplay::drawStrokedText(ballSpeedCText, 0.09f, 120, 250, 0);
                    PongDisplay::drawStrokedText(startGameText, 0.15f, 110, 400, 0);
                    break;
            }
        }
    
        glfwSwapBuffers(window);
        glfwSwapInterval(1);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}

void moveBall(){
	if (glfwGetTime() - gameTime > 1.0) {
		gameTime = glfwGetTime() * gameSpeed;
		ballX += ballVelocityX;
		storedVelocity += ballVelocityY;
		
		if (storedVelocity > 1.0f) {
			ballY += 1;
			storedVelocity -= 1;
		}
		else if(storedVelocity < -1.0f) {
			ballY -= 1;
			storedVelocity += 1;
		}
	}
	
	// do collision checking here to see if the ball needs to bounce or score
	// right edge detection
	if (ballX > width - bumperWidth*2) {
		if (inbetween(ballY, rightBumperYPos + bumperHeight, rightBumperYPos)) {
			
			ballVelocityX = -ballVelocityX;
			
			if (inbetween(ballY, rightBumperYPos, rightBumperYPos + bumperHeight*0.3))
				ballVelocityY -= 0.3;
			else if (inbetween(ballY, rightBumperYPos + bumperHeight - bumperHeight*0.3, rightBumperYPos + bumperHeight))
				ballVelocityY += 0.3;
		}
		else {
			player1Score++;
			setup();
		}
	}
	// left edge detection
	else if (ballX < bumperWidth) {
		if(inbetween(ballY, leftBumperYPos + bumperHeight, leftBumperYPos)) {
			
			ballVelocityX = -ballVelocityX;
			
			if(inbetween(ballY, leftBumperYPos, leftBumperYPos + bumperHeight*0.3))
				ballVelocityY -= 0.3;
			else if(inbetween(ballY,  leftBumperYPos + bumperHeight - bumperHeight*0.3, rightBumperYPos + bumperHeight))
				ballVelocityY += 0.3;
		}
		else {
			player2Score++;
			setup();
		}
	}
	// top edge detection
	else if(ballY < 0) {
		ballY = 0;
		ballVelocityY = -ballVelocityY;
	}
	// bottom edge detection
	else if(ballY > height - ballHeight){
		ballY = height - ballHeight;
		ballVelocityY = -ballVelocityY;
	}
}


bool inbetween(int target, int a , int b ){
	int max;
	int min;
	
	if(a > b) {
		max = a;
		min = b;
	}
	else {
		max = b;
		min = a;
	}
	
	if(target < max && target > min)
		return true;
	
	return false;
}

void setup(){
	// setup ball position
	ballX = width/2 - 5;
	ballY = height/2 - 5;
	glfwSetTime(0.0);
	gameTime = 0.0;
	ballVelocityX = ballSpeed;
	ballVelocityY = 0;
	storedVelocity = 0;
    nameInput = 0;
	
	if (multipleBalls == true) {
		addBall();
	}
	
	// setup bumpet positions
	leftBumperYPos = height/2 - bumperHeight/2;
	rightBumperYPos = height/2 - bumperHeight/2;
	
	// update the score display
	snprintf(scoreText, 20, "%d - %d\n", player1Score, player2Score);
}

void addBall(){
	ball2X = width/2 - 5;
	ball2Y = height/2 - 5;
	ball2VelocityX = -ballSpeed;
	ball2VelocityY = 0;
	storedVelocity2 = 0;
	gameTime2 = glfwGetTime();
}

void moveBall2(){
	if (glfwGetTime() - gameTime2 > 1.0) {
		gameTime2 = glfwGetTime() * gameSpeed;
		ball2X += ball2VelocityX;
		storedVelocity2 += ball2VelocityY;
		
		if (storedVelocity2 > 1.0f) {
			ball2Y += 1;
			storedVelocity2 -= 1;
		}
		else if(storedVelocity2 < -1.0f) {
			ball2Y -= 1;
			storedVelocity2 += 1;
		}
	}
	
	// do collision checking here to see if the ball needs to bounce or score
	// right edge detection
	if (ball2X > width - bumperWidth*2) {
		if (inbetween(ball2Y, rightBumperYPos + bumperHeight, rightBumperYPos)) {
			
			ball2VelocityX = -ball2VelocityX;
			
			if (inbetween(ball2Y, rightBumperYPos, rightBumperYPos + bumperHeight*0.3))
				ball2VelocityY -= 0.3;
			else if (inbetween(ball2Y, rightBumperYPos + bumperHeight - bumperHeight*0.3, rightBumperYPos + bumperHeight))
				ball2VelocityY += 0.3;
		}
		else {
			player1Score++;
			setup();
		}
	}
	// left edge detection
	else if (ball2X < bumperWidth) {
		if(inbetween(ball2Y, leftBumperYPos + bumperHeight, leftBumperYPos)) {
			
			ball2VelocityX= -ball2VelocityX;
			
			if(inbetween(ball2Y, leftBumperYPos, leftBumperYPos + bumperHeight*0.3))
				ball2VelocityY -= 0.3;
			else if(inbetween(ball2Y,  leftBumperYPos + bumperHeight - bumperHeight*0.3, rightBumperYPos + bumperHeight))
				ball2VelocityY += 0.3;
		}
		else {
			player2Score++;
			setup();
		}
	}
	// top edge detection
	else if(ball2Y < 0) {
		ball2Y = 0;
		ball2VelocityY= -ball2VelocityY;
	}
	// bottom edge detection
	else if(ball2Y > height - ballHeight){
		ball2Y = height - ballHeight;
		ball2VelocityY = -ball2VelocityY;
	}
}
