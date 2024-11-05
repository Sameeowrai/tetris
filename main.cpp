#include <iostream>
#include "clikit.hpp"

using namespace std;
struct cubeVector{
    int area[3][3];
};
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