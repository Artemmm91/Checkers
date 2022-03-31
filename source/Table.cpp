#include "Table.hpp"
#include <iostream>


bool isWhite(int32_t state) {
    return state == WhitePawn || state == WhiteKing;
}

bool isBlack(int32_t state) {
    return state == BlackPawn || state == BlackKing;
}

bool isKing(int32_t state) {
    return state == WhiteKing || state == BlackKing;
}


bool onBoard(Vector2D cell) {
    return cell.x >= 0 && cell.x < 8 && cell.y >= 0 && cell.y < 8;
}

bool GameState::isEnd() const {
    bool white_seen = false;
    bool black_seen = false;
    for (auto &row: table_) {
        for (int32_t cell: row) {
            if (isWhite(cell)) white_seen = true;
            if (isBlack(cell)) black_seen = true;
        }
    }
    if (white_seen && black_seen) {
        return false;
    }
    is_white_win_ = white_seen;
    return true;
}

bool GameState::isBotWin() const {
    return is_white_turn_ == is_white_win_;
}

bool GameState::isEnemyPawn(Vector2D cell) const { // своя ли шашка
    auto i = cell.x;
    auto j = cell.y;
    return table_[i][j] != Empty && is_white_turn_ != isWhite(table_[i][j]);
}

bool GameState::isYourPawn(Vector2D cell) const { // чужая ли шашка
    auto i = cell.x;
    auto j = cell.y;
    return table_[i][j] != Empty && is_white_turn_ == isWhite(table_[i][j]);
}

bool GameState::isEmpty(Vector2D cell) const { // пустая ли клетка
    return table_[cell.x][cell.y] == Empty;
}

bool GameState::isLastLine(Vector2D cell) const { // дошли ли до последней линии
    return (cell.x == 0 && !is_white_turn_) || (cell.x == 8 && is_white_turn_);
}

void GameState::SetPawn(Vector2D cell) { // ставим пешку своего цвета
    table_[cell.x][cell.y] = is_white_turn_ ? WhitePawn : BlackPawn;
}

void GameState::SetKing(Vector2D cell) { // ставим дамку своего цвета
    table_[cell.x][cell.y] = is_white_turn_ ? WhiteKing : BlackKing;
}

void GameState::SetEmpty(Vector2D cell) { // очищаем клетку
    table_[cell.x][cell.y] = Empty;
}

void GameState::clearLine(Vector2D cell, Vector2D land) {
    Vector2D base_step = {1, 1};
    if (cell.x > land.x) base_step.x = -1;
    if (cell.y > land.y) base_step.y = -1;
    // очищаем часть диагонали
    for (auto base = cell; base != land; base += base_step) {
        SetEmpty(base);
    }
}

void GameState::clearMoves() {
    moves_history_.clear();
}

GameState GameState::makeMove(Vector2D cell, Vector2D land) const{
    // делает ход, но не проверяет что ход легальный

    bool is_king = isKing(table_[cell.x][cell.y]);
    auto new_table = table_;
    auto new_moves = moves_history_;
    if(new_moves.empty()) {
        new_moves.push_back(cell);
    }
    new_moves.push_back(land);
    auto new_state = GameState{is_white_turn_, std::move(new_table), std::move(new_moves)};

    //снимаем с доски
    new_state.clearLine(cell, land);
    // приземляемся
    if (is_king || isLastLine(land)) {
        new_state.SetKing(land);
    } else {
        new_state.SetPawn(land);
    }

    return new_state;
}

std::vector<GameState> GameState::getKills(Vector2D cell) const {
    // Пока можем бьем чужие шашки
    int32_t max_step = isKing(table_[cell.x][cell.y]) ? 7 : 1;

    std::vector<GameState> possible_states{};
    std::vector<Vector2D> basic_steps = {
            {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };
    for (auto base_step: basic_steps) {
        bool kills_by_move = false; //убили ли кого то за передвижение

        for (int32_t length = 1; length <= max_step; ++length) {
            auto to_kill = base_step * length + cell; // если пешка то должны убить
            auto land = to_kill + base_step; // куда приземляемся
            if (!onBoard(land)) {
                continue;
            }
            kills_by_move |= isEnemyPawn(to_kill); // правда ли что по пути убили кого-то
            if (!kills_by_move || !isEmpty(land)) {
                continue;
            }

            // делаем шаг
            auto new_state = makeMove(cell, land);

            // Если потом никого не смогли убить - то это ответ, иначе ответ - то что вернули
            auto result = new_state.getKills(land);
            if (result.empty()) {
                possible_states.emplace_back(std::move(new_state));
            } else {
                for (auto &res_state: result) {
                    possible_states.emplace_back(std::move(res_state));
                }
            }
        }
    }

    return possible_states;
}

std::vector<GameState> GameState::getMoves(Vector2D cell) const {
    std::vector<GameState> moves{};
    std::vector<Vector2D> steps{};
    std::vector<Vector2D> basic_steps = {
            {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };
    bool is_king = isKing(table_[cell.x][cell.y]);
    int32_t max_step = is_king ? 7 : 1;
    if (!is_king) {
        if (is_white_turn_) {
            steps = {{-1,  1},
                     {-1, -1}};
        } else {
            steps = {{1, -1},
                     {1,  1}};
        }
    } else {
        steps = basic_steps;
    }
    for (auto step: steps) {
        for (int32_t length = 1; length <= max_step; ++length) {
            auto land = cell + step * length;
            if (!onBoard(land) || !isEmpty(land)) {
                if (!onBoard(land)) {
                    length = max_step; // если вышел за доску то дальше проверять не надо
                }
                continue;
            }
            auto new_state = makeMove(cell, land);
            moves.emplace_back(std::move(new_state));
        }
    }
    return moves;
}

std::vector<GameState> GameState::getPossibleStatesFromCell(Vector2D cell) const{
    if (!isYourPawn(cell)) {
        return {};
    }

    auto kill_moves = getKills(cell);
    if (!kill_moves.empty()) {
        return kill_moves;
    }
    return getMoves(cell);
}

std::vector<GameState> GameState::getPossibleStates() const {
    std::vector<GameState> possible_states{};
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            auto result_from_cell = getPossibleStatesFromCell({i, j});
            for (auto& state: result_from_cell) {
                state.ChangePlayer();
                possible_states.emplace_back(std::move(state));
            }
        }
    }
    return possible_states;
}

void GameState::InitBoard() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 8; ++j) {
            if ((i + j) % 2 == 0) {
                table_[i][j] = BlackPawn;
            }
        }
    }

    for (int i = 5; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if ((i + j) % 2 == 0) {
                table_[i][j] = WhitePawn;
            }
        }
    }
}

void GameState::SetPlayer(bool is_bot_white) {
    is_white_turn_ = is_bot_white;
}

void GameState::PrintBoard() {
    for (int i = 0; i < 8; ++i) {
        std::cout << 8 - i << " ";
        for (int j = 0; j < 8; ++j) {
            switch (table_[i][j]) {
                case WhitePawn:
                    std::cout << "w "; break;
                case WhiteKing:
                    std::cout << "W "; break;
                case BlackPawn:
                    std::cout << "b "; break;
                case BlackKing:
                    std::cout << "B "; break;
                default:
                    std::cout << "_ "; break;
            }
        }
        std::cout << "\n";
    }
    std::cout << "  a b c d e f g h\n";
}

bool GameState::canKill(Vector2D cell) {
    return !getKills(cell).empty();
}

void GameState::ChangePlayer() {
    is_white_turn_ = !is_white_turn_;
}
