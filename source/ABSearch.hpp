#ifndef CHECKERS_ABSEARCH_HPP
#define CHECKERS_ABSEARCH_HPP

#include "Table.hpp"
#include <chrono>


class ABSearch {
private:
    const int32_t INF = 1e9;
    bool is_bot_white_ = true;
    int32_t depth_{};
    std::chrono::time_point<std::chrono::high_resolution_clock> start_search_time_{};
public:
    const int64_t MAX_TIME = 10; // в секундах на итерацию

private:
    int32_t maxValue(const GameState& state, int32_t alpha, int32_t beta, int32_t depth);
    int32_t minValue(const GameState& state, int32_t alpha, int32_t beta, int32_t depth);

    int32_t alphaBetaSearch(const GameState& state, int32_t alpha, int32_t beta, int32_t depth);
    static int32_t evaluateFunction(const GameState& state);

public:
    ABSearch() = default;

    void SetPlayer(bool is_bot_white);
    void SetDepth(int32_t difficulty);

    std::vector<Vector2D> FindMove(const GameState& state);
};

#endif //CHECKERS_ABSEARCH_HPP
