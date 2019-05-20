//============================================================================
// Author      : Franck Nassé - February 05, 2016
// Version     : v1.0
// Copyright   : Copyright (c) 2016, Franck Nassé. All rights reserved.
// Description :
//============================================================================
#include <cmath>
#include <queue>
#include "model_board.h"

namespace Core {

const std::array<int, ModelBoard::kNumPossibleMovements>
    ModelBoard::kAllMovements = {1, 0, -1};

ModelBoard::MoveType operator+(const ModelBoard::MoveType& move_left,
                               const ModelBoard::DirectionType& move_right) {
  assert(move_left.size() == move_right.size());
  ModelBoard::MoveType v_result(move_left.size());
  std::transform(move_left.begin(), move_left.end(), move_right.begin(),
                 v_result.begin(),
                 std::plus<ModelBoard::MoveType::value_type>());
  return v_result;
}

ModelBoard::MoveType& operator+=(ModelBoard::MoveType& move_left,
                                 const ModelBoard::DirectionType& move_right) {
  assert(move_left.size() == move_right.size());
  ModelBoard::MoveType v_result(move_left.size());
  std::transform(move_left.begin(), move_left.end(), move_right.begin(),
                 move_left.begin(),
                 std::plus<ModelBoard::MoveType::value_type>());
  return move_left;
}

ModelBoard::DirectionType operator*(const ModelBoard::DirectionType& move_left,
                                    const SSizeT factor) {
  ModelBoard::DirectionType v_result(move_left.size());
  std::transform(move_left.begin(), move_left.end(), v_result.begin(),
                 [factor](const SSizeT& x) { return x * factor; });
  return v_result;
}

// piece_IDs contains the ID of each piece and the order of the elements in
// the std::vector defines the player's turns.
// For example if piece_IDs contains (2, 1, 3) then the player who owns the
// piece numbered 2 will start the game and the next player will be the one
// who owns the piece numbered 1 and so on.
ModelBoard::ModelBoard(std::vector<PieceIDType> piece_IDs,
                       const std::vector<std::size_t>& dimensions,
                       uint8_t num_connected)
    : board_(dimensions)
    , num_win_connected_(num_connected)
    , piece_IDs_(piece_IDs)
    , current_state_(States::OnGoing)
    , current_chip_index_(0) {
  assert(piece_IDs.size() > kDefaultNumChips);
  assert(dimensions.size() > kMinNumDimensions);
  assert(std::find(piece_IDs_.begin(), piece_IDs_.end(), kEmptyPosition) ==
         piece_IDs_.end());
  std::sort(piece_IDs.begin(), piece_IDs.end());
  assert(std::unique(piece_IDs.begin(), piece_IDs.end()) == piece_IDs.end());
  assert(num_connected > kDefaultMinNumConnected);
  board_.Fill(kEmptyPosition);
  GeneratePossibleInitialMoves();
  DirectionType direction(dimensions.size(), 0);
  std::size_t num_directions_limit =
      (std::pow(kNumPossibleMovements, direction.size()) - 1) / 2;
  GenerateDirections(num_directions_limit, direction, 0, dimensions.size());
}

void ModelBoard::SetMove(const MoveType& move) {
  assert(current_state_ == States::OnGoing);
  assert(move.size() == board_.GetDimensions().size());
  assert(IsItSafeToMove(move));
  CoMoveType move_up(move.begin() + 1, move.end());
  assert(possible_moves_.find(move_up) != possible_moves_.end());
  assert(possible_moves_[move_up] == *move.begin());
  SetMoveInc(move);
}

void ModelBoard::SetMoveInc(const MoveType& move) {
  board_[move] = GetNextChipId();
  history_moves_.push(move);
  if(CheckConnected(move)) {
    current_state_ = States::Win;
  } else if(history_moves_.size() == board_.capacity()) {
    current_state_ = States::Draw;
  } else {
    current_state_ = States::OnGoing;
  }
  UpdatePossibleMoves(move);
  current_chip_index_ = (current_chip_index_ + 1) % piece_IDs_.size();
}

void ModelBoard::GetPossibleMoves(std::vector<MoveType>& moves) {
  moves.clear();
  if(States::OnGoing == GetCurrentState()) {
    MoveType temp(board_.GetDimensions());
    int i = 0, m = 0, n = possible_moves_.size(), zm = n / 2 - 1;
    for(auto it : possible_moves_) {  // std::map<CoMoveType,std::size_t>
      temp[0] = it.second;
      std::copy(it.first.begin(), it.first.end(), temp.begin() + 1);
      moves.push_back(temp);
      m++;
      if(m >= zm && (m > -1) && m < n && i < n) {
        std::swap(moves[m - 1], moves[i]);
        i++;
      }
    }
  }
}

void ModelBoard::Undo() {
  if(history_moves_.empty()) {
    return;
  }
  board_[history_moves_.top()] = kEmptyPosition;
  UpdatePossibleMoves(history_moves_.top(), true);
  history_moves_.pop();
  current_chip_index_ = GetIndexCurrentChip();
  winning_moves_.clear();
  current_state_ = States::OnGoing;
}

/**
 * @brief Clears board
 * Clears moves history.
 * The Game is reset to its original state.
 */
void ModelBoard::Reset() {
  board_.Fill(kEmptyPosition);
  current_chip_index_ = 0;
  current_state_ = States::OnGoing;
  winning_moves_.clear();
  ClearMoveHistory();
  GeneratePossibleInitialMoves();
}

ModelBoard::PieceIDType ModelBoard::GetCurrentChipId() const {
  return piece_IDs_[GetIndexCurrentChip()];
}

bool ModelBoard::IsItSafeToMove(const MoveType& move) const {
  std::size_t dim_index = 0;
  auto check_dim_lb = [&dim_index, this](const std::size_t& x) {
    return x >= board_.GetDimensionSize(dim_index++);
  };
  auto it = std::find_if(move.begin(), move.end(), check_dim_lb);
  return it == move.end();
}

bool ModelBoard::IsItSafeToMove(const MoveType& move,
                                const DirectionType& directions) const {
  for(std::size_t j = 0; j < move.size(); j++) {
    SSizeT xtemp = move[j] + directions[j];
    if(xtemp >= static_cast<SSizeT>(board_.GetDimensionSize(j)) || xtemp < 0) {
      return false;
    }
  }
  return true;
}

bool ModelBoard::ExploreMove(const DirectionType& direction,
                             MoveType move,
                             int8_t& counter,
                             PieceIDType lookup_chip) {
  while(counter < num_win_connected_) {
    if(IsItSafeToMove(move, direction)) {
      move = move + direction;
      if(board_[move] == lookup_chip) {
        counter++;
        winning_moves_.push_back(move);
        continue;
      }
    }
    break;
  }
  return counter == num_win_connected_;
}

bool ModelBoard::CheckConnected(const MoveType& move) {
  DirectionType vect_dir;
  for(DirectionType& cur_dir : directions_) {
    winning_moves_.clear();
    int8_t num_connected_chips = 1;
    if(ExploreMove(cur_dir, move, num_connected_chips, board_[move])) {
      winning_moves_.push_back(move);
      break;
    }
    vect_dir = cur_dir * -1;
    if(ExploreMove(vect_dir, move, num_connected_chips, board_[move])) {
      winning_moves_.push_back(move);
      break;
    }
  }
  return winning_moves_.size() >= num_win_connected_;
}

bool ModelBoard::GenerateDirections(std::size_t num_directions_limit,
                                    DirectionType& current_direction,
                                    std::size_t pos,
                                    std::size_t depth) {
  bool stop = false;
  if(directions_.size() >= num_directions_limit) {
    stop = true;
  } else if(depth == 0) {
    directions_.push_back(current_direction);
    stop = false;
  } else {
    for(auto cur_increment : kAllMovements) {
      current_direction[pos] = cur_increment;
      if(GenerateDirections(num_directions_limit, current_direction, pos + 1,
                            depth - 1)) {
        stop = true;
        break;
      }
    }
  }
  return stop;
}

struct NodePossibleMove {
  ModelBoard::CoMoveType possible_move_;
  std::size_t current_dim_;
};

void ModelBoard::GeneratePossibleInitialMoves() {
  std::queue<NodePossibleMove> nodes;
  nodes.emplace(NodePossibleMove({{}, 0}));
  NodePossibleMove curNode, childNode;
  possible_moves_.clear();

  while(!nodes.empty()) {
    curNode = nodes.front();
    if(curNode.current_dim_ == board_.GetDimensions().size() - 1) {
      possible_moves_.insert(std::make_pair(curNode.possible_move_,
                                            board_.GetDimensionSize(0) - 1));
    } else {
      for(std::size_t i = 0;
          i < board_.GetDimensionSize(curNode.current_dim_ + 1); i++) {
        childNode.possible_move_ = curNode.possible_move_;
        childNode.possible_move_.push_back(i);
        childNode.current_dim_ = curNode.current_dim_ + 1;
        nodes.push(childNode);
      }
    }
    nodes.pop();
  }
}

void ModelBoard::UpdatePossibleMoves(const MoveType& move, bool undo) {
  CoMoveType move_up(move.begin() + 1, move.end());
  auto it = possible_moves_.find(move_up);
  if(!undo) {
    assert(it != possible_moves_.end());
    if(static_cast<SSizeT>(possible_moves_[move_up]) - 1 >= 0) {
      possible_moves_[move_up]--;
    } else {
      possible_moves_.erase(it);
    }
  } else {
    if(it == possible_moves_.end()) {
      possible_moves_[move_up] = 0;
    } else if(possible_moves_[move_up] + 1 < board_.GetDimensionSize(0)) {
      possible_moves_[move_up]++;
    } else {
      possible_moves_.erase(it);
    }
  }
}

void ModelBoard::ClearMoveHistory() {
  while(!history_moves_.empty()) {
    history_moves_.pop();
  }
}

}  // namespace Core