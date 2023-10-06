// Гра Тетріс 
// Комар А. М.
// налаштування консолі
// Properties -> Шрифт -> Точкові шрифти -> Poзмір 12Х16
#pragma once
#include <iostream>
#include <ctime>
#include <vector>
#include <conio.h>
#include <Windows.h>
#include <fstream>
#include <string>
using namespace std;

const int  HEIGHT(20);   
const int  WIDTH(10) ;
const int  EMPTY  = 0;   // заповнення площі ініціалізації
const int  BORDER = 1;   // поля для ініціалізації
const int  BLOCK  = 2;   // для фіксації фігури
const int  STAT_Y = 4;   // зміщення статистики по Y
const char FIGURE = 254; // символ фігури
const char FIELDS = 176; // символ поля гри
enum Key { UP = 72, DOWN = 80, LEFT = 75, RIGHT = 77, ESC = 27 };

enum Colors
{
    black,          //  0 
    dark_blue,      //  1
    dark_green,     //  2
    dark_cyan,      //  3
    dark_red,       //  4
    dark_magenta,   //  5
    dark_yellow,    //  6
    light_gray,     //  7
    dark_gray,      //  8
    light_blue,     //  9
    light_green,    // 10
    light_cyan,     // 11
    light_red,      // 12
    light_magenta,  // 13
    light_yellow,   // 14
    white           // 15
};
void SetColor(int bg, int text)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bg << 4) | text);
}
void HideConsoleCursor() // Приховати курсор консолі
{
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void goTo_X_Y(int x, int y) // позиція курсора
{
    COORD point = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}

class Shape 
{
private:
    struct Point
    {
        int x;
        int y;
    };
    vector<Point> coordinates;
    int kind;
    int delay = 0;
public:

    int getX(int i)const { return coordinates[i].x; }
    int getY(int i)const { return coordinates[i].y; }
    int getCoordinatesSize()const { return coordinates.size(); }
    void setDelay(int d) { this->delay = d; }
    void push(int x, int y)
    {
        Point tmp;
        tmp.x = x;
        tmp.y = y;
        coordinates.push_back(tmp);
    }
    void show()const
    {
        switch (kind)
        {
        case 0: // I-фігура
            SetColor(white, light_blue);
            break;
        case 1: // L-фігура
            SetColor(white, light_green);
            break;
        case 2: // O-фігура
            SetColor(white, light_cyan);
            break;
        case 3: // T-фігура
            SetColor(white, light_red);
            break;
        case 4: // Z-фігура
            SetColor(white, light_magenta);
            break;
        case 5: // Z-фігура в право
            SetColor(white, dark_red);
            break;
        case 6: // L-фігура в ліво
            SetColor(white, dark_blue);
            break;
        }
        for (int i = 0; i < coordinates.size(); i++)
        {
            goTo_X_Y(coordinates[i].x, coordinates[i].y);
            cout << FIGURE;
        }
        SetColor(white, black);
    }
    void hide()const // очистка
    {
        for (int i = 0; i < coordinates.size(); i++)
        {
            goTo_X_Y(coordinates[i].x, coordinates[i].y);
            cout << ' ';
        }
    }
    bool moveDelay() // затримка фігури
    {
        if (delay < 10)
        {
            delay++;
            return false;
        }
        delay = 0;
        return true;

    }
    void muveDown() // рух в низ
    {
        for (int i = 0; i < coordinates.size(); i++)
        {
            coordinates[i].y++;
        }
    }
    void muveUp()// рух в верх
    {
        for (int i = 0; i < coordinates.size(); i++)
        {
            coordinates[i].y--;
        }
    }
    void muveLeft()//рух ліворуч
    {
        for (int i = 0; i < coordinates.size(); i++)
        {
            coordinates[i].x--;
        }
    }
    void muveRight()//рух праворуч
    {
        for (int i = 0; i < coordinates.size(); i++)
        {
            coordinates[i].x++;
        }
    }
    void spawn()//генерація фігури
    {
        coordinates.clear();
        kind = rand() % 7;
        switch (kind)
        {
        case 0: // I-фігура
            this->push(4, 0);
            this->push(5, 0);
            this->push(6, 0);
            this->push(7, 0);
            break;
        case 1: // L-фігура
            this->push(4, 1);
            this->push(5, 1);
            this->push(6, 1);
            this->push(6, 0);
            break;
        case 2: // O-фігура
            this->push(4, 0);
            this->push(5, 0);
            this->push(4, 1);
            this->push(5, 1);
            break;
        case 3: // T-фігура
            this->push(4, 0);
            this->push(5, 0);
            this->push(6, 0);
            this->push(5, 1);
            break;
        case 4: // Z-фігура
            this->push(4, 0);
            this->push(5, 0);
            this->push(5, 1);
            this->push(6, 1);
            break;
        case 5: // Z-фігура в право
            this->push(6, 0);
            this->push(5, 0);
            this->push(5, 1);
            this->push(4, 1);
            break;
        case 6: // L-фігура в ліво
            this->push(4, 0);
            this->push(5, 0);
            this->push(6, 0);
            this->push(6, 1);
            break;
        }
    }
    void rotateRight()  // поворот фігури за часовою стрілкою
    {
        Point center = coordinates[1]; // Визначаємо центральну точку (перша координата)
        for (int i = 0; i < coordinates.size(); i++)
        {
            int x = -(coordinates[i].y - center.y) + center.x;
            int y =  (coordinates[i].x - center.x) + center.y;
            // Оновлюємо координати фігури
            coordinates[i].x = x;
            coordinates[i].y = y;
        }
    }
    void rotateLeft()  // поворот фігури проти часової стрілки
    {
        Point center = coordinates[1]; // Визначаємо центральну точку (перша координата)
        for (int i = 0; i < coordinates.size(); i++)
        {
            int x =  (coordinates[i].y - center.y) + center.x;
            int y = -(coordinates[i].x - center.x) + center.y;
            // Оновлюємо координати фігури
            coordinates[i].x = x;
            coordinates[i].y = y;
        }
    }
};
class Matrix 
{
    vector<vector<int>> grid;
public:
    void init(int widtch, int height) // iніціалізація вектора
    {
        SetColor(white, black);
        vector<int>tmp;
        tmp.push_back(BORDER);
        for (int i = 1; i < height + 1; i++)
        {
            tmp.push_back(EMPTY);
        }
        tmp.push_back(BORDER);

        for (int i = 0; i < widtch + 2; i++)
        {
            grid.push_back(tmp);
        }
        tmp.clear();
        for (int i = 0; i <= height + 1; i++)
        {
            tmp.push_back(BORDER);
        }
        grid.push_back(tmp);
    }
    void show()
    {
        // для настройки
       /* for (int i = 0; i < grid.size(); i++)
        {
            goTo_X_Y(19, i);
            cout << i << " ";

                if (i<10)
                    cout << " " ;
                for (int j = 0; j < grid[i].size(); j++)
                {
                    cout << grid[i][j];
                }
            cout << endl;
        }*/
        
        // для гри
        goTo_X_Y(0, 0);
        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = 0; j < grid[i].size(); j++)
            {
                if (grid[i][j] == BORDER)
                    cout << FIELDS;
                if (grid[i][j] == BLOCK)
                {
                    SetColor(white, dark_yellow);
                    cout << FIGURE;
                    SetColor(white, black);
                }
                else if (grid[i][j] == EMPTY)
                    cout << ' ';
            }
            cout << endl;
        }
    }

    // Видалення заповненої лінії
    void deleteLine(int widtch)
    {
        vector<int> lines; // Вектор для збереження індексів ліній, які потрібно видалити

        for (int i = grid.size() - 1; i >= 0; i--)
        { // Перебираємо рядки ігрового поля знизу вгору

            int n = 0; 
            for (int j = 1; j < grid[i].size() - 1; j++)
            { // Перебираємо комірки рядка, щоб підрахувати кількість блоків

                if (grid[i][j] == BLOCK)
                {
                    n++;
                }
                else
                    break;
            }
            if (n == WIDTH)
            { // Якщо рядок заповнений (кількість блоків дорівнює WIDTH), то додаємо індекс рядка у вектор lines
                lines.push_back(i);
            }
        }
        for (int i = 0; i < lines.size(); i++)
        {// Видаляємо вибрані лінії з головного ігрового поля
            grid.erase(grid.begin() + lines[i]);
        }

        vector<int> tmp;
        tmp.push_back(BORDER);
        // Створюємо новий рядок tmp, який складається з BORDER та порожніх комірок EMPTY
        for (int i = 1; i < widtch + 1; i++)
        {
            tmp.push_back(EMPTY);
        }
        tmp.push_back(BORDER);

        // Вставляємо новий рядок tmp вгору головного ігрового поля для відновлення поля після видалення ліній
        for (int i = 0; i < lines.size(); i++)
        {
            grid.insert(grid.begin(), tmp);
        }
    }
    bool isCollision(const Shape& shape)const //перевірки на зіткнення з фігурами, полями  гри, або вихід за межі 
    {
        for (int i = 0; i < shape.getCoordinatesSize(); i++)
        {
            if (shape.getY(i) < 0)
                return true;
            if (shape.getX(i) <= 0)
                return true;
            if (shape.getX(i) >= WIDTH + 1)
                return true;
            if (grid[shape.getY(i)][shape.getX(i)] == BORDER)
                return true;
            if (grid[shape.getY(i)][shape.getX(i)] == BLOCK)
                return true;
        }
        return false;
    }
    bool isCollisionGameOver(const Shape& shape)const// перевірка чи поле для гри заповнене до верху
    {
        for (int i = 0; i < shape.getCoordinatesSize(); i++)
        {
            if (grid[0][shape.getX(i)] == BLOCK|| grid[1][shape.getX(i)] == BLOCK)
                return true;
        }
        return false;
    }
    void fixed(Shape& shape) // фіксація фігур
    {
        for (int i = 0; i < shape.getCoordinatesSize(); i++)
        {
            grid[shape.getY(i)][shape.getX(i)] = BLOCK;
        }
    }

};
class Statistic
{
private:
    string player_nickname;
    int time_game;
    int s_time;
    int f_time;
    int count;
    int time_record;
    int record;
public:
    Statistic() :player_nickname(""), time_game(0), s_time(0), f_time(0), count(0), record(0), time_record(0){}
    void setCount(int c)
    {
        this->count += c;
    }
    void setS_time(int t)
    {
       this->s_time = t;
    }
    void setF_time(int t)
    {
        this->f_time = t;
    }
    void setTime_game()
    {
        this->time_game = f_time - s_time;// Обчислення часу гри
    }
    void printMyStat()const
    {
        goTo_X_Y(WIDTH + 3, STAT_Y);

        int h = time_game / 3600;
        int m = (time_game % 3600) / 60;
        int s = time_game % 60;
        SetColor(black, light_yellow);

        cout << "Game time :: " << (h < 10 ? "0" : "") << h << ":" << (m < 10 ? "0" : "") << m << ":" << (s < 10 ? "0" : "") << s << endl;
        goTo_X_Y(WIDTH + 3, STAT_Y + 1);
        cout << "Score     :: " << count << endl;
    }
    bool recordСheck()//перевірка рекорду
    {
        if (count > record)
        {
            record = count;
            goTo_X_Y(3, HEIGHT+4);
            cout << "<< You set a new record >>" << endl;
            goTo_X_Y(3, HEIGHT+5);
            cout << "   Enter your nickname:  ";
            cin >> player_nickname;
            return true;
        }
        if (count == record)
        {
            if (time_record > time_game)
            {
                goTo_X_Y(3, HEIGHT+4);
                cout << "<< You set a new record >>" << endl;
                goTo_X_Y(3, HEIGHT+5);
                cout << "   Enter your nickname:  ";
                cin >> player_nickname;
                return true;
            }
        }
        return false;
    }
    void saveStatistic()const// збереження статистики у файл
    {
        ofstream outputFile("statistics.txt");
        if (outputFile.is_open())
        {
            outputFile << player_nickname << endl;
            outputFile << time_game << endl;
            outputFile << record << endl;
            outputFile.close();
            goTo_X_Y(0, HEIGHT+4);
            cout << "\tStatistics saved!" << endl;
        }
        else
        {
            goTo_X_Y(0, HEIGHT+4);
            cout << "\tError saving statistics." << endl;
        }
    }
    void loadStatistic()
    {
        ifstream inputFile("statistics.txt");
        if (inputFile.is_open())
        {
            // Зчитування значень з файлу та присвоєння їх відповідним полям
            getline(inputFile, player_nickname);
            inputFile >> time_record >> record;
            inputFile.close();

            goTo_X_Y(WIDTH + 3, STAT_Y+3);

            int h = time_record / 3600;
            int m = (time_record % 3600) / 60;
            int s = time_record % 60;
            SetColor(black, light_yellow);
            cout << "Set record:: " << player_nickname << endl;
            goTo_X_Y(WIDTH + 3, STAT_Y + 4);

            cout << "Game time :: " << (h < 10 ? "0" : "") << h << ":" << (m < 10 ? "0" : "") << m << ":" << (s < 10 ? "0" : "") << s << endl;
            goTo_X_Y(WIDTH + 3, STAT_Y + 5);
            cout << "Record    :: " << record << endl;
        }
        else
        {
            goTo_X_Y(0, 24);
            cout << "No statistics found." << endl;
        }
    }
};
class Game 
{
public:
    void ShowMenu()const
    {
        SetColor(black, light_blue);

        goTo_X_Y(WIDTH+3, STAT_Y+9);
        cout << " << Control keys >>" << endl;
        goTo_X_Y(WIDTH + 3, STAT_Y+11);
        cout << "\t     TURN" << endl;
        goTo_X_Y(WIDTH + 3, STAT_Y+12);
        cout << "\t      " << (char)30 << endl;
        goTo_X_Y(WIDTH + 3, STAT_Y+13);
        cout << endl;
        goTo_X_Y(WIDTH + 3, STAT_Y+14);
        cout << "LEFT " << (char)17 << "   " << (char)31 << "   " << (char)16 << " RIGH " << endl;
        goTo_X_Y(WIDTH + 3, STAT_Y+15);
        cout << "\t     DOWN" << endl;

    }
    void GameOver(Statistic &stat)const // гру закінчено
    {
        SetColor(black, light_yellow);

        system("cls"); // Очищення екрану

        cout << "==================================" << endl;
        cout << "           GAME OVER!" << endl;
        cout << "==================================" << endl;
        // Вивід статистики гри
        stat.printMyStat();
        cout << endl;
        stat.loadStatistic();
        // Перевірка та оновлення рекорду
        if (stat.recordСheck())
        {
            stat.saveStatistic();
        }
        cout << "Press any key to exit...";
        _getch(); // Очікування натискання клавіші користувачем перед виходом
    }

    int run()
    {
        srand(time(0));
        Statistic stat;
        stat.setS_time(time(0));// Отримує час старту
        ShowMenu();
        Matrix matrix;
        matrix.init(HEIGHT, WIDTH);
        matrix.show();
        Shape fig;
        fig.spawn();
        while (!matrix.isCollisionGameOver(fig))
        {
            while (!_kbhit())
            {
                fig.hide();
                if (fig.moveDelay())
                    fig.muveDown();
                if (matrix.isCollision(fig))
                {
                    fig.muveUp();
                    matrix.fixed(fig);
                    stat.setCount(1);
                    matrix.deleteLine(WIDTH);
                    matrix.show();
                    if (matrix.isCollisionGameOver(fig))
                    {
                        GameOver(stat);
                        break;
                    }
                    fig.spawn();
                }

                // Отримує поточний час 
                stat.setF_time(time(0));
                stat.setTime_game();
                // Виведіть тривалість гри на екран
                stat.printMyStat();
                stat.loadStatistic();
               fig.show();
               Sleep(50);
            }
            if (_kbhit()) // повертає істину, якщо нажата яка-небудь клавіша на клавіатурі
            {
                Key key = static_cast<Key>(_getch()); // щоб прочитати один символ з клавіатури
                switch (key)
                {
                case LEFT:
                    fig.hide();
                    fig.muveLeft(); 
                    if (matrix.isCollision(fig))
                    {
                        fig.muveRight();
                    }
                    break;
                case RIGHT:
                    fig.hide();
                    fig.muveRight();
                    if (matrix.isCollision(fig))
                    {
                        fig.muveLeft();
                    }
                    break;
                case DOWN:
                    fig.setDelay(10);
                    if (matrix.isCollision(fig))
                    {
                        fig.setDelay(0);
                    }
                    break;
                case UP:
                    fig.hide();
                    fig.rotateRight();
                    if (matrix.isCollision(fig))
                    {
                        fig.rotateLeft();
                        fig.muveRight();
                        fig.muveLeft();
                    }
                    break;
                case ESC:
                    GameOver(stat);
                    return 0;
                }

            }

        } 
    }
};

int main()
{
    HideConsoleCursor();

    Game game;
    game.run();
}
