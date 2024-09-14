#include <LiquidCrystal.h>
#include "LedControl.h"
#include "ArduinoComponents.h"
// #include <vector>

// stacker activate
const int buttonPin = 34;
// snake activate
const int buttonPin2 = 50;

const int ledPin = 22;

// Reset button
const int mainPin = 51;

// JoyStick set up buttons
int VRx = A0;
int VRy = A1;
int SW = 41;

int xVal = 0;
int yVal;
int SW_Val;
int mapX;
int mapY;

int buttonState = 0;
int buttonState2 = 0;
int mainState = 0;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

LedControl ld = LedControl(7, 8, 9, 1);
unsigned i;

typedef struct task
{
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
} task;

const unsigned tasksNum = 3;
task tasks[tasksNum];

bool bool_StackGame = false;
bool bool_SnakeGame = false;

// Overarching state machine running arcade
enum Game_States
{
  Init,
  Stacker,
  Snake
};
int Game_GameFunction(int state)
{
  switch (state)
  {
  case Init:
    if (buttonState == HIGH)
    {

      state = Stacker;
    }
    else if (buttonState2 == HIGH)
    {
      state = Snake;
    }
    else
    {
      state = Init;
    }
    break;

  case Stacker:
    if (mainState == HIGH)
    {
      state = Init;
    }
    else
    {
      bool_StackGame = true;
      state = Stacker;
    }
    break;

  case Snake:
    if (mainState == HIGH)
    {
      state = Init;
    }
    else
    {
      bool_SnakeGame = true;
      state = Snake;
    }
    break;
  }

  switch (state)
  {
  case Init:
    // Print a message to the LCD.

    lcd.clear();
    lcd.print("1. Stacker");
    lcd.setCursor(0, 1);
    lcd.print("2. Snake");
    lcd.setCursor(17, 2);
    break;

  case Stacker:
    // bool_StackGame = true;
    // bool_SnakeGame = false;

    break;

  case Snake:
    // bool_SnakeGame = true;
    // bool_StackGame = false;

    break;
  }
  return state;
}

int currRow;
unsigned currPos;
bool initialNums[8];
unsigned resetVal;
unsigned checkGameOver;

enum Stacker_States
{
  StartStacker,
  Glow,
  RightLevel,
  LeftLevel,
  Drop,
  GameOver,
  StackerVictory
};
int Game_StackerFunction(int state)
{
  // switch states
  switch (state)
  {
  case StartStacker:

    if (bool_StackGame == true)
    {
      currRow = 7;
      currPos = 0;
      for (resetVal = 0; resetVal < 8; ++resetVal)
      {
        initialNums[resetVal] = true;
      }
      checkGameOver = 0;

      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print("Stacker!");
      state = Glow;
    }
    else
    {
      state = StartStacker;
    }
    break;

  case Glow:
    state = RightLevel;
    break;

  case RightLevel:
    if (SW_Val == LOW)
    {
      state = Drop;
    }
    else if (currPos == 6)
    {
      state = LeftLevel;
    }
    else
    {
      state = RightLevel;
    }
    break;

  case LeftLevel:
    if (SW_Val == LOW)
    {
      state = Drop;
    }
    else if (currPos == 1)
    {
      state = RightLevel;
    }
    else
    {
      state = LeftLevel;
    }
    break;

  case Drop:
    checkGameOver = 0;
    for (resetVal = 0; resetVal < 8; ++resetVal)
    {
      if (initialNums[resetVal] == false)
      {
        ++checkGameOver;
      }
    }
    if (checkGameOver == 8)
    {
      state = GameOver;
    }
    else if (currRow == -1)
    {
      // Change this to victory screen
      state = StackerVictory;
    }
    else
    {
      state = Glow;
    }
    break;

  case StackerVictory:

    if (mainState == HIGH)
    {
      ld.clearDisplay(0);
      state = StartStacker;
    }
    else
    {
      state = StackerVictory;
    }
    bool_StackGame = false;
    break;

  case GameOver:

    if (mainState == HIGH)
    {
      bool_StackGame = false;
      ld.clearDisplay(0);
      state = StartStacker;
    }
    else
    {

      state = GameOver;
    }

    break;
  }

  // action states
  switch (state)
  {
  case StartStacker:

    break;

  case Glow:
    currPos = 2;
    ld.setLed(0, currRow, currPos - 1, true);
    ld.setLed(0, currRow, currPos, true);
    ld.setLed(0, currRow, currPos + 1, true);

    break;

  case RightLevel:
    // k = k;
    ld.setLed(0, currRow, currPos - 1, false);
    ld.setLed(0, currRow, currPos, true);
    ld.setLed(0, currRow, currPos + 1, true);
    ld.setLed(0, currRow, currPos + 2, true);
    ++currPos;

    break;

  case LeftLevel:
    // k = k;
    ld.setLed(0, currRow, currPos + 1, false);
    ld.setLed(0, currRow, currPos, true);
    ld.setLed(0, currRow, currPos - 1, true);
    ld.setLed(0, currRow, currPos - 2, true);
    --currPos;

    break;

  case Drop:

    if (currRow == 7)
    {
      for (resetVal = 0; resetVal < 8; ++resetVal)
      {
        initialNums[resetVal] = false;
      }
      initialNums[currPos - 1] = true;
      initialNums[currPos] = true;
      initialNums[currPos + 1] = true;
    }
    else if (currRow < 7)
    {
      if (initialNums[currPos - 1] == false && initialNums[currPos] == false && initialNums[currPos + 1] == false)
      {

        for (resetVal = 0; resetVal < 8; ++resetVal)
        {
          initialNums[resetVal] = false;
        }
        ld.setLed(0, currRow, currPos - 1, false);
        ld.setLed(0, currRow, currPos, false);
        ld.setLed(0, currRow, currPos + 1, false);
      }
      else if (initialNums[currPos - 1] == false && initialNums[currPos] == false && initialNums[currPos + 1] == true)
      {
        for (resetVal = 0; resetVal < 8; ++resetVal)
        {
          initialNums[resetVal] = false;
        }
        initialNums[currPos - 1] = false;
        initialNums[currPos] = false;
        initialNums[currPos + 1] = true;
        ld.setLed(0, currRow, currPos - 1, false);
        ld.setLed(0, currRow, currPos, false);
        ld.setLed(0, currRow, currPos + 1, true);
      }
      else if (initialNums[currPos - 1] == false && initialNums[currPos] == true && initialNums[currPos + 1] == true)
      {
        for (resetVal = 0; resetVal < 8; ++resetVal)
        {
          initialNums[resetVal] = false;
        }
        initialNums[currPos - 1] = false;
        initialNums[currPos] = true;
        initialNums[currPos + 1] = true;
        ld.setLed(0, currRow, currPos - 1, false);
        ld.setLed(0, currRow, currPos, true);
        ld.setLed(0, currRow, currPos + 1, true);
      }
      else if (initialNums[currPos - 1] == true && initialNums[currPos] == true && initialNums[currPos + 1] == true)
      {
        for (resetVal = 0; resetVal < 8; ++resetVal)
        {
          initialNums[resetVal] = false;
        }
        initialNums[currPos - 1] = true;
        initialNums[currPos] = true;
        initialNums[currPos + 1] = true;
        ld.setLed(0, currRow, currPos - 1, true);
        ld.setLed(0, currRow, currPos, true);
        ld.setLed(0, currRow, currPos + 1, true);
      }
      else if (initialNums[currPos - 1] == false && initialNums[currPos] == true && initialNums[currPos + 1] == false)
      {
        for (resetVal = 0; resetVal < 8; ++resetVal)
        {
          initialNums[resetVal] = false;
        }
        initialNums[currPos - 1] = false;
        initialNums[currPos] = true;
        initialNums[currPos + 1] = false;
        ld.setLed(0, currRow, currPos - 1, false);
        ld.setLed(0, currRow, currPos, true);
        ld.setLed(0, currRow, currPos + 1, false);
      }
      else if (initialNums[currPos - 1] == true && initialNums[currPos] == true && initialNums[currPos + 1] == false)
      {
        for (resetVal = 0; resetVal < 8; ++resetVal)
        {
          initialNums[resetVal] = false;
        }
        initialNums[currPos - 1] = true;
        initialNums[currPos] = true;
        initialNums[currPos + 1] = false;
        ld.setLed(0, currRow, currPos - 1, true);
        ld.setLed(0, currRow, currPos, true);
        ld.setLed(0, currRow, currPos + 1, false);
      }
      else if (initialNums[currPos - 1] == true && initialNums[currPos] == false && initialNums[currPos + 1] == false)
      {
        for (resetVal = 0; resetVal < 8; ++resetVal)
        {
          initialNums[resetVal] = false;
        }
        initialNums[currPos - 1] = true;
        initialNums[currPos] = false;
        initialNums[currPos + 1] = false;
        ld.setLed(0, currRow, currPos - 1, true);
        ld.setLed(0, currRow, currPos, false);
        ld.setLed(0, currRow, currPos + 1, false);
      }
    }
    // ld.setLed(0, currRow, currPos-2, true);

    --currRow;
    break;

  case GameOver:
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Game Over!");
    break;

  case StackerVictory:
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Congratulations!!");
    break;
  }
  return state;
}

struct point
{
  int xC;
  int yC;
};

point fullBody[100];
point tempBody[100];
point snack;

unsigned snakeSize;

point newPoint;

unsigned movingVal;

bool snackieCheck;

bool touchingBody;
unsigned b;

// bool onBody( point checker){
//   touchingBody = false;
//     for ( b = 1; b < snakeSize; ++b ){
//       if ( (checker.xC == fullBody[b].xC) && (checker.yC == fullBody[b].yC) ){
//         touchingBody = true;
//       }
//     }
//     return touchingBody;
// }

enum Snake_States
{
  StartSnake,
  Spawn,
  Right,
  Left,
  Up,
  Down,
  SnakeGameOver
};
int Game_SnakeFunction(int state)
{
  switch (state)
  {
  case StartSnake:
    // Make sure to set this to start of new state
    if (bool_SnakeGame == true)
    {
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print("Snake!");
      ld.setLed(0, fullBody[0].yC, fullBody[0].xC, true);
      ld.setLed(0, fullBody[1].yC, fullBody[1].xC, true);
      ld.setLed(0, fullBody[2].yC, fullBody[2].xC, true);
      snakeSize = 3;
      state = Spawn;
    }
    else
    {
      state = StartSnake;
    }
    break;

  case Spawn:
    state = Right;
    break;

  case Right:

    if ((fullBody[0].xC == snack.xC) && (fullBody[0].yC == snack.yC))
    {
      ++snakeSize;
      ld.setLed(0, snack.yC, snack.xC, false);
      snack.xC = rand() % 8;
      snack.yC = rand() % 8;
      ld.setLed(0, snack.yC, snack.xC, true);
      state = Right;
    }
    else if ((fullBody[0].xC == 8) || (fullBody[0].yC == 8) || (fullBody[0].xC == -1) || (fullBody[0].yC == -1))
    {
      state = SnakeGameOver;
    }
    else if (touchingBody == true)
    {
      state = SnakeGameOver;
    }
    if (xVal < 200)
    {
      state = Left;
    }
    else if (xVal > 1000)
    {
      state = Right;
    }
    else if (yVal > 1000)
    {
      state = Down;
    }
    else if (yVal < 300)
    {
      state = Up;
    }
    break;

  case Left:

    if ((fullBody[0].xC == snack.xC) && (fullBody[0].yC == snack.yC))
    {
      ++snakeSize;
      ld.setLed(0, snack.yC, snack.xC, false);
      snack.xC = rand() % 8;
      snack.yC = rand() % 8;
      ld.setLed(0, snack.yC, snack.xC, true);
      state = Left;
    }
    else if ((fullBody[0].xC == 8) || (fullBody[0].yC == 8) || (fullBody[0].xC == -1) || (fullBody[0].yC == -1))
    {
      state = SnakeGameOver;
    }
    else if (touchingBody == true)
    {
      state = SnakeGameOver;
    }
    if (xVal < 200)
    {
      state = Left;
    }
    else if (xVal > 1000)
    {
      state = Right;
    }
    else if (yVal > 1000)
    {
      state = Down;
    }
    else if (yVal < 300)
    {
      state = Up;
    }
    break;

  case Up:

    if ((fullBody[0].xC == snack.xC) && (fullBody[0].yC == snack.yC))
    {
      ++snakeSize;
      ld.setLed(0, snack.yC, snack.xC, false);
      snack.xC = rand() % 8;
      snack.yC = rand() % 8;
      ld.setLed(0, snack.yC, snack.xC, true);
      state = Up;
    }
    else if ((fullBody[0].xC == 8) || (fullBody[0].yC == 8) || (fullBody[0].xC == -1) || (fullBody[0].yC == -1))
    {
      state = SnakeGameOver;
    }
    else if (touchingBody == true)
    {
      state = SnakeGameOver;
    }
    if (xVal < 200)
    {
      state = Left;
    }
    else if (xVal > 1000)
    {
      state = Right;
    }
    else if (yVal > 1000)
    {
      state = Down;
    }
    else if (yVal < 300)
    {
      state = Up;
    }
    break;

  case Down:

    if ((fullBody[0].xC == snack.xC) && (fullBody[0].yC == snack.yC))
    {
      ++snakeSize;
      ld.setLed(0, snack.yC, snack.xC, false);
      snack.xC = rand() % 8;
      snack.yC = rand() % 8;
      ld.setLed(0, snack.yC, snack.xC, true);
      state = Down;
    }
    else if ((fullBody[0].xC == 8) || (fullBody[0].yC == 8) || (fullBody[0].xC == -1) || (fullBody[0].yC == -1))
    {
      state = SnakeGameOver;
    }
    else if (touchingBody == true)
    {
      state = SnakeGameOver;
    }
    if (xVal < 200)
    {
      state = Left;
    }
    else if (xVal > 1000)
    {
      state = Right;
    }
    else if (yVal > 1000)
    {
      state = Down;
    }
    else if (yVal < 300)
    {
      state = Up;
    }
    break;

  case SnakeGameOver:
    if (mainState == HIGH)
    {
      ld.clearDisplay(0);
      state = StartSnake;
    }
    else
    {
      state = SnakeGameOver;
    }
    bool_SnakeGame = false;
    break;
  }

  switch (state)
  {
  case StartSnake:
    // ld.setLed(0, 1, 1, true);
    // Head of Snake
    fullBody[0].xC = 3;
    fullBody[0].yC = 4;

    fullBody[1].xC = 2;
    fullBody[1].yC = 4;
    // Tale of snake equal size - 1
    fullBody[2].xC = 1;
    fullBody[2].yC = 4;

    // fullBody[3].xC = 0;
    // fullBody[3].yC = 4;

    break;

  case Spawn:
    // ld.setLed(0, 2, 3, true);
    snack.xC = rand() % 8;
    snack.yC = rand() % 8;
    ld.setLed(0, snack.yC, snack.xC, true);
    break;

  case Right:

    // check if body is being touched by self
    touchingBody = false;
    for (b = 1; b < snakeSize; ++b)
    {
      if ((fullBody[0].xC == fullBody[b].xC) && (fullBody[0].yC == fullBody[b].yC))
      {
        touchingBody = true;
        break;
      }
    }

    for (movingVal = 0; movingVal < snakeSize; ++movingVal)
    {
      tempBody[movingVal].xC = fullBody[movingVal].xC;
      tempBody[movingVal].yC = fullBody[movingVal].yC;
    }

    for (movingVal = 1; movingVal < snakeSize; ++movingVal)
    {
      fullBody[movingVal].xC = tempBody[(movingVal - 1)].xC;
      fullBody[movingVal].yC = tempBody[(movingVal - 1)].yC;
    }

    fullBody[0].xC = fullBody[0].xC + 1;

    // Display points
    ld.clearDisplay(0);
    for (movingVal = 0; movingVal < snakeSize; ++movingVal)
    {
      ld.setLed(0, fullBody[movingVal].yC, fullBody[movingVal].xC, true);
    }
    ld.setLed(0, snack.yC, snack.xC, true);

    break;

  case Left:

    touchingBody = false;
    for (b = 1; b < snakeSize; ++b)
    {
      if ((fullBody[0].xC == fullBody[b].xC) && (fullBody[0].yC == fullBody[b].yC))
      {
        touchingBody = true;
        break;
      }
    }

    for (movingVal = 0; movingVal < snakeSize; ++movingVal)
    {
      tempBody[movingVal].xC = fullBody[movingVal].xC;
      tempBody[movingVal].yC = fullBody[movingVal].yC;
    }

    for (movingVal = 1; movingVal < snakeSize; ++movingVal)
    {
      fullBody[movingVal].xC = tempBody[(movingVal - 1)].xC;
      fullBody[movingVal].yC = tempBody[(movingVal - 1)].yC;
    }

    fullBody[0].xC = fullBody[0].xC - 1;

    // Display points
    ld.clearDisplay(0);
    for (movingVal = 0; movingVal < snakeSize; ++movingVal)
    {
      ld.setLed(0, fullBody[movingVal].yC, fullBody[movingVal].xC, true);
    }
    ld.setLed(0, snack.yC, snack.xC, true);
    break;

  case Up:

    touchingBody = false;
    for (b = 1; b < snakeSize; ++b)
    {
      if ((fullBody[0].xC == fullBody[b].xC) && (fullBody[0].yC == fullBody[b].yC))
      {
        touchingBody = true;
        break;
      }
    }

    for (movingVal = 0; movingVal < snakeSize; ++movingVal)
    {
      tempBody[movingVal].xC = fullBody[movingVal].xC;
      tempBody[movingVal].yC = fullBody[movingVal].yC;
    }

    for (movingVal = 1; movingVal < snakeSize; ++movingVal)
    {
      fullBody[movingVal].xC = tempBody[(movingVal - 1)].xC;
      fullBody[movingVal].yC = tempBody[(movingVal - 1)].yC;
    }

    fullBody[0].yC = fullBody[0].yC - 1;

    // Display points
    ld.clearDisplay(0);
    for (movingVal = 0; movingVal < snakeSize; ++movingVal)
    {
      ld.setLed(0, fullBody[movingVal].yC, fullBody[movingVal].xC, true);
    }
    ld.setLed(0, snack.yC, snack.xC, true);
    break;

  case Down:

    touchingBody = false;
    for (b = 1; b < snakeSize; ++b)
    {
      if ((fullBody[0].xC == fullBody[b].xC) && (fullBody[0].yC == fullBody[b].yC))
      {
        touchingBody = true;
        break;
      }
    }

    for (movingVal = 0; movingVal < snakeSize; ++movingVal)
    {
      tempBody[movingVal].xC = fullBody[movingVal].xC;
      tempBody[movingVal].yC = fullBody[movingVal].yC;
    }

    for (movingVal = 1; movingVal < snakeSize; ++movingVal)
    {
      fullBody[movingVal].xC = tempBody[(movingVal - 1)].xC;
      fullBody[movingVal].yC = tempBody[(movingVal - 1)].yC;
    }

    fullBody[0].yC = fullBody[0].yC + 1;

    // Display points
    ld.clearDisplay(0);
    for (movingVal = 0; movingVal < snakeSize; ++movingVal)
    {
      ld.setLed(0, fullBody[movingVal].yC, fullBody[movingVal].xC, true);
    }
    ld.setLed(0, snack.yC, snack.xC, true);
    break;

  case SnakeGameOver:
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Game Over!");
    break;
  }
  return state;
}

void setup()
{
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(mainPin, INPUT);

  // Joystick setup
  Serial.begin(9600);
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  ld.shutdown(0, false);
  ld.setIntensity(0, 8);
  // ld.setLed(0, 0, 0, true);
  //  ld.setRow(0, 0)

  i = 0;
  tasks[i].state = Init;
  tasks[i].period = 250;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &Game_GameFunction;
  ++i;
  tasks[i].state = StartStacker;
  tasks[i].period = 150;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &Game_StackerFunction;
  ++i;
  tasks[i].state = StartSnake;
  tasks[i].period = 200;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &Game_SnakeFunction;

  // srand(time(NULL));
}

// snake systemo set

void loop()
{
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(buttonPin);
  buttonState2 = digitalRead(buttonPin2);
  mainState = digitalRead(mainPin);
  xVal = analogRead(VRx);
  yVal = analogRead(VRy);
  SW_Val = digitalRead(SW);
  // Serial.print(" X Value = ");
  // Serial.print(xVal);
  // Serial.print(" Y Value = ");
  // Serial.print(yVal);
  mapX = map(xVal, 0, 1023, -512, 512);
  mapY = map(yVal, 0, 1023, -512, 512);

  for (int i = 0; i < tasksNum; ++i)
  {
    if ((millis() - tasks[i].elapsedTime) >= tasks[i].period)
    {
      tasks[i].state = tasks[i].TickFct(tasks[i].state);
      tasks[i].elapsedTime = millis(); // have to count the time compared to last time it was run
    }
  }
}
