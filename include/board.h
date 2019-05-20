//============================================================================
// Author      : Franck Nassé - February 05, 2016
// Version     : v1.0
// Copyright   : Copyright (c) 2016, Franck Nassé. All rights reserved.
// Description : Model of the generic board.
//============================================================================
#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <cstdint>
#include <map>
#include <vector>

namespace UI
{
const int8_t  kPieceEmptyCell = -1;
const char kUnknownPieceID[] = "This Piece ID is unknown"
                               " by the board";
template <typename TypePiece>
/**
 * @class Board
 * @brief Generic in-memory representation of a multi-dimensional board
 * The cells are designed to handle a generic type of pieces (TypePiece).
 * A piece is identified by an integer ID. In the case of ConnectX we would use two different types one per each player.
 * This class is abstract and just defines a contract. It is possible to draw a board, clear it and put a piece 
 * at a given position.
 * It can be used for other games like Chess and checkers, etc.
 */
class Board {
 public:
  Board(const std::map<int8_t,TypePiece>& chips, 
        const std::vector<std::size_t>& dimensions)
      : chips_(chips), dimensions_(dimensions) {}
  virtual ~Board() = 0;
  
  const std::vector<std::size_t>& GetDimensions() const {
    return dimensions_;
  }
  virtual void ClearBoard() = 0;
  virtual void DrawBoard()  = 0;
  // Assigning a piece to a given position on the board using the piece ID
  virtual void SetChip(const std::vector<std::size_t>& position, 
                       int8_t piece_ID = kPieceEmptyCell) = 0;
  const TypePiece& GetPiece(int8_t piece_ID) {
    auto it = chips_.find(piece_ID);
    assert(it != chips_.end()); 
    return it->second;
  }

    protected:
  // associative container of piece type and its ID.
  std::map<int8_t,TypePiece> chips_;
  //Contains the size in each dimension
  std::vector<std::size_t> dimensions_;
};

template <typename TypePiece>
inline Board<TypePiece>::~Board() {}
}
#endif //GAME_BOARD_H