#ifndef CHECKERS_GAME_HPP
#define CHECKERS_GAME_HPP

#include "Table.hpp"
#include "ABSearch.hpp"

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

#endif //CHECKERS_GAME_HPP
