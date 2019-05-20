//============================================================================
// Author      : Franck Nassé - February 05, 2016
// Version     : v1.0
// Copyright   : Copyright (c) 2016, Franck Nassé. All rights reserved.
// Description : The ModelBoard class represents the connect4 game on the board.
//============================================================================
#ifndef CONNECT4_MODEL_BOARD_H_
#define CONNECT4_MODEL_BOARD_H_

#include <list>
#include <stack>
#include <cstdint>
#include <cassert>
#include <map>
#include <iterator>
#include <array>
#include "multi_dim_array.h"

namespace Core {
enum class States {
  Draw,
  OnGoing,
  Win
};
typedef long long                  SSizeT;
const int8_t kEmptyPosition           = 0;
const uint8_t kDefaultConnectedFour   = 4;
const uint8_t kDefaultMinNumConnected = 2;
const uint8_t kDefaultNumChips        = 1;
const uint8_t kMinNumDimensions       = 1;

/* 
 * This class represents the board game and defines the rules of 
 * Connect Four.
 *
 *  
 * The class can support multiple dimensions (3D, 4D, etc.),
 * multiple players (1, 2, 3, 4, ...) and different winning conditions 
 * (number of chips of the same kind to be connected).
 *
 * 2D Coordinate System:
 * In the grid below, the chip X located in the second column and third row has
 * the following coordinates:(2, 1)
 *   
 *   |_0_|_1_|_2_|_3_|_4_|_5_|_6_|
 * 0 |___|___|___|___|___|___|___|
 * 1 |___|___|___|___|___|___|___|
 * 2 |___|_X_|___|___|___|___|___|
 * 3 |___|___|___|___|___|___|___|
 * 4 |___|___|___|___|___|___|___|
 * 5 |___|___|___|___|___|___|___|
 *
 */
class ModelBoard {
 public:
  typedef std::vector<std::size_t> MoveType;
  typedef std::vector<std::size_t> CoMoveType;
  typedef std::vector<SSizeT>      DirectionType;
  typedef int8_t                   PieceIDType;

  /**
   * @brief Constructor of the class. It gets all that is needed to simulate a game.
   * @param piece_IDs: The different IDs of the pieces of the different players
   * @param dimensions: Dimensions of the board
   * @param num_connected: the number of pieces to connect to win the game
   */
  ModelBoard(std::vector<PieceIDType> piece_IDs,
      const std::vector<std::size_t>& dimensions, 
      uint8_t num_connected = kDefaultConnectedFour);
  
  // Simulate a move on the board
  virtual void SetMove(const MoveType& move);
  // returns all the current possible moves
          void GetPossibleMoves(std::vector<MoveType>& moves);
  virtual void Undo();
  // resets the game
  virtual void Reset();
  PieceIDType  GetCurrentChipId() const;
  PieceIDType  GetNextChipId() const { return piece_IDs_[current_chip_index_]; }
  States       GetCurrentState() const { return current_state_; }
  
  // Returns the last move
  MoveType     GetLastMove() {
    assert(!history_moves_.empty());
    return history_moves_.top();
  }
  
  // Returns the board's dimension
  const std::vector<std::size_t>& GetDimensions() const {
    return board_.GetDimensions();
  }
  
  /**
   * @brief Returns the winning move as a list of positions on the board
   * @param positions
   */
  void GetWinningPositions(std::list<MoveType>& positions) const {
    positions = winning_moves_;
  }
  
  // returns the number of moves so far in the current game
  std::size_t GetHistoryCount() { return history_moves_.size(); }
  
 protected:
  static const std::size_t kNumPossibleMovements = 3;
  static const std::array<int, kNumPossibleMovements> kAllMovements;
  
  bool IsItSafeToMove(const MoveType& move) const;
  bool IsItSafeToMove(const MoveType& move, 
                      const DirectionType& direction) const;
  virtual void SetMoveInc(const MoveType& move);
  std::list<DirectionType> GetDirections() {
    return directions_;
  }
  
  MultiDimArray<PieceIDType> board_;
  const uint8_t num_win_connected_;
  std::map<CoMoveType,std::size_t> possible_moves_;
  std::vector<PieceIDType> piece_IDs_;

 private:
  bool ExploreMove(const DirectionType& direction, MoveType move, 
                   int8_t& counter, PieceIDType lookup_chip);
  bool CheckConnected(const MoveType& move);
  bool GenerateDirections(std::size_t num_directions_limit, 
                          DirectionType& current_direction, 
                          std::size_t pos, std::size_t depth);
  void GeneratePossibleInitialMoves();
  void UpdatePossibleMoves(const MoveType& move, bool undo = false);
  void ClearMoveHistory();
  
  std::size_t GetIndexCurrentChip() const {
    return current_chip_index_ == 0 ? 
        piece_IDs_.size() - 1 :  current_chip_index_ - 1;
  }
  
  States current_state_;
  std::size_t current_chip_index_;
  std::stack<MoveType> history_moves_;
  std::list<MoveType>  winning_moves_;
  std::list<DirectionType> directions_;
};

ModelBoard::MoveType operator+(const ModelBoard::MoveType& move_left, 
                               const ModelBoard::DirectionType& move_right);
ModelBoard::MoveType& operator+=(ModelBoard::MoveType& move_left, 
                                 const ModelBoard::DirectionType& move_right);
ModelBoard::DirectionType operator*(const ModelBoard::DirectionType& move_left, 
                                    const SSizeT factor);
}
#endif  //CONNECT4_MODEL_BOARD_H_