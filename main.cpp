#include <curses.h>
#include <fstream>

#define MAP_HEIGHT 22
#define MAP_WIDTH 44
#define FLOOR '#'
#define STICK 'I'
using namespace std;

char map[MAP_HEIGHT][MAP_WIDTH];

string map_name("1.txt");
bool game = true;
short dir = 0;

bool showPosition = false;

int spawn_x = 0;
int spawn_y = 0;

struct character{
    int x;
    int y;
    char symbol;
    char weapoon;
    int hp;

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

struct character player = {spawn_y, spawn_x, '!', FLOOR, 100};
void loadMap(){
    ifstream map_file;
    map_file.open(map_name);
    for(int y = 0; y < MAP_WIDTH - 1; y++){
        map_file >> map[y];
    }
    
}
void setup(){
    loadMap();
    initscr();
    noecho();
    halfdelay(10);
    refresh();
}

void draw(){
    clear();
    for(int y = 0; y < 22; y++){
        for(int x = 0; x < 44; x++){
            printw("%c" ,y == player.y && x == player.x ? player.symbol : map[y][x]);
        }
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
                    printw("\nx: %d, y: % d", player.x, player.y);
                    printw("\t %c", player.weapoon);
                }
                
        }
        printw("\n");
    }
}
void input(){
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
    if(dir < 2){
        if(((player.y > 0) && (dir == -1)) || ((player.y < MAP_HEIGHT - 1) && (dir == 1))){
            switch(map[player.y + dir][player.x]){
                case STICK:
                case FLOOR:
                    player.moveY(dir);
                    break;
            }
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
    endwin();
    return 0; 
}
