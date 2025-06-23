#include <Arduino.h>
#include <LiquidCrystal.h>

#include "Game.h"

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Game game(&lcd);

void setup(){
  game.init_lcd();
  game.init_inputs();
}


void loop(){
  game.game_loop();
}