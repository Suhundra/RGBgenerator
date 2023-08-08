#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

#define BUTTON_PROTECTION 50
#define HOLD_TIME 500
#define NEXT_BUTTUN_PIN 4
#define PREV_BUTTON_PIN 3
#define UP_VALUE_BUTTON_PIN 6
#define DOWN_VALUE_BUTTON_PIN 5
#define RESET_VALUE_BUTTON_PIN 2
#define RANDOM_GENERATOR_VALUE_BUTONN_PIN 7

#define RED_CURSOR 0
#define GREEN_CURSOR 5
#define BLUE_CURSOR 12

#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

const uint8_t RED = 1, GREEN = 2, BLUE = 3;

int red = 0, green = 0, blue = 0;

class Button{
private:
  uint8_t pin;
  bool prevState = false;
  uint64_t buttonTimer = 0;
  uint64_t holdTimer = 0;
  bool buttonPressedFlag = true;
  bool buttonHoldFlag = false;
 
public:
  Button(uint8_t pin){
    pinMode(pin, INPUT_PULLUP);
    this->pin = pin;
  }

  bool isClick(){
    bool reading = digitalRead(pin);
    uint64_t thisMls = millis();

    if (reading != prevState && thisMls - buttonTimer > BUTTON_PROTECTION) {
      buttonTimer = thisMls;
      if (!buttonPressedFlag && prevState) {
        buttonPressedFlag = true;
        return true;
      } else {
        buttonPressedFlag = false;
        return false;
      }
    }
    prevState = reading;
    return false;
  }

  bool isHold(){
    bool reading = digitalRead(pin);
    uint64_t thisMls = millis();

    if(thisMls - holdTimer > BUTTON_PROTECTION && !reading && reading == prevState){
      holdTimer = thisMls;
      if(thisMls - buttonTimer > HOLD_TIME)
        return true;
    }else{
      buttonHoldFlag = false;
    }

    prevState = reading;
    return false;
  }
};

Button nextButton(NEXT_BUTTUN_PIN);
Button prevButton(PREV_BUTTON_PIN);
Button upValueButton(UP_VALUE_BUTTON_PIN);
Button downValueButton(DOWN_VALUE_BUTTON_PIN);
Button resetValueButton(RESET_VALUE_BUTTON_PIN);
Button randomGenerateButton(RANDOM_GENERATOR_VALUE_BUTONN_PIN);

void setup() {
  lcd.init();                     
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Red  Green  Blue");

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  randomSeed(analogRead(0));

}

uint8_t selectedOpinion = 1; 
uint32_t holdCheck = 0;
void loop() {
  if (nextButton.isClick()){
    if(selectedOpinion < 3){ selectedOpinion++; }
  } 
  if (prevButton.isClick()){
    if(selectedOpinion > 1){ selectedOpinion--; }
  } 
  if (upValueButton.isClick()){
    uptadeRGB(1, 1);
    } 
  if (downValueButton.isClick()){
    uptadeRGB(-1, 1);
  } 
  if(millis() - holdCheck > HOLD_TIME){
    holdCheck = millis();
    if(upValueButton.isHold()){
      uptadeRGB(1, 10);
    }
    if (downValueButton.isHold()){
      uptadeRGB(-1, 10);
    } 
  }

  if(resetValueButton.isClick()){
    red = 0; green = 0; blue = 0;
  }
  

  if(randomGenerateButton.isClick()){
    red = random(0, 255); green = random(0, 255); blue = random(0, 255);
  }

  updateValueRGB();
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}




void updateValueRGB(){
  lcd.setCursor(RED_CURSOR, 1);
  lcd.print(red);
  if(red < 10){
    lcd.setCursor(RED_CURSOR+1, 1);
    lcd.print(" ");
  }
  if(red < 100){
    lcd.setCursor(RED_CURSOR+2, 1);
    lcd.print(" ");
  }
  lcd.setCursor(GREEN_CURSOR, 1);
  lcd.print(green);
  if(green < 10){
    lcd.setCursor(GREEN_CURSOR+1, 1);
    lcd.print(" ");
  }
  if(green < 100){
    lcd.setCursor(GREEN_CURSOR+2, 1);
    lcd.print(" ");
  }
  lcd.setCursor(BLUE_CURSOR, 1);
  lcd.print(blue);
  if(blue < 10){
    lcd.setCursor(BLUE_CURSOR+1, 1);
    lcd.print(" ");
  }
  if(blue < 100){
    lcd.setCursor(BLUE_CURSOR+2, 1);
    lcd.print(" ");
  }

  switch(selectedOpinion){
    case 1:
      lcd.setCursor(RED_CURSOR+3, 1);
      lcd.print("<");
      lcd.setCursor(GREEN_CURSOR+3, 1);
      lcd.print(" ");
      lcd.setCursor(BLUE_CURSOR+3, 1);
      lcd.print(" ");
      break;
    case 2:
      lcd.setCursor(RED_CURSOR+3, 1);
      lcd.print(" ");
      lcd.setCursor(GREEN_CURSOR+3, 1);
      lcd.print("<");
      lcd.setCursor(BLUE_CURSOR+3, 1);
      lcd.print(" ");
      break;
    case 3:
      lcd.setCursor(RED_CURSOR+3, 1);
      lcd.print(" ");
      lcd.setCursor(GREEN_CURSOR+3, 1);
      lcd.print(" ");
      lcd.setCursor(BLUE_CURSOR+3, 1);
      lcd.print("<");
      break;
  }
}

void uptadeRGB(int i, uint8_t step){
  switch(selectedOpinion){
    case 1:
    if(red + (i*step) >= 0 && red + (i*step) <= 255) red = red + (i*step);
      break;
    case 2:
      if(green + (i*step) >= 0 && green + (i*step) <= 255) green = green + (i*step);
      break;
    case 3:
      if(blue + (i*step) >= 0 && blue + (i*step) <= 255) blue = blue + (i*step);
      break;
  }
}
