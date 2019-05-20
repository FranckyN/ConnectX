//============================================================================
// Author      : Franck Nassé - February 05, 2016
// Version     : v1.0
// Copyright   : Copyright (c) 2016, Franck Nassé. All rights reserved.
// Description :
//============================================================================
#include <algorithm>
#include <iterator>
#include <memory>
#include "connectx_board.h"
#include "utilities.h"

namespace UI {
ConnectXBoard::ConnectXBoard(const std::map<int8_t, TextPiece>& chips,
                             const std::vector<std::size_t>& dimensions,
                             const std::string& line_indent)
    : TextBoard(chips, dimensions, line_indent) {
  PrintColumnNumber();
}

void ConnectXBoard::SetConnectedChips(const std::list<std::vector<std::size_t> >& positions, int8_t piece_ID) {
  std::for_each(positions.begin(), positions.end(), [this, piece_ID](const std::vector<std::size_t>& val) {
    ChangeBackgroundCell(val, piece_ID, kEmptyChar, kDotChar);
  });
}

void ConnectXBoard::UnSetConnectedChips(const std::list<std::vector<std::size_t> >& positions, int8_t piece_ID) {
  std::for_each(positions.begin(), positions.end(), [this, piece_ID](const std::vector<std::size_t>& val) {
    ChangeBackgroundCell(val, piece_ID, kDotChar, kEmptyChar);
  });
}

void ConnectXBoard::ChangeBackgroundCell(const std::vector<std::size_t>& position,
                                         int8_t piece_ID,
                                         char replaced_char,
                                         char new_char) {
  assert(position.size() == kMaxDimensions);
  board_stream_.seekp(0, board_stream_.beg);
  std::size_t pos = (position[kfirstAxis] * line_length_ * (h_cell_size_)) + kTopMargin + 1 + line_indent_.size() +
                    position[kSecondAxis] * (l_cell_size_ + 1);
  board_stream_.seekp(pos, board_stream_.beg);
  std::string temp;
  if(piece_ID != kPieceEmptyCell) {
    auto it = chips_.find(piece_ID);
    if(it != chips_.end()) {
      auto array_string = it->second.GetSymbol();
      for(std::size_t i = 0; i < array_string.size(); i++) {
        temp = array_string[i];
        std::replace(temp.begin(), temp.end(), replaced_char, new_char);
        board_stream_ << temp;
        board_stream_.seekp(line_length_ - l_cell_size_, board_stream_.cur);
      }
    }
  }
}

void ConnectXBoard::PrintColumnNumber() {
  assert(number_digits(dimensions_[kSecondAxis]) < l_cell_size_ - 1);
  std::string temp_str;
  std::string temp_str_r;
  board_stream_ << line_indent_;
  std::size_t tmp_col;
  for(std::size_t k = 0; k < dimensions_[kSecondAxis]; k++) {
    tmp_col = k + 1;
    std::size_t pos_digit = (l_cell_size_ - number_digits(tmp_col)) / 2;
    std::size_t num_digit_right = l_cell_size_ - number_digits(tmp_col) - pos_digit;
    temp_str.resize(pos_digit, kEmptyChar);
    temp_str_r.resize(num_digit_right, kEmptyChar);
    board_stream_ << kVerticalLine << temp_str << (k + 1) << temp_str_r;
  }
  board_stream_ << kVerticalLine;
  PrintEmptyLine(1);
}

void ConnectXBoard::ClearBoard() {
  TextBoard::ClearBoard();
  PrintColumnNumber();
}
}  // namespace UI
