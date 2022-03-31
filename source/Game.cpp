#include "Game.hpp"
#include <iostream>
#include <string>
#include <unistd.h>

#define try_read(value, predicat)                                       \
    std::cin >> value;                                                  \
    while(predicat) {                                                   \
        std::cout << "Ответ не в дипозоне значений. Введите еще раз\n"; \
        std::cin >> value;                                              \
    }

std::string getNameCell(Vector2D cell) {
    std::string ans;
    ans += cell.x + 'a';
    ans += cell.y + '0';
    return ans;
}

Game::Game() {
    state_.InitBoard();
    Start();
}

void Game::Start() {
    std::cout << "Привет!\n Это бот для игры в русские шашки.\n" <<
                 "Белые ходят первыми. Выберите цвет (black/white):\n";
    std::string color;
    try_read(color, color != "black" && color != "white");
    bool is_bot_white = color == "black";
    bot_.SetPlayer(is_bot_white);
    state_.SetPlayer(true); // ход белых первый
    is_bot_turn_ = is_bot_white;

    std::cout << "Выбери сложность от 1 до 10: P.S.(чем выше тем дольше боту нужно думать)\n";
    int32_t difficulty;
    try_read(difficulty, difficulty > 10 || difficulty < 0);
    bot_.SetDepth(difficulty);

    std::cout << "Начинаем!\n";
    MainGame();
}

void Game::End() {
    std::string win_msg;
    if (state_.isBotWin()) {
        win_msg = "Я победил! ";
    } else {
        win_msg = "Вы победили! ";
    }
    std::cout << win_msg << " Спасибо за игру!";
    sleep(1);
}

void Game::PlayerTurn() {
    sleep(10);
}

void Game::BotTurn() {
    auto moves = bot_.FindMove(state_);
    if (moves.size() < 2) {
        std::cout << "Что-то пошло не так!!!\n";
    }
    for (size_t i = 1; i < moves.size(); ++i) {
        std::cout << "Делаю ход \n";
        std::cout << getNameCell(moves[i - 1]) << "-" << getNameCell(moves[i]) << "\n";
        auto new_state = state_.makeMove(moves[i - 1], moves[i]);
        state_ = std::move(new_state);
        state_.PrintBoard();
    }
    std::cout << "Ход закончен!\n";
}

void Game::MainGame() {
    while (!state_.isEnd()) {
        state_.PrintBoard();
        if (is_bot_turn_) {
            std::cout << "Мой ход!\n";
            BotTurn();
        } else {
            std::cout << "Ваш ход!\n";
            PlayerTurn();
        }
        is_bot_turn_ = !is_bot_turn_;
    }
    End();
}
