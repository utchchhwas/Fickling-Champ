/*
CSE102 Term Project, BUET
Project Type: Game
Project Name: Fickling Champ
Project By:
1. Pritam Saha
   Student No: 201805102
2. Utchchhwas Singha
   Student No: 201805100
Last modified at 11:20 pm on 15 September, 2019
*/

#include "iGraphics.h"

#define N_MAIN_PILLARS 3   // Number of main pillars
#define N_EXTRA_PILLARS 3   // Number of extra pillars
#define BOUNDARY_WIDTH 20   // Width of the upper and lower boundary
#define REF_X 80            // Initial x co-ordinate of the bird
#define FILE_NAME "highscores"

const int _WinX = 800, _WinY = 600; // Window size

//  variables for pillars
int _pillars_gap;   // Gap between two adjacent pillars
int _pillars_space; // Gap between a upward and a downward pillar
int _pillars_width; // Width of a pillar
int _main_pillars_x[N_MAIN_PILLARS];
int _main_up_pillars_y[N_MAIN_PILLARS];
int _main_down_pillars_y[N_MAIN_PILLARS];
int _extra_pillars_x[N_EXTRA_PILLARS];
int _extra_up_pillars_y[N_EXTRA_PILLARS];
int _extra_down_pillars_y[N_EXTRA_PILLARS];
int _pillars_dx;
//

//  variables for boundary
int _boundary_line_starting_point_x;
//

//  variables for bird
const int _bird_x = REF_X;      // x co_ordinate of the bird. This does not change
int _bird_y;                    // y co_ordinate of the bird. This changes
const int _bird_r = 24;         // Radius of the bird. This does not change
int _bird_dy;                   // The decrease of the y co-ordinate of the bird
int _bird_dy_input;             // The increase of the y co-ordinate of the bird when the user presses space
//

//  variable for detecting collision
int _collision_pillar_index;    // This is used to identify which pillars is to be considered for collision
int _free_up_Y;                 // Highest value of the free space along the y axis for the bird
int _free_down_Y;               // Lowest value of the free space along the y axis for the bird
//

//  control variables
int _bird_fly_ctrl;             // Used to determine which flying animation to show depending on the situation
int _game_menu_play_ctrl;       // Used to know if the user has pressed play from the main menu
int _game_menu_options_ctrl;    // Used to know if the user has presses options from the main menu
int _game_menu_help_ctrl;
int _hide_pillars_ctrl;         // Used to hide pillars after collision
int _move_pillars_ctrl;         // Used to move pillars horizontally
int _game_over_ctrl;            // Used to know if the game is over
int _dynamic_pillars_ctrl;      // Used to move pillars vertically
int _pause_ctrl;                // Used to know if the user has paused the game
int _play_again_ctrl = 0;       //  This is initialized just once
int _pillars_update_ctrl;       // This is used to control the looping of main and extra pillars
int _count_pillars_ctrl;
int _loading_screen_ctrl = 1;
int _game_menu_highscores_ctrl;
int _power_collision_ctrl;
int _activate_power_ctrl;
int _game_stages_ctrl;
int _draw_ghost_ctrl;
int _collision_ctrl;
int _sound_ctrl = 1;
//

//  additional variables
int _game_menu_index;           // Used for the main menu indexing
int _game_over_menu_index;      // Used for the game over menu indexing
char _pt[10];                   // String format of the points
int _passed_pillars_count;      // Number of pillars passed by the bird
int _theme_index = 0;           // Used to select themes
int _bg_index = 0;              // Used to change background after a while
int _dynamic_main_pillars_state[N_MAIN_PILLARS];
int _dynamic_extra_pillars_state[N_EXTRA_PILLARS];
int _dynamic_pillars_dy;
int _is_high_score;
int _previous_power_pillar_count;
int _power_index;
char _player_name[100] = "";
char _popup[100];
typedef struct {
    char name[100];
    int score;
} highscore;
highscore _high_scores[6];
int _stage1, _stage2, _stage3, _stage4;
//

//  perk variables
int _Cx, _Cy;
const int _Cr = 20;
//


//  functions for controlling variables
void reset_all_values();
void update_main_pillars_values();
void update_extra_pillars_values();
void update_power_circle();
//

//  draw functions
void DrawStartButtons();
void DrawPauseMenu();
void DrawOptionsMenu();
void DrawHelp();
void DrawBoundary();
void DrawBackgroud();
void DrawBuildings();
void DrawBird();
void DrawPillars();
void DrawGameOver();
void DrawLoadingScreen();
void DrawHighScores();
void DrawPowerCircle();
void DrawPopup();
void DrawSoundIcon();
//

//  control functions
void move_pillars();
void move_bird();
void move_boundary_line();
void bird_fly();
void collision();
void dynamic_pillars();
void hide_pillars();
void move_power_circle();
void PowerCollision();
void ActivatePower();
void GameStages();
void ClearPopup();
//

//  additional functions
void ShowNumber(int x, int y, char n, int from);
int cmpt(const void *p, const void *q);
void rand_gen(int *arr, int n, int max, int min, int d_min);
void rand_switch(int *arr, int n);
void get_high_score();
void update_high_score();
void get_power_index();
//

void iDraw() {
    iClear();

    if (_loading_screen_ctrl) {
        DrawLoadingScreen();
    }
    else {
        if (_game_over_ctrl)
            DrawGameOver();
        else {
            DrawBackgroud();
            DrawPillars();
            DrawBoundary();
            DrawBird();

            if (!_power_collision_ctrl) {
                DrawPowerCircle();
            }
        }

        if (!_game_menu_play_ctrl) {
            if (_game_menu_options_ctrl) {
                DrawOptionsMenu();
            }
            else if (_game_menu_highscores_ctrl) {
                DrawHighScores();
            }
            else if (_game_menu_help_ctrl) {
                DrawHelp();
            }
            else {
                DrawStartButtons();
            }
        }
        else {
            if (_pause_ctrl) {
                DrawPauseMenu();
            }
        }
    }

//  this shows how to pause at the beginning of the game
    if (_game_menu_play_ctrl && !_passed_pillars_count && !_game_over_ctrl) {
        iShowBMP2(10, _WinY - 80, "start_prompt.bmp", 0);
    }

//  below is the code for showing score on the game screen
    if (_game_menu_play_ctrl && !_game_over_ctrl) {
        if (_passed_pillars_count < 10) {
            _pt[0] = _pt[1] = _pt[2] = '0';
            itoa(_passed_pillars_count, &_pt[3], 10);
        }
        else if (_passed_pillars_count < 100) {
            _pt[0] = _pt[1] = '0';
            itoa(_passed_pillars_count, &_pt[2], 10);
        }
        else if (_passed_pillars_count < 1000) {
            _pt[0] = '0';
            itoa(_passed_pillars_count, &_pt[1], 10);
        }
        else if (_passed_pillars_count < 10000) {
            itoa(_passed_pillars_count, &_pt[0], 10);
        }
        else {
            strcpy(_pt, "9999");
        }

        for (int i=0, x=680; _pt[i]; i++, x+=30) {
            ShowNumber(x, 545, _pt[i], 1);
        }
    }
//

    if (!_game_over_ctrl) DrawPopup();

    if (_game_menu_options_ctrl || _pause_ctrl) DrawSoundIcon();
}

void iMouseMove(int mx, int my) {

}

void iMouse(int button, int state, int mx, int my) {
    if(!_loading_screen_ctrl && !_game_menu_play_ctrl && !_game_menu_options_ctrl && !_game_menu_highscores_ctrl && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if ((mx >= 250 && mx <= 540) && (my >= 450 && my <= 535)) {
//            printf("play\n");
            if (!_play_again_ctrl) {
                _game_menu_play_ctrl = 1;   // Switching to play

//                  starting all timers for game play
                iSetTimer(10, move_pillars);
                iSetTimer(10, move_bird);
                iSetTimer(250, bird_fly);
                iSetTimer(20, dynamic_pillars);
                iSetTimer(2000, ClearPopup);
//
            }
            else {
                _game_menu_play_ctrl = 1;
                iResumeTimer(0); // This resumes move_pillars()
//                    iPauseTimer(1); // The move_bird() can't be paused because if deals with all events after the collision
                iResumeTimer(2); // This resumes move_bird()
//                   iResumeTimer(3); // This resumes bird_fly()
                iResumeTimer(3);    // This resumes dynamic_pillars()
                }
        }
        else if ((mx >= 250 && mx <= 540) && (my >= 350 && my <= 435)) {
//            printf("high scores\n");
            _game_menu_highscores_ctrl = 1;
        }
        else if ((mx >= 250 && mx <= 540) && (my >= 250 && my <= 335)) {
//            printf("options\n");
            _game_menu_options_ctrl = 1;
        }
        else if ((mx >= 250 && mx <= 540) && (my >= 150 && my <= 235)) {
//            printf("help\n");
            _game_menu_help_ctrl = 1;
            }
        else if ((mx >= 300 && mx <= 495) && (my >= 50 && my <= 145)) {
//            printf("exit\n");
                exit(0);
        }
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (_game_menu_options_ctrl || _pause_ctrl)) {
        if ((mx >= 10 && mx <= 35) && (my >= 30) && (my <= 55)) {
            if(_sound_ctrl) {
                PlaySound(NULL, NULL, SND_ASYNC);

                _sound_ctrl = 0;
            }
            else {
                PlaySound("playback.wav", NULL, SND_ASYNC | SND_LOOP);

                _sound_ctrl = 1;
            }
        }
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && _game_over_ctrl) {
        if ((mx >= 300 && mx <= 390) && (my >= 60) && (my <= 97)) {
            reset_all_values();

            _play_again_ctrl = 1;
        }
        else if ((mx >= 420 && mx <= 485) && (my >= 60) && (my <= 97)) {
            exit(0);
        }
    }
}

void iKeyboard(unsigned char key) {
    if (key == 's' && !_loading_screen_ctrl) {
        if(_sound_ctrl) {
            PlaySound(NULL, NULL, SND_ASYNC);

            _sound_ctrl = 0;
        }
        else {
            PlaySound("playback.wav", NULL, SND_ASYNC | SND_LOOP);

            _sound_ctrl = 1;
        }
    }


    if (_loading_screen_ctrl) {
        if ((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z')) {
            char temp[2];
            temp[0] = key;
            temp[1] = '\0';
            strcat(_player_name, temp);
        }
    }

    if (key == '\r') {
        if (_loading_screen_ctrl) {
            _loading_screen_ctrl = 0;
        }
        else if (!_game_menu_play_ctrl) {
            if (_game_menu_options_ctrl) {
                _game_menu_options_ctrl = 0;
            }
            else if (_game_menu_highscores_ctrl) {
                _game_menu_highscores_ctrl = 0;
            }
            else if (_game_menu_help_ctrl) {
                _game_menu_help_ctrl = 0;
            }
            else if (_game_menu_index == 0) {
                if (!_play_again_ctrl) {
                    _game_menu_play_ctrl = 1;   // Switching to play

//                  starting all timers for game play
                    iSetTimer(10, move_pillars);
                    iSetTimer(10, move_bird);
                    iSetTimer(250, bird_fly);
                    iSetTimer(20, dynamic_pillars);
                    iSetTimer(2000, ClearPopup);
//
                }
                else {
                    _game_menu_play_ctrl = 1;

                    iResumeTimer(0); // This resumes move_pillars()
//                    iPauseTimer(1); // The move_bird() can't be paused because if deals with all events after the collision
                    iResumeTimer(2); // This resumes move_bird()
//                    iResumeTimer(3); // This resumes bird_fly()
                    iResumeTimer(3);    // This resumes dynamic_pillars()
                }
            }
            else if (_game_menu_index == 1) {
                _game_menu_highscores_ctrl = 1;
            }
            else if (_game_menu_index == 2) {
                _game_menu_options_ctrl = 1;
            }
            else if (_game_menu_index == 3) {
                _game_menu_help_ctrl = 1;
            }
            else if (_game_menu_index == 4) {
                exit(0);
            }

        }
        else if (_game_over_ctrl) {
            if (_game_over_menu_index) {
                reset_all_values();

                _play_again_ctrl = 1;
            }
            else {
                exit(0);
            }
        }
        else if (_pause_ctrl) {
            _pause_ctrl = 0;

            iResumeTimer(0);
            iResumeTimer(1);
            iResumeTimer(2);
            iResumeTimer(3);
        }
        else if (_game_menu_play_ctrl) {
            _pause_ctrl = 1;

            iPauseTimer(0);
            iPauseTimer(1);
            iPauseTimer(2);
            iPauseTimer(3);
        }

    }
    else if (key == ' ') {
        if (_game_menu_play_ctrl && !_pause_ctrl) {
            _bird_y += _bird_dy_input;
            _bird_fly_ctrl = 1;
        }
    }
}

void iSpecialKeyboard(unsigned char key) {
    if (key == GLUT_KEY_END) {
        exit(0);
    }
    else if (key == GLUT_KEY_UP) {
        if (_game_menu_play_ctrl && !_pause_ctrl) {
            _bird_y += _bird_dy_input;
            _bird_fly_ctrl = 1;
        }
        else if (!_game_menu_play_ctrl) {
            if (_game_menu_options_ctrl) {
                _theme_index--;
                if (_theme_index < 0) _theme_index = 2;
            }
            else {
                if (_game_menu_index == 0)
                    _game_menu_index = 4;
                else
                    _game_menu_index--;
            }
        }
    }
    else if (key == GLUT_KEY_DOWN) {
        if (!_game_menu_play_ctrl) {
            if (_game_menu_options_ctrl) {
                _theme_index++;
                _theme_index %= 3;
            }
            else {
                ++_game_menu_index %= 5;
            }
        }
    }
    else if (key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT) {
        if (_game_over_ctrl) {
            _game_over_menu_index = !_game_over_menu_index;
        }
    }
}

int main() {
    srand(time(0)); // For seeding rand()

    reset_all_values();
    get_high_score();

    PlaySound("playback.wav", NULL, SND_ASYNC | SND_LOOP);

    iInitialize(_WinX, _WinY, "Fickling Champ");

    return 0;
}

void DrawSoundIcon() {
    if (_sound_ctrl) {
        iShowBMP2(10, 30, "sound_on.bmp", 255);
    }
    else {
        iShowBMP2(10, 30, "sound_off.bmp", 255);
    }

}

void DrawPopup() {
    iSetColor(255, 255, 255);
    iText(10, _WinY - 50, _popup, GLUT_BITMAP_TIMES_ROMAN_24);
}

void DrawHelp() {
    iShowBMP(0, 0, "help_screen.bmp");
}

void ClearPopup() {
    strcpy(_popup, "");
}

void reset_all_values() {
    _bird_y = _WinY / 2;
    _bird_dy = 2;
    _bird_dy_input = 25;

    _collision_pillar_index = 0;
    _free_up_Y = _WinY;
    _free_down_Y = 0;

    _passed_pillars_count = 0;
    char temp_pt[10] = "0";
    strcpy(_pt, temp_pt);


    strcpy(_popup, "");

    _boundary_line_starting_point_x = 0;

    _game_menu_play_ctrl = 0;
    _game_menu_options_ctrl = 0;
    _game_menu_help_ctrl = 0;
    _pause_ctrl = 0;
    _game_over_ctrl = 0;
    _bird_fly_ctrl = 0;
    _hide_pillars_ctrl = 0;
    _move_pillars_ctrl = 1;
    _pillars_update_ctrl = 1;
    _count_pillars_ctrl = 0;
    _dynamic_pillars_ctrl = 0;
    _game_menu_highscores_ctrl = 0;
    _power_collision_ctrl = 0;
    _activate_power_ctrl = 0;
    _draw_ghost_ctrl = 0;
    _collision_ctrl = 1;
    _game_stages_ctrl = 0;


    _stage1 = 10;
    _stage2 = 20;
    _stage3 = 30;
    _stage4 = 40;

    _power_index = 0;

    _is_high_score = 0;

    _Cx = -100;
    _Cy = 100;

    _previous_power_pillar_count = 0;

    _game_menu_index = 0;
    _game_over_menu_index = 1;

    _pillars_gap = 350;
    _pillars_space = 200;
    _pillars_width = 100;
    _main_pillars_x[0] = _WinX;
    _pillars_dx = 3;    // initial value 3
    _dynamic_pillars_dy = 2;

    rand_gen(_main_up_pillars_y, N_MAIN_PILLARS, 500, 300, 50);
    update_main_pillars_values();

    _extra_pillars_x[0] = _main_pillars_x[N_MAIN_PILLARS-1] + _pillars_gap;
    rand_gen(_extra_up_pillars_y, N_EXTRA_PILLARS, 500, 300, 50);
    update_extra_pillars_values();

}

void update_main_pillars_values() {
    _count_pillars_ctrl %= (N_MAIN_PILLARS + N_EXTRA_PILLARS);

    for (int i=0; i<N_MAIN_PILLARS; i++) {
        if (i != 0) _main_pillars_x[i] = _main_pillars_x[i-1] + _pillars_gap;

        _main_down_pillars_y[i] = _main_up_pillars_y[i] - _pillars_space;

        if (_count_pillars_ctrl == i && (_main_pillars_x[_count_pillars_ctrl] <= REF_X - _pillars_width - _bird_r)) {
            _passed_pillars_count++;
            _count_pillars_ctrl++;

//          for changing background
            if (_passed_pillars_count % 10 == 0) {
                _bg_index++;

            if (_bg_index == 4) _bg_index = 0;
            }
        }
    }


    if (_pillars_update_ctrl == 1 && _main_pillars_x[0] <= 0) {
        _extra_pillars_x[0] = _main_pillars_x[N_MAIN_PILLARS-1] + _pillars_gap;
        rand_gen(_extra_up_pillars_y, N_EXTRA_PILLARS, 500, 300, 50);

        if (_dynamic_pillars_ctrl == 1) {
            rand_switch(_dynamic_extra_pillars_state, N_EXTRA_PILLARS);
        }

        _pillars_update_ctrl = 2;
    }
}

void update_extra_pillars_values() {
    for (int i=0; i<N_EXTRA_PILLARS; i++) {
        if (i) _extra_pillars_x[i] = _extra_pillars_x[i-1] + _pillars_gap;

        _extra_down_pillars_y[i] = _extra_up_pillars_y[i] - _pillars_space;

        if ((_count_pillars_ctrl - N_MAIN_PILLARS) == i && (_extra_pillars_x[_count_pillars_ctrl - N_MAIN_PILLARS] <= REF_X - _pillars_width - _bird_r)) {
            _passed_pillars_count++;
            _count_pillars_ctrl++;

//          for changing background
            if (_passed_pillars_count % 10 == 0) {
                _bg_index++;

            if (_bg_index == 4) _bg_index = 0;
            }
        }
    }


    if (_pillars_update_ctrl == 2 && _extra_pillars_x[0] <= 0) {
        _main_pillars_x[0] = _extra_pillars_x[N_EXTRA_PILLARS-1] +_pillars_gap;
        rand_gen(_main_up_pillars_y, N_MAIN_PILLARS, 500, 300, 50);


        if (_dynamic_pillars_ctrl == 1) {
            rand_switch(_dynamic_main_pillars_state, N_MAIN_PILLARS);
        }

        _pillars_update_ctrl = 1;
    }
}

void move_pillars() {
    if (_hide_pillars_ctrl) hide_pillars();

    if (_move_pillars_ctrl) {
        _main_pillars_x[0] -= _pillars_dx;
        _extra_pillars_x[0] -= _pillars_dx;

        move_boundary_line();           // since the move_boundary_line() is inside the move_pillars(), there is no need to call it with another iSetTimer()
    }

    update_main_pillars_values();
    update_extra_pillars_values();

    move_power_circle();

    GameStages();
}

void GameStages() {
        if ((_game_stages_ctrl != 1) && (_passed_pillars_count == _stage1)) {
        _game_stages_ctrl = 1;      // Pillars starts to move a little faster
//        printf("Game stage: %d\n", _game_stages_ctrl);
        strcpy(_popup, "Game Stage 1");

        _pillars_dx = 4;
        _dynamic_pillars_ctrl = 0;
    }
    else if ((_game_stages_ctrl != 2) && (_passed_pillars_count == _stage2)) {
        _game_stages_ctrl = 2;      // Some pillars become dynamic
//        printf("Game stage: %d\n", _game_stages_ctrl);
        strcpy(_popup, "Game Stage 2");

        rand_switch(_dynamic_main_pillars_state, N_MAIN_PILLARS);
        rand_switch(_dynamic_extra_pillars_state, N_EXTRA_PILLARS);

        _dynamic_pillars_ctrl = 1;
    }
    else if ((_game_stages_ctrl != 3) && (_passed_pillars_count == _stage3)) {
        _game_stages_ctrl = 3;      // Pillars starts to move faster
//        printf("Game stage: %d\n", _game_stages_ctrl);
        strcpy(_popup, "Game Stage 3");

        _pillars_dx = 5;
    }
    else if ((_game_stages_ctrl != 4) && (_passed_pillars_count == _stage4)) {
        _game_stages_ctrl = 4;
//        printf("Game stage: %d\n", _game_stages_ctrl);
        strcpy(_popup, "Game Stage 4");

//        _bird_dy = 3;

        for (int i = 0; i < N_MAIN_PILLARS; i++) {
            _dynamic_main_pillars_state[i] = 1;
        }
        for (int i = 0; i < N_EXTRA_PILLARS; i++) {
            _dynamic_extra_pillars_state[i] = 1;
        }

        _dynamic_pillars_ctrl = 2;

//        printf("Dynamic pillars ctrl : %d\n", _dynamic_pillars_ctrl);
    }
}

void hide_pillars() {
    for (int i=0; i<N_MAIN_PILLARS; i++) {
        _main_up_pillars_y[i] += 2;
    }

    for (int i=0; i<N_EXTRA_PILLARS; i++) {
        _extra_up_pillars_y[i] += 2;
    }

    _pillars_space += 20;
}

void bird_fly() {
    _bird_fly_ctrl = 0;
}

void move_bird() {
    if (_collision_ctrl)
        collision();    // collision() is called to get the values of _free_down_Y and _free_up_Y


//  this detects downward collision with the boundary
    if (_bird_y <= BOUNDARY_WIDTH + _bird_r) {
//      first step is to pause all the timers associated with game play

        iPauseTimer(0);         // This pauses move_pillars() from being called
//        iPauseTimer(1);       // The move_bird() can't be paused because if deals with all events after the collision
        iPauseTimer(2);         // This pauses bird_fly() from being called
        iPauseTimer(3);         // This resumes dynamic_pillars()
//
        _bird_dy = 0;                       // This stops the bird from falling further
        _bird_dy_input = 0;                 // This prevents the used from controlling the bird any longer
        _bird_y = BOUNDARY_WIDTH + _bird_r; // This stops the bird right on the lower boundary

        if (_game_over_ctrl == 0) {
            update_high_score();
        }

        _game_over_ctrl = 1;                // This shows the game over screen
    }
//

//  this detects upward collision with the boundary
    else if (_bird_y >= _WinY - BOUNDARY_WIDTH - _bird_r) {
//      first step is to pause all the timers associated with game play
//        iPauseTimer(0);   // This pauses move_pillars() from being called
//        iPauseTimer(1);   // The move_bird() can't be paused because if deals with all events after the collision;

        iPauseTimer(2);     // This pauses bird_fly() from being called
        iPauseTimer(3);     // This resumes dynamic_pillars()
//
        _bird_dy_input = 0;     // This prevents the used from controlling the bird any longer
        _bird_dy = 10;          // _bird_dy is increased significantly to show falling effect

        _hide_pillars_ctrl = 1; // This start hiding the pillars
        _move_pillars_ctrl = 0; // This makes the pillars to stop
    }
//

//  this detects collision with the pillars
    else if ((_bird_y <= _free_down_Y) || (_bird_y >= _free_up_Y)) {
//      first step is to pause all the timers associated with game play
//        iPauseTimer(0);   // This pauses move_pillars() from being called
//        iPauseTimer(1);   // The move_bird() can't be paused because if deals with all events after the collision

        iPauseTimer(2);     // This pauses bird_fly() from being called
        iPauseTimer(3);     // This resumes dynamic_pillars()
//
        _bird_dy_input = 0;     // This prevents the used from controlling the bird any longer
        _bird_dy = 10;          // _bird_dy is increased significantly to show falling effect

        _hide_pillars_ctrl = 1; // This start hiding the pillars
        _move_pillars_ctrl = 0; // This makes the pillars to stop
    }
//

    if (_game_menu_play_ctrl) {
        _bird_y -= _bird_dy;    // This decreases the y co-ordinate of the bird
    }
}

void collision() {
    _collision_pillar_index %= (N_MAIN_PILLARS + N_EXTRA_PILLARS);

    if (_collision_pillar_index < N_MAIN_PILLARS) {
        if (_main_pillars_x[_collision_pillar_index] <= REF_X - _pillars_width) {
            _collision_pillar_index++;
            _free_up_Y = _WinY;
            _free_down_Y = 0;

            return;
        }

    if (_main_pillars_x[_collision_pillar_index] <= REF_X + _bird_r) {
            _free_up_Y = _main_up_pillars_y[_collision_pillar_index] - _bird_r;
            _free_down_Y = _main_down_pillars_y[_collision_pillar_index] + _bird_r;
        }
        else {
            _free_up_Y = _WinY;
            _free_down_Y = 0;
        }
    }
    else {
        if (_extra_pillars_x[_collision_pillar_index - N_MAIN_PILLARS] <= REF_X - _pillars_width) {
                _collision_pillar_index++;
            _free_up_Y = _WinY;
            _free_down_Y = 0;

            return;
        }

        if (_extra_pillars_x[_collision_pillar_index - N_MAIN_PILLARS] <= REF_X + _bird_r) {
            _free_up_Y = _extra_up_pillars_y[_collision_pillar_index - N_MAIN_PILLARS] - _bird_r;
            _free_down_Y = _extra_down_pillars_y[_collision_pillar_index - N_MAIN_PILLARS] + _bird_r;
        }
        else {
            _free_up_Y = _WinY;
            _free_down_Y = 0;
        }
    }
}

void move_boundary_line() {
    if (_boundary_line_starting_point_x <= -800)
        _boundary_line_starting_point_x = 0;
    else
        _boundary_line_starting_point_x -= _pillars_dx;
}

void dynamic_pillars() {
    if (_dynamic_pillars_ctrl) {
//      for main pillars
        static int control[N_MAIN_PILLARS] = {0};
        for (int i=0; i<N_MAIN_PILLARS; i++) {
            if (_dynamic_main_pillars_state[i]) {
                if (i % 2 == 0) {
                    if (control[i]) {
                        if (_main_up_pillars_y[i] > _pillars_space + BOUNDARY_WIDTH + 100)
                            _main_up_pillars_y[i] -= _dynamic_pillars_dy;
                        else {
                            control[i] = !control[i];
                            _main_up_pillars_y[i] += _dynamic_pillars_dy;
                        }
                    }
                    else {
                        if (_main_up_pillars_y[i] < _WinY - BOUNDARY_WIDTH - 100)
                            _main_up_pillars_y[i] += _dynamic_pillars_dy;
                        else {
                            control[i] = !control[i];
                            _main_up_pillars_y[i] -= _dynamic_pillars_dy;
                        }
                    }
                }
                else {
                    if (control[i]) {
                        if (_main_up_pillars_y[i] < _WinY - BOUNDARY_WIDTH - 100)
                            _main_up_pillars_y[i] += _dynamic_pillars_dy;
                        else {
                            control[i] = !control[i];
                            _main_up_pillars_y[i] += _dynamic_pillars_dy;
                        }
                    }
                    else {
                        if (_main_up_pillars_y[i] > _pillars_space + BOUNDARY_WIDTH + 100)
                            _main_up_pillars_y[i] -= _dynamic_pillars_dy;
                        else {
                            control[i] = !control[i];
                            _main_up_pillars_y[i] -= _dynamic_pillars_dy;
                        }
                    }
                }
            }
        }
//
//      for extra pillars
        static int control_1[N_EXTRA_PILLARS] = {0};
        for (int i=0; i<N_EXTRA_PILLARS; i++) {
            if (_dynamic_extra_pillars_state[i]) {
                if (i % 2 == 0) {
                    if (control_1[i]) {
                        if (_extra_up_pillars_y[i] > _pillars_space + BOUNDARY_WIDTH + 100)
                            _extra_up_pillars_y[i] -= _dynamic_pillars_dy;
                        else {
                            control_1[i] = !control_1[i];
                            _extra_up_pillars_y[i] += _dynamic_pillars_dy;
                        }
                    }
                    else {
                        if (_extra_up_pillars_y[i] < _WinY - BOUNDARY_WIDTH - 100)
                            _extra_up_pillars_y[i] += _dynamic_pillars_dy;
                        else {
                            control_1[i] = !control_1[i];
                            _extra_up_pillars_y[i] -= _dynamic_pillars_dy;
                        }
                    }
                }
                else {
                    if (control_1[i]) {
                        if (_extra_up_pillars_y[i] < _WinY - BOUNDARY_WIDTH - 100)
                            _extra_up_pillars_y[i] += _dynamic_pillars_dy;
                        else {
                            control_1[i] = !control_1[i];
                            _extra_up_pillars_y[i] += _dynamic_pillars_dy;
                        }
                    }
                    else {
                        if (_extra_up_pillars_y[i] > _pillars_space + BOUNDARY_WIDTH + 100)
                            _extra_up_pillars_y[i] -= _dynamic_pillars_dy;
                        else {
                            control_1[i] = !control_1[i];
                            _extra_up_pillars_y[i] -= _dynamic_pillars_dy;
                        }
                    }
                }
            }
        }
//
    }
}

void ShowNumber(int x, int y, char n, int from) {
    if (from == 1) {
        switch (n) {
        case 48:
            iShowBMP2(x, y, "0s.bmp", 0);
            break;
        case 49:
            iShowBMP2(x, y, "1s.bmp", 0);
            break;
        case 50:
            iShowBMP2(x, y, "2s.bmp", 0);
            break;
        case 51:
            iShowBMP2(x, y, "3s.bmp", 0);
            break;
        case 52:
            iShowBMP2(x, y, "4s.bmp", 0);
            break;
        case 53:
            iShowBMP2(x, y, "5s.bmp", 0);
            break;
        case 54:
            iShowBMP2(x, y, "6s.bmp", 0);
            break;
        case 55:
            iShowBMP2(x, y, "7s.bmp", 0);
            break;
        case 56:
            iShowBMP2(x, y, "8s.bmp", 0);
            break;
        case 57:
            iShowBMP2(x, y, "9s.bmp", 0);
            break;
        }
    }
    else if (from == 2) {
        switch (n) {
        case 48:
            iShowBMP2(x, y, "0.bmp", 0);
            break;
        case 49:
            iShowBMP2(x, y, "1.bmp", 0);
            break;
        case 50:
            iShowBMP2(x, y, "2.bmp", 0);
            break;
        case 51:
            iShowBMP2(x, y, "3.bmp", 0);
            break;
        case 52:
            iShowBMP2(x, y, "4.bmp", 0);
            break;
        case 53:
            iShowBMP2(x, y, "5.bmp", 0);
            break;
        case 54:
            iShowBMP2(x, y, "6.bmp", 0);
            break;
        case 55:
            iShowBMP2(x, y, "7.bmp", 0);
            break;
        case 56:
            iShowBMP2(x, y, "8.bmp", 0);
            break;
        case 57:
            iShowBMP2(x, y, "9.bmp", 0);
            break;
        }
    }
}

void DrawOptionsMenu() {
    iShowBMP2(100, _WinY - 100, "theme.bmp", 0);

    if (_theme_index == 0) {
        iShowBMP2(360, _WinY - 100, "default_1.bmp", 0);
        iShowBMP2(360, _WinY - 160, "forest_0.bmp", 0);
        iShowBMP2(360, _WinY - 220, "mountain_0.bmp", 0);
    }
    else if (_theme_index == 1) {
        iShowBMP2(360, _WinY - 100, "default_0.bmp", 0);
        iShowBMP2(360, _WinY - 160, "forest_1.bmp", 0);
        iShowBMP2(360, _WinY - 220, "mountain_0.bmp", 0);
    }
    else if (_theme_index == 2) {
        iShowBMP2(360, _WinY - 100, "default_0.bmp", 0);
        iShowBMP2(360, _WinY - 160, "forest_0.bmp", 0);
        iShowBMP2(360, _WinY - 220, "mountain_1.bmp", 0);
    }

    iShowBMP2(150, 100, "return.bmp", 0);


    iSetColor(255, 255, 255);
    iText(130, 180, "Click the sound icon or press 's' to turn sound on/off", GLUT_BITMAP_TIMES_ROMAN_24);
}

void DrawPauseMenu() {
    iShowBMP2(200, 350, "paused.bmp", 0);

    iShowBMP2(250, 160, "pause_prompt.bmp", 0);
}

void DrawBoundary() {
    iSetColor(162, 117, 40);
    iFilledRectangle(0, 0, _WinX, BOUNDARY_WIDTH);                          // Lower boundary
    iFilledRectangle(0, _WinY-BOUNDARY_WIDTH, _WinX, BOUNDARY_WIDTH);       // Upper boundary

    int x = _boundary_line_starting_point_x;
    for (int i=1; i<=20; i++, x += 80) {
        iSetColor(211, 226, 75);
//      upper boundary line
        iLine(x,   0, x+5, BOUNDARY_WIDTH);
        iLine(x+1, 0, x+6, BOUNDARY_WIDTH);
        iLine(x+2, 0, x+7, BOUNDARY_WIDTH);
//      lower boundary line
        iLine(x,   _WinY - BOUNDARY_WIDTH, x+5, _WinY);
        iLine(x+1, _WinY - BOUNDARY_WIDTH, x+6, _WinY);
        iLine(x+2, _WinY - BOUNDARY_WIDTH, x+7, _WinY);
//      three lines are used to increase the thickness of the lines
    }
}

void DrawBird() {
//  ball model of the bird
//    iSetColor(255, 255, 255);
//    iFilledCircle(_bird_x, _bird_y, _bird_r);
//

//  actual bird with animation
    if (!_draw_ghost_ctrl) {
        if(_bird_fly_ctrl) {
            iShowBMP2(_bird_x - _bird_r, _bird_y - _bird_r, "bird_up_n.bmp", 255);
        }
        else
            iShowBMP2(_bird_x - _bird_r, _bird_y - _bird_r, "bird_down.bmp", 255);
    }
    else {
        if(_bird_fly_ctrl) {
            iShowBMP2(_bird_x - _bird_r, _bird_y - _bird_r, "bird_up_bw.bmp", 0);
        }
        else
            iShowBMP2(_bird_x - _bird_r, _bird_y - _bird_r, "bird_down_bw.bmp", 0);
    }
}

void DrawHighScores() {
    iShowBMP2(150, 450, "highscore title.bmp", 0);

    iSetColor(240, 240, 240);
    iFilledRectangle(250, 150, 300, 300);

    for (int i = 0; i < 5; i++) {
        if (i == 0) iSetColor(153, 153, 0);
        else iSetColor(0, 0, 0);

        char text[120] = "", temp[20];
        itoa(i + 1, temp, 10);
        strcat(text, temp);
        strcat(text, ". ");
        strcat(text, _high_scores[i].name);
        strcat(text, " - ");
        itoa(_high_scores[i].score, temp, 10);
        strcat(text, temp);
        iText(260, 400 - (50 * i), text, GLUT_BITMAP_TIMES_ROMAN_24);
    }

    iShowBMP2(150, 100, "return.bmp", 0);
}

void DrawStartButtons() {
    iShowBMP2(250, 450, "play.bmp", 255);
    iShowBMP2(250, 350, "high scores.bmp", 255);
    iShowBMP2(250, 250, "options.bmp", 255);
    iShowBMP2(250, 150, "help.bmp", 255);
    iShowBMP2(300,  50, "exit_1.bmp", 255);

    switch (_game_menu_index) {
    case 0:
        iShowBMP2(150, 450, "arrow1.bmp", 255);
        break;
    case 1:
        iShowBMP2(150, 350, "arrow1.bmp", 255);
        break;
    case 2:
        iShowBMP2(150, 250, "arrow1.bmp", 255);
        break;
    case 3:
        iShowBMP2(150,  150, "arrow1.bmp", 255);
        break;
    case 4:
        iShowBMP2(200,  50, "arrow1.bmp", 255);
        break;
    }
}

void DrawPillars() {
    if (_activate_power_ctrl) {
        iSetColor(139, 30, 166);
    }
    else {
        iSetColor(38, 138, 52);     // Color of the pillars
    }

//  Main upward pillars
    for (int i=0; i<N_MAIN_PILLARS; i++) {
        iFilledRectangle(_main_pillars_x[i], _main_up_pillars_y[i], _pillars_width, (_WinX - _main_up_pillars_y[i]));
        iFilledRectangle(_main_pillars_x[i], 0, _pillars_width, _main_down_pillars_y[i]);
    }
//
//  Extra upward pillars
    for (int i=0; i<N_EXTRA_PILLARS; i++) {
        iFilledRectangle(_extra_pillars_x[i], _extra_up_pillars_y[i], _pillars_width, (_WinX - _extra_up_pillars_y[i]));
        iFilledRectangle(_extra_pillars_x[i], 0, _pillars_width, _extra_down_pillars_y[i]);
    }
//
}

void DrawBackgroud() {
    if (_theme_index == 0) {
            iSetColor(20, 200, 255);
            iFilledRectangle(0, 0, _WinX, _WinY);

            double cloud_ground = 230;

            iSetColor(175, 220, 235);

            iFilledRectangle(0, 0, _WinX, cloud_ground + 100);

            iFilledCircle(100, cloud_ground + 100, 80);
            iFilledCircle(315, cloud_ground + 120, 90);
            iFilledCircle(520, cloud_ground + 140, 80);
            iFilledCircle(700, cloud_ground + 120, 80);
            iFilledCircle(200, cloud_ground + 130, 60);
            iFilledCircle(420, cloud_ground + 145, 60);
            iFilledCircle(600, cloud_ground + 130, 60);
            iFilledCircle(800, cloud_ground + 130, 60);
            iFilledCircle(-30, cloud_ground + 80, 150);

            DrawBuildings();

            iShowBMP2(0, 0, "trees1.bmp", 255);
    }
    else if (_theme_index == 1) {
        if (_bg_index == 0) iShowBMP(0, 0, "forest_bg_0.bmp");
        else if (_bg_index == 1) iShowBMP(0, 0, "forest_bg_1.bmp");
        else if (_bg_index == 2) iShowBMP(0, 0, "forest_bg_2.bmp");
        else if (_bg_index == 3) iShowBMP(0, 0, "forest_bg_3.bmp");
    }
    else if (_theme_index == 2) {
        if (_bg_index == 0) iShowBMP(0, 0, "mountain_bg_0.bmp");
        else if (_bg_index == 1) iShowBMP(0, 0, "mountain_bg_1.bmp");
        else if (_bg_index == 2) iShowBMP(0, 0, "mountain_bg_2.bmp");
        else if (_bg_index == 3) iShowBMP(0, 0, "mountain_bg_3.bmp");
    }
}

void DrawGameOver() {
    iShowBMP(0, 0, "gameover_16bit.bmp");

    int y = 420;

    iShowBMP2(240, y, "score.bmp", 0);

    itoa(_passed_pillars_count, _pt, 10);

    for (int i=0, x=400; _pt[i]; i++, x+=50) {
        ShowNumber(x, y, _pt[i], 2);
    }

    if (_game_over_menu_index) {
        iShowBMP2(300, 63, "yes_red.bmp", 0);
        iShowBMP2(420, 60, "no_white.bmp", 255);
    }
    else {
        iShowBMP2(300, 60, "yes_white.bmp", 255);
        iShowBMP2(420, 63, "no_red.bmp", 0);
    }
}

void DrawLoadingScreen() {
    DrawBackgroud();

    iShowBMP2(150, 500, "logo1.bmp", 0);


    char text[120] = "Enter your name: ";
    strcat(text, _player_name);
    iSetColor(255, 255, 255);
    iText(230, 100, text, GLUT_BITMAP_TIMES_ROMAN_24);

    iText(250, 50, "Press Enter to continue", GLUT_BITMAP_TIMES_ROMAN_24);

    iText(400, 480, "By Pritam and Utchchhwas", GLUT_BITMAP_TIMES_ROMAN_24);
}

void PowerCollision() {
    if( sqrt (((_Cx - _bird_x)*(_Cx - _bird_x)) + ((_Cy - _bird_y)*(_Cy - _bird_y))) <= (_bird_r + _Cr)) {
        _power_collision_ctrl = 1;

        _activate_power_ctrl = 1;
    }
}

void update_power_circle() {
    if (_power_collision_ctrl == 0)
        PowerCollision();

    if (!_activate_power_ctrl && (_passed_pillars_count > 0 ) && (_passed_pillars_count != _previous_power_pillar_count) && (_passed_pillars_count % 6 == 0)) {
        _power_collision_ctrl = 0;

        get_power_index();

//        printf("Generated Power index: %d\n", _power_index);

        _Cx =  _main_pillars_x[2] + _pillars_width + ((_pillars_gap - _pillars_width) / 2);

        rand_gen(&_Cy, 1, 500, 100, 50);

        _previous_power_pillar_count = _passed_pillars_count;
    }
}

void get_power_index() {
//    if (_game_stages_ctrl == 1 || _game_stages_ctrl == 0) {
//        _power_index = 0;
//    }
//    else if (_game_stages_ctrl == 2) {
//        int temp = rand() % 3;
//
//        if (temp == 0) _power_index = 1;
//        else _power_index = 0;
//    }
//    else if (_game_stages_ctrl == 3) {
//        _power_index = rand() % 4;
//    }
//    else if (_game_stages_ctrl == 4) {
//        int temp = rand() % 10;
//
//        if (temp == 0) {
//            _power_index = 5;
//        }
//        else if (temp == 1) {
//            _power_index = 4;
//        }
//        else {
//            _power_index = (rand() % 3) + 1;
//        }
//    }

    _power_index++;
    if (_power_index == 6)
        _power_index = 1;
}

void DrawPowerCircle() {
    switch (_power_index) {
    case 1 :
        iShowBMP2(_Cx - _Cr, _Cy - _Cr, "ghost_perk.bmp", 0);
        break;
    case 2 :
        iShowBMP2(_Cx - _Cr, _Cy - _Cr, "slowdown_perk.bmp", 255);
        break;
    case 3 :
        iShowBMP2(_Cx - _Cr, _Cy - _Cr, "static_perk.bmp", 255);
        break;
    case 4 :
        iShowBMP2(_Cx - _Cr, _Cy - _Cr, "fastforward_perk.bmp", 255);
        break;
    case 5 :
        iShowBMP2(_Cx - _Cr, _Cy - _Cr, "skull_perk.bmp", 0);
        break;
//    default :
//        iSetColor(255, 255, 255);
//        iFilledCircle(_Cx, _Cy, _Cr, 100);
//        break;
    }
}

void move_power_circle() {
    if (_move_pillars_ctrl) {
        update_power_circle();

        _Cx -= _pillars_dx;
    }

    ActivatePower();
}

void ActivatePower() {
    static int count = 0;
    static int temp = 0;

    switch (_power_index) {
    case 1 :    // Ghost perk
        if (_activate_power_ctrl == 1) {
//            printf("Ghost Perk activated.\n");
            strcpy(_popup, "Ghost Perk activated");

            count = _passed_pillars_count + 5;

            _draw_ghost_ctrl = 1;
            _collision_ctrl = 0;

            _activate_power_ctrl = 2;
        }
        else if (_activate_power_ctrl == 2) {
            if (_passed_pillars_count == count) {
//               printf("Ghost Perk deactivated.\n");
                strcpy(_popup, "Ghost Perk deactivated");

                _draw_ghost_ctrl = 0;
                _collision_ctrl = 1;

                count = 0;
                _activate_power_ctrl = 0;
            }
        }
        break;
    case 2 :    // Slow down perk
        if (_activate_power_ctrl == 1) {
//            printf("Slow-down Perk activated.\n");
            strcpy(_popup, "Slow-down Perk activated");


            count = _passed_pillars_count + 5;

            temp = _pillars_dx;
            _pillars_dx = 3;
            _dynamic_pillars_dy = 1;

            _activate_power_ctrl = 2;
        }
        else if (_activate_power_ctrl == 2) {
            if (_passed_pillars_count == count) {
//                printf("Slow-down Perk deactivated.\n");
                strcpy(_popup, "Slow-down Perk deactivated");

                _pillars_dx = temp;
                _dynamic_pillars_dy = 2;

                count = 0;
                _activate_power_ctrl = 0;
            }
        }
        break;
    case 3 :    // Static perk
        if (_activate_power_ctrl == 1) {
//            printf("Static Perk activated.\n");
            strcpy(_popup, "Static Perk activated");

            count = _passed_pillars_count + 5;

            temp = _dynamic_pillars_ctrl;
            _dynamic_pillars_ctrl = 0;

            _activate_power_ctrl = 2;
        }
        else if (_activate_power_ctrl == 2) {
            if (_passed_pillars_count == count) {
//                printf("Static Perk deactivated.\n");
                strcpy(_popup, "Static Perk deactivated");

                _dynamic_pillars_ctrl = temp;

                count = 0;
                _activate_power_ctrl = 0;
            }
        }
        break;
    case 4: // fast forward perk
        if (_activate_power_ctrl  == 1) {
//            printf("Fast Forward Perk activated.\n");
            strcpy(_popup, "Fast Forward Perk activated");

            count = _passed_pillars_count + 9;

            temp = _pillars_dx;
            _pillars_dx = 20;

            _draw_ghost_ctrl = 1;
            _collision_ctrl = 0;
            _dynamic_pillars_ctrl = 0;

            _activate_power_ctrl = 2;
        }
        else if (_activate_power_ctrl == 2) {
            if (_passed_pillars_count >= count) {
//                printf("Fast Forward Perk ending soon.\n");
                strcpy(_popup, "Fast Forward Perk ending soon");

                count += 1;

                _pillars_dx = 2;
                _dynamic_pillars_dy = 1;

                _activate_power_ctrl = 3;
                }
            }
        else if (_activate_power_ctrl == 3) {
            if (_passed_pillars_count >= count) {
//               printf("Fast Forward Perk deactivated.\n");
                strcpy(_popup, "Fast Forward Perk deactivated");

                _pillars_dx = temp;
                _dynamic_pillars_dy = 2;

                _draw_ghost_ctrl = 0;
                _collision_ctrl = 1;
                _dynamic_pillars_ctrl = 2;

                count = 0;
                _activate_power_ctrl = 0;
            }
        }
        break;
        case 5 :    // Skull perk
        if (_activate_power_ctrl == 1) {
//           printf("Skull Perk collected.\n");
            strcpy(_popup, "Skull Perk collected");

            if (_passed_pillars_count < 5) _passed_pillars_count = 0;
            else _passed_pillars_count -= 10;

            _stage1 = _passed_pillars_count + 1;
            _stage2 = _stage1 + 10;
            _stage3 = _stage2 + 10;
            _stage4 = _stage3 + 10;

            _game_stages_ctrl = 0;

            _activate_power_ctrl = 0;
        }
        break;
//    default :
//        printf("Default.\n");
//        break;
    }

}

int cmpt(const void *p, const void *q) {
    highscore a = *((highscore *) p);
    highscore b = *((highscore *) q);

    return b.score - a.score;
}

void rand_gen(int *arr, int n, int max, int min, int d_min) {
    for (int i = 0; i < n; i++) {
        int r = rand();
        arr[i] = min + (d_min * (r % (((max - min) / d_min) + 1)));
    }
}

void rand_switch(int *arr, int n) {
    for (int i = 0; i <n; i++) {
        arr[i] = rand() % 2;
    }
}

void get_high_score() {
    FILE *fp;

    if ((fp = fopen(FILE_NAME, "rb")) == NULL) {
            printf("Cannot open file. (1)\n");
            exit(1);
    }

    fread(_high_scores, sizeof(highscore), 5, fp);

    fclose(fp);
}

void update_high_score() {
    FILE *fp;

    if ((fp = fopen(FILE_NAME, "wb")) == NULL) {
        printf("Cannot open file. (2)\n");
        exit(1);
    }

    strcpy(_high_scores[5].name, _player_name);
    _high_scores[5].score = _passed_pillars_count;

    qsort(_high_scores, 6, sizeof(highscore), cmpt);

    if (_passed_pillars_count == _high_scores[0].score) {
        _is_high_score = 1;
    }

    fwrite(_high_scores, sizeof(highscore), 5, fp);

    fclose(fp);
}

void DrawBuildings() {
//  The code below is for drawing the default background

    double b_g = 200;
    double f = 4;

//  1st building

    double x = 15;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 135/f, 370/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 115/f, 360/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  260/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  310/f, 25/f, 40/f);


//  2nd building

    x = 200;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 100/f, 470/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 80/f, 460/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  310/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  360/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  360/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  410/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  410/f, 25/f, 40/f);


//  3rd building

    x = 320;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 135/f, 320/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 115/f, 310/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x+ 45)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  260/f, 25/f, 40/f);


//  4th building

    x = 480;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 135/f, 470/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 115/f, 460/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  260/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  310/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  360/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  360/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  360/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  410/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  410/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  410/f, 25/f, 40/f);


//  5th building

    x = 650;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 100/f, 370/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 80/f, 360/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  310/f, 25/f, 40/f);


//  6th building

    x = 800;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 100/f, 620/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 80/f, 610/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  310/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  360/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  360/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  410/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  410/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  460/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  460/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  510/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  510/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  560/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  560/f, 25/f, 40/f);


//  7th building

    x = 950;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 135/f, 370/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 115/f, 360/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  260/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  310/f, 25/f, 40/f);


//  8th building

    x = 1120;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 100/f, 670/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 80/f, 660/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  310/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  360/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  360/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  410/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  410/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  460/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  460/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  510/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  510/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  560/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  560/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  610/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  610/f, 25/f, 40/f);

//  9th building

    x = 1260;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 135/f, 470/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 115/f, 460/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  260/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  310/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  360/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  360/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  360/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  410/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  410/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  410/f, 25/f, 40/f);

//  10th building

    x = 1430;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 100/f, 370/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 80/f, 360/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  310/f, 25/f, 40/f);


//  11th building

    x = 1550;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 100/f, 470/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 80/f, 460/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  310/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  360/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  360/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  410/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  410/f, 25/f, 40/f);


//  12th building

    x = 1680;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 135/f, 320/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 115/f, 310/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x+ 45)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  260/f, 25/f, 40/f);

//  13th building

    x = 1840;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 100/f, 620/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 80/f, 610/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  310/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  360/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  360/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  410/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  410/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  460/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  460/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  510/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  510/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  560/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  560/f, 25/f, 40/f);


//  14th building

    x = 1960;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 100/f, 470/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 80/f, 460/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  310/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  360/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  360/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  410/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  410/f, 25/f, 40/f);


//  15th building

    x = 2080;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 100/f, 370/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 80/f, 360/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  310/f, 25/f, 40/f);



//  16th building

    x = 2220;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 135/f, 320/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 115/f, 310/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x+ 45)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  260/f, 25/f, 40/f);


//  17th building

    x = 2380;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 135/f, 470/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 115/f, 460/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  260/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  310/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  360/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  360/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  360/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  410/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  410/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  410/f, 25/f, 40/f);


//  18th building

    x = 2550;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 135/f, 370/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 115/f, 360/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  260/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  310/f, 25/f, 40/f);


//  19th building

    x = 2710;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 100/f, 670/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 80/f, 660/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  310/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  360/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  360/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  410/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  410/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  460/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  460/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  510/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  510/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  560/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  560/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  610/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  610/f, 25/f, 40/f);

//  20th building

    x = 2860;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 135/f, 470/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 115/f, 460/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  260/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  310/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  360/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  360/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  360/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  410/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  410/f, 25/f, 40/f);
    iFilledRectangle((x + 80)/f, b_g +  410/f, 25/f, 40/f);

//  21st building

    x = 3040;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 100/f, 370/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 80/f, 360/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  310/f, 25/f, 40/f);


//  11th building

    x = 1550;

    iSetColor(0, 180, 200);
    iFilledRectangle((x - 10)/f, b_g, 100/f, 470/f);

    iSetColor(225, 225, 225);
    iFilledRectangle(x/f, b_g, 80/f, 460/f);

    iSetColor(190, 190, 190);

    iFilledRectangle((x + 10)/f, b_g +  10/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  10/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  60/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  60/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  110/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  110/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  160/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  160/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  210/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  210/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  260/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  260/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  310/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  310/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  360/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  360/f, 25/f, 40/f);

    iFilledRectangle((x + 10)/f, b_g +  410/f, 25/f, 40/f);
    iFilledRectangle((x + 45)/f, b_g +  410/f, 25/f, 40/f);
}
