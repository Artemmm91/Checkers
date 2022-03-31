#ifndef CHECKERS_TABLE_HPP
#define CHECKERS_TABLE_HPP

#include <vector>
#include "Vector.hpp"


// состояния клетки на доске
enum State: int32_t {
    Empty,
    WhitePawn,
    WhiteKing,
    BlackPawn,
    BlackKing
};

bool isWhite(int32_t state);
bool isBlack(int32_t state);
bool isKing(int32_t state);
bool onBoard(Vector2D cell);
// проверяет находится ли клетка на доске 8х8

class GameState{
    friend class ABSearch;
private:
    bool is_white_turn_ = true; // чей ход у бота
    mutable bool is_white_win_ = false;
    std::vector<std::vector<int32_t>> table_{8, std::vector<int32_t>(8, Empty)};
    std::vector<Vector2D> moves_history_{};

public:
    GameState(bool is_white, std::vector<std::vector<int32_t>> table, std::vector<Vector2D> moves):
            is_white_turn_(is_white), table_(std::move(table)), moves_history_(std::move(moves))
    {}

    GameState() = default;

    [[nodiscard]] bool isEnd() const; // проверяет все ли шашки съедены
    [[nodiscard]] bool isBotWin() const; // когда все шашки съедены

    [[nodiscard]] GameState makeMove(Vector2D cell, Vector2D land) const;
    // делает ход из cell в land
    // предполагается что ход корректен

    [[nodiscard]] std::vector<GameState> getPossibleStates() const;
    // возвращает все возможные состояни игры из данной клетки

    void InitBoard(); // заполняет доску шашками
    void SetPlayer(bool is_bot_white);
    void ChangePlayer();
    void clearMoves();

    void PrintBoard();
    bool canKill(Vector2D cell);
private:
    [[nodiscard]] bool isEnemyPawn(Vector2D cell) const;
    [[nodiscard]] bool isYourPawn(Vector2D cell) const;
    [[nodiscard]] bool isEmpty(Vector2D cell) const;
    [[nodiscard]] bool isLastLine(Vector2D cell) const;// дошли ли до последней линии

    void SetPawn(Vector2D cell); // ставим пешку своего цвета
    void SetKing(Vector2D cell); // ставим дамку своего цвета
    void SetEmpty(Vector2D cell);
    void clearLine(Vector2D cell, Vector2D land); // очищаем отрезокт диагонали

    [[nodiscard]] std::vector<GameState> getKills(Vector2D cell) const;
        // Возвращает все состояния которые можно получить из
        // данного только путем поедания фигур

    [[nodiscard]] std::vector<GameState> getMoves(Vector2D cell) const;
        // состояния которые можно получить из данного если нельзя никого съесть

    [[nodiscard]] std::vector<GameState> getPossibleStatesFromCell(Vector2D cell) const;
        // возвращает все возможные состояни игры из данной клетки
};

#endif
