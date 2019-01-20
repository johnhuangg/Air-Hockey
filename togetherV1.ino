#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFF

#define LINEWIDTH 4
#define PADDLELENGTH 50
#define PADDLEWIDTH 5
#define SCREENWIDTH 480
#define BALLRAD 5
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
int screenWidth=tft.width();
int screenHeight=tft.height();


int joystick1x=0;
int joystick1y=0;
int joystick2x=0;
int joystick2y=0;
int boundx = 30;
int boundy = 30;
int left_score = 0;
int right_score = 0;
//ball starts center of the screen
int ballx = SCREENWIDTH/2;
int bally = tft.height()/2;
int oldballx = ballx;
int oldbally = bally;
//int ballW = 4;
//int ballH = 4;

 //velocity of the ball
int vx=5;
int vy=5;
 //paddles
int leftx = LINEWIDTH + 5*PADDLEWIDTH;
int lefty = 100;
int oldleftx = leftx;
int oldlefty = lefty;
int rightx = SCREENWIDTH - LINEWIDTH - 5*PADDLEWIDTH - PADDLEWIDTH;
int righty = 100;
int oldrightx = rightx;
int oldrighty = righty;
int temp = 0;

int state = 1;
int left_ready = 0;
int right_ready = 0;


void change_score(int left_score, int right_score) { //FAILS IF SCORE INCREMENTS BY MORE THAN ONE
//  tft.setCursor(195, LINEWIDTH +2);
//  tft.fillRect(195,0,50,20, BLACK);
  tft.setTextSize(3);
  tft.setCursor(196, LINEWIDTH+2);
  tft.setTextColor(BLACK);
  temp = left_score -1;
  tft.print(temp);
  tft.print(" : ");
  temp = right_score -1;
  tft.println(temp);
  
  tft.setCursor(196, LINEWIDTH+2);
  tft.setTextColor(WHITE);
  tft.print(left_score);
  tft.print(" : ");
  tft.println(right_score);
}
void display_score(int left_score, int right_score) { //FAILS IF SCORE INCREMENTS BY MORE THAN ONE
//  tft.setCursor(195, LINEWIDTH +2);
//  tft.fillRect(195,0,50,20, BLACK);
  tft.setTextSize(3);
  tft.setCursor(196, LINEWIDTH+2);
  tft.setTextColor(BLACK);
  tft.print(left_score);
  tft.print(" : ");
  tft.println(right_score);
  
  tft.setCursor(196, LINEWIDTH+2);
  tft.setTextColor(WHITE);
  tft.print(left_score);
  tft.print(" : ");
  tft.println(right_score);
}
void displayPuck1(int x1,int y1){
  tft.fillCircle(x1,y1,12,WHITE);
}
void deletePuck1(int x1,int y1){
  tft.fillCircle(x1,y1,12,BLACK);
}

void draw_background(){

  tft.fillRect(0,0, tft.width(), tft.height(), WHITE);
  tft.fillRect(LINEWIDTH, LINEWIDTH, (tft.width()-(LINEWIDTH*2)), (tft.height() - (LINEWIDTH*2)), BLACK);
  tft.fillRect((tft.width() - LINEWIDTH)/2, 0, LINEWIDTH, tft.height(), WHITE);

}
void reset_game(){ 
  state = 1;
  draw_background();
  left_score=0;
  right_score=0;
  ballx = SCREENWIDTH/2;
  bally = tft.height()/2;
  left_ready = 0;
  right_ready = 0;
}

void display_winner(int won){ //0 for left, 1 for right
  tft.setTextSize(3);
  if(won == 0){
    tft.setRotation(2);
    tft.setCursor(90,360);
    tft.setTextColor(WHITE);
    tft.print("WINNER!!");
    tft.setRotation(0);
    tft.setCursor(50,360);
    tft.setTextColor(WHITE);
    tft.print("you lose...");
  } else if (won == 1){
    tft.setRotation(0);
    tft.setCursor(90,360);
    tft.setTextColor(WHITE);
    tft.print("WINNER!!");
    tft.setRotation(2);
    tft.setCursor(50,360);
    tft.setTextColor(WHITE);
    tft.print("you lose...");
  }
  tft.setRotation(1);
  delay(5000);
  reset_game();
}

void setup() {
  // put your setup code here, to run once:
  //pinMode(7,INPUT);
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));
  
  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());
  Serial.print(tft.height());
//  tft.fillScreen(BLACK); 
  tft.setRotation(1);
  tft.setCursor(195, 0);
  tft.setTextColor(RED);  tft.setTextSize(3);
  draw_background();
}


void loop() {

  //states
  //1 = title screen
  //2 = main game

  while (state == 1){
    joystick1x=analogRead(A9);
//    Serial.print("X1:");
//    Serial.print(joystick1x,DEC);
  
    joystick1y=analogRead(A8);
//    Serial.print(" | Y1:");
//    Serial.print(joystick1y);
  
    joystick2x=analogRead(A11);
//    Serial.print("   X2:");
//    Serial.print(joystick2x,DEC);
  
    joystick2y=analogRead(A10);
//    Serial.print(" | Y2:");
//    Serial.println(joystick2y);
    
    if(left_ready == 0){
      tft.setRotation(2);
      tft.setTextColor(BLACK);
      tft.setCursor(30,390);
      tft.print("Move Joystick To Start");
    }
    if(right_ready == 0){
      tft.setRotation(0);
      tft.setTextColor(BLACK);
      tft.setCursor(30,390);
      tft.print("Move Joystick To Start");
    }
    
    tft.setRotation(2);
    tft.setCursor(110,360);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.print("Player 1\n");
    if(left_ready == 0){
      tft.setCursor(30,390);
      tft.print("Move Joystick To Start");
    } else if(left_ready == 1){
      tft.setTextColor(GREEN);
      tft.setCursor(130,390);
      tft.print("Ready");
    }
    tft.setRotation(0);
    tft.setCursor(110,360);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.print("Player 2\n");
    if(right_ready == 0){
      tft.setCursor(30,390);
      tft.print("Move Joystick To Start");
    } else if (right_ready == 1) {
      tft.setTextColor(GREEN);
      tft.setCursor(130,390);
      tft.print("Ready");
    }
    

    if(left_ready == 1 && right_ready == 1) {
      Serial.println("gets here");
      Serial.println(state);
      delay(1000);
      tft.setRotation(1);
      draw_background();
      change_score(0,0);
      state = 2;
    }
      
    if(joystick1y > 600|| joystick1y < 400) {
      tft.setRotation(2);
      tft.setTextColor(BLACK);
      tft.setCursor(30,390);
      tft.print("Move Joystick To Start");
      left_ready = 1;
    }
    if(joystick2y > 600 || joystick2y < 400) {
      tft.setRotation(0);
      tft.setTextColor(BLACK);
      tft.setCursor(30,390);
      tft.print("Move Joystick To Start");
      right_ready = 1;
    }
    delay(300);
  }
   
  while (state = 2){
    tft.setRotation(1);
    Serial.println("this think");
    joystick1x=analogRead(A9);
//    Serial.print("X1:");
//    Serial.print(joystick1x,DEC);
  
    joystick1y=analogRead(A8);
//    Serial.print(" | Y1:");
//    Serial.print(joystick1y, DEC);
  
    joystick2x=analogRead(A11);
//    Serial.print("   X2:");
//    Serial.print(joystick2x,DEC);
  
    joystick2y=analogRead(A10);
//    Serial.print(" | Y2:");
//    Serial.println(joystick2y, DEC);
  // put your main code here, to run repeatedly:

  //paddle movement code
  if ((lefty-(joystick1y-500)/10) > LINEWIDTH && (lefty-(joystick1y-500)/10 < tft.height()-LINEWIDTH - PADDLELENGTH)){
    lefty-=(joystick1y-500)/20;
  } else if((lefty-(joystick1y-500)/10) < LINEWIDTH) {
    lefty = LINEWIDTH;
  } else if (lefty-(joystick1y-500)/10 > tft.height()-LINEWIDTH - PADDLELENGTH) {
    lefty = tft.height()-LINEWIDTH - PADDLELENGTH;
  }
  
  if ((righty+(joystick2y-500)/10) > LINEWIDTH && (righty+(joystick2y-500)/10 < tft.height()-LINEWIDTH - PADDLELENGTH)){
    righty+=(joystick2y-500)/20;
  } else if((righty+(joystick2y-500)/10) < LINEWIDTH) {
    righty = LINEWIDTH;
  } else if (righty+(joystick2y-500)/10 > tft.height()-LINEWIDTH - PADDLELENGTH) {
    righty = tft.height()-LINEWIDTH - PADDLELENGTH;
  }
//  if ((leftx+(joystick1x-500)/10)>0&&(leftx+(joystick1x-500)/10<tft.width())){
//    leftx+=(joystick1x-500)/20;
//  }
//  if ((rightx+(joystick2x-500)/10)>0&&(rightx+(joystick2x-500)/10<tft.width())){
//    rightx+=(joystick2x-504)/20;  
//  }

  //paddle redraw code
  if (oldlefty != lefty || oldleftx != leftx) {
    tft.fillRect(oldleftx,oldlefty,PADDLEWIDTH, PADDLELENGTH,BLACK);
  }
  if (oldrighty != righty || oldrightx != rightx) {
    tft.fillRect(oldrightx,oldrighty,PADDLEWIDTH, PADDLELENGTH,BLACK);
  }
  tft.fillRect(rightx,righty,PADDLEWIDTH, PADDLELENGTH,BLUE);
  tft.fillRect(leftx,lefty,PADDLEWIDTH, PADDLELENGTH,RED);
  
  oldlefty=lefty;
  oldrighty=righty;
  oldleftx=leftx;
  oldrightx=rightx;
  //joystick1 = digitalRead(7);  // read Button state [0,1]
  //Serial.print(" | Button:");
  //Serial.println(joystick1, DEC);

  //ball movement in y direction
  if ((bally+vy > LINEWIDTH+BALLRAD) && (bally+vy < tft.height()-LINEWIDTH-BALLRAD)){
    bally+=vy;
  } else if((bally+vy <= LINEWIDTH+BALLRAD)) {
    bally = LINEWIDTH + BALLRAD;
    vy = -vy;

  } else if (bally+vy >= tft.height()-LINEWIDTH - BALLRAD){
    bally = tft.height() - LINEWIDTH - BALLRAD-1;
    vy = -vy;

  }

  //paddle collision detection (also a bit of movement in x direction)
  if(ballx+vx <= leftx + PADDLEWIDTH + BALLRAD && bally >= lefty && bally <= lefty + PADDLELENGTH) { //left paddle
      ballx = leftx + PADDLEWIDTH + BALLRAD;
      vx += 1;
      vx = -vx;      
  } else if (ballx + vx >= rightx - BALLRAD && bally >= righty && bally <= righty + PADDLELENGTH) { //right paddle
    ballx = rightx - BALLRAD;
    vx += 1;
    vx = -vx;
  } 
  //ball movement in x direction
  if ((ballx+vx > LINEWIDTH+BALLRAD) && (ballx+vx < SCREENWIDTH - LINEWIDTH - BALLRAD)){
    if ((ballx+vx>(SCREENWIDTH/2-50))&&ballx+vx<(SCREENWIDTH/2+50)){
      tft.fillRect((tft.width() - LINEWIDTH)/2, 0, LINEWIDTH, tft.height(), WHITE);
    }
    ballx+=vx;
  } else if((ballx+vx <= LINEWIDTH+BALLRAD)) {
    ballx = LINEWIDTH + BALLRAD;
    vx = -vx;
    if (right_score == 9){
      display_winner(1);
      reset_game();
      break;
    } if (right_score<9){
      right_score++;      
      vx = 5;
    }    
    change_score(left_score,right_score);
    ballx=SCREENWIDTH/2;
    bally=tft.height()/2;
    if (oldbally != bally || oldballx != ballx) {
       tft.fillCircle(oldballx,oldbally,BALLRAD,BLACK);
    }
    delay(500);
  } else if (ballx+vx >= SCREENWIDTH - LINEWIDTH - BALLRAD){
    ballx = SCREENWIDTH - LINEWIDTH - BALLRAD-1;
    vx = -vx;
    if (left_score == 9){
      display_winner(0);
      reset_game();
      break;
    }
    if (left_score<9){
      left_score++;
      vx = 5;
    }
    change_score(left_score,right_score);
    ballx=SCREENWIDTH/2;
    bally=tft.height()/2;
    if (oldbally != bally || oldballx != ballx) {
      tft.fillCircle(oldballx,oldbally,BALLRAD,BLACK);
    }
    delay(500);
  }
  if (ballx>(SCREENWIDTH/2-50)&&ballx<(SCREENWIDTH/2+50)&&bally>LINEWIDTH&&bally<60){
    display_score(left_score,right_score);
  }
  if (oldbally != bally || oldballx != ballx) {
    tft.fillCircle(oldballx,oldbally,BALLRAD,BLACK);
  }
  

  tft.fillCircle(ballx,bally,BALLRAD,WHITE);

    
  oldballx = ballx;
  oldbally = bally;


    
  delay(10);
  }
}
