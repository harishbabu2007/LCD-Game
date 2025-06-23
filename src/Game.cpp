#include "Game.h"

#define BUTTON_TOP_PIN 9
#define BUTTON_BOTTOM_PIN 8


byte playerChar[] = {
  0x00,
  0x01,
  0x03,
  0x1D,
  0x1D,
  0x03,
  0x01,
  0x00
};


byte obstacleChar[] = {
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F
};

Game::Game(LiquidCrystal* lcd){
    lcd_display = lcd;

    game_over = true;
    can_show_start = true;

    player.x = 15;
    player.y = 0;
    player.score = 0;

    obs_spawn_rate = 5000;
    obs_speed = 500;
}

void Game::init_lcd(){
    lcd_display->begin(16,12);

    lcd_display->createChar(0, playerChar);
    lcd_display->createChar(1, obstacleChar);
}

void Game::init_inputs(){
    Serial.begin(9600);


    pinMode(BUTTON_TOP_PIN, INPUT);
    pinMode(BUTTON_BOTTOM_PIN, INPUT);
}

void Game::clear_lcd(){
    lcd_display->setCursor(0,0);
    lcd_display->print("                ");

    lcd_display->setCursor(0,1);
    lcd_display->print("                ");
}


void Game::show_text_testing(){
    lcd_display->print("Test!~");
}

void Game::show_start_screen(){
    clear_lcd();

    lcd_display->setCursor(1,0);
    lcd_display->print("LCD Game!!");

    lcd_display->setCursor(1,1);
    lcd_display->print("Press any key..");
}


void Game::show_player_score(){
    clear_lcd();

    lcd_display->setCursor(1,0);
    lcd_display->print("GAME OVER!!!");

    lcd_display->setCursor(1,1);
    lcd_display->print("Score~");

    lcd_display->setCursor(10,1);
    lcd_display->print((String)player.score);
}


void Game::get_inputs(){
    if (digitalRead(BUTTON_TOP_PIN) == HIGH){
        button_up_action();
    } else if (digitalRead(BUTTON_BOTTOM_PIN) == HIGH) {
        button_down_action();
    }
}


void Game::change_wait_scene(){
    can_show_start = false;
    game_over = false;

    elapsed_time = 0;
}


void Game::button_up_action(){
    is_top = true;
    player.y = 0;

    if (game_over && can_show_start){
        change_wait_scene();
    }
}

void Game::button_down_action(){
    is_top = false;
    player.y = 1;

    if (game_over && can_show_start){
        change_wait_scene();
    } 
} 

void Game::move_obstacles(){
    if (elapsed_obs_time >= obs_speed){
        elapsed_obs_time = 0;
        start_obs_time = millis();
        
        // move the obstacles
        int n = obstacles.size();

        for (int i=0; i<n; i++){
            if (obstacles[i].x == 15){
                obstacles.erase(obstacles.begin() + i);
            } else {
                obstacles[i].x += 1;
            }
        }

        player.score += 1;

    } else {
        current_obs_time = millis();
        elapsed_obs_time = current_obs_time - start_obs_time;
    }
}

void Game::create_update_obstacles(){
    if (elapsed_time >= obs_spawn_rate){
        elapsed_time = 0;
        start_time = millis();
        // spawn the obstacles

        Obstacle new_obs;
        new_obs.x = 0;
        new_obs.y = (int)random(0,2);

        obstacles.push_back(new_obs);
    } else {
        current_time = millis();
        elapsed_time = current_time - start_time;
    }
}

void Game::detect_collsions(){
    int n = obstacles.size();

    for (int i=0; i<n; i++){
        if ((obstacles[i].x == 15) && (player.y == obstacles[i].y)){
            obstacles.clear();
            
            game_over = true;
            can_show_start = false;
        }
    }
}


void Game::render_player(){
    if (is_top){
        lcd_display->setCursor(player.x,player.y);
    } else {
        lcd_display->setCursor(player.x,player.y);
    }

    lcd_display->write((byte)0);
}


void Game::render_obstacles(){
    for (Obstacle obstacle: obstacles){
        lcd_display->setCursor(obstacle.x, obstacle.y);
        lcd_display->write((byte)1);
    }
}

void Game::render(){
    clear_lcd();

    // show player
    render_player();
    render_obstacles();
}


void Game::game_loop(){
    if (can_show_start == true && game_over == true){
        show_start_screen();
    }

    if (game_over == true && can_show_start == false){
        show_player_score();
    }

    detect_collsions();

    if (!game_over){

        create_update_obstacles();
        move_obstacles();

        render();
    }

    get_inputs();
}

