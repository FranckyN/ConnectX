//============================================================================
// Author  : Franck Nassé - February 05, 2016
// Version   : v1.0
// Copyright   : Copyright (c) 2016, Franck Nassé. All rights reserved.
// Description :
//============================================================================
#include <algorithm>
#include <iostream>
#include <iterator>
#include "cursor_console.h"
#include "text_board.h"

namespace UI {

// constructor
TextBoard::TextBoard(const std::map<int8_t, TextPiece>& chips,
                     const std::vector<std::size_t>& dimensions,
                     const std::string& line_indent)
    : Board<TextPiece>(chips, dimensions)
    , line_length_(0)
    , line_indent_(line_indent) {
  assert(dimensions_.size() == kMaxDimensions);
  assert(!chips.empty());
  h_cell_size_ = chips_.begin()->second.GetSymbol().size() + 1;
  l_cell_size_ = chips_.begin()->second.GetSymbol()[0].size();
  empty_cell_string_.resize(l_cell_size_, kEmptyChar);
  lower_cell_string_.resize(l_cell_size_, kUnderScore);
  Clear();
}

// Prints the board on the console.
void TextBoard::DrawBoard() {
  if(previous_.empty()) {
    PrepareCursor();
    UI::WriteToWindow(board_stream_);
    previous_ = board_stream_.str();
    UI::RefreshWindow();
    return;
  }
  std::string current = board_stream_.str();
  if(cursor_.first < std::numeric_limits<int>::max()) {
    std::size_t x = cursor_.first;
    std::size_t y = cursor_.second;
    for(std::size_t i = 0; i < previous_.size(); i++) {
      if(previous_[i] == current[i] && current[i] != kNewLineChar) {
        x++;
        continue;
      }
      UI::SetCursorPosition(x, y);
      if(current[i] == kNewLineChar) {
        y++;
        x = cursor_.first;
      } else {
        x++;
      }
      UI::WriteToWindow(current[i]);
    }
  }
  previous_ = board_stream_.str();
  UI::RefreshWindow();
}

// Reset
void TextBoard::Clear() {
  ClearBoard();
  cursor_.first = cursor_.second = std::numeric_limits<int>::max();
  previous_.clear();
}

// Clears the board from the console and display an empty board with no pieces.
void TextBoard::ClearBoard() {
  ClearBoardStream();
}

//Initializes the stream, empty board
void TextBoard::ClearBoardStream() {
  board_stream_.str("");
  PrintEmptyLine(kTopMargin);
  for(std::size_t i = 0; i < dimensions_[kfirstAxis]; i++) {
    for(std::size_t j = 0; j < h_cell_size_; j++) {
      board_stream_ << line_indent_;
      PrintLine(j);
    }
  }
  line_length_ =
      line_indent_.size() + dimensions_[kSecondAxis] * (l_cell_size_ + 1) + 2;
}

// Given a position on the board, prints a board with the chip
// displayed in the cell.
void TextBoard::SetChip(const std::vector<std::size_t>& position,
                        int8_t piece_ID) {
  SetChipInc(position, piece_ID);
}

// Draws the piece in the specified location.
void TextBoard::SetChipInc(const std::vector<std::size_t>& position,
                           int8_t piece_ID) {
  assert(position.size() == kMaxDimensions);
  board_stream_.seekp(0, board_stream_.beg);
  std::size_t pos = (position[kfirstAxis] * line_length_ * (h_cell_size_)) +
                    kTopMargin + 1 + line_indent_.size() +
                    position[kSecondAxis] * (l_cell_size_ + 1);
  board_stream_.seekp(pos, board_stream_.beg);

  if(piece_ID != kPieceEmptyCell) {
    // once the cell is identified in the stream, the next step
    // is to print the piece's symbol, line by line
    std::map<int8_t, TextPiece>::const_iterator it = chips_.find(piece_ID);
    if(it != chips_.end()) {
      auto vector_string = it->second.GetSymbol();
      for(std::size_t i = 0; i < vector_string.size(); i++) {
        board_stream_ << vector_string[i];
        board_stream_.seekp(line_length_ - l_cell_size_, board_stream_.cur);
      }
    }
  } else {
    WriteToCell(empty_cell_string_);
  }
}

// Write a string in the cell, line by line
void TextBoard::WriteToCell(const std::string& str) {
  assert(str.size() == l_cell_size_);
  for(std::size_t i = 1; i < h_cell_size_; i++) {
    board_stream_ << str;
    // positioning the cursor to the next line
    board_stream_.seekp(line_length_ - l_cell_size_, board_stream_.cur);
  }
}

// Drawing a single line of the board given the coordinates (cline)
void TextBoard::PrintLine(std::size_t cline) {
  if(cline < h_cell_size_ - 1) {
    for(std::size_t k = 0; k < dimensions_[kSecondAxis]; k++) {
      board_stream_ << kVerticalLine << empty_cell_string_;
    }
  } else {
    // If last line, the bottom border is printed |______.
    for(std::size_t k = 0; k < dimensions_[kSecondAxis]; k++) {
      board_stream_ << kVerticalLine << lower_cell_string_;
    }
  }
  board_stream_ << kVerticalLine << std::endl;
}

void TextBoard::PrintEmptyLine(std::size_t lineCount) {
  while(lineCount > 0) {
    board_stream_ << kNewLineChar;
    lineCount--;
  }
}

void TextBoard::PrepareCursor() {
  GetCursorPosition(cursor_);
}

}  // namespace UI
