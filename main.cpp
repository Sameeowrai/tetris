#include <iostream>
#include "clikit.hpp"

using namespace std;

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