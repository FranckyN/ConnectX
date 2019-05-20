//============================================================================
// Author      : Franck Nassé - May 19, 2019
// Version     : v1.0
// Copyright   : Copyright (c) 2019, Franck Nassé. All rights reserved.
// Description : MarginBoard prints the header and footer of the board
//============================================================================
#ifndef MARGIN_BOARD_H
#define MARGIN_BOARD_H
#include "game_options.h"
#include <sstream>
#include <vector>
#include <map>
#include <board.h>
#include "text_piece.h"

namespace UI {


class MarginBoard
{
  std::stringstream header_top_stream_;
  std::stringstream header_btm_stream_;
  std::stringstream footer_stream_;
  GameOptions options_;
  std::size_t width_;
  std::vector<const char *> level_names_;
  std::map<int8_t, UI::TextPiece> chips_;
  std::vector<std::size_t> dimensions_;
public:
  MarginBoard(const GameOptions&,
              std::size_t,
              const std::vector<const char *>&,
              const std::map<int8_t, UI::TextPiece>&,
              const std::vector<std::size_t>&);
  ~MarginBoard();
  void PrintTopHeader();
  void PrintBtmHeader();
  void DrawHeader();
  void PrintCurrentPlayerName(int8_t);
  void PrintLastAndPossibleMoves(const std::vector<std::size_t>&, int);
  void PrintFooter(int8_t,
                   const std::vector<std::size_t>&,
                   int);
  void DrawFooter();
};

}
#endif // MARGIN_BOARD_HPP
