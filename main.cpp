#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <cstdlib>
#include <thread>
#include <mutex>

char pipechar = 'p';
char playerCharacter = 'b';
bool gravityforce = true;
bool phaze = false;
int arsizex = 15;
int arsizey = 25;
int playerX, playerY;
bool gameover;
char map[15][25];
char art[15][25]{
    {' ', '=', '=', ' ', '=', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, //00
    {'=', '=', '=', '=', '=', '=', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, //01
    {'=', '=', '=', '=', '=', '=', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '=', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '=', '='}, //02
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '=', '=', '=', '=', ' ', ' ', ' ', ' ', ' ', '=', '=', '='}, //03
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '=', '=', '=', '=', ' ', ' ', ' ', ' ', ' ', ' ', '='}, //04
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, //05
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, //06
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, //07
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '.', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, //08
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '.', '.', '.', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, //09
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '-', '-', ' ', '-', '-', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, //10
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '/', ' ', ' ', '.', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, //11
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '-', '-', ' ', '_', '_', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, //12
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|', '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, //13
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|', '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, //14
}; //nothing

std::mutex playerMutex;
void starter(){
    for (int i = 0; i < arsizex; i++) {
        for (int j = 0; j < arsizey; j++){
            map[i][j] = ' ';
        }
    }
    map[5][8] = playerCharacter;
    gameover = false;
}
void ending(){
    std::cout << "\033[33m";
    std::cout <<
        "     .aMMMMP .aMMMb  dMMMMMMMMb dMMMMMP        .aMMMb  dMP dMP dMMMMMP dMMMMb \n"
        "    dMP\"    dMP\"dMP dMP\"dMP\"dMPdMP            dMP\"dMP dMP dMP dMP     dMP.dMP \n"
        "   dMP MMP\"dMMMMMP dMP dMP dMPdMMMP          dMP dMP dMP dMP dMMMP   dMMMMK\"  \n"
        "  dMP.dMP dMP dMP dMP dMP dMPdMP            dMP.aMP  YMvAP\" dMP     dMP\"AMF   \n"
        "  VMMMP\" dMP dMP dMP dMP dMPdMMMMMP         VMMMP\"    VP\"  dMMMMMP dMP dMP    \n";
    std::cout << "\033[0m";
}
void start(){
    std::cout << "\033[33m"; // ANSI escape code for yellow color
    std::cout <<
        "    ____                         ______         _____ __             __ \n"
        "   / __ \\________  __________   /_  __/___     / ___// /_____ ______/ /_\n"
        "  / /_/ / ___/ _ \\/ ___/ ___/    / / / __ \\    \\__ \\/ __/ __ `/ ___/ __/\n"
        " / ____/ /  /  __(__  |__  )    / / / /_/ /   ___/ / /_/ /_/ / /  / /_  \n"
        "/_/   /_/   \\___/____/____/    /_/  \\____/   /____/\\__/\\__,_/_/   \\__/  \n";
    std::cout << "\033[0m";
}
void goagain(){
    std::cout << "\033[33m"; // ANSI escape code for yellow color
    std::cout <<
        "   ______         ___               _     ___ \n"
        "  / ____/___     /   | ____ _____ _(_)___/__ \\\n"
        " / / __/ __ \\   / /| |/ __ `/ __ `/ / __ \\/ _/\n"
        "/ /_/ / /_/ /  / ___ / /_/ / /_/ / / / / /_/  \n"
        "\\____/\\____/  /_/  |_\\__, /\\__,_/_/_/ /_(_)   \n"
        "                    /____/         y/n           \n";
    std::cout << "\033[0m";
}
void findXPosition() {
    for (int i = 0; i < arsizex; i++) {
        for (int j = 0; j < 2 * arsizey; j++) {
            if (map[i][j] == playerCharacter) {
                playerX = i;
                playerY = j;
                return;
            }
        }
    }
}

void render() {
    findXPosition();
        for (int i = 0; i < arsizex; i++) {
            for (int j = 0; j < arsizey; j++) {
                if (map[i][j] == playerCharacter || map[i][j] == pipechar){

                    if (map[i][j]==pipechar){
                        std::cout<<"\e[1;94m";
                        std::cout << map[i][j];

                    }
                    else if (map[i][j]== playerCharacter){
                        std::cout<<"\e[1;93m";
                        std::cout << map[i][j];
                    }
                }
                else{
                    std::cout <<"\e[0;36m";
                    std::cout << art[i][j];
                }
            }
        std::cout << std::endl;
        }
}

void jump() {
    int jumpheight = 4;
    bool found = false;
    int newPlayerX = playerX;

    gravityforce = false;
    for (int i = 1; i <= jumpheight; i++) {
        if (map[playerX - i][playerY] != ' ') {
            if (phaze) {
                newPlayerX = playerX - i - 1;
            } else {
                newPlayerX = playerX - i + 1;
            }
            found = true;
            break;
        }
    }

    if (!found) {
        newPlayerX = playerX - jumpheight + 1;
    }
    for (int i = playerX - 1; i >= newPlayerX; i--) {
        map[playerX][playerY] = ' ';
        map[i][playerY] = playerCharacter;
        playerX = i;
        system("clear");
        render();
        usleep(100000);
    }
    gravityforce = true;
}

char getKeyPress() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void gravity() {
    std::lock_guard<std::mutex> lock(playerMutex);
    system("clear");
    render();
    if (gravityforce) {
        if (map[playerX + 1][playerY] == ' ') {
            map[playerX][playerY] = ' ';
            playerX += 1;
            map[playerX][playerY] = playerCharacter;
            system("clear");
            render();
            usleep(050000); // Adjust the sleep time as needed
        }
    }
}
void pipe_generator() {
    int position = 1;
    while (position == 1 || position == 2 || position == 0 || position == 14 || position == 3) {
        position = rand() % 14;
    }
    for (int i = 22; i < 25; i++) {
        for (int j = 0; j < arsizex; j++) {
            map[j][i] = pipechar;
        }
    }
    for (int i = 22; i < 25; i++) {
        for (int j = position - 3; j < position; j++) {
            map[j][i] = ' ';
        }
    }
}
void pipe_pusher() {
    for (int i = 0; i < arsizex; i++) {
        for (int j = 0; j < arsizey; j++) {
            if (map[i][j] == pipechar) {
                map[i][j] = ' ';
                if (map[i][j - 1] == 'b'){
                    gameover = 1;
                }
                else{
                    map[i][j - 1] = pipechar;
                }
            }
        }
    }
    for (int i = 0; i < arsizex; i++) {
        map[i][arsizey - 1] = ' '; // Clear last column
    }
    usleep(150000);
}
void gravityThread() {
    while (!gameover) {
        gravity();
        usleep(30000);
    }
}

void inputThread() {
    while (!gameover) {
        char key = getKeyPress();
        {
            std::lock_guard<std::mutex> lock(playerMutex);
            if (key == ' ') {
                jump();
            }
        }
        system("clear");
        render();
    }
}

void pushThread() {
    while (!gameover) {
        int space_between = rand() % 6 + 10; // Adjust spacing between pipes

        pipe_generator();
        for (int i = 0; i < space_between; i++) {
            pipe_pusher();
        }
    }
}

int main() {
    bool NGpLoop = true;
    char loopkey;
    srand(time(0));

    start();
    char key = getKeyPress();

    while (NGpLoop){
        starter();

        std::thread inputThreadObj(inputThread);
        std::thread gravityThreadObj(gravityThread);
        std::thread pushTgreadObj(pushThread);

        inputThreadObj.join();
        gravityThreadObj.join();
        pushTgreadObj.join();

        system("clear");
        ending();
        sleep(1);
        system("clear");
        goagain();

        std::cin >> loopkey;
        if (loopkey == 'n'){
            NGpLoop = false;
        }
    }

    return 0;
}
