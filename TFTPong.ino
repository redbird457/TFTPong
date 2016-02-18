/*

 TFT Pong, with two paddles, score and hold-to-pause button

 This example for the Arduino screen reads the values
 of 2 potentiometers to move a rectangular platform
 on the x and y axes. The platform can intersect
 with a ball causing it to bounce.

 This example shows the score when a momentary contact button is pressed

 This example code is in the public domain.

 Created by Tom Igoe December 2012
 Modified 15 April 2013 by Scott Fitzgerald
 Further modified 15 February 2016 by Ron & Jon Sege

 Parts
 -----
 160x128 tft display ST7735
 
 Momentary contact button

 2- 10x potenteometers
 
 Arduino Uno

 Wiring
 ------
 lite = vcc
 MISO = 12
 SCK = 13
 MOSI = 11
 TFT_CS = 10
 DC = 9
 Vcc = 5v
 Gnd = GND

 Pot1 = 5v, A0, GN
 Pot2 = 5v, A1, GN

 button = A2, 5V
 
 http://www.arduino.cc/en/Tutorial/TFTPong

 */

#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8

// pin definition for the Leonardo
// #define cs   7
// #define dc   0
// #define rst  1

TFT TFTscreen = TFT(cs, dc, rst);

// variables for the position of the ball and paddle
int paddleX1 = 0;
int paddleY1 = 0;
int oldPaddleX1, oldPaddleY1;
int ballDirectionX = 1;
int ballDirectionY = 1;
int paddleWidth1 = 15;
int paddleX2 = 0;
int paddleY2 = 0;
int oldPaddleX2, oldPaddleY2;
int paddleWidth2 = 15;
int score1 = 0;
int score2 = 0;
char s1Printout[4], s2Printout[4];

int ballSpeed = 10; // lower numbers are faster

int ballX, ballY, oldBallX, oldBallY;

//button variables
//int buttonpress = 0;
//int pause = 0;
//int oldpause = 0;
int counter1 = 0;

void setup() {
  // initialize the display
  TFTscreen.begin();
  // black background
  TFTscreen.background(0, 0, 0);
  Serial.begin(9600);
}

void loop() {

  // save the width and height of the screen
  int myWidth = TFTscreen.width();
  int myHeight = TFTscreen.height();

//Show score when button pressed

while (analogRead(A2) > 900) {
  score(score1, 5, TFTscreen.height()/2, score2, TFTscreen.width() - 10, TFTscreen.height()/2);
  counter1 = 1;
}

//Clear score after button released

if (counter1 == 1) {
    clearScore(score1, 5, TFTscreen.height()/2, score2, TFTscreen.width() - 10, TFTscreen.height()/2);
    counter1 = 0;
}

      
      // map the paddle's location to the position of the potentiometers
      paddleX1 = map(analogRead(A0), 512, -512, myWidth/2, myWidth*1.5) - paddleWidth1 / 2; //ToLow and ToHigh adjusted to fit paddle on screen
      paddleY1 = myHeight/6;

      paddleX2 = map(analogRead(A1), 512, -512, myWidth/2, myWidth*1.5) - paddleWidth2 / 2; //ToLow and ToHigh adjusted to fit paddle on screen
      paddleY2 = 5*myHeight/6;

      // set the fill color to black and erase the previous
      // position of the paddle if different from present
      TFTscreen.fill(0, 0, 0);

     if (oldPaddleX1 != paddleX1 || oldPaddleY1 != paddleY1) {
        TFTscreen.rect(oldPaddleX1, oldPaddleY1, paddleWidth1, 5);
      }

     if (oldPaddleX2 != paddleX2 || oldPaddleY2 != paddleY2) {
        TFTscreen.rect(oldPaddleX2, oldPaddleY2, paddleWidth2, 5);
      }

      // draw the paddle on screen, save the current position
      // as the previous.
      TFTscreen.fill(255, 255, 255);

      TFTscreen.rect(paddleX1, paddleY1, paddleWidth1, 5);
      TFTscreen.rect(paddleX2, paddleY2, paddleWidth2, 5);
      oldPaddleX1 = paddleX1;
      oldPaddleY1 = paddleY1;

      oldPaddleX2 = paddleX2;
      oldPaddleY2 = paddleY2;

      // update the ball's position and draw it on screen
      if (millis() % ballSpeed < 2) {
        moveBall();
      }
    }


// this function determines the ball's position on screen
void moveBall() {
  // if the ball goes offscreen, reverse the direction:
  if (ballX > TFTscreen.width() || ballX < 0) {
    ballDirectionX = -ballDirectionX;
  }

  if (ballY > TFTscreen.height() || ballY < 0) {
    ballDirectionY = -ballDirectionY;
  }

  // check if the ball and the paddle occupy the same space on screen
  if (inPaddle(ballX, ballY, paddleX1, paddleY1, paddleWidth1, 5) || inPaddle(ballX, ballY, paddleX2, paddleY2, paddleWidth2, 5)) {
    ballDirectionX = ballDirectionX;
    ballDirectionY = -ballDirectionY;
  }

  // update the ball's position
  ballX += ballDirectionX;
  ballY += ballDirectionY;

  // erase the ball's previous position
  TFTscreen.fill(0, 0, 0);

  if (oldBallX != ballX || oldBallY != ballY) {
    TFTscreen.rect(oldBallX, oldBallY, 5, 5);
  }


  // draw the ball's current position
  TFTscreen.fill(255, 255, 255);
  TFTscreen.rect(ballX, ballY, 5, 5);

  oldBallX = ballX;
  oldBallY = ballY;

  //Update the score if the ball hits a boundary
  
  if (ballY > TFTscreen.height() & ballDirectionY > 0){
   score1 = score1 + 1;
   }

 if (ballY < 0 & ballDirectionY < 0) {
    score2 = score2 + 1;
  }

}

//This function writes the score

void score(int score1, int16_t x1, int16_t y1, int score2, int16_t x2, int16_t y2){

  // erase the old paddle and ball objects
  TFTscreen.fill(0, 0, 0);
  TFTscreen.stroke(0,0,0);
  TFTscreen.rect(oldPaddleX1, oldPaddleY1, paddleWidth1, 5);
  TFTscreen.rect(oldPaddleX2, oldPaddleY2, paddleWidth2, 5);
  TFTscreen.rect(oldBallX, oldBallY, 5, 5);
  
  // convert the current score to a character array
  String s1 = String(score1);
  s1.toCharArray(s1Printout, 4);
  String s2 = String(score2);
  s2.toCharArray(s2Printout, 4);

  // write the current score
  TFTscreen.stroke(255, 255, 255);  
  TFTscreen.text(s1Printout, x1, y1);
  TFTscreen.text(s2Printout, x2, y2);
  
}

//This function clears the score

void clearScore(int score1, int16_t x1, int16_t y1, int score2, int16_t x2, int16_t y2){
  
  // clear any previous score that was written
  TFTscreen.stroke(0,0,0);
  TFTscreen.fill(0,0,0);
  TFTscreen.text(s1Printout, x1, y1);
  TFTscreen.text(s2Printout, x2, y2);
  
}

// this function checks the position of the ball
// to see if it intersects with the paddle
boolean inPaddle(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight) {
  boolean result = false;

  if ((x >= rectX && x <= (rectX + rectWidth)) &&
      (y >= rectY && y <= (rectY + rectHeight))) {
    result = true;
  }

  return result;
}
