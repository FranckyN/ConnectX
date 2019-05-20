//============================================================================
// Author      : Franck Nassé - February 05, 2016
// Version     : v1.0
// Copyright   : Copyright (c) 2016, Franck Nassé. All rights reserved.
// Description :
//============================================================================
#include "intelligent_board.h"
#include <random>

namespace Core {

IntelligentBoard::IntelligentBoard(const std::vector<Party>& parties,
                                   const std::vector<std::size_t>& dimensions,
                                   uint8_t num_connected)
    : ModelBoard(std::vector<PieceIDType>(parties.begin(), parties.end()),
                 dimensions,
                 num_connected) {
  for(auto it = parties.begin(); it != parties.end(); it++) {
    if(it->depth_ > 0) {
      intelligent_pieces_.insert(std::make_pair(it->chip_id_, it->depth_));
    }
  }
  std::for_each(piece_IDs_.begin(), piece_IDs_.end(),
                [this](const PieceIDType& chip) { piece_positions_[chip]; });
  InitPatterns();
  thinking_time_ = std::chrono::milliseconds(0);
}

void IntelligentBoard::SetAIDepth(PieceIDType piece_ID, int8_t depth) {
  assert(std::find(piece_IDs_.begin(), piece_IDs_.end(), piece_ID) !=
         piece_IDs_.end());
  if(depth > 0) {
    intelligent_pieces_[piece_ID] = depth;
  } else {
    auto m_it = intelligent_pieces_.find(piece_ID);
    if(m_it != intelligent_pieces_.end()) {
      intelligent_pieces_.erase(m_it);
    }
  }
}

int8_t IntelligentBoard::GetAIDepth(PieceIDType piece_ID) const {
  assert(std::find(piece_IDs_.begin(), piece_IDs_.end(), piece_ID) !=
         piece_IDs_.end());
  auto m_it = intelligent_pieces_.find(piece_ID);
  if(m_it != intelligent_pieces_.end()) {
    return m_it->second;
  } else {
    return kLevelExternalPlayer;
  }
}

void IntelligentBoard::SetMoveInc(const MoveType& move) {
  ModelBoard::SetMoveInc(move);
  piece_positions_[GetCurrentChipId()].push_back(move);
}

void IntelligentBoard::GetAIPlayerMove(
    MoveType& ai_move,
    const std::chrono::milliseconds& thinking_time) {
  DepthType max_depth = intelligent_pieces_[GetNextChipId()];
  assert(max_depth > 0);  // Is it the turn of an AI player?
  std::vector<MoveType> candidates;
  GetPossibleMoves(candidates);
  IntelligentBoard grid_candidate(*this);
  std::vector<MoveType> movess;
  grid_candidate.GetPossibleMoves(movess);
  EvaluationResult solution;
  int maxCandidate = 0;
  num_evaluations_ = 0;
  prev_best_move_ = -1;
  auto start = std::chrono::steady_clock::now();
  if(GetHistoryCount() < 2 and max_depth < 5) {
    maxCandidate = GetMaxCandidate(candidates);
  } else {
    solution = IterativeDeepening(grid_candidate, max_depth, thinking_time);
    auto end = std::chrono::steady_clock::now();
    thinking_time_ =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    if(solution.best_candidate_ >= 0) {
      maxCandidate = solution.best_candidate_;
    }
  }
  ai_move = candidates[maxCandidate];
}

IntelligentBoard::EvaluationResult IntelligentBoard::IterativeDeepening(
    IntelligentBoard& grid_candidate,
    DepthType depth,
    const std::chrono::milliseconds& thinking_time) {
  DepthType l_depth = 0;
  EvaluationResult solution;
  prev_best_move_ = -1;
  auto start = std::chrono::steady_clock::now();
  for(; l_depth < depth; ++l_depth) {
    solution = FindMove(grid_candidate, l_depth + 1, -kPlayerWon, kPlayerWon,
                        GetNextChipId());

    if(l_depth > 0) {
      if(solution.best_candidate_ == 0) {
        solution.best_candidate_ = prev_best_move_;
      } else if(solution.best_candidate_ == prev_best_move_) {
        solution.best_candidate_ = 0;
      }
    }
    auto end = std::chrono::steady_clock::now();
    if(std::chrono::duration_cast<std::chrono::milliseconds>(end - start) >=
       thinking_time) {
      break;
    }
    prev_best_move_ = solution.best_candidate_;
  }

  return solution;
}

IntelligentBoard::EvaluationResult IntelligentBoard::FindMove(
    IntelligentBoard& grid_candidate,
    DepthType depth,
    int alpha,
    int beta,
    PieceIDType maximizing_chip) {
  int best_score, best_candidate = -1, score, last_move_index;
  if(grid_candidate.GetCurrentState() == States::Win) {
    score = kPlayerWon;
    if(grid_candidate.GetCurrentChipId() != maximizing_chip) {
      score = -score;
    }
    return {-1, score};
  } else if(grid_candidate.GetCurrentState() == States::Draw) {
    return {-1, 0};
  }

  if(depth == 0) {
    score = grid_candidate.EvaluateGrid(maximizing_chip);
    num_evaluations_++;
    return {-1, score};
  }

  if(grid_candidate.GetNextChipId() == maximizing_chip) {
    best_score = -kPlayerWon;
    std::vector<MoveType> moves;
    grid_candidate.GetPossibleMoves(moves);
    last_move_index = prev_best_move_;
    if(prev_best_move_ > -1) {
      std::swap(moves[0], moves[prev_best_move_]);
      prev_best_move_ = -1;
    }
    for(std::size_t candID = 0; candID < moves.size(); candID++) {
      grid_candidate.SetMoveInc(moves[candID]);
      score = FindMove(grid_candidate, depth - 1, alpha, beta, maximizing_chip)
                  .score_;
      grid_candidate.Undo();
      if(score > best_score) {
        best_score = score;
        best_candidate = candID;
        alpha = std::max(alpha, best_score);
        if(best_score == kPlayerWon) {
          break;
        }
      }
      if(beta <= alpha) {
        break;
      }
    }
    prev_best_move_ = last_move_index;
    return {best_candidate, best_score};
  } else {
    best_score = kPlayerWon;
    std::vector<MoveType> moves;
    grid_candidate.GetPossibleMoves(moves);
    for(std::size_t candID = 0; candID < moves.size(); candID++) {
      grid_candidate.SetMoveInc(moves[candID]);
      score = FindMove(grid_candidate, depth - 1, alpha, beta, maximizing_chip)
                  .score_;
      grid_candidate.Undo();
      if(score < best_score) {
        best_score = score;
        best_candidate = candID;
        beta = std::min(beta, best_score);
        if(best_score == -kPlayerWon) {
          break;
        }
      }
      if(beta <= alpha) {
        break;
      }
    }
    return {best_candidate, best_score};
  }
  return {1, 1};
}

void IntelligentBoard::Reset() {
  ModelBoard::Reset();
  piece_positions_.clear();
}

void IntelligentBoard::Undo() {
  ModelBoard::Undo();
  auto it = piece_positions_.find(GetNextChipId());
  if(piece_positions_.end() != it) {
    it->second.pop_back();
  }
}

int IntelligentBoard::EvaluateGrid(PieceIDType maximizing_chip) {
  int final_score =
      EvalPieceType(piece_positions_[maximizing_chip], maximizing_chip);
  auto mit = piece_positions_.begin();
  int score_opponent;
  while(mit != piece_positions_.end()) {
    if(mit->first != maximizing_chip) {
      score_opponent = EvalPieceType(mit->second, mit->first);
      final_score -= score_opponent;
    }
    mit++;
  }
  return final_score;
}

int IntelligentBoard::EvalPieceType(const std::list<MoveType>& moves,
                                    PieceIDType chip_evaluated) {
  const std::list<DirectionType>& ref_directions = GetDirections();
  auto it_dir = ref_directions.begin();
  std::list<MoveType>::const_iterator it_pos;
  MultiDimArray<PieceIDType> visiting_grid(board_.GetDimensions());
  int score = 0;
  while(it_dir != ref_directions.end()) {
    visiting_grid.Fill(kUnVisited);
    it_pos = moves.begin();
    while(it_pos != moves.end()) {
      if(visiting_grid[*it_pos] == kUnVisited) {
        score += ScorePatterns(visiting_grid, *it_pos, *it_dir, chip_evaluated);
      }
      ++it_pos;
    }
    it_dir++;
  }
  return score;
}

int IntelligentBoard::ScorePatterns(MultiDimArray<PieceIDType>& visiting_grid,
                                    const MoveType& current_position,
                                    const DirectionType& direction,
                                    PieceIDType chip_evaluated) {
  std::string connect_string;
  int num_pieces(1);
  num_pieces +=
      ExploreConnectedPieces(visiting_grid, current_position, direction,
                             chip_evaluated, connect_string);
  DirectionType reverse_dir = direction * -1;
  std::reverse(connect_string.begin(), connect_string.end());
  connect_string += kOccupiedPlace;
  num_pieces +=
      ExploreConnectedPieces(visiting_grid, current_position, reverse_dir,
                             chip_evaluated, connect_string);
  int score = num_pieces;
  if(connect_string.size() >= num_win_connected_) {
    if(num_pieces >= num_win_connected_ - 1) {
      if(CheckPattern(connect_string)) {
        score = kPlayerMaxScore;
      } else if(GetNextChipId() == chip_evaluated) {
        score = kPlayerMidScore;
      } else {
        score *= num_win_connected_;
      }
    } else {
      score *= num_win_connected_;
    }
  }
  return score;
}

int Core::IntelligentBoard::ExploreConnectedPieces(
    MultiDimArray<PieceIDType>& visiting_grid,
    const MoveType& current_position,
    const DirectionType& direction,
    PieceIDType chip_evaluated,
    std::string& current_string) {
  MoveType cur_pos = current_position;
  int counter = 0, num_empty = 0;
  while(IsItSafeToMove(cur_pos, direction)) {
    cur_pos += direction;
    if(board_[cur_pos] == chip_evaluated) {
      visiting_grid[cur_pos] = kVisited;
      counter++;
      num_empty = 0;
      current_string += kOccupiedPlace;
    } else if(board_[cur_pos] == kEmptyPosition &&
              num_empty < num_win_connected_ - 1) {
      num_empty++;
      current_string += kEmptyPlace;
    } else {
      break;
    }
  }
  return counter;
}

bool IntelligentBoard::CheckPattern(const std::string& curr_pattern) {
  auto it = patterns_.begin();
  while(it != patterns_.end()) {
    if(curr_pattern.size() >= (*it).size() &&
       curr_pattern.find(*it) != std::string::npos) {
      return true;
    }
    it++;
  }
  return false;
}

int IntelligentBoard::GetMaxCandidate(const std::vector<MoveType>& candidates) {
  static std::random_device rd;
  static std::mt19937 mt(rd());
  static std::uniform_int_distribution<int> distribution(0, candidates.size());
  return distribution(mt);
}

void Core::IntelligentBoard::InitPatterns() {
  std::string temp;
  std::string range_1(num_win_connected_ - 1, kOccupiedPlace);
  std::string range_2(num_win_connected_ - 2, kOccupiedPlace);

  temp += kEmptyPlace;
  temp += range_1;
  temp += kEmptyPlace;
  patterns_.push_back(temp);

  temp = kOccupiedPlace;
  temp += kEmptyPlace;
  temp += range_2;
  temp += kEmptyPlace;
  temp += kOccupiedPlace;
  patterns_.push_back(temp);

  temp = range_2;
  temp += kEmptyPlace;
  temp += kOccupiedPlace;
  temp += kEmptyPlace;
  temp += range_2;
  patterns_.push_back(temp);
}

}  // namespace Core
