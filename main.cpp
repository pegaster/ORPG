#include <curses.h>
#include <fstream>
// constants
#define MAP_HEIGHT 22
#define MAP_WIDTH 44
#define LEFT_MAP 2
#define RIGHT_MAP 3
#define UP_MAP 0
#define DOWN_MAP 1
#define FLOOR '#'
#define DRAW_FLOOR ' '
#define STICK 'I'
using namespace std;
// variables for map managment
char map[MAP_HEIGHT][MAP_WIDTH];
short exits = 4;
string current_map = "1.bin";
string maps[4] = {"1.bin"};
bool isDirectionUsed[4];
int map_number = 0;
// variable for end game
bool game = true;
// player direction variable
short dir = 0;

bool showPosition = false;
// spawn point
int spawn_x = 0;
int spawn_y = 0;

// struct for player
struct character{
    int x;
    int y;
    char symbol;
    char weapoon;
    int hp;
    bool attack;
    void moveX(int dir){
        x += dir;
    }
    void moveY(int dir){
        y += dir;
    }
    void goTo(int new_x, int new_y){
        x = new_x;
        y = new_y;
    }
    void goToX(int new_x){
        x = new_x;
    }
    void goToY(int new_y){
        y = new_y;
    }
    void hurt(int damage){
        hp -= damage;
        if(hp < 0){
            hp = 0;
        }
    }
    void getWeapoon(){
        weapoon = map[y][x];
    }
};

character player = {spawn_y, spawn_y, 'O', FLOOR, 100, false};

void loadMap(){
    //getting map config info
    char exit_type;
    for(int i = 0; i < 4; i++){
        isDirectionUsed[i] = false;
    }

    ifstream map_file;
    map_file.open(maps[map_number]);
    map_file >> exits;
    for(int i = 0; i < exits; i++){
        int index = 0;
        map_file >> exit_type;
        switch (exit_type)
        {
        case 'U':
            index = UP_MAP;
            break;

        case 'D':
            index = DOWN_MAP;
            break;

        case 'L':
            index = LEFT_MAP;
            break;

        case 'R':
            index = RIGHT_MAP;
            break;

        }
        map_file >> maps[index]; 
        isDirectionUsed[index] = true;
    }
    // getting map
    for(int y = 0; y < MAP_WIDTH - 1; y++){
        map_file >> map[y];
    }
    
}
void setup(){
    loadMap();
// functions for curses
    initscr();
    noecho();
    halfdelay(10);
    refresh();
}

void draw(){
    clear();
    for(int i = 0; i <= MAP_WIDTH + 1; i++){
        printw("#");
    }
    printw("\n");
    for(int y = 0; y < MAP_HEIGHT; y++){
        printw("#");
        for(int x = 0; x < MAP_WIDTH; x++){
            if(y == player.y && x == player.x){
                printw("%c", player.symbol);
            }
            else{
                switch (map[y][x])
                {
                case FLOOR:
                    printw("%c", DRAW_FLOOR);
                    break;
                
                default:
                    printw("%c", map[y][x]);
                    break;
                }
            }
        }
        printw("#");
        // additional information
        switch(y){
            case 2:
                printw("\tHealth: %d", player.hp);
                break;
            case 4:
                printw("\tWeapoon: ");
                switch(player.weapoon){
                    case FLOOR:
                        printw("None");
                        break;
                    case STICK:
                        printw("Stick");
                        break;
                }
                break;
            case MAP_HEIGHT - 1:
                if(showPosition){
                    printw("\t\t x: %d, y: % d", player.x, player.y);
                    printw("\t %c", player.weapoon);
                }
                
        }
        printw("\n");
    }
    for(int i = 0; i <= MAP_WIDTH + 1; i++){
        printw("#");
    }
}
void input(){
    // just controls
    switch (getch()){
        case 'w':
            dir = -1;
            break;
        
        case 's':
            dir = 1;
            break;
        
        case 'a':
            dir = 2;
            break;
        case 'd':
            dir = 4;
            break;
        case 'q':
            game = false;
            break;
        case 'k':
            showPosition = !showPosition;
            break;
        case 'g':
            char buffer = player.weapoon;
            player.getWeapoon();
            map[player.y][player.x] = buffer;
            break;
        }
}
void logic(){
    // logic of all game
    if(dir < 2){
        if(((player.y > 0) && (dir == -1)) || ((player.y < MAP_HEIGHT - 1) && (dir == 1))){
            switch(map[player.y + dir][player.x]){
                case STICK:
                case FLOOR:
                    player.moveY(dir);
                    break;
            }
        }
        else if((player.y == 0) && (dir == -1)){            
            if(isDirectionUsed[UP_MAP]){
                map_number = UP_MAP;
                player.goToY(MAP_HEIGHT - 1);
                loadMap();
            }
        }
        else if((player.y == MAP_HEIGHT - 1) && (dir == 1) && isDirectionUsed[DOWN_MAP]){
            map_number = DOWN_MAP;
            player.goToY(0);
            loadMap();
            
        }

    }
    else{
        dir -= 3;
        if(((player.x > 0) && (dir == -1)) || ((player.x < MAP_WIDTH - 1) && (dir == 1))){
            switch(map[player.y][player.x + dir]){
                case STICK:
                case FLOOR:
                    player.moveX(dir);
                    break;
            }
        }
        else if((player.x == MAP_WIDTH - 1) && (dir == 1) && isDirectionUsed[RIGHT_MAP]){
            map_number = RIGHT_MAP;
            player.goToX(0);
            loadMap();
            
        }
        else if((player.x == 0) && (dir == -1) && isDirectionUsed[LEFT_MAP]){
            map_number = LEFT_MAP;
            player.goToX(MAP_WIDTH - 1);
            loadMap();
            
        }
    }
    dir = 0;
}


int main() 
{
    setup();
    while (game)
    {
        draw();
        input();
        logic();
    }
    // function for curses
    endwin();
    return 0; 
}
