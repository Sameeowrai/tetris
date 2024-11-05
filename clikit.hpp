#ifndef CLIKIT_HPP
#define CLIKIT_HPP

#include <iostream>
#include <vector>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sstream>

const unsigned int microsecond = 1000000;
using namespace std;

const string defaultColor = "\e[0;37m";
const float gameSpeed = 0.01;
#define ARROW_UP    65
#define ARROW_DOWN  66
#define ENTER       10

// Function declarations
inline void enableRawMode() {
    termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &term);
}

inline void disableRawMode() {
    termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &term);
}

inline void print(string text, float time, string colorCode = defaultColor, bool endline = 1) {
    cout << colorCode;
    for (char c : text) {
        cout << c;
        cout.flush();
        usleep(time * microsecond);
    }
    if(endline == 1)
        cout << endl;
}

inline void cls(){
    #ifdef _WIN32
        system("cls");
    #elif __linux__
        system("clear");
    #endif
}

termios original_term;
int original_flags;

inline void unblockInput() {
    tcgetattr(STDIN_FILENO, &original_term);
    original_flags = fcntl(STDIN_FILENO, F_GETFL);
    termios term = original_term;
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    fcntl(STDIN_FILENO, F_SETFL, original_flags | O_NONBLOCK);
}

inline void resetInput() {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_term);
    fcntl(STDIN_FILENO, F_SETFL, original_flags);
}

// MenuItem struct
template <typename T>
struct MenuItem {
    string text;
    string color;
    T value;
};

// Menu class
template <typename T>
class Menu {
    vector<MenuItem<T>> m_menuItems;
    string m_header;
    int selectedItem = 0;
    float displaySpeed;
    int m_maxVisibleItems;

public:
    Menu(string header, vector<MenuItem<T>> menuItems, float speed = 0.01, int maxVisibleItems = 5) 
        : m_menuItems(menuItems), m_header(header), displaySpeed(speed), m_maxVisibleItems(maxVisibleItems) {}

    void Display(float time) {
        cls();
        print(m_header,time);
        int startIndex = max(0, selectedItem - (m_maxVisibleItems / 2));
        int endIndex = min(static_cast<int>(m_menuItems.size()), startIndex + m_maxVisibleItems);

        if (startIndex > 0) {
            print("\u2191", time);
        }
        if (endIndex - startIndex < m_maxVisibleItems) {
            startIndex = max(0, endIndex - m_maxVisibleItems);
        }
        for (int i = startIndex; i < endIndex; ++i) {
            if (i == selectedItem) {
                print("-> " + m_menuItems[i].text, time, "\033[1;32m");
            } else {
                print("  " + m_menuItems[i].text, time, m_menuItems[i].color);
            }
        }
        if (endIndex < m_menuItems.size()) {
            print("\u2193", time);
        }
    }

    T Get() {
        enableRawMode();
        char c;
        Display(displaySpeed);
        while (true) {
            read(STDIN_FILENO, &c, 1);
            if (c == 27) { // Escape sequence
                char buf[3];
                if (read(STDIN_FILENO, buf, 2) == 2) {
                    if (buf[1] == ARROW_UP && selectedItem > 0) selectedItem--;
                    else if (buf[1] == ARROW_DOWN && selectedItem < m_menuItems.size() - 1) selectedItem++;
                }
            } else if (c == ENTER) {
                disableRawMode();
                return m_menuItems[selectedItem].value;
            }
            Display(0);
        }
    }

    void Insert(MenuItem<T> item) {
        m_menuItems.push_back(item);
    }
};

// Input class
template <typename T>
class Input{
private:
    string m_text;
    float speed;
    string m_color;
public:
    Input(string text, float time, string color = defaultColor): m_text(text),speed(time),m_color(color){}
    
    T Read() {
        print(m_text, speed, m_color, 0);
        string input;
        getline(cin, input);
        while (!IsValid(input)) {
            getline(cin, input);
        }
        return convertInput(input);
    }

    string ReadSecret(const char& symbol = ' '){
        print(m_text,speed,m_color,0);
        string input;
        int index = 0;
        char c;

        enableRawMode();

        while(1){
            c = getchar();
            if(c == '\n'){
                break;
            }else if(c == 127 || c == '\b'){
                if(!input.empty()){
                    input.pop_back();
                    cout << "\b \b";
                }
            }
            else{
                input += c;
                cout << symbol;
            }
        }

        disableRawMode();
        cout << endl;
        return input;
    }

    vector<T> ReadByDelimiter(const string& delimiter) {
        print(m_text, speed, m_color, 0);
        string input;
        getline(cin, input);
        
        vector<T> results;
        stringstream ss(input);
        string item;

        while (getline(ss, item, delimiter[0])) {
            if (IsValid(item,0)) {
                results.push_back(convertInput(item));
            }
        }

        return results;
    }

    bool IsValid(const string& input, bool debug = true, const string& errorText = "Invalid Input!") {
        if constexpr (is_same<T, string>::value) {
            return true;
        }

        stringstream ss(input);
        T type;
        ss >> type;

        if (ss.fail() || !ss.eof()) {
            if (debug) print(errorText, speed, "\e[0;31m");
            cout << defaultColor;
            return false;
        }
        
        return true;
    }

    T convertInput(const string& input) {
        if constexpr (std::is_same<T, std::string>::value) {
            return input;
        }

        stringstream ss(input);
        T result;
        
        if (!(ss >> result)) {
            throw runtime_error("Conversion failed!");
        }
        
        return result;
    }
};

#endif // CLIKIT_HPP
