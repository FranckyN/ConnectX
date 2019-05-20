//============================================================================
// Author      : Franck Nassé - February 05, 2016
// Version     : v1.0
// Copyright   : Copyright (c) 2016, Franck Nassé. All rights reserved.
// Description : 
//============================================================================
#ifndef CONNECT4_INTELLIGENCE_BOARD_H_
#define CONNECT4_INTELLIGENCE_BOARD_H_

#include "model_board.h"
#include <list>
#include <map>
#include <vector>
#include <chrono>

namespace Core {

typedef int8_t DepthType;
const int8_t kLevelExternalPlayer = -1;

/**
 * @class IntelligentBoard
 * @file intelligent_board.h
 * @brief This class inherits from ModelBoard and add some new simulation features.
 * It computes the next move based on the state of the current board and the rules.
 * All the functionalities related to the game itself are implemented in the base class,
 * this derived class deals with the evaluation of the game at any given point.
 * A twisted version of the minimax algorithm with alpha-beta pruning is implemented.
 */
class IntelligentBoard : public ModelBoard {
    public:
    /**
     * @class nested Party
     * @brief This class represent a player, whether or not it is human or artifical.
     * it contains the thinking depth and a piece ID
     */
  struct Party {
    DepthType depth_ = kLevelExternalPlayer;
    PieceIDType chip_id_;
    Party(PieceIDType piece_ID) : chip_id_(piece_ID) {}
    Party(PieceIDType piece_ID, DepthType depth) : depth_(depth), 
        chip_id_(piece_ID) {}
    operator PieceIDType(void) const { return chip_id_; }
  };

  IntelligentBoard(const std::vector<Party>& parties,
      const std::vector<std::size_t>& dimensions, 
      uint8_t num_connected = kDefaultConnectedFour);
  
  void SetAIDepth(PieceIDType piece_ID, DepthType depth);
  int8_t GetAIDepth(PieceIDType piece_ID) const;
  void Reset();
  // returns the next move as predicted by the algorithm
  void GetAIPlayerMove(MoveType& ai_move, 
      const std::chrono::milliseconds& thinking_time);
  std::size_t GetNumEvaluations() const { return num_evaluations_; }
  std::chrono::milliseconds 
      GetActualThinkingTime() const { return thinking_time_; }
  void Undo();
  int EvaluateGrid(PieceIDType maximizing_chip);
  
 protected:
  struct EvaluationResult {
    int best_candidate_;
    int score_;
  };
  
  const char kEmptyPlace    = '0';
  const char kOccupiedPlace = '1';
  const int8_t kVisited     = 1;
  const int8_t kUnVisited   = 0;
  const int kPlayerWon      = std::numeric_limits<int>::max();
  const int kPlayerMaxScore =  1000;
  const int kPlayerMidScore =  kPlayerMaxScore/2;
  
  EvaluationResult FindMove(IntelligentBoard & grid_candidate, 
      DepthType depth, int alpha, int beta, PieceIDType maximizing_chip);
  EvaluationResult IterativeDeepening(IntelligentBoard & grid_candidate, 
     DepthType depth, 
     const std::chrono::milliseconds& thinking_time);
  int EvalPieceType(const std::list<MoveType>& moves, 
      PieceIDType chip_evaluated);
  int ScorePatterns(MultiDimArray<PieceIDType>& visiting_grid,
      const MoveType& current_position, 
      const DirectionType& direction,
      PieceIDType chip_evaluated);
  int ExploreConnectedPieces(MultiDimArray<PieceIDType>& visiting_grid,
      const MoveType& current_position, 
      const DirectionType& direction,
      PieceIDType chip_evaluated,
      std::string& current_string);
  void InitPatterns();
  int GetMaxCandidate(const std::vector<MoveType>&);
  bool CheckPattern(const std::string& curr_pattern);
  void SetMoveInc(const MoveType& move);
  
  std::map<PieceIDType,int8_t> intelligent_pieces_;
  std::map<PieceIDType, std::list<MoveType> > piece_positions_;
  std::list<std::string> patterns_;
  std::size_t num_evaluations_;
  std::chrono::milliseconds thinking_time_;
  int prev_best_move_;
};



}
#endif //CONNECT4_INTELLIGENCE_BOARD_H_