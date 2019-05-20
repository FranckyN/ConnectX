//============================================================================
// Author      : Franck Nassé - May 19, 2019
// Version     : v1.0
// Copyright   : Copyright (c) 2019, Franck Nassé. All rights reserved.
// Description : MarginBoard prints the header and footer of the board
//============================================================================
#include <iostream>
#include <map>
#include "constants.h"
#include "cursor_console.h"
#include "game_options.h"
#include "margin_board.h"

namespace UI {

using namespace Constants;

MarginBoard::MarginBoard(const GameOptions& options,
                         std::size_t width,
                         const std::vector<const char*>& level_names,
                         const std::map<int8_t, UI::TextPiece>& chips,
                         const std::vector<std::size_t>& dimensions)
    : options_(options)
    , width_(width)
    , level_names_(level_names)
    , chips_(chips)
    , dimensions_(dimensions) {
}

MarginBoard::~MarginBoard() {
}

void MarginBoard::DrawHeader() {
  UI::WriteToWindow(header_top_stream_);
  UI::WriteToWindow(header_btm_stream_);
}

// Prints the header of the board.
// prints the players' names on both sides.
void MarginBoard::PrintTopHeader() {
  header_top_stream_.str(kEmptyNullStr);
  std::string smiddle(
      std::max<int>(
          0, width_ - 2 * chips_[kFirstPlayerChipId].GetSymbol()[0].size()),
      kEmptyChar);
  std::map<int8_t, Player>::const_iterator it_player1 =
      options_.players_.find(kFirstPlayerChipId);
  auto it_player2 = options_.players_.find(kFirstPlayerChipId + 1);

  std::size_t const num_rows = chips_[kFirstPlayerChipId].GetSymbol().size();
  for(std::size_t j = 0; j < num_rows; j++) {
    header_top_stream_ << kIndentScreen
                       << chips_[kFirstPlayerChipId].GetSymbol()[j] << smiddle
                       << chips_[kFirstPlayerChipId + 1].GetSymbol()[j]
                       << std::endl;
  }
  header_top_stream_ << std::endl;
  std::string smiddlename(
      std::max<int>(
          0, dimensions_[k2ndDim] *
                     (chips_[kFirstPlayerChipId].GetSymbol()[0].size() + 1) -
                 (it_player1->second.GetName().size() +
                  it_player2->second.GetName().size())),
      kEmptyChar);

  header_top_stream_ << kIndentScreen << kEmptyChar
                     << it_player1->second.GetName() << smiddlename
                     << it_player2->second.GetName();
}

// Prints the footer of the board
// Displays information about the current game.
void MarginBoard::PrintBtmHeader() {
  header_btm_stream_.str(kEmptyNullStr);
  std::size_t board_width = width_;
  std::string temp_msg, temp_str;
  if(kPlayerModeEnum[options_.player_mode_] == kSinglePlayer) {
    temp_msg = kCurrentLevelMsg;
    temp_msg += level_names_[options_.difficulty_];
    temp_str.resize(std::max<int>(0, (board_width - temp_msg.size()) / 2),
                    kEmptyChar);
    header_btm_stream_ << kIndentScreen << temp_str << temp_msg;
  } else if(kPlayerModeEnum[options_.player_mode_] == kTwoAIPlayers) {
    std::map<int8_t, int8_t>::const_iterator it_depth_player1 =
        options_.simulation_depths_.find(kFirstPlayerChipId);
    auto it_depth_player2 =
        options_.simulation_depths_.find(kFirstPlayerChipId + 1);
    std::stringstream ss;
    ss << kDepthsAIMsg1 << static_cast<int>(it_depth_player1->second)
       << kDepthsAIMsg2 << static_cast<int>(it_depth_player2->second);
    temp_msg = ss.str();
    temp_str.resize(std::max<int>(0, (board_width - temp_msg.size()) / 2),
                    kEmptyChar);
    header_btm_stream_ << kIndentScreen << temp_str << temp_msg;
  }
}

// Prints the name of the player who has to play
void MarginBoard::PrintCurrentPlayerName(int8_t current_piece) {
  std::map<int8_t, Player>::const_iterator it =
      options_.players_.find(current_piece);
  footer_stream_ << kIndentScreen << it->second.GetName() << kTurnMsg1;
}

// Prints the last move and all the current possible moves.
void MarginBoard::PrintLastAndPossibleMoves(
    const std::vector<std::size_t>& current_positions,
    int last_move) {
  if(last_move >= 0) {
    footer_stream_ << kLastMoveIndent << kLastMoveMsg << last_move + 1;
  }
  footer_stream_ << std::endl << kIndentScreen << kPossibleMovesMsg;
  for(std::size_t i = 0; i < current_positions.size(); i++) {
    if(current_positions[i] > 0) {
      footer_stream_ << i + 1 << kEmptyChar;
    }
  }
}

// Prints the footer
void MarginBoard::PrintFooter(int8_t current_piece,
                              const std::vector<std::size_t>& current_positions,
                              int last_move) {
  footer_stream_.str("");
  footer_stream_ << std::endl;
  PrintCurrentPlayerName(current_piece);
  PrintLastAndPossibleMoves(current_positions, last_move);
  footer_stream_ << " " << std::endl;
}

void MarginBoard::DrawFooter() {
  UI::WriteToWindow(footer_stream_);
}

}  // namespace UI