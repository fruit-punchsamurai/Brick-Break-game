#include <iostream>
#include <cstdlib>
#include <time.h>
#include <graphics.h>
#define WINDOW_LENGTH 800
#define WINDOW_BREADTH 1500
#define RIGHT_ARROW 77
#define LEFT_ARROW 75
#define ONE_BY_ROOT_TWO 0.707106



using namespace std;

int bat[4];
int ball[2];
int direction = 0;
int page = 0;
int radius = 10;

void Rectangle(int point[], int color){
    setcolor(color);
    setfillstyle(SOLID_FILL,color);
    rectangle(point[0],point[1],point[2],point[3]);
    floodfill(point[0]+1,point[1]+1,color);
}

void Circle(int center[],int color){
    setcolor(color);
    setfillstyle(SOLID_FILL,color);
    circle(center[0],center[1],radius);
    floodfill(center[0],center[1],color);
}





void SetStartingPosition() {
    bat[0] = (WINDOW_BREADTH/2)-80;
    bat[1] = (WINDOW_LENGTH)-30;
    bat[2] = (WINDOW_BREADTH/2)+80;
    bat[3] = (WINDOW_LENGTH)-10;
    ball[0] = WINDOW_BREADTH/2;
    ball[1] = WINDOW_LENGTH/2;
}

void TranslateBat(int amount, int direction){
    cleardevice();
    if(direction == 0){
        bat[0] -= amount;
        bat[2] -= amount;
    }
    else{
        bat[0] += amount;
        bat[2] += amount;
    }
    if(bat[0]<5){
        bat[0] += amount;
        bat[2] += amount;
    }
    if(bat[2]>WINDOW_BREADTH){
        bat[0] -= amount;
        bat[2] -= amount;
    }
    Rectangle(bat,15);
    Circle(ball,15);
}


void CheckCollision(){
    switch(direction){
    case 1:
        //up
        if((ball[1]-radius)<0){
            ball[1] += amount;
            direction = 2;
        }
        break;
    case 2:

        //down
        if((ball[1]+radius)>WINDOW_LENGTH){
            ball[1] -= amount;
            direction = 1;
        }
        break;
    case 3:
        //top left
        if((ball[0]-radius)<0){
            ball[0] += ONE_BY_ROOT_TWO * amount;
            direction = 4;
        }
        if((ball[1]-radius)<0){
            ball[1] += ONE_BY_ROOT_TWO * amount;
            direction = 5;
        }
        break;
    case 4:
        //top right
        if((ball[0]+radius)>WINDOW_BREADTH){
            ball[0] -= ONE_BY_ROOT_TWO * amount;
            direction = 3;
        }
        if((ball[1]-radius)<0){
            ball[1] += ONE_BY_ROOT_TWO * amount;
            direction = 6;
        }
        break;
    case 5:
        //bottom left
        if((ball[0]-radius)<0){
            ball[0] += ONE_BY_ROOT_TWO * amount;
            direction = 6;
        }
        if((ball[1]+radius)>WINDOW_LENGTH){
            ball[1] -= ONE_BY_ROOT_TWO * amount;
            direction = 3;
        }
        break;
    case 6:
        //bottom right
        if((ball[0]+radius)>WINDOW_BREADTH){
            ball[0] -= ONE_BY_ROOT_TWO * amount;
            direction = 5;
        }
        if((ball[1]+radius)>WINDOW_LENGTH){
            ball[1] -= ONE_BY_ROOT_TWO * amount;
            direction = 4;
        }
        break;
    case 7:
        //left
        if((ball[0]-radius)<0){
            ball[0] += amount;
            direction = 8;
        }
        break;
    case 8:
        //right
        if((ball[0]+radius)>WINDOW_BREADTH){
            ball[0] -= amount;
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
        CheckCollision();


        break;
    case 2:
        //down
        ball[1] += amount;
        CheckCollision();

        break;
    case 3:
        //top left
        ball[0] -= ONE_BY_ROOT_TWO * amount;
        ball[1] -= ONE_BY_ROOT_TWO * amount;
        CheckCollision();

        break;
    case 4:
        //top right
        ball[0] += ONE_BY_ROOT_TWO * amount;
        ball[1] -= ONE_BY_ROOT_TWO * amount;
        CheckCollision();

        break;
    case 5:
        //bottom left
        ball[0] -= ONE_BY_ROOT_TWO * amount;
        ball[1] += ONE_BY_ROOT_TWO * amount;
        CheckCollision();

        break;
    case 6:
        //bottom right
        ball[0] += ONE_BY_ROOT_TWO * amount;
        ball[1] += ONE_BY_ROOT_TWO * amount;
        CheckCollision();

        break;
    case 7:
        //left
        ball[0] -= amount;
        CheckCollision();

        break;
    case 8:
        //right
        ball[0] += amount;
        CheckCollision();

        break;
        default:
            break;
    }
    Rectangle(bat,15);
    Circle(ball,10,15);
}

int main(){
    int i;
    char keypress;
    srand(time(NULL));
    do{
        direction = rand()% (6 - 5 + 1) + 5;
        cout<<direction<<endl;
    }while((direction!=5) && (direction !=6));

    SetStartingPosition();

    initwindow(WINDOW_BREADTH,WINDOW_LENGTH);
    setactivepage(page);


    Rectangle(bat,15);
    Circle(ball,15);
    setvisualpage(page);
    do{
        page==1?page--:page++;
        setactivepage(page);
        TranslateBall(1,radius);
        if(kbhit()){
        keypress = getch();
        switch(int(keypress)){
        case LEFT_ARROW:
            //translate right;
            TranslateBat(15,0);
            break;
        case RIGHT_ARROW:

            //translate left;
            TranslateBat(15,1);
            break;
        default:
            //do nothing;
            break;
        }
        }
        setvisualpage(page);
        delay(2);
    }while(keypress!='q');

    closegraph();
    return 0;
}






