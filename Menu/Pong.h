#include "lib.h"

//Define Pins
#define OLED_RESET 4
#define BEEPER 3
#define CONTROL_A A0
#define CONTROL_B A1

//Define Visuals
#define FONT_SIZE 2
#define SCREEN_WIDTH 83  //real size minus 1, because coordinate system starts with 0
#define SCREEN_HEIGHT 47  //real size minus 1, because coordinate system starts with 0
#define PADDLE_WIDTH 4
#define PADDLE_HEIGHT 10
#define PADDLE_PADDING 10
#define BALL_SIZE 3
#define SCORE_PADDING 10

#define PADDLE_SPEED 4

#define EFFECT_SPEED 0.5
#define MIN_Y_SPEED 0.1
#define MAX_Y_SPEED 1.5


//Define Variables
short int paddleLocationA = 0;
short int paddleLocationB = 0;

float ballX = SCREEN_WIDTH/2;
float ballY = SCREEN_HEIGHT/2;
float ballSpeedX = 0.2;
float ballSpeedY = 0.1;

short int lastPaddleLocationA = 0;
short int lastPaddleLocationB = 0;

short int scoreA = 0;
short int scoreB = 0;

short int controlA = 500;
short int controlB = 500;

bool QuitFlag = false;

void Movement();
void draw();
void addEffect(int paddleSpeed);
void centerPrint(char *text, int y, int size);
void splash();
void setupPong();

void Movement(){

  if(keyDetection(0,7) && controlA>10){
    controlA -= PADDLE_SPEED;
  }
  if(keyDetection(3,7) && controlA<1013){
    controlA += PADDLE_SPEED;
  }
  if(keyDetection(4,7) && controlB>10){
    controlB -= PADDLE_SPEED;
  }
  if(keyDetection(7,7) && controlB<1013){
    controlB += PADDLE_SPEED;
  }

  if(keyDetection(5,7)){
    paddleLocationA = 0;
    paddleLocationB = 0;

    ballX = SCREEN_WIDTH/2;
    ballY = SCREEN_HEIGHT/2;
    ballSpeedX = 0.2;
    ballSpeedY = 0.1;

    lastPaddleLocationA = 0;
    lastPaddleLocationB = 0;

    scoreA = 0;
    scoreB = 0;

    controlA = 500;
    controlB = 500;

    return setupPong();
  }

  paddleLocationA = map(controlA, 0, 1023, 0, SCREEN_HEIGHT - PADDLE_HEIGHT);
  paddleLocationB = map(controlB, 0, 1023, 0, SCREEN_HEIGHT - PADDLE_HEIGHT);

  int paddleSpeedA = paddleLocationA - lastPaddleLocationA;
  int paddleSpeedB = paddleLocationB - lastPaddleLocationB;

  ballX += ballSpeedX;
  ballY += ballSpeedY;

  //bounce from top and bottom
  if (ballY >= SCREEN_HEIGHT - BALL_SIZE || ballY <= 0) {
    ballSpeedY *= -1;
    //soundBounce();
  }

  //bounce from paddle A
  if (ballX >= PADDLE_PADDING && ballX <= PADDLE_PADDING+BALL_SIZE && ballSpeedX < 0) {
    if (ballY > paddleLocationA - BALL_SIZE && ballY < paddleLocationA + PADDLE_HEIGHT) {
      //soundBounce();
      ballSpeedX *= -1;
    
      addEffect(paddleSpeedA);
    }

  }

  //bounce from paddle B
  if (ballX >= SCREEN_WIDTH-PADDLE_WIDTH-PADDLE_PADDING-BALL_SIZE && ballX <= SCREEN_WIDTH-PADDLE_PADDING-BALL_SIZE && ballSpeedX > 0) {
    if (ballY > paddleLocationB - BALL_SIZE && ballY < paddleLocationB + PADDLE_HEIGHT) {
      //soundBounce();
      ballSpeedX *= -1;
    
      addEffect(paddleSpeedB);
    }

  }

  //score points if ball hits wall behind paddle
  if (ballX >= SCREEN_WIDTH - BALL_SIZE || ballX <= 0) {
    if (ballSpeedX > 0) {
      scoreA++;
      ballX = SCREEN_WIDTH / 4;
    }
    if (ballSpeedX < 0) {
      scoreB++;
      ballX = SCREEN_WIDTH / 4 * 3;
    }
    ballSpeedY = 0.1;
    //soundPoint();   
  }

  //set last paddle locations
  lastPaddleLocationA = paddleLocationA;
  lastPaddleLocationB = paddleLocationB;  
}

void draw(){
  u8g2.firstPage();
  //u8g2.clearDisplay(); 
  do{
    //draw paddle A
    u8g2.drawBox(PADDLE_PADDING,paddleLocationA,PADDLE_WIDTH,PADDLE_HEIGHT);

    //draw paddle B
    u8g2.drawBox(SCREEN_WIDTH-PADDLE_WIDTH-PADDLE_PADDING,paddleLocationB,PADDLE_WIDTH,PADDLE_HEIGHT);

    //draw center line
    for (int i=0; i<SCREEN_HEIGHT; i+=4) {
      u8g2.drawVLine(SCREEN_WIDTH/2, i, 2);
    }

    //draw ball
    u8g2.drawBox(ballX,ballY,BALL_SIZE,BALL_SIZE);

    //print scores
    
    int scoreAWidth = 5 * FONT_SIZE;
    if (scoreA > 9) scoreAWidth += 6 * FONT_SIZE;
    if (scoreA > 99) scoreAWidth += 6 * FONT_SIZE;
    if (scoreA > 999) scoreAWidth += 6 * FONT_SIZE;
    if (scoreA > 9999) scoreAWidth += 6 * FONT_SIZE;

    u8g2.setCursor(SCREEN_WIDTH/2 - SCORE_PADDING - scoreAWidth,10);
    u8g2.print(scoreA);

    u8g2.setCursor(SCREEN_WIDTH/2 + SCORE_PADDING+1,10); //+1 because of dotted line.
    u8g2.print(scoreB);
  }while(u8g2.nextPage());
} 

void addEffect(int paddleSpeed){
  float oldBallSpeedY = ballSpeedY;

  //add effect to ball when paddle is moving while bouncing.
  //for every pixel of paddle movement, add or substact EFFECT_SPEED to ballspeed.
  for (int effect = 0; effect < abs(paddleSpeed); effect++) {
    if (paddleSpeed > 0) {
      ballSpeedY += EFFECT_SPEED;
    } else {
      ballSpeedY -= EFFECT_SPEED;
    }
  }

  //limit to minimum speed
  if (ballSpeedY < MIN_Y_SPEED && ballSpeedY > -MIN_Y_SPEED) {
    if (ballSpeedY > 0) ballSpeedY = MIN_Y_SPEED;
    if (ballSpeedY < 0) ballSpeedY = -MIN_Y_SPEED;
    if (ballSpeedY == 0) ballSpeedY = oldBallSpeedY;
  }

  //limit to maximum speed
  if (ballSpeedY > MAX_Y_SPEED) ballSpeedY = MAX_Y_SPEED;
  if (ballSpeedY < -MAX_Y_SPEED) ballSpeedY = -MAX_Y_SPEED;
}

void centerPrint(char *text, int y, int Size){
  u8g2.setCursor(SCREEN_WIDTH/2 - ((strlen(text))*Size)/2,y);
  u8g2.print(text);
}

//Splash Screen
void splash(){
  bool AReady = false;
  bool BReady = false;
  while(!AReady || !BReady){
    u8g2.firstPage();
    AReady = keyDetection(3,7);
    BReady = keyDetection(7,7);
    if(keyDetection(5,7)){QuitFlag = true; return;}
      do{
        if(AReady){u8g2.drawStr(0,24,"ready");}
        if(BReady){u8g2.drawStr(64,24,"ready");}
        centerPrint("PONG", 10, 4);
        centerPrint("Both Player", 30, 4);
        centerPrint("press down to start", 40, 4);
        //u8g2.drawStr(30,10,"PONG");
      }while (u8g2.nextPage());
  }
}

//Setup 
void setupPong(){
  ControlPinsSetup();
  u8g2.begin();  // initialize with the I2C addr 0x3D (for the 128x64)
  u8g2.setFont(u8g2_font_micro_tr);
  u8g2.clearDisplay(); 
  splash();
}

//Loop
void loopPong(){
  while(!QuitFlag){
    Movement();
    draw();
  }
}

void StartPong(){
  setupPong();
  loopPong();
  QuitFlag = false;
  u8g2.setFont(u8g2_font_6x10_tf);
}
