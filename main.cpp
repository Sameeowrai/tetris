#include <iostream>
#include "clikit.hpp"

using namespace std;

const int width = 10;
const int height = 20;

struct cubeVector{
    int area[3][3];
};

cubeVector L = {{{1,0,0},{1,0,0},{1,1,0}}};
cubeVector J = {{{0,0,1},{0,0,1},{0,1,1}}};
cubeVector I = {{{1,0,0},{1,0,0},{1,0,0}}};
cubeVector O = {{{1,1,0},{1,1,0},{0,0,0}}};
cubeVector S = {{{0,0,0},{0,1,1},{1,1,0}}};
cubeVector Z = {{{0,0,0},{1,1,0},{0,1,1}}};
cubeVector T = {{{0,0,0},{1,1,1},{0,1,0}}};
cubeVector Zero = {{{0,0,0},{0,0,0},{0,0,0}}};

vector<cubeVector> shapesVector = {L,J,I,O,S,Z,T};

class Shape{
    int area[3][3];
public:
    void set(cubeVector area){
        for(int i=0; i< 3;i++){
            for(int j = 0; j < 3;j++){
                this->area[i][j] = area.area[i][j];
            }
        }
    }

    void rotate(){
        cubeVector zero = {{{0,0,0},{0,0,0},{0,0,0}}};
        Shape rotated_shape;
        rotated_shape.set(zero);
        for(int i=0;i < 3; i++){
            for(int j=0; j < 3;j++){
                if(area[i][j] == 1){
                    rotated_shape.area[(2-j)][i] = 1;
                }
            }
        }
        for(int i=0; i < 3; i++){
            for(int j=0; j < 3; j++){
                if(rotated_shape.area[i][j] == 1){
                    area[i][j] = 1;
                }else{
                    area[i][j] = 0;
                }
            }
        }
    }

    int getArea(int i, int j) {
        return area[i][j];
    }

class Game{
    Shape currentShape;
    int shapeX,shapeY;
    public:
    int score = 0;
    int map[height][width];
    bool isGameover = false;
    string blockColor = "\e[1;92m";
    Game(){
        currentShape.set(Zero);
        for(int i=0; i < height; i++){
            for(int j=0;j < width;j++){
                map[i][j] = 0;
            }
        }
        produce();
    }

    void produce() {
        int randomNumber = rand() % shapesVector.size();
        cubeVector vector = shapesVector[randomNumber];
        currentShape.set(vector);
        shapeX = width / 2 - 1; 
        shapeY = 0;
    }
};

int main(){
    Menu<int> menu("Menu",{
        {"Start",defaultColor,0},
        {"High Score",defaultColor,1},
        {"Settings",defaultColor,2},
        {"Exit", "\e[0;31m",3}
    },0.01,10);
    while(1){
        int op = menu.Get();
        switch (op)
        {
        case 0:{
            break;
        }

        case 1:{
            break;
        }

        case 2:{
            break;
        }
        case 3:{
            exit(0);
        }
        default:
            break;
        }
    }   
}