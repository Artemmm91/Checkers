#pragma once
#include "Table.hpp"
#include "ABSearch.cpp"

class Game {
private:
    GameState state_;
    ABSearch bot_;
    bool is_bot_turn_{};
    std::vector<Vector2D> moves_history_; // чтобы посмотреть партию потом

public:
    Game();

private:
    void Start();
    void End();
    void PlayerTurn();
    void BotTurn();
    void MainGame();

};