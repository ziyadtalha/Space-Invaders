#include <iostream>
#include "mygraphics.h"
#include "myconsole.h"
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <sstream>
#include <string.h>
#include <fstream>
#include <vector>
using namespace std;

COLORREF blue = RGB(0, 0, 200);
COLORREF green = RGB(0, 200, 0);
COLORREF black = RGB(10, 10, 10);
COLORREF red = RGB(200, 0, 0);
COLORREF silver = RGB(210, 210, 210);
COLORREF lightblue = RGB(0, 205, 255);

class Enemy
{
public:

    Enemy(int x1, int y1, int coord1, int coord2);
    Enemy();
    ~Enemy();

    void moveDown(int** level) {
        //display black rectangle where enemy currently is
        displayBlackEnemy();

        level[c1][c2] = 0;

        //moving one cell above on screen
        y = y + 30;

        //moving bullet one row above in levels array
        c1++;
        level[c1][c2] = 4;

        //resetting the counter
        moveDownCounter = 500;
    }

    void decrementCounter() {
        if (moveDownCounter > 0)
        {
            moveDownCounter--;
        }
    }

    void displayEnemy(int** level) {
        myRect(x, y, x + 60, y + 30, red, green);
        myEllipse(x + 20, y + 5, x + 40, y + 25, blue, red);

        decrementCounter();

        if (moveDownCounter == 0)
        {
            moveDown(level);
        }
    }

    void displayBlackEnemy() {
        myRect(x, y, x + 60, y + 30, black, black);
    }

    int getC1() {
        return c1;
    }

    int getC2() {
        return c2;
    }

    void dieSoft() {
        dead = true;
    }

    bool isDead() {
        return dead;
    }

private:
    int x;
    int y;

    int c1;
    int c2;

    bool dead;

    int moveDownCounter;
};

Enemy::Enemy()
{
    x = 0;
    y = 0;
    c1 = 0;
    c2 = 0;
    moveDownCounter = 500;
    dead = false;
}

Enemy::Enemy(int x1, int y1, int coord1, int coord2)
{
    x = x1;
    y = y1;
    c1 = coord1;
    c2 = coord2;
    moveDownCounter = 500;
    dead = false;
}

Enemy::~Enemy()
{
}

//=====================================================================================

class bullet
{
public:
    bullet();
    bullet(int x1, int y1, int C1, int C2);
    ~bullet();

    int getC1() {
        return c1;
    }

    int getC2() {
        return c2;
    }

    void displayBullet(int rowAbovePlayer) {
        myRect(x + 20, y, x + 40, y + 30, green, blue);
        //printing black bullet under the bullet but only if bullet isn't at the start (right above the player)
        if (c1 != rowAbovePlayer)
        {
            myRect(x + 20, y + 30, x + 40, y + 60, black, black);
        }
    }

    void displayBlackBullet() {
        myRect(x + 20, y + 30, x + 40, y + 60, black, black);
    }

    void moveUp(int** level) {
        level[c1][c2] = 0;

        //moving one cell above on screen
        y = y - 30;

        //moving bullet one row above in levels array
        c1--;
        level[c1][c2] = 3;
    }

    bool checkCollisionWithBorder(int** level)
    {
        //collides with edge of board
        if (level[c1 - 1][c2] == 1)
        {
            //print a black bullet where that bullet last was
            myRect(x + 20, y + 30, x + 40, y + 60, black, black);
            
            //make cell with the bullet in 2D array 0
            level[c1][c2] = 0;

            return true;
        }
        else
        {
            return false;
        }
    }

    bool checkCollisionWithEnemy(int** level, Enemy* &enemies, int enemyCount)
    {
        //collides with enemy
        if (level[c1 - 1][c2] == 4)
        {
            for (int i = 0; i < enemyCount; i++)
            {
                //if enemy is alive, then we return true
                if (enemies[i].isDead() == false)
                {
                    if (enemies[i].getC2() == c2)
                    {
                        if (enemies[i].getC1() == (c1 - 1) || (enemies[i].getC1() == c1))
                        {

                            enemies[i].displayBlackEnemy();
                            enemies[i].dieSoft();

                            //make cells with the bullet and the enemy in 2D array 0
                            level[c1][c2] = 0;
                            level[c1 - 1][c2] = 0;

                            return true;
                        }
                    }
                }
            }
        }
        else
        {
            return false;
        }
    }

private:
    //position on screen
    int x;
    int y;

    int c1;//row
    int c2;//column
};

bullet::bullet()
{
    x = 0;
    y = 0;
    c1 = 0;
    c2 = 0;
}

bullet::bullet(int x1, int y1, int C1, int C2)
{
    x = x1;
    y = y1;
    c1 = C1;
    c2 = C2;
}

bullet::~bullet()
{
    //empty
}

//==============================================================

class Player
{
public:
    Player();
    Player(int x1, int y1, int c1, int c2);
    ~Player();

    void displayPlayer() {
        myRect(x, y, x + 60, y + 30, blue, green);
        myEllipse(x + 5, y + 5, x + 25, y + 25, blue, red);
        myEllipse(x + 35, y + 5, x + 55, y + 25, blue, red);
    }

    void displayBlackPlayer() {
        //print a black rectangle at the player's (former usually) position
        myRect(x, y, x + 60, y + 30, black, black);
    }

    void moveUp(int** level) {
        if(positionRelativeToLastRow > -3)
        {
            level[c1][c2] = 0;

            displayBlackPlayer();

            //moving one cell above on screen
            y = y - 30;

            //moving player one row above in levels array
            c1--;
            level[c1][c2] = 2;
            positionRelativeToLastRow--;
        }
    }

    void moveDown(int** level) {
        if(positionRelativeToLastRow < 0)
        {
            level[c1][c2] = 0;

            displayBlackPlayer();

            //moving one cell down on screen
            y = y + 30;

            //moving player one row below in levels array
            c1++;
            level[c1][c2] = 2;
            positionRelativeToLastRow++;
        }
    }

    //can be used in load game I think
    void setCoordinates(int C1, int C2) {
        c1 = C1;
        c2 = C2;
    }

    int getC1() {
        return c1;
    }

    int getC2() {
        return c2;
    }

    void updateC2Right(int** level) {
        c2++;
        level[c1][c2] = 2;
        level[c1][c2 - 1] = 0;

        x = x + 60;
    }

    void updateC2Left(int** level) {
        c2--;
        level[c1][c2] = 2;
        level[c1][c2 + 1] = 0;

        x = x - 60;
    }

    //creates a bullet in the cell above the player and returns it
    bullet* shootBullet() {
        bullet* b = new bullet(x, y - 30, c1 - 1, c2);
        return b;
    }

private:
    //position graphically
    int x;
    int y;

    //position in the 2D array
    int c1; //row
    int c2; //col

    int positionRelativeToLastRow;
};

Player::Player()
{
    x = 0;
    y = 0;
    c1 = 0;
    c2 = 0;

    positionRelativeToLastRow = 0;
}

Player::Player(int x1, int y1, int coord1, int coord2) {
    x = x1;
    y = y1;
    c1 = coord1;
    c2 = coord2;

    positionRelativeToLastRow = 0;
}

Player::~Player()
{
    //empty
}

//==============================================================

class board
{
public:
    board();
    board(string filename);
    
    ~board();

    void displayEnemies() {
        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i].isDead() == false)
            {
                enemies[i].displayEnemy(level);
            }
        }
    }

    void checkBulletCollisions(int** level, int& score) {
        for (int i = 0; i < bullets->size(); i++)
        {
            //if bullet collided with border
            if (bullets->at(i).checkCollisionWithBorder(level) == true) {
                //delete that bullet from our vector
                bullets->erase(bullets->begin() + i);
            }
            else
            {
                //if bullet collided with enemy
                if (bullets->at(i).checkCollisionWithEnemy(level, enemies, enemyCount) == true)
                {
                    //delete bullet from 2D level array
                    //level[bullets->at(i).getC1()][bullets->at(i).getC2()] = 0;

                    //printing black bullet where the bullet last was
                    bullets->at(i).displayBlackBullet();

                    //delete that bullet from our vector
                    bullets->erase(bullets->begin() + i);

                    score++;
                }
                //if no collision, then print bullet and move bullet up
                else
                {
                    bullets->at(i).displayBullet(p->getC1() - 1);
                    bullets->at(i).moveUp(level);
                }
            }
        }
    }

    bool checkEnemyPositions() {
        for (int i = 0; i < enemyCount; i++)
        {
            if (enemies[i].getC1() == (rows - 1))
            {
                return true;
            }
        }
        return false;
    }

    void displayBoard() {
        int y = 10;
        int x = 10;
        for (int i = 0; i < rows; i++)
        {
            x = 10;
            for (int j = 0; j < cols; j++)
            {
                //if same row as the player
                if (i == p->getC1())
                {
                    //display a black rectangle, as player does not properly overwrite its older graphic sometimes
                    if (level[i][j] == 0)
                    {
                        myRect(x, y, x + 60, y + 30, black, black);
                    }
                }

                //if wall
                if (level[i][j] == 1)
                {
                    myRect(x, y, x + 60, y + 30, red, black);
                }
                else
                {
                    //if player
                    if (level[i][j] == 2)
                    {
                        p->displayPlayer();
                    }
                    else
                    {
                        //IF ENEMY
                        if (level[i][j] == 4)
                        {
                            displayEnemies();
                        }
                        else
                        {
                            //idk
                        }
                    }
                }
                
                x = x + 60;
            }

            y = y + 30;
        }

        x = x / 2;
        y = y + 30;

        char pressP[] = "Press P to Pause Game";
        myDrawTextWithFont(x, y, 30, pressP, black, red);
    }

    void TITLESCREEN() //Displays the name of game on first screen
    {
        //x variable is used to slowly turn the title screen black, as it is decremented with each loop
        int x = 5;

        char title[] = "Space Invaders";

        //displays the title screen over the course of a few seconds
        for (int i = 0; i <= 30; i++)
        {
            COLORREF redFade = RGB(200 - x, 0, 0);
            COLORREF yellowFade = RGB(0, 200 - x, 200 - x);
            myDrawTextWithFont(100, 200, 150, title, redFade, yellowFade);
            x += 5;
            Sleep(50);
        }
    }

    void WINNINGSCREEN() //Displays YOU WIN
    {
        char w[] = "YOU WIN";

        for (int i = 0; i <= 30; i++)
        {
            myDrawTextWithFont(800, 450, 150, w, silver, lightblue);
            Sleep(100);
        }
    }

    void pauseScreen() {
        char paused[] = "Paused";
        myDrawTextWithFont(150, 250, 200, paused, black, red);
    }

    void startGame() {
        char title[] = "Space Invaders";

        int w = 0, h = 0;
        GetWindowDimensions(w, h);

        PlaceCursor(0, edgeOfGameY);
        SetWindowSize(w, h);
        SetWindowTitle(title);
        TITLESCREEN();
        ClearScreen();

        int score = 0;

        bool flag1 = false;

        while (flag1 == false)
        {
            char t = ' ';
            while (_kbhit() == 1)
            {
                t = _getch();

                int playerC2 = p->getC2();
                if (t == 'd')
                {
                    //if player has reached right side of level
                    if (playerC2 == (cols - 2))
                    {
                        //don't move
                    }
                    else
                    {
                        p->updateC2Right(level);
                    }
                }
                else
                {
                    if (t == 'a')
                    {
                        //if player has reached left side of level
                        if (playerC2 == 1)
                        {
                            //don't move
                        }
                        else
                        {
                            p->updateC2Left(level);
                        }
                    }
                    else
                    {
                        if (t == ' ')
                        {
                            level[p->getC1() - 1][p->getC2()] = 3;
                            bullets->push_back(*(p->shootBullet()));
                        }
                        else
                        {
                            if (t == 'w')
                            {
                                p->moveUp(level);
                            }
                            else
                            {
                                if (t == 's')
                                {
                                    p->moveDown(level);
                                }
                                else
                                {
                                    if (t == 'p') {
                                        ClearScreen();
                                        pauseScreen();
                                        t = _getch();
                                        ClearScreen();
                                    }
                                }
                            }
                        }
                    }
                }
            }

            checkBulletCollisions(level, score);
            displayBoard();
            //Sleep(100);

            //if an enemy reaches bottom of screen
            if (checkEnemyPositions() == true)
            {
                flag1 = true;
                ClearScreen();
                cout << "YOU LOSE" << endl;
            }

            //if winning condition reached
            if (score == scoreReq)
            {
                flag1 = true;
                ClearScreen();
                WINNINGSCREEN();
            }
        }
    }

private:
    //board itself is actually a dynamic 2D array of integers
    int** level;
    int rows;
    int cols;

    int scoreReq;

    //board contains a player
    Player* p;

    //board contains enemies
    Enemy* enemies;
    int enemyCount;

    //board contains bullets shot by the player
    vector<bullet>* bullets;

    int edgeOfGameY;
};

board::board() {
    level = nullptr;
    rows = 0;
    cols = 0;
    scoreReq = 0;
    p = nullptr;
    bullets->resize(1);
    enemies = nullptr;
    enemyCount = 0;
    edgeOfGameY = 0;
}

//parameterised constructor reads level from a .txt file
board::board(string filename)
{
    bullets = new vector<bullet>;

    rows = 0;
    cols = 0;

    enemyCount = 0;

    p = nullptr;

    // Read from the text file
    ifstream myFile(filename);

    string rawinput = "";
    if (myFile.is_open()) {   //checking whether the file is open
        string temp;
        while (getline(myFile, temp)) { //read data from file object and put it into string.
            rawinput = rawinput + temp;
        }
        myFile.close(); //close the file object.
    }

    //GETTING ROWS AND COLUMNS OF LEVEL FROM FILE
    string t;
    int k = 0;
    for (int i = 0; i < 2; i++)
    {
        t = "";
        while (rawinput[k] != ',')
        {
            t = t + rawinput[k];
            k = k + 1;
        }
        k = k + 1;
        if (i == 0)
        {
            stringstream geek1(t);
            geek1 >> rows;
        }
        if (i == 1)
        {
            stringstream geek1(t);
            geek1 >> cols;
        }
    }

    //making a temp array to store the file inputs in
    int* temparr = new int[rows * cols];
    for (int i = 0; i < rows * cols; i++)
    {
        t = "";
        while (rawinput[k] != ',')
        {
            t = t + rawinput[k];
            k = k + 1;
        }
        k = k + 1;
        stringstream geek1(t);
        geek1 >> temparr[i];
    }
    
    //initialising level 2d array
    level = new int* [rows];
    for (int i = 0; i < rows; i++)
    {
        level[i] = new int[cols];
    }

    int offSet = 0;
    int y = 10;
    int x;
    scoreReq = 0;

    for (int i = 0; i < rows; i++)
    {
        x = 10;
        for (int k = 0; k < cols; k++)
        {
            *(level[i] + k) = temparr[offSet];
            offSet++;

            //player position on screen and coordinates in level are passed to the player object
            if (level[i][k] == 2)
            {
                p = new Player(x, y, i, k);
            }
            else
            {
                //enemy position on screen and coordinates in level are pushed into the vector of enemies
                //if enemy cell detected then enemy count incremented
                if (level[i][k] == 4)
                {
                    enemyCount++;
                    
                    //increment score required
                    scoreReq++;
                }
            }

            x = x + 60;
        }
        y = y + 30;
    }

    edgeOfGameY = y;

    //dynamic 1d array of enemies
    enemies = new Enemy[enemyCount];

    int enemyIndex = 0;

    //traverse the level array again, this time calculating the enemy position
    y = 10;
    for (int i = 0; i < rows; i++)
    {
        x = 10;
        for (int k = 0; k < cols; k++)
        {
            //enemy position on screen and coordinates in level are pushed into the vector of enemies
            if (level[i][k] == 4)
            {
                Enemy* e = new Enemy(x, y, i, k);
                enemies[enemyIndex] = *e;

                enemyIndex++;
            }

            x = x + 60;
        }
        y = y + 30;

        //making the loop slightly more effective
        //we exit as soon as we have the last enemy added
        if ((enemyIndex + 1) == enemyCount)
        {
            break;
        }
    }
}

board::~board()
{
    for (int i = 0; i < rows; i++)
    {
        delete level[i];
        level[i] = nullptr;
    }
    delete level;
    level = nullptr;
}

int main() {
    board b("space.txt");
    b.startGame();
}