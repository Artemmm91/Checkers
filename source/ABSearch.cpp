#pragma once
#include "ABSeach.hpp"

int32_t ABSearch::evaluateFunction(const GameState& state) {
    int32_t white_score = 0;
    int32_t black_score = 0;
    const int32_t pawn_score = 1;
    const int32_t king_score = 2;

    for (auto& row: state.table_) {
        for (auto cell: row) {
            switch (cell) {
                case WhitePawn:
                    white_score += pawn_score; break;
                case WhiteKing:
                    white_score += king_score; break;
                case BlackPawn:
                    black_score += pawn_score; break;
                case BlackKing:
                    black_score += king_score; break;
                default:
                    break;
            }

        }
    }
    if (state.is_white_turn_) {
        return white_score - black_score;
    }
    return black_score - white_score;
}

int32_t ABSearch::maxValue(const GameState& state, int32_t alpha, int32_t beta, int32_t depth) {
    auto val = -INF;
    for (auto& successor: state.getPossibleStates()) {
        val = std::max(val, alphaBetaSearch(successor, alpha, beta, depth));
        if (val >= beta) {
            return val;
        }
        alpha = std::max(alpha, val);
    }
    return val;
}

int32_t ABSearch::minValue(const GameState& state, int32_t alpha, int32_t beta, int32_t depth) {
    auto val = INF;
    for (auto& successor: state.getPossibleStates()) {
        val = std::min(val, alphaBetaSearch(successor, alpha, beta, depth - 1));
        if (val <= alpha) {
            return val;
        }
        beta = std::min(beta, val);
    }
    return val;
}


int32_t ABSearch::alphaBetaSearch(const GameState& state, int32_t alpha, int32_t beta, int32_t depth) {
    if (state.isEnd()) {
        if (state.isBotWin()) {
            return INF;
        } else {
            return -INF;
        }
    }
    if (depth <= 0 or elapsed_time(start_search_time_) > MAX_TIME) {
        return evaluateFunction(state);
    }

    if (state.is_white_turn_ == is_bot_white_) {
        return maxValue(state, alpha, beta, depth);
    }
    return minValue(state, alpha, beta, depth);
}

std::vector<Vector2D> ABSearch::FindMove(const GameState& state) {
    start_search_time_ = std::chrono::high_resolution_clock::now();

    std::vector<Vector2D> best_move{};

    if (elapsed_time(start_search_time_) > MAX_TIME) {
        return best_move;
    }
    auto val = -INF;
    for (auto& successor: state.getPossibleStates()) {
        auto score = alphaBetaSearch(successor, -INF, INF, depth_);
        if (score > val) {
            val = score;
            best_move = successor.moves_history_;
        }
    }
    return best_move;
}

void ABSearch::SetPlayer(bool is_bot_white) {
    is_bot_white_ = is_bot_white;
}

void ABSearch::SetDepth(int32_t difficulty) {
    depth_ = difficulty;
}
