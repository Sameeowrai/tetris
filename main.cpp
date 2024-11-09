#include <iostream>
#include <ctime>
#include "clikit.hpp"
#include "tenenc.hpp"

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

    int getBlocksCount(){
        int counter = 0;
        for(int i=0; i < 3; i++){
            for(int j=0; j < 3; j++){
                if(area[i][j] == 1)
                counter++;
            }
        }
        return counter;
    }
};


class Game{
    Shape currentShape;
    int shapeX,shapeY;
    public:
    int score = 0;
    int highScore = 0;
    float speed = 0.25;
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

    void display() {
        cls();
        cout << defaultColor << "Score: " << score << " | coordinate : { " << shapeX << "," << shapeY << "}" << endl;
        
        int tempMap[height][width];
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                tempMap[i][j] = map[i][j];
            }
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (currentShape.getArea(i, j) == 1) {
                    int mapY = shapeY + i;
                    int mapX = shapeX + j;
                    if (mapY >= 0 && mapY < height && mapX >= 0 && mapX < width) {
                        tempMap[mapY][mapX] = 1;
                    }
                }
            }
        }

        for (int i = 0; i < height; i++) {
            cout << "\e[1;33m| " << defaultColor;
            for (int j = 0; j < width; j++) {
                if (tempMap[i][j] == 0) {
                    cout << " -";
                } else if (tempMap[i][j] == 1) {
                    cout << blockColor << "[]"<<defaultColor;
                }
            }
            cout << " \e[1;33m|" << endl;
        }
        cout << "| ";
        for (int k = 0; k < width; k++) {
            cout << " -";
        }
        cout << " |" << endl;
    }

    void moveLeft(){
        if(!checkLeftCollide(currentShape) && !checkLeftOut(currentShape)) shapeX--;
    }

    void moveRight(){
        if(!checkRightCollide(currentShape) && !checkRightOut(currentShape)) shapeX++;
    }

    void moveDown(){
        if(!checkDownCollide() && shapeY+3 < height){
            shapeY++;
        }
    }

    void rotate(){
        Shape temp = currentShape;
        temp.rotate();
        if(!checkLeftCollide(temp) && !checkRightCollide(temp)){
            if(checkLeftOut(temp)){
                shapeX = 0;
            }else if(checkRightOut(temp)){
                shapeX = width - 3;
            }
            currentShape.rotate();      
        }
    }

    void produce() {
        int randomNumber = rand() % shapesVector.size();
        cubeVector vector = shapesVector[randomNumber];
        currentShape.set(vector);
        shapeX = width / 2 - 1; 
        shapeY = 0;
    }

    bool checkLeftOut(Shape shape){
        for(int i=0;i<3; i++){
            if(shapeX <= 0 && shape.getArea(i,-shapeX) == 1){
                return true;
            }
        }
        return false;
    }

    bool checkRightOut(Shape shape){
        for(int i=0;i<3; i++){
            if(shapeX+3 >= width && shape.getArea(i,2-(shapeX+3-width)) == 1){
                return true;
            }
        }
        return false;
    }

    bool checkOnGround(){
        for(int i=0;i<3; i++){
            if(shapeY+3 >= height && currentShape.getArea(2-(shapeY+3-height),i) == 1){
                return true;
            }
        }
        return false;
    }

    bool checkLeftCollide(Shape shape){
        for(int i=2; i >= 0; i--){
            for(int j=0; j <= 2;j++){
                if(map[shapeY+i][shapeX+j-1] == 1 && shape.getArea(i,j) == 1){
                    return true;
                }
            }
        }
        return false;
    }

    bool checkRightCollide(Shape shape){
        for(int i=2; i >= 0; i--){
            for(int j=2; j >= 0;j--){
                if(map[shapeY+i][shapeX+j+1] == 1 && shape.getArea(i,j) == 1){
                    return true;
                }
            }
        }
        return false;
    }

    bool checkDownCollide(){
        for(int i=3;i>0;i--){
            for(int j=0; j < 3; j++){
                if(map[shapeY+i][shapeX+j] == 1 && currentShape.getArea(i-1,j) == 1){
                    return true;
                }
            }
        }
        
        return false;
    }

    void fall(int i){
        for(i; i > 0; i--){
            for(int j=0; j < width;j++){
                    int tmp = map[i-1][j];
                    map[i-1][j] = 0;
                    map[i][j] = tmp;
            }
        }
    }

    void checkRow(){
        bool isComplete;
        for(int i=height-1;i > 0; i--){
            isComplete = true;
            for(int j=0; j < width;j++){
                if(map[i][j] != 1){
                    isComplete = false;
                    break;
                }
            }
            if(isComplete){
                score += width;
                fall(i);
            }
        }
    }

    void update(){
        if(!checkDownCollide() && !checkOnGround()){
            shapeY++;
        }else{
            if(shapeY == 0){
                isGameover = true;
                return;
            }

            for(int i=0; i < 3;i++){
                for(int j=0; j < 3;j++){
                    if (currentShape.getArea(i, j) == 1) {
                        int mapY = shapeY + i;
                        int mapX = shapeX + j;
                        if (mapY >= 0 && mapY < height && mapX >= 0 && mapX < width) {
                            map[mapY][mapX] = 1;
                        }
                    }
                }
            }

            score += currentShape.getBlocksCount();
            produce();
        }
        checkRow();
    }

    void restart(){
        score = 0;
        for(int i=0; i < height; i++){
            for(int j=0; j < width;j++){
                map[i][j] = 0;
            }
        }
        isGameover = false;
    }

    void updateHighScore(){
        if(score > highScore){
            ofstream file("score.dat");
            Tenenc tenenc;
            if(file.is_open()){
                file << tenenc.encode(to_string(score));
                file.close();
            }
            highScore = score;
        }
    }

    void loadHighScore(){
        ifstream scoreFile("score.dat");
        if(scoreFile.is_open()){
            string line;
            Tenenc tenenc;
            getline(scoreFile,line);
            highScore = stoi(tenenc.decode(line));
        }else{
            highScore = 0;
        }
    }

    void place(){
        while(!checkDownCollide() && shapeY+3 < height){
            shapeY++;
        }
    }
};

int main(){
    srand(time(0));
    Menu<int> menu("Menu",{
        {"Start",defaultColor,0},
        {"High Score",defaultColor,1},
        {"Settings",defaultColor,2},
        {"Exit", "\e[0;31m",3}
    },0.01,10);
    Menu<int> setting("Menu",{
        {"Change Block Color", defaultColor, 0},
        {"Change Game Speed", defaultColor, 1},
    },0.01,10);
    Menu<string> colorMenu("Colors", {
      {"Black",defaultColor,"\e[1;30m"},
      {"Red",defaultColor,"\e[1;31m"},
      {"Green",defaultColor,"\e[1;32m"},  
      {"Yellow",defaultColor,"\e[1;33m"},
      {"Blue",defaultColor,"\e[1;34m"},
      {"White",defaultColor,"\e[1;37m"},
    },0.01,10);
    Game game;
    Tenenc tenenc;
    while(1){
        int op = menu.Get();
        switch (op)
        {
        case 0:{
            game.restart();
            char c;
            unblockInput();
            while(!game.isGameover){
                game.display();
                usleep(microsecond*game.speed);
                if(read(STDIN_FILENO, &c,1) > 0){
                    if(c == 'a'){
                        game.moveLeft();
                    }else if(c == 'd'){
                        game.moveRight();
                    }else if(c == 's'){
                        game.moveDown();
                    }else if(c == 'r'){
                        game.rotate();
                    }else if(c == 'f'){
                        game.place();
                    }
                }
                game.update();
                tcflush(STDIN_FILENO,TCIFLUSH);
            }
            game.updateHighScore();
            resetInput();
            print("Game Over",0.25,"\e[1;31m");
            print("Press Any Key To Continue ...", 0.05, "\e[1;33m",0);
            getchar();
            break;
        }

        case 1:{
            cls();
            game.loadHighScore();
            Menu<int> back(to_string(game.highScore),{
                {"Back", defaultColor, 0},
            });
            back.Get();
            break;
        }

        case 2:{
            int s_op = setting.Get();
            switch (s_op)
            {
            case 0:
                game.blockColor = colorMenu.Get();
                break;
            case 1:{
                Input<float> speedInput("Enter game speed between 0 & 1:",0.01);
                float speed = speedInput.Read();
                while(speed <= 0 || speed > 1){
                    cout << "Invalid number!" << endl;
                    speed = speedInput.Read();
                }
                game.speed = speed;
                break;
            }
            default:
                break;
            }
            break;
        }
        case 3:{
            exit(0);
        }
        default:
            break;
        }
    }
    return 0;
}