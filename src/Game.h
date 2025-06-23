#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <StandardCplusplus.h>
#include <vector>

struct Obstacle
{
    int x;
    int y;
};


struct Player
{
    int x;
    int y;
    int score;
};



class Game{
    private:
        LiquidCrystal* lcd_display;
        
        bool game_over;
        bool can_show_start;

        bool is_top=true;

        byte lastButtonUpState = LOW;
        byte lastButtonDownState = LOW;


        Player player;
        std::vector<Obstacle> obstacles;
        
        unsigned long start_time, current_time, elapsed_time, obs_spawn_rate, obs_speed;
        unsigned long start_obs_time, current_obs_time, elapsed_obs_time;

    public:
        Game(LiquidCrystal* lcd);

        void init_lcd();
        void init_inputs();
        void show_text_testing();
        void show_player_score();
        void clear_lcd();

        void game_loop();
        void show_start_screen();
        void change_wait_scene();

        void render();
        void render_player();
        void render_obstacles();

        void create_update_obstacles();
        void move_obstacles();
        void detect_collsions();

        void get_inputs();
        void button_up_action();
        void button_down_action();
};


#endif

