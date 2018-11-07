#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <math.h>

// X: 0 - 83 = 84
// Y: 0 - 47 = 48

struct coord {
  float x;
  float y;

  coord(float _x, float _y) {
    x = _x;
    y = _y;
  }
};

// PORTAS USADAS PELO DISPLAY
const int pd1 = 8;
const int pd2 = 9;
const int pd3 = 10;
const int pd4 = 11;
const int pd5 = 12;

Adafruit_PCD8544 display = Adafruit_PCD8544(pd1, pd2, pd3, pd4, pd5);

enum state
{
  INITIALIZING = -1,
  TITLE_SCREEN = 0,
  MAIN_MENU = 1,
  SNAKE = 2,
  PONG = 3,
  BREAKOUT = 4
};

state current = INITIALIZING;

float currentTime = 0;
float lastTime = 0;
float deltaTime = 0;

const int buttonPin[4] = {2, 3, 4, 5}; //up, down, left, right

void drawError()
{
  display.setTextSize(2);
  display.setCursor(0, 6);
  display.println("ERROR");
}

void drawTitleScreen()
{
  display.setTextSize(2);
  display.setCursor(0, 6);
  display.println("CARDBOY");
  
  display.drawLine(0, 3, 83, 3, 1);
  display.drawLine(0, 22, 83, 22, 1);

  display.setTextSize(1);
  display.setCursor(0, 35);
  display.println("*press start*");
}

void drawMainMenu()
{
  display.fillRect(0,0,39,11,BLACK);
  display.setTextSize(1);
  
  display.setTextColor(WHITE);
  display.setCursor(2,2);
  display.println("SELECT");

  display.drawRect(0,0,83,48,BLACK);

  display.setTextColor(BLACK);
  
  display.setCursor(15,15);
  display.println("SNAKE");

  display.setCursor(15,25);
  display.println("PONG");

  display.setCursor(15,35);
  display.println("BREAKOUT");
}

void prepareScene()
{
  /*
    clear display
    check state
    draw new scene accordingly
  */
  display.clearDisplay();
  
  switch(current)
  {
    default:
    {
      drawError();
      break;
    }
    case 0:
    {
      drawTitleScreen();
      break;
    }
    case 1:
    {
      drawMainMenu();
      break;
    }
    case 2:
    {
      
      break;
    }
    case 3:
    {
      pong();
      break;
    }
    case 4:
    {
      
      break;
    }
  }
}

void setup() 
{
  Serial.begin(9600);

  for (int i = 0; i < 4; i++){
    pinMode(buttonPin[i], INPUT);
  }
  
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  display.setTextColor(BLACK);

  display.setTextSize(2);
  display.setCursor(0, 6);
  display.println("CARDBOY");
  
  display.drawLine(0, 3, 83, 3, 1);
  display.drawLine(0, 22, 83, 22, 1);

  display.display();

  delay(1500);

  //current = TITLE_SCREEN;
  current = PONG;

  lastTime = millis();
}

/*
    LOOP BASIC STRUCTURE:
    draw current scene
    check for inputs
    apply game logic
    display scene
*/

void loop() 
{
  lastTime = currentTime;
  currentTime = millis();
  deltaTime = (currentTime - lastTime)/1000;
  prepareScene();
  
  display.display();
}

/* --------------------------------------------------------- */

coord ball = coord(42, 24);
float players[2] = {24-48/4/2, 24-48/4/2};

bool right = true;
bool up = true;

void pong() { 
  ball.x += ((right?1:-1) * 20) * deltaTime;
  ball.y += ((up?1:-1) * 20) * deltaTime;

  //movimentos  

  //player
  if (GetButtonPress("Up")){
    players[0]-= 10 * deltaTime;
  }
  else if (GetButtonPress("Up")){
    players[0]+= 10 * deltaTime;
  }

  //ai
  players[1] += (ball.y - players[1])/2; //sqrt(pow(ball.y - players[1], 2));
  
  //colisões
  if (ball.x > 82){
    //player 1 ponto
    right = false;
  }

  if (ball.x < 2){
    //player 2 ponto
    right = true;
  }

  if (ball.y > 46){
    up = false;
  }
  else if (ball.y < 2){
    up = true;
  }

  //players
  if (ball.x > 84-6){
    if (ball.y > players[0] && ball.y < players[0] + 48/4){
      right = false;
    }
  } 
  
  if (ball.x < 6){
    if (ball.y > players[1] && ball.y < players[1] + 48/4){
      right = true;
    }
  }
  
  
  display.drawFastVLine(42, 0, 48, 1);
  display.drawRect(0, 0, 84, 48, 1);

  display.drawFastVLine(4, players[0], 48/4, 1);
  display.drawFastVLine(6, players[0], 48/4, 1);
  
  display.drawFastVLine(84-4, players[1], 48/4, 1);
  display.drawFastVLine(84-6, players[1], 48/4, 1);
  
  display.fillCircle(ball.x, ball.y, 2, 1);

  Serial.println(ball.x);
  //Serial.println(ball.y);
}

bool GetButtonPress(String key){
  if (key == "Up"){
    return digitalRead(buttonPin[0]);
  }
  else if (key == "Down"){
    return digitalRead(buttonPin[1]);
  }
  else if (key == "Left"){
    return digitalRead(buttonPin[2]);
  }
  else if (key == "Right"){
    return digitalRead(buttonPin[3]);
  }

  return false;
}

