/***********

snake
    by evan guadalajara

***********/



#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>

#define ROWS 20
#define COLS 30

#define GAMEOVER_X 1
#define GAMEOVER_Y 8




typedef struct food 
{
    int x;
    int y;

} Food;


typedef struct segment
{
    //char letter;
    int x;
    int y;

} Segment;

void splashScreen();
void drawBorders();
void drawScore(int score);
void lose(int score);
int printGameOver(int score);
void updateFood(Food *food);
void drawSnake(int len, Segment segments[]);
void snakeMove(int *x, int *y, const int key);
void updateSnakeBody(int len, Segment segments[], int x, int y);
void updateSnakeHead(Segment segments[], int x, int y);
int checkCollision(int len, Segment segments[]);


int main()
{

    //initalize window and window settings
    initscr();
    keypad(stdscr, TRUE);
    //nodelay(stdscr, TRUE);
    noecho();
    curs_set(FALSE);
        
    //initialize variables
    int key; //for last key pressed
    //int snake_x, snake_y; //snake x and y positions
    int snake_len;
    int x,y; // vars x and y keep track of snakes current direction
    int score; //increments by 1 with each food collected
    int high_score1; //high score var will be filled with data from file
    int high_score2;
    int high_score3;
    Food food; //foooooood
    Segment segments[ROWS*COLS] = {0}; //initalizes array of segments for snake
    



    //starting game parameters
    srand(time(NULL)); //generates random seed
    snake_len = 1; //only one segment at start
    segments[0].x = 5; //snake x=5
    segments[0].y = 10;//snake y=10
    score = 0; //score begins at zero
    x = 1; //starts snake moving right
    y = 0;
    updateFood(&food); //randomizes food starting position


    //get high score
    /*
    FILE * fp = NULL;
    fp = fopen("scores.csv", )
    */
    //splash screen
    splashScreen();
    refresh();
    getch();
    //sets terminal mode to nodelay so snake will constantly move without user input
    nodelay(stdscr, TRUE);

    while(1)
    {   
        

        //food ate condition
        if(segments[0].x == food.x && segments[0].y == food.y)
        {
            score++;
            updateFood(&food);
            snake_len++;
        }

        updateSnakeBody(snake_len, segments, x, y);
        updateSnakeHead(segments, x, y);

       //lose condition
        if(segments[0].x == 0 || segments[0].y == 5 || segments[0].x == COLS-1 || segments[0].y == ROWS-1 || checkCollision(snake_len, segments))
        {

            lose(score);
            break;
        }
       
        //renders game
        erase();
        drawBorders();
        drawScore(score);

        //prints food and snake
        drawSnake(snake_len, segments);
        mvaddstr(food.y, food.x, "A");
        
        //user input for direction
        key = getch();
        
        //changes direction
        snakeMove(&x, &y, key);
       


        //delay cuz game would be broken if not
        usleep(100000);
        
    }

    //end curses mode
    endwin();
    return EXIT_SUCCESS;
}


//updates pos of food
void updateFood(Food *food)
{
    food->x = rand() % ((COLS-3) + 1 - 2) + 2;
    food->y = rand() % ((ROWS-3) + 1 - 6) + 6;
}

//updates pos of snake segments (aka segments[i])
void updateSnakeBody(int len, Segment segments[], int x, int y)
{
    
    for(int i = len-1; i > 0; i--)
    {
        segments[i].x = segments[i-1].x;
        segments[i].y = segments[i-1].y;
    }

}
//updates pos of snakehead (aka segments[0])
void updateSnakeHead(Segment segments[], int x, int y)
{
    segments[0].x += x;
    segments[0].y += y;
}


//draws entire snake
void drawSnake(int len, Segment segments[]) 
{
    for(int i = 0; i < len; i++)
    {
        mvaddch(segments[i].y, segments[i].x, 'S');
    }
}


//snake movement direction
void snakeMove(int *x, int *y, const int key)
{

    /* will change logic to a switch statement later 
    switch (key)
    {}
    */
    if(key == KEY_RIGHT)
    {
        *x = 1;
        *y = 0;
    }
    else if(key == KEY_LEFT)
    {
        *x = -1;
        *y = 0;
    }
    else if(key == KEY_UP)
    {
        *x = 0;
        *y = -1;
    }
    else if(key == KEY_DOWN)
    {
        *x = 0;
        *y = 1;
    }
}


//splash screen for startup
void splashScreen()
{

    mvprintw(1, 0, "   ___ _ __   __ _| | _____ \n");
    mvprintw(2, 0, "  / __| '_ \\ / _` | |/ / _ \\\n");
    mvprintw(3, 0, "  \\__ \\ | | | (_| |   <  __/\n");
    mvprintw(4, 0, "  |___/_| |_|\\__,_|_|\\_\\___|\n");
    mvprintw(5,7, "Use arrow keys to move.");
    mvprintw(6,9, "Press any key to start.");
}

//game borders and score

void drawBorders()
{

    for(int i = 5; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            if(i == 5)
            {
                mvaddch(i, j, '_');
            }
            else if( i == ROWS-1)
            {
                mvaddch(i, j, '=');
            }
            else if(j == 0 || j == COLS-1)
            {
                mvaddch(i, j, '|');
            }
            else
            {
                mvaddch(i, j, ' ');
            }
        }
        printw("\n");
    }
    mvprintw(0, 0, "   ___ _ __   __ _| | _____ \n");
    mvprintw(1, 0, "  / __| '_ \\ / _` | |/ / _ \\\n");
    mvprintw(2, 0, "  \\__ \\ | | | (_| |   <  __/\n");
    mvprintw(3, 0, "  |___/_| |_|\\__,_|_|\\_\\___|\n");
    mvprintw(4,0, " ~~~~~~~~~~~~~~~~~~~~~~~~~~");
}



void drawScore(int score)
{
    mvprintw(4, 11, "Score: %d", score);
}

//losing game stuff

int checkCollision(int len, Segment segments[])
{
    for(int i = 1; i < len; i++)
    {
        if(segments[0].x == segments[i].x && segments[0].y == segments[i].y)
        {
            return 1;
        }
    }

    return 0;
}

void lose(int score)
{
    erase();
    drawBorders();
    printGameOver(score);
    refresh();
    usleep(100000);
    nodelay(stdscr, FALSE);
    getch();
    getch();
}


int printGameOver(int score)
{
    mvprintw(GAMEOVER_Y, GAMEOVER_X,"  ____    _    __  __ _____ ");
    mvprintw(GAMEOVER_Y+1, GAMEOVER_X," / ___|  / \\  |  \\/  | ____|");
    mvprintw(GAMEOVER_Y+2, GAMEOVER_X,"| |  _  / _ \\ | |\\/| |  _|  ");
    mvprintw(GAMEOVER_Y+3, GAMEOVER_X,"| |_| |/ ___ \\| |  | | |___");
    mvprintw(GAMEOVER_Y+4, GAMEOVER_X," \\____/_/   \\_\\_|__|_|_____|");

    mvprintw(GAMEOVER_Y+5, GAMEOVER_X," / _ \\ \\   / / ____|  _ \\   ");
    mvprintw(GAMEOVER_Y+6, GAMEOVER_X,"| | | \\ \\ / /|  _| | |_) |  ");
    mvprintw(GAMEOVER_Y+7, GAMEOVER_X,"| |_| |\\ V / | |___|  _ <   ");
    mvprintw(GAMEOVER_Y+8, GAMEOVER_X," \\___/  \\_/  |_____|_| \\_\\  ");
    
    mvprintw(GAMEOVER_Y+9, 15, "SCORE: %d", score);
}

