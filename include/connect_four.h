//============================================================================
// Author      : Franck Nassé - February 05, 2016
// Version     : v1.0
// Copyright   : Copyright (c) 2016, Franck Nassé. All rights reserved.
// Description : User interaction management.
//============================================================================
#ifndef CONNECTFOUR_H
#define CONNECTFOUR_H

#include <vector>
#include <list>
#include <sstream>
#include <memory>
#include <map>
#include <functional>
#include "player.h"
#include "constants.h"
#include "game_options.h"

namespace UI {
class ConnectXBoard;
class MarginBoard;
}

namespace Core {
class IntelligentBoard;
}
/* ConnectX's main class
 * This class represent the game itself
 * it takes care of managing the board, turns, and the players
 */
class ConnectFour {
 public:
  ConnectFour();
  ~ConnectFour();
  /**
   * @brief Play is the game loop
   */
  void Play();

 protected:
  
  static const std::vector<uint8_t> kLevelsArray;
  static std::string GetFinalUserInput();
  static void PrintWelcomeMsg();
  static void DisplayNameSizeLimits();
  
  bool IsOnGoing() const;
  bool IsHumanPlayerTurn() const;
  std::string GetInput();
  std::string GetUserInput() const;
  void DropChip();
  void DrawBoard();
  void PrintTopHeader();
  void PrintBtmHeader();
  void DrawHeader();
  void PrintCurrentPlayerName();
  void PrintLastAndPossibleMoves();
  void DrawFooter();
  void PrepareCursor();
  void Display();
  void ProcessInput(const std::string& input);
  bool HandleQuit(const std::string& command);
  bool HandleContinue(const std::string& command);
  bool HandleDifficultyChange(const std::string& command);
  bool HandleRestart(const std::string& command);
  bool HandleUndo(const std::string& command);
  bool HandleMove(const std::string& command);
  void ChangeAIDepthsOptions();
  void ConfigureOptions();
  void GetLevels();
  void GetAIPlayerLevels();
  void GetVariants();
  void GetPlayerConfig();
  void GetDifficulty();
  void GetFirstPlayer();
  void Reset();
  void DisplayEndofGame() const;
  void InitializeGameVariant(uint8_t& num_chips_connected);
  void InitializeModel(uint8_t num_chips_connected);
  void InitializeGame();
  void GetPlayerDetails();
  void FillForConnectFive();
  void FillColumn(std::size_t column, std::size_t num_stop_row);
  
  int last_move_;
  bool running_;
  bool b_played_;
  std::size_t offset_history_count_;
  std::list<std::function<bool(const std::string&)> > commands_;
  GameOptions current_options_;
  std::unique_ptr<UI::ConnectXBoard> board_;
  std::unique_ptr<Core::IntelligentBoard> model_board_;
  std::unique_ptr<UI::MarginBoard> margin_board_;
  std::vector<std::size_t> latest_positions_;
  std::vector<const char *> level_names_;
};

#endif // CONNECTFOUR_H
