#ifndef TENENC_HPP
#define TENENC_HPP

#include <iostream>
#include <fstream>

using namespace std;

class Tenenc{
    public: 
    string encode(string text){
        string result = "";
        for(int i=0; i < text.length(); i++){
            string newValue;
            if(text[i] == '\n'){
                newValue = "9999";
            }else{
                int AsciiValue = int(text[i]);
                newValue = to_string(AsciiValue*10);
            }
            while(newValue.length()<4){
                newValue = "0" + newValue;
            }
            result += newValue;
        }
        return result;
    }

    string decode(string text) {
        string result = "";
        for (size_t i = 0; i < text.length(); i += 4) {
            int value = stoi(text.substr(i, 4)) / 10;
            if(value==9999){
                result += "\n";
            }else{
                result += static_cast<char>(value); 
            }
        }
        return result;
    }

    int find(string str, char* argv[], int size){
        for(int i=0; i < size; i++){
            if(string(argv[i]) == str){
                return i;
            }
        }
        return -1;
    }
};

#endif // TENENC_HPP
