#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>

//Ball settings
const int numBalls = 5;
const float rMax = 5;
const float rMin = 1.5;
const float vRange = 0.25;


//View settings
const float xMin = -20;
const float xMax = 20;
const float yMin = -10;
const float yMax = 10;
const int screenW = 250;
const int screenH = 60;

const float charH = (yMax-yMin)/((float)screenH);
const float charW = (xMax-xMin)/((float)screenW);

const char charLookup[][4] = {
        " ","┌","┐","─","└","│","┼","┘","┘","┼","│","└","─","┐","┌"," "
};

float randf(float min, float max){
    return min + static_cast <float> (std::rand()) /( static_cast <float> (RAND_MAX/(max-min)));
}

float weird(float x, float y){
    return x*cosf(y)+y*cosf(x);
}

struct cir {
    float r,x,y,vX,vY;
};

void init(cir *balls){
    for(int i=0; i<numBalls; i++) {
        balls[i].r = randf(rMin, rMax);
        balls[i].x = randf(xMin+balls[i].r,xMax-balls[i].r);
        balls[i].y = randf(yMin+balls[i].r,yMax-balls[i].r);
        balls[i].vX = randf(0-vRange,vRange);
        balls[i].vY = randf(0-vRange,vRange);
    }
}
float eval(float x, float y, cir *balls){
    float sum = 0;
    for(int i=0; i<numBalls; i++) {
        sum += (2.0f*balls[i].r/(float)sqrt((x-balls[i].x)*(x-balls[i].x) + (y-balls[i].y) * (y-balls[i].y))) - 1.0f;
    }
    return sum;
}

void move(cir *balls){
    for(int i=0; i<numBalls; i++) {
        //Collisions:
        if(balls[i].x-balls[i].r < xMin || balls[i].x+balls[i].r > xMax)
            balls[i].vX *= -1;
        if(balls[i].y-balls[i].r < yMin || balls[i].y+balls[i].r > yMax)
            balls[i].vY *= -1;
        //Movement
        balls[i].x += balls[i].vX;
        balls[i].y += balls[i].vY;
    }
}

void line(){
    for(int x = 0; x < screenW; x++) {
        std::cout << charLookup[3];
    }
}
void top(){
    std::cout << charLookup[1];
    line();
    std::cout << charLookup[2] << std::endl;
}
void bot(){
    std::cout << charLookup[4];
    line();
    std::cout << charLookup[8] << std::endl;
}

int main(){
    float grid[screenH+1][screenW+1] = {0};
    cir balls[numBalls];
    init(balls);
    while(true) {
        move(balls);
        for (int y = 0; y < screenH+1; y++) {
            for (int x = 0; x < screenW + 1; x++) {
                grid[y][x] = eval(xMin + x*charW, yMax - y*charH, balls);
            }
        }
        std::cout << "\033[2J\033[1;1H" << std::flush;
        top();
        for (int y = 0; y < screenH; y++) {
            std::cout << charLookup[5];
            for (int x = 0; x < screenW; x++) {
                std::cout << charLookup[
                        (grid[y][x] < 0 ? 8 : 0) +
                        (grid[y][x + 1] < 0 ? 4 : 0) +
                        (grid[y + 1][x] < 0 ? 2 : 0) +
                        (grid[y + 1][x + 1] < 0 ? 1 : 0)];
            }
            std::cout << charLookup[5] << std::endl;
        }
        bot();
        std::cout << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return 0;
}