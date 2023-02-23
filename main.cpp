#include <iostream>
#include <cstdlib>
#include <time.h>
#include <graphics.h>
#include <math.h>
#include <sstream>
#include <cstdio>
#define WINDOW_LENGTH 800
#define WINDOW_BREADTH 1500
#define RIGHT_ARROW 77
#define LEFT_ARROW 75
#define PI 3.141592

using namespace std;

struct obstacle{
    int arr[4];
    int state;
    int left;
    int right;
    int mid;
    int color;
};
typedef obstacle obstacle;
int bat[4];
int ball[2];
int direction = 0;
int page = 0;
int radius = 10;
float angle = 0.0;
int k;
int batLeft;
int batRight;
int batMid;
obstacle **brick;
int prevDirection = 3;
int gameFinish = 0;
int life = 30;

/*Function Declaration*/
void Rectangle (int point[], int color);
void Circle (int center[], int color);
void TranslateBall();
void CheckCollision(int changeX, int changeY );
void MakeBricks();
void DrawBricks();
void TranslateBat();
void SetStartingState();
void CheckGameFinish();
void GameComplete();
void Menu();
void GameOver();
void DisplayLife();


void Rectangle(int point[], int color){
    setcolor(color);
    setfillstyle(SOLID_FILL,color);
    rectangle(point[0],point[1],point[2],point[3]);
    floodfill(point[0]+1,point[1]+1,color);
}

void Circle(int center[], int color){
    setcolor(color);
    setfillstyle(SOLID_FILL,color);
    circle(center[0],center[1],radius);
    floodfill(center[0],center[1],color);
}

void SetStartingState() {
    int i,j;
    bat[0] = (WINDOW_BREADTH/2)-80;
    bat[1] = (WINDOW_LENGTH)-30;
    bat[2] = (WINDOW_BREADTH/2)+80;
    bat[3] = (WINDOW_LENGTH)-10;
    batLeft = bat[0] + 40;
    batRight = bat[2] -40;
    batMid = bat[0] + 80;
    ball[0] = WINDOW_BREADTH/2;
    ball[1] = WINDOW_LENGTH/2;

    for(i = 0; i < 6; i++){
        for(j = 0; j < 8; j++){
            brick[i][j].state = 1;
        }
    }
    MakeBricks();
    DisplayLife();
}



void TranslateBat(int amount, int batDirection){
    int firstChange = 0;
    cleardevice();
    prevDirection == batDirection ? firstChange = 0 : firstChange = 40;
    if(batDirection == 0){
        bat[0] -= amount + firstChange;
        bat[2] -= amount + firstChange;
        prevDirection = 0;
    }
    else{
        bat[0] += amount + firstChange;
        bat[2] += amount + firstChange;
        prevDirection = 1;
    }
    if(bat[0]<5){
        bat[0] += amount ;
        bat[2] += amount;
    }
    if(bat[2]>WINDOW_BREADTH){
        bat[0] -= amount;
        bat[2] -= amount;
    }
    batLeft = bat[0] + 40;
    batRight = bat[2] -40;
    batMid = bat[0] + 80;
    Rectangle(bat,15);
    Circle(ball,15);
    DrawBricks();
    rectangle(5,5,WINDOW_BREADTH-5,WINDOW_LENGTH-5);
    DisplayLife();
}


void CheckCollision(int changeX = 0,int changeY = 0){
    int i, j;
    switch(direction){
    case 1:
        //up
        if((ball[1]-radius)<0){
            ball[1] += changeX;
            direction = 2;
        }
        break;
    case 2:

        //down
        if((ball[1]+radius)>WINDOW_LENGTH){
            ball[1] -= changeX;
            direction = 1;
        }
        break;
    case 3:
        //top left
        if((ball[0]-radius)<0){
            ball[0] += changeX;
            direction = 4;
        }
        if((ball[1]-radius)<0){
            ball[1] += changeY;
            direction = 5;
        }
        for(i = 0; i< 6; i++){
            for(j = 0; j < 8; j++){
                if(brick[i][j].state > 0){
                if((((ball[1]+radius)>(brick[i][j].arr[1]))&&((ball[1]-radius)<(brick[i][j].arr[3])))&&(((ball[0]+radius)>=brick[i][j].arr[0])&&((ball[0]-radius)<=brick[i][j].arr[2]))){
                    ball[1] +=changeY;
                    brick[i][j].state--;
                    if((ball[0]+radius>=brick[i][j].arr[0])&&(ball[0]-radius<=brick[i][j].left)){
                        angle = PI/2 - angle;
                        direction = 5;
                    }
                    else if(ball[0]+radius>=brick[i][j].right && ball[0]-radius<=brick[i][j].arr[2]){
                        angle = PI/2 - angle;
                        direction = 6;
                    }
                    else{
                        direction = 5;
                    }
                }
                if((((ball[0]+radius)>(brick[i][j].arr[0]))&&((ball[0]-radius)<(brick[i][j].arr[2])))&&(((ball[1]+radius)>=brick[i][j].arr[1])&&((ball[1]-radius)<=brick[i][j].arr[3]))){
                    ball[0] +=changeX;
                    direction = 4;
                    brick[i][j].state--;
                }
                }
            }
        }
        break;
    case 4:
        //top right
        if((ball[0]+radius)>WINDOW_BREADTH){
            ball[0] -= changeX;
            direction = 3;
        }
        if((ball[1]-radius)<0){
            ball[1] += changeY;
            direction = 6;
        }
        for(i = 0; i < 6; i++){
            for(j = 0; j < 8; j++){
                if(brick[i][j].state > 0){
                if((((ball[1]+radius)>(brick[i][j].arr[1]))&&((ball[1]-radius)<(brick[i][j].arr[3])))&&(((ball[0]+radius)>=brick[i][j].arr[0])&&((ball[0]-radius)<=brick[i][j].arr[2]))){
                    ball[1] +=changeY;
                    brick[i][j].state--;
                    if((ball[0]+radius>=brick[i][j].arr[0])&&(ball[0]-radius<=brick[i][j].left)){
                        angle = PI/2 - angle;
                        direction = 5;
                    }
                    else if(ball[0]+radius>=brick[i][j].right && ball[0]-radius<=brick[i][j].arr[2]){
                        angle = PI/2 - angle;
                        direction = 6;
                    }
                    else{
                        direction = 6;
                    }
                }
                if((((ball[0]+radius)>(brick[i][j].arr[0]))&&((ball[0]-radius)<(brick[i][j].arr[2])))&&(((ball[1]+radius)>=brick[i][j].arr[1])&&((ball[1]-radius)<=brick[i][j].arr[3]))){
                    ball[0] -=changeX;
                    direction = 3;
                    brick[i][j].state--;
                }
                }
            }
        }
        break;
    case 5:
        //bottom left
        if((ball[0]-radius)<0){
            ball[0] += changeX;
            direction = 6;
        }
        if((ball[1]+radius)>WINDOW_LENGTH){
            ball[1] -= changeY;
            direction = 3;
            life--;
        }
        if(((ball[1]+radius)>(bat[1]))&&(((ball[0]+radius)>=bat[0])&&((ball[0]-radius)<=bat[2]))){
            ball[1] -=changeY;
            if((ball[0]+radius>=bat[0])&&(ball[0]-radius<=batLeft)){
                angle = PI/2 - angle;
                direction = 3;
            }
            else if(ball[0]+radius>=batRight && ball[0]-radius<=bat[2]){
                angle = PI/2 - angle;
                direction = 4;
            }
            else{
                direction = 3;
            }
        }
        for(i = 0; i < 6; i++){
            for(j = 0; j < 8; j++){
                if(brick[i][j].state > 0){
                if((((ball[1]+radius)>(brick[i][j].arr[1]))&&((ball[1]-radius)<(brick[i][j].arr[3])))&&(((ball[0]+radius)>=brick[i][j].arr[0])&&((ball[0]-radius)<=brick[i][j].arr[2]))){
                    ball[1] -=changeY;
                    brick[i][j].state--;
                    if((ball[0]+radius>=brick[i][j].arr[0])&&(ball[0]-radius<=brick[i][j].left)){
                        angle = PI/2 - angle;
                        direction = 3;
                    }
                    else if(ball[0]+radius>=brick[i][j].right && ball[0]-radius<=brick[i][j].arr[2]){
                        angle = PI/2 - angle;
                        direction = 4;
                    }
                    else{
                        direction = 3;
                    }
                }
                if((((ball[0]+radius)>(brick[i][j].arr[0]))&&((ball[0]-radius)<(brick[i][j].arr[2])))&&(((ball[1]+radius)>=brick[i][j].arr[1])&&((ball[1]-radius)<=brick[i][j].arr[3]))){
                    ball[0] +=changeX;
                    direction = 6;
                    brick[i][j].state--;
                }
                }
            }
        }

        break;
    case 6:
        //bottom right
        if((ball[0]+radius)>WINDOW_BREADTH){
            ball[0] -= changeX;
            direction = 5;

        }
        if((ball[1]+radius)>WINDOW_LENGTH){
            ball[1] -= changeY;
            direction = 4;
            life--;

        }
        if(( (ball[1]+radius)>(bat[1]))&&(((ball[0]+radius)>=bat[0])&&((ball[0]-radius)<=bat[2]))){
            ball[1] -=changeY;
            if((ball[0]+radius>=bat[0])&&(ball[0]-radius<=batLeft)){
                angle = PI/2 - angle;
                direction = 3;
            }
            else if(ball[0]+radius>=batRight && ball[0]-radius<=bat[2]){
                angle = PI/2 - angle;
                direction = 4;
            }
            else{
                direction = 4;
            }

        }
        for(i = 0; i < 6; i++){
            for(j = 0; j < 8; j++){
                if(brick[i][j].state > 0){
                if((((ball[1]+radius)>(brick[i][j].arr[1]))&&((ball[1]-radius)<(brick[i][j].arr[3])))&&(((ball[0]+radius)>=brick[i][j].arr[0])&&((ball[0]-radius)<=brick[i][j].arr[2]))){
                    brick[i][j].state--;
                    ball[1] -=changeY;
                    if((ball[0]+radius>=brick[i][j].arr[0])&&(ball[0]-radius<=brick[i][j].left)){
                        angle = PI/2 - angle;
                        direction = 3;
                    }
                    else if(ball[0]+radius>=brick[i][j].right && ball[0]-radius<=brick[i][j].arr[2]){
                        angle = PI/2 - angle;
                        direction = 4;
                    }
                    else{
                        direction = 4;
                    }
                }
                if((((ball[0]+radius)>(brick[i][j].arr[0]))&&((ball[0]-radius)<(brick[i][j].arr[2])))&&(((ball[1]+radius)>=brick[i][j].arr[1])&&((ball[1]-radius)<=brick[i][j].arr[3]))){
                    ball[0] -=changeX;
                    direction = 5;
                    brick[i][j].state--;
                }
                }
            }
        }
        break;
    case 7:
        //left
        if((ball[0]-radius)<0){
            ball[0] += changeX;
            direction = 8;
        }
        break;
    case 8:
        //right
        if((ball[0]+radius)>WINDOW_BREADTH){
            ball[0] -= changeX;
            direction = 7;
        }
        break;
    default:
        break;
    }

}



void TranslateBall(int speed,int radius){
    cleardevice();
    int amount = 8 * speed;
    switch(direction){
    case 1:
        //up
        ball[1] -= amount;
        CheckCollision(amount);


        break;
    case 2:
        //down
        ball[1] += amount;
        CheckCollision(amount);

        break;
    case 3:
        //top left
        ball[0] -= cos(angle) * amount;
        ball[1] -= sin(angle) * amount;
        CheckCollision(cos(angle) * amount,sin(angle) * amount);

        break;
    case 4:
        //top right
        ball[0] += cos(angle) * amount;
        ball[1] -= sin(angle) * amount;
        CheckCollision(cos(angle) * amount,sin(angle) * amount);

        break;
    case 5:
        //bottom left
        ball[0] -= cos(angle) * amount;
        ball[1] += sin(angle) * amount;
        CheckCollision(cos(angle) * amount,sin(angle) * amount);

        break;
    case 6:
        //bottom right
        ball[0] += cos(angle) * amount;
        ball[1] += sin(angle) * amount;
        CheckCollision(cos(angle) * amount,sin(angle) * amount);

        break;
    case 7:
        //left
        ball[0] -= amount;
        CheckCollision(amount);

        break;
    case 8:
        //right
        ball[0] += amount;
        CheckCollision(amount);

        break;
        default:
            break;
    }
    Rectangle(bat,15);
    Circle(ball,15);
    DrawBricks();
    rectangle(5,5,WINDOW_BREADTH-5,WINDOW_LENGTH-5);
    DisplayLife();
}

void MakeBricks(){
    int arr[5];
    int i, j, whiteRedBlue;
    int a, b, c, d;
    a = 40;
    b = 10;
    c = a + 160;
    d = b + 20;
    for(i = 0; i < 6; i++){
//        i % 2 == 0? a = 40 : a = 220;
//        i % 2 == 0? j = 0: j = 2;
        a = 40;
        c = a + 160;
        for(j = 0 ; j< 8; j++){
            brick[i][j].arr[0] = a;
            brick[i][j].arr[1] = b;
            brick[i][j].arr[2] = c;
            brick[i][j].arr[3] = d;
            brick[i][j].left = a + 40;
            brick[i][j].right = c - 40;
            brick[i][j].mid = a + 80;
            a = c + 20;
            c = a + 160;
            if(brick[i][j].state > 0){
                if(i % 2 == 0){
                    if(j % 2 == 0){
                        if (whiteRedBlue % 2 == 0){
                            brick[i][j].state = 6;
                        }
                        else{
                            brick[i][j].state = 1;
                        }
                        whiteRedBlue++;
                    }
                    else{
                        brick[i][j].state = 4;
                    }
                }
                else{
                    if(j % 2 == 0){
                        if (whiteRedBlue % 2 == 0){
                            brick[i][j].state = 4;
                        }
                        else{
                            brick[i][j].state = 1;
                        }
                        whiteRedBlue++;
                    }
                    else{
                        brick[i][j].state = 6;
                    }
                }
            }
        }
        whiteRedBlue = 0;
        b = d + 40;
        d = b + 20;
    }
}

void DrawBricks(){
    int i, j;
    for(i = 0; i < 6; i++){
        for(j = 0; j< 8; j++){
            if(brick[i][j].state > 0){
                if(brick[i][j].state == 1){
                    brick[i][j].color = 15;
                }
                else if(brick[i][j].state >= 2 && brick[i][j].state <= 4){
                    brick[i][j].color = 1;
                }
                else{
                    brick[i][j].color = 4;
                }
                Rectangle(brick[i][j].arr,brick[i][j].color);
            }
        }
    }
}

void CheckGameFinish(){
    int i, j;
    gameFinish = 1;
    for(i = 0; i < 6; i++){
        for(j = 0; j < 8; j++){
            if(brick[i][j].state<0){
                brick[i][j].state = 0;
            }
            if(brick[i][j].state != 0){

                gameFinish = 0;
                break;
            }
        }
        if(gameFinish == 0){
            break;
        }
    }
}

void GameComplete(){
    cleardevice();
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    setcolor(15);
    outtextxy(WINDOW_BREADTH/2-400,WINDOW_LENGTH/2,"Game Complete!!! You Won");
    setvisualpage(page);
    getch();
    exit(0);
}

void Menu(){
}

void GameOver(){
    cleardevice();
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    setcolor(15);
    outtextxy(WINDOW_BREADTH/2-400,WINDOW_LENGTH/2,"Game Over!!! You lost");
    setvisualpage(page);
    getch();
    exit(0);
}

void DisplayLife(){
    char lifeStr[20];
    sprintf(lifeStr,"Life : %d",life);
    setcolor(15);
    outtextxy(WINDOW_BREADTH-60,WINDOW_LENGTH-30,lifeStr);
}

int main(){
    brick = new obstacle *[6];
    for(k = 0; k<6; k++){
        brick[k] = new obstacle[8];
    }
    int i,ub,lb;
    char keypress;
    ub = 80;
    lb = 20;
    srand(time(NULL));
    int j;


//    do{
//        direction = rand()% (ub - lb + 1) + lb;
//        cout<<direction<<endl;
//    }while((direction!=5) && (direction !=6));
    angle = rand()% (ub - lb + 1) + lb;
    angle = 30;
    angle *= PI/180;
    direction = rand()%(6-5+1)+5;
    direction = 4;
    initwindow(WINDOW_BREADTH,WINDOW_LENGTH);

    SetStartingState();
    for(i = 0; i<6; i++){
        for(j = 0; j<8; j++){
            brick[i][j].state = 1;
        }
    }
//    for(i = 0; i<8; i++){
//        brick[0][i].state = 1;
//    }
    rectangle(5,5,WINDOW_BREADTH-5,WINDOW_LENGTH-5);
    setactivepage(page);


    Rectangle(bat,15);
    Circle(ball,15);
    DrawBricks();
    setvisualpage(page);
    getch();
    //GameComplete();
    do{
        page==1?page--:page++;
        setactivepage(page);
        TranslateBall(1,radius);
        if(kbhit()){
            keypress = getch();
            switch(int(keypress)){
            case LEFT_ARROW:
                //translate right;
                TranslateBat(40,0);
                break;
            case RIGHT_ARROW:

                //translate left;
                TranslateBat(40,1);
                break;
            default:
                //do nothing;
                break;
            }
        }
        setvisualpage(page);
        delay(2);
        CheckGameFinish();
        if (gameFinish == 1){
            page==1?page--:page++;
            setactivepage(page);
            GameComplete();
        }
        if(life == 0){
            page==1?page--:page++;
            setactivepage(page);
            GameOver();
        }
    }while(keypress!='q');

    closegraph();
    return 0;
}







