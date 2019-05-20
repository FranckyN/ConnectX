//============================================================================
// Author      : Franck Nassé - February 05, 2016
// Version     : v1.0
// Copyright   : Copyright (c) 2016, Franck Nassé. All rights reserved.
// Description : TextBoard
//============================================================================
#ifndef TEXT_BOARD_H_
#define TEXT_BOARD_H_

#include "text_piece.h"
#include <map>
#include <cassert>
#include "board.h"
#include <sstream>
#include "board.h"

namespace UI {
const char k2DOnly[]         = "Only 2D board is supported";
const char kWithCellSymbol[] = "Width of symbol must be equal to"
                               " width of cell."; 
/**
 * @class Board
 * @brief Concrete representation of a board for the console. This class derives from a specialization of the Board 
 * class and uses the 'TextPiece' type. This class is used to draw the board on the screen using basic symbols and ASCII 
 * characters.
 */
class TextBoard : public Board<TextPiece> {
 public:
  TextBoard(const std::map<int8_t,TextPiece>& chips, 
            const std::vector<std::size_t>& dimensions,
            const std::string& line_indent);
  ~TextBoard() {}

  virtual void ClearBoard();
  virtual void Clear();
  virtual void DrawBoard();
  virtual void SetChip(const std::vector<std::size_t>& position, 
                       int8_t piece_ID = kPieceEmptyCell);
        void PrepareCursor();
  std::size_t GetNumCharLine() {
    return line_length_ - line_indent_.size();
  }

    protected:
  // Below are the characters used to draw a board on the console
  static const char kVerticalLine = '|';
  static const char kUnderScore   = '_';
  static const char kEmptyChar    = ' ';
  static const char kDotChar      = '.';
  static const char kDashChar     = '-';
  static const char kNewLineChar  = '\n';
  static const int kMaxDimensions = 2;
  static const int kfirstAxis     = 0;
  static const int kSecondAxis    = 1;
  static const int kTopMargin     = 2;


  virtual void ClearBoardStream();
  virtual void PrintLine(std::size_t cline);
          void PrintEmptyLine(std::size_t lineCount = 1);
          void WriteToCell(const std::string& str);
  virtual void SetChipInc(const std::vector<std::size_t>& position, 
                          int8_t piece_ID = kPieceEmptyCell);
      
  std::stringstream board_stream_;
  std::string previous_;
  std::string empty_cell_string_ ;
  std::string lower_cell_string_ ;
  std::size_t h_cell_size_;
  std::size_t l_cell_size_;
  std::size_t line_length_;
  std::string line_indent_;
  std::pair<int, int> cursor_;
};
}
#endif //TEXT_BOARD_H_
