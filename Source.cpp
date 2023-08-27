
#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include<iostream>
#include<ctime>
#include<vector>
#include<fstream>
#include<math.h>
using namespace sf;
using namespace std;
struct Position
{
    int x;
    int y;
    
};
enum GameState {
    GAME,
    GAME_OVER,
};

enum GameCheck
{
    GAME_CONT,
    WIN_CROSSES,
    WIN_ZEROES,
    DRAW
};

constexpr float cellSize = 150.0f;
constexpr int fieldSize = 3;
/* VARIABLES */

GameState state = GAME;

struct GameUpdate
{
    GameUpdate(sf::RenderWindow& _window) : window{ _window }
    {}
    sf::RenderWindow& window;
    int c = 0;
    bool mousePressed = false;
    int loop = 0;
    int rl{};
    vector<Position>v;
    vector<int> res = vector<int>(9, 3);

    string gameResult;

    sf::Texture ph;
    sf::Texture lh;
    Sprite cr;
    Sprite zr;
    sf::RectangleShape block = sf::RectangleShape(Vector2f(cellSize, cellSize));
    int xl = 0;
    int yl = 0;

    void init()
    {
        block.setFillColor(sf::Color::Cyan);
        
        ph.loadFromFile("C:\\programmingC++\\tic\\tak\\tictac\\Paint\\cross.png");

        cr.setTexture(ph);
        cr.setScale(0.75, 0.75);
        
        lh.loadFromFile("C:\\programmingC++\\tic\\tak\\tictac\\Paint\\zerro.png");

        zr.setTexture(lh);
        zr.setScale(0.75, 0.75);
    }

    bool check_line(vector<int>res, bool horizont, int index) {
        if (horizont)
        {
            return res[3 * index] == res[3 * index + 1] && res[3 * index + 2] == res[3 * index] && res[3 * index] != 3;
        }
        return res[index + 3] == res[index] && res[index + 6] == res[index] && res[index] != 3;
    }

    GameCheck check_win(vector<int>res, int loop, sf::RectangleShape& line) {

        for (int index = 0; index < 3; index++)
        {
            if (check_line(res, true, index)) {
                line.setPosition(cellSize * index + 80, 0);
                line.rotate(90);
                return res[3 * index] == 1 ? WIN_CROSSES : WIN_ZEROES;
            }
            if (check_line(res, false, index)) {
                line.setPosition(0, cellSize * index + 70);
                return res[index] == 1 ? WIN_CROSSES : WIN_ZEROES;
            }
        }
        if (res[0] == res[4] && res[0] == res[8] && res[0] != 3 && loop != 1) {
            line.rotate(45);
            return res[4] == 1 ? WIN_CROSSES : WIN_ZEROES;
        }
        if (res[2] == res[4] && res[2] == res[6] && res[2] != 3 && loop != 1) {
            line.setPosition(cellSize * 3, 0);
            line.rotate(135);
            return res[4] == 1 ? WIN_CROSSES : WIN_ZEROES;
        }
        return c == 9 ? DRAW : GAME_CONT;
    }

    void updateGame(sf::RectangleShape& line)
    {
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        
        bool mouseState = Mouse::isButtonPressed(sf::Mouse::Left);
        bool mouseDown = !mousePressed && mouseState;
        bool mouseUp = mousePressed && !mouseState;

        mousePressed = mouseState;

        if (mouseDown && pos.x <= cellSize * fieldSize && pos.y <= cellSize * fieldSize)
        {
            int r1 = pos.x / cellSize;
            int r2 = pos.y / cellSize;
            int f = 3 * r1 + r2;
            if (res[f] == 3) {
                Position pos{ r1, r2 };
                v.push_back(pos);
                res[f] = 1 - (c % 2);
                c++;

                auto gameState = check_win(res, loop, line);
                if (gameState != GAME_CONT) {
                    updateGameOver(gameState);
                }
            }
        }

        drawField();
    }

    void drawField()
    {
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++) {
                window.draw(block);
                block.setPosition(i * 151, j * 151);
            }
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                cr.setPosition(i * 151, j * 151);
                zr.setPosition(i * 151, j * 151);
            }
        }

        for (int i = 0; i < c; i++)
        {
            Sprite* sprite = i % 2 ? &zr : &cr;
            sprite->setPosition(v[i].x * 151, v[i].y * 151);
            window.draw(*sprite);
        }
    }

    void updateGameOver(GameCheck h) {
        if (h == WIN_CROSSES) {
            gameResult = "crosses\n   win";
        }
        else if (h == WIN_ZEROES) {
            gameResult = "zeroes\n   win";
        }
        else if (h == DRAW) {
            gameResult = "draw";
        }
        state = GAME_OVER;
    }

    void reset()
    {
        fill(res.begin(), res.end(), 3);
        c = 0;
        v.clear();
    }
};




int main()
{
    sf::RenderWindow window{ sf::VideoMode(450, 450), "tic-tac-toe!" };
    GameUpdate game(window);
    game.init();

    Font font;
    font.loadFromFile("C:\\programmingC++\\game\\sfml_project\\Project1\\ArialMdm.ttf");
    Text text("", font, 70);
    text.setFillColor(Color::Color(0,128,128));
    text.setStyle(Text::Bold);
    sf::RectangleShape line(sf::Vector2f(750.f, 10.f));
    line.setFillColor(sf::Color::Magenta);
    line.setPosition(0, -20);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(Color(0, 0, 100));
        
        switch (state)
        {
        case GAME:
            game.updateGame(line);
            break;
        case GAME_OVER:
            game.drawField();
            text.setString(game.gameResult);
            text.setPosition(110, 150);
            window.draw(text);
            window.draw(line);
            
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                state = GAME;
                game.reset();
            }

            break;
        }
        
        //window.draw(text);
        window.display();
    }

    return 0;
}