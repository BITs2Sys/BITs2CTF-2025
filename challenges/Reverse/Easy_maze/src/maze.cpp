#include "maze.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

using namespace std;
int a, b, inputjudge = 0;

void Maze::right() {
    b = b + 1;
};
void Maze::left() {
    b = b - 1;
};
void Maze::up() {
    a = a + 1;
};
void Maze::down() {
    a = a - 1;
};
void Maze::front() {
    b = b - 6;
};
void Maze::back() {
    b = b + 6;
};

void Maze::ifchar(char c) {
    if (c >= 'A' && c <= 'I') {
        up();
        if (c == 'A') {
            left();
            front();
        }
        else if (c == 'B')
            front();
        else if (c == 'C') {
            right();
            front();
        }
        else if (c == 'D')
            left();
        else if (c == 'F')
            right();
        else if (c == 'G') {
            left();
            back();
        }
        else if (c == 'H')
            back();
        else if (c == 'I') {
            right();
            back();
        }
    }
    else if (c >= 'R' && c <= 'Z') {
        down();
        if (c == 'R') {
            left();
            front();
        }
        else if (c == 'S')
            front();
        else if (c == 'T') {
            right();
            front();
        }
        else if (c == 'U')
            left();
        else if (c == 'W')
            right();
        else if (c == 'X') {
            left();
            back();
        }
        else if (c == 'Y')
            back();
        else if (c == 'Z') {
            right();
            back();
        }
    }
    else if (c >= 'J' && c <= 'Q') {
        if (c == 'J') {
            left();
            front();
        }
        else if (c == 'K')
            front();
        else if (c == 'L') {
            right();
            front();
        }
        else if (c == 'M')
            left();
        else if (c == 'N')
            right();
        else if (c == 'O') {
            left();
            back();
        }
        else if (c == 'P')
            back();
        else if (c == 'Q') {
            right();
            back();
        }
    }
    else
        inputjudge = 1;
}
int Maze::checkend() {
    cube[0][0] = 0;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 36; j++) {
            if (cube[i][j] == 0)
                continue;
            else
                goto label;
        }
    }
    cout << "you find it!!!" << endl;
    return 0;
label:
    cout << "you are wrong!" << endl;
    return 1;
}
int Maze::check() {
    cvbe[0][0] = 0;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 36; j++) {
            if (cvbe[i][j] == 0)
                continue;
            else
                goto label;
        }
    }
    cout << "you are right?" << endl;
    return 0;
label:
    cout << "you are wrong!" << endl;
    return 1;
}
bool isdebug()
{
	return IsDebuggerPresent();
}

int main(int argc, char** argv) {
    char* input;
    if (argc < 2) {
        cout << "Input the flag:" << endl;
        string str;
        cin >> str;
        input = new char[str.length()];
        strcpy(input, str.data());
    }
    else {
        argv++;
        input = *argv;
    }
    int len = strlen(input);
    if (strncmp("BITs2CTF{", input, 9) || len != 37 || *(input + len - 1) != '}') {
        cout << "You are wrong!" << endl;
    }
    else {
        Maze maze;
        if (isdebug()){
        	for (int i = 9; i < len - 1; i++) {
            	maze.ifchar(*(input + i));
            	if (a < 0 || a > 5 || b < 0 || b > 35 || inputjudge == 1)
                	break;
            	if (maze.cvbe[a][b] % 2 == 0)
                	break;
            	else
                	maze.cvbe[a][b] = 0;
        	}
        	maze.check();
        }
        else{
			for (int i = 9; i < len - 1; i++) {
            	maze.ifchar(*(input + i));
            	if (a < 0 || a > 5 || b < 0 || b > 35 || inputjudge == 1)
                	break;
            	if (maze.cube[a][b] % 2 == 0)
                	break;
            	else
                	maze.cube[a][b] = 0;
        	}
        	maze.checkend();
		}
    }
    system("pause");
    return 0;
}
