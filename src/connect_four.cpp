//============================================================================
// Author      : Franck Nassé - February 05, 2016
// Version     : v1.0
// Copyright   : Copyright (c) 2016, Franck Nassé. All rights reserved.
// Description : Manages user interaction
//============================================================================
#include <string.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include "connect_four.h"
#include "connectx_board.h"
#include "cursor_console.h"
#include "intelligent_board.h"
#include "margin_board.h"
#include "utilities.h"

using namespace Constants;

// The elements of this array represent the number of moves ahead
// analysed by the artificial intelligence module. The elements
// are sorted by levels of difficulty as defined in the array level_names_
const std::vector<uint8_t> ConnectFour::kLevelsArray = {2, 4, 6, 8};

ConnectFour::ConnectFour()
    : last_move_(kLastMoveBoardEmpty)
    , running_(true)
    , b_played_(false)
    , offset_history_count_(0) {
  commands_.push_back(
      std::bind(&ConnectFour::HandleQuit, this, std::placeholders::_1));
  commands_.push_back(
      std::bind(&ConnectFour::HandleRestart, this, std::placeholders::_1));
  commands_.push_back(
      std::bind(&ConnectFour::HandleUndo, this, std::placeholders::_1));
  commands_.push_back(
      std::bind(&ConnectFour::HandleContinue, this, std::placeholders::_1));
  commands_.push_back(std::bind(&ConnectFour::HandleDifficultyChange, this,
                                std::placeholders::_1));

  level_names_.push_back(k0EasyLevel);
  level_names_.push_back(k1BeginnerLevel);
  level_names_.push_back(k2IntermediateLevel);
  level_names_.push_back(k3AdvancedLevel);
  assert(kLevelsArray.size() == level_names_.size());
}

ConnectFour::~ConnectFour() {
}

/**
 * @brief This is the game main loop
 * It displays the instructions
 * display the board
 * retrieve the inputs
 * display the result
 */
void ConnectFour::Play() {
  UI::EnterGameMode();
  ConfigureOptions();
  Display();
  UI::RefreshWindow();
  std::string user_input;
  while(running_) {
    user_input = GetInput();
    ProcessInput(user_input);
    Display();
  }
  UI::ClearWindowScreen();
  UI::CloseWindow();
  std::cout << kIndentScreen << std::endl << kByeMsg << std::endl;
}

// Checks if there is game going on.
bool ConnectFour::IsOnGoing() const {
  return (Core::States::OnGoing == model_board_->GetCurrentState());
}

// returns true if it is the human player's turn.
bool ConnectFour::IsHumanPlayerTurn() const {
  return model_board_->GetAIDepth(model_board_->GetNextChipId()) ==
         Core::kLevelExternalPlayer;
}

/**
 * @brief depending on the state of the game the user inputs are retrieved
 * @return user input string
 */
std::string ConnectFour::GetInput() {
  if(!IsOnGoing()) {
    std::pair<int, int> current_cursor;
    UI::GetCursorPosition(current_cursor);
    UI::ClearWindowCurrentLine(2);
    UI::SetCursorPosition(current_cursor.first, current_cursor.second);
    return GetFinalUserInput();
  } else if(IsHumanPlayerTurn()) {
    return GetUserInput();
  }
  return kEmptyNullStr;
}

// Gets the choice of the user at the end of a game
// the user can decide to continue, undo the last move, restart or change the
// level
std::string ConnectFour::GetFinalUserInput() {
  std::string message = kEndOfGameMsg_1;
  message += kQuitGameCmd;
  message += kEndOfGameMsg_2;
  message += kContinueCmd;
  message += kEndOfGameMsg_3;
  char temp_char;
  UI::GotoNextWindowLine();
  GetValueFromConsoleUser(
      temp_char, kIndentScreen + message, [](const char& x) {
        return (x == kQuitGameCmd || x == kContinueCmd || x == kUndoGameCmd ||
                x == kRestartCmd || x == kDifficultyCmd);
      });
  return std::string(1, temp_char);
}

// Prints welcoming message
void ConnectFour::PrintWelcomeMsg() {
  UI::GotoNextWindowLine();
  std::stringstream stemp;
  stemp << kIndentScreen << kMushuStudioMsg << std::endl;
  stemp << kIndentScreen << kInstructionsMsg_1 << std::endl;
  stemp << kIndentScreen << kInstructionsMsg_2 << kQuitGameCmd
        << kInstructionsMsg_3 << kUndoGameCmd << kInstructionsMsg_4
        << kRestartCmd << kInstructionsMsg_5 << std::endl;
  stemp << kIndentScreen << kInstructionsMsg_6 << kDifficultyCmd
        << kInstructionsMsg_7 << std::endl;
  UI::WriteToWindow(stemp);
}

// Gets user inputs during a game (the column index)
// the user can also abandon, undo the last move or retstart.
std::string ConnectFour::GetUserInput() const {
  std::string user_input;
  std::stringstream s_stream;
  std::size_t temp_value;
  int errors = 0;
  std::pair<int, int> input_position;
  int message_lenght = kIndentScreen.size() + strlen(kEnterMoveMsg);
  UI::GetCursorPosition(input_position);
  do {
    s_stream.clear();
    if(errors > 0) {
      UI::SetCursorPosition(input_position.first, input_position.second);
    }
    UI::WriteToWindow(kIndentScreen);
    UI::WriteToWindow(kEnterMoveMsg);
    UI::ClearRemainingWindow();
    UI::SetCursorPosition(input_position.first + message_lenght,
                          input_position.second);
    UI::GetCharacter(user_input, 1);
    if(user_input.compare(kQuitGameCmdStr) == 0 ||
       user_input.compare(kUndoGameCmdStr) == 0 ||
       user_input.compare(kRestartCmdStr) == 0) {
      break;
    }
    s_stream.str(user_input);
    s_stream >> temp_value;
    errors++;
  } while(!((temp_value > 0 &&
             temp_value <= model_board_->GetDimensions()[k2ndDim])) ||
          s_stream.fail());

  return user_input;
}

/**
 * @brief This method actually draws the board with the last move.
 * If the game is over and there is a winner it will draw a new board with the
 * connected chips
 */
void ConnectFour::DropChip() {
  Core::ModelBoard::MoveType last_move = model_board_->GetLastMove();
  std::size_t row = 0;
  while(row < last_move[k1stDim]) {
    board_->SetChip({row, last_move[k2ndDim]},
                    model_board_->GetCurrentChipId());
    row++;
    board_->DrawBoard();
    board_->SetChip({row - 1, last_move[k2ndDim]});
    std::this_thread::sleep_for(std::chrono::milliseconds(
        (model_board_->GetAIDepth(model_board_->GetCurrentChipId()) ==
                 Core::kLevelExternalPlayer ?
             kDelayHuman :
             kDelayMachine)));
  }

  b_played_ = false;
  board_->SetChip(last_move, model_board_->GetCurrentChipId());
  if(model_board_->GetCurrentState() == Core::States::Win) {
    std::list<Core::ModelBoard::MoveType> win_positions;
    model_board_->GetWinningPositions(win_positions);
    board_->SetConnectedChips(win_positions, model_board_->GetCurrentChipId());
  }
}

// This draws the board on the screen
void ConnectFour::DrawBoard() {
  if(last_move_ >= 0 && b_played_) {
    DropChip();
    if(model_board_->GetCurrentState() == Core::States::Win) {
    UI::FlashBeep(2);
  }
  }
  board_->DrawBoard();
}

// This draws the header on the screen
void ConnectFour::DrawHeader() {
  margin_board_->DrawHeader();
}

// Prints the name of the player who has to play
void ConnectFour::PrintCurrentPlayerName() {
  std::map<int8_t, Player>::const_iterator it =
      current_options_.players_.find(model_board_->GetNextChipId());
  assert(it != current_options_.players_.end());
  UI::WriteToWindow(kIndentScreen);
  UI::WriteToWindow(it->second.GetName());
  UI::WriteToWindow(kTurnMsg1);
}

// Prints the last move and all the current possible moves.
void ConnectFour::PrintLastAndPossibleMoves() {
  std::stringstream stemp;
  if(last_move_ >= 0) {
    stemp << kLastMoveIndent << kLastMoveMsg << last_move_ + 1;
  }
  stemp << std::endl << kIndentScreen << kPossibleMovesMsg;
  for(std::size_t i = 0; i < latest_positions_.size(); i++) {
    if(latest_positions_[i] > 0) {
      stemp << i + 1 << kEmptyChar;
    }
  }
  UI::WriteToWindow(stemp);
}

// Drawing the footer
void ConnectFour::DrawFooter() {
  margin_board_->PrintFooter(model_board_->GetNextChipId(), latest_positions_,
                             last_move_);
  margin_board_->DrawFooter();
}

// Displays the board and all the details on the screen
void ConnectFour::Display() {
  DrawBoard();
  std::pair<int, int> origin_cursor;
  UI::GetCursorPosition(origin_cursor);
  UI::ClearWindowCurrentLine(3);
  UI::SetCursorPosition(origin_cursor.first, origin_cursor.second);
  if(IsOnGoing()) {
    DrawFooter();
  } else {
    DisplayEndofGame();
  }
}

/**
 * @brief Processes the input, the method implements a pattern.
 * the input is distributed to all the external game commands, the process stops
 * as soon as the input is handled. If the input is not a command then it is
 * assumed that it is a move.
 * @param user input
 */
void ConnectFour::ProcessInput(const std::string& input) {
  auto it_func = commands_.begin();
  bool processed = false;
  while(it_func != commands_.end()) {
    if((*it_func++)(input)) {
      processed = true;
      break;
    }
  }
  if(!processed) {
    HandleMove(input);
  }
}

// Handles the Quit Game command
bool ConnectFour::HandleQuit(const std::string& command) {
  if(command.compare(kQuitGameCmdStr) == 0) {
    running_ = false;
    return !running_;
  }
  return false;
}

// Handles the continue command
bool ConnectFour::HandleContinue(const std::string& command) {
  if(command.compare(kContinueCmdStr) == 0) {
    Reset();
    return true;
  }
  return false;
}

// Handles the command to change the difficulty level
bool ConnectFour::HandleDifficultyChange(const std::string& command) {
  if(command.compare(kDifficultyCmdStr) == 0) {
    ChangeAIDepthsOptions();
    return true;
  }
  return false;
}

// Handles the command to restart a game
bool ConnectFour::HandleRestart(const std::string& command) {
  if(command.compare(kRestartCmdStr) == 0) {
    Reset();
    ConfigureOptions();
    return true;
  }
  return false;
}

// Handles the command to undo the last move depending on the state of the game
bool ConnectFour::HandleUndo(const std::string& command) {
  if(command.compare(kUndoGameCmdStr) == 0) {
    int undo_counter =
        std::min(kAtomicUndoCounter,
                 model_board_->GetHistoryCount() - offset_history_count_);
    if(model_board_->GetCurrentState() == Core::States::Win) {
      std::list<Core::ModelBoard::MoveType> win_positions;
      model_board_->GetWinningPositions(win_positions);
      board_->UnSetConnectedChips(win_positions,
                                  model_board_->GetCurrentChipId());
    }
    while(undo_counter > 0) {
      board_->SetChip(model_board_->GetLastMove());
      model_board_->Undo();
      latest_positions_[last_move_]++;
      if(model_board_->GetHistoryCount() - offset_history_count_ > 0) {
        last_move_ = model_board_->GetLastMove()[k2ndDim];
      } else {
        last_move_ = kLastMoveBoardEmpty;
      }
      undo_counter--;
    }
    return true;
  }
  return false;
}

bool ConnectFour::HandleMove(const std::string& command) {
  Core::ModelBoard::MoveType current_move(model_board_->GetDimensions().size());
  if(IsHumanPlayerTurn()) {
    std::size_t col = atoi(command.c_str()) - 1;
    if(latest_positions_[col] > 0) {  // is not full
      current_move[k2ndDim] = col;
      current_move[k1stDim] = --latest_positions_[current_move[k2ndDim]];
      model_board_->SetMove(current_move);
      b_played_ = true;
    }
  } else {
    model_board_->GetAIPlayerMove(current_move,
                                  std::chrono::milliseconds(1000));
    model_board_->SetMove(current_move);
    std::this_thread::sleep_for(std::chrono::milliseconds(kDelayAIPlayer));
    --latest_positions_[current_move[k2ndDim]];
    b_played_ = true;
  }
  if(b_played_) {
    last_move_ = current_move[k2ndDim];
  }
  return true;
}

void ConnectFour::ChangeAIDepthsOptions() {
  if(kPlayerModeEnum[current_options_.player_mode_] == kSinglePlayer) {
    Reset();
    UI::ClearWindowScreen();
    GetDifficulty();
    std::map<int8_t, Player>::const_iterator mit =
        current_options_.players_.begin();
    while(mit != current_options_.players_.end()) {
      if(model_board_->GetAIDepth(mit->first) > 0) {
        model_board_->SetAIDepth(mit->first,
                                 kLevelsArray[current_options_.difficulty_]);
        break;
      }
      ++mit;
    }
  } else if(kPlayerModeEnum[current_options_.player_mode_] == kTwoAIPlayers) {
    Reset();
    UI::ClearWindowScreen();
    GetAIPlayerLevels();
    auto mit = current_options_.simulation_depths_.begin();
    while(mit != current_options_.simulation_depths_.end()) {
      model_board_->SetAIDepth(mit->first, mit->second);
      ++mit;
    }
  } else {
    return;
  }
  UI::ClearWindowScreen();
  InitializeGame();
  PrintTopHeader();
  PrintBtmHeader();
  DrawHeader();
}

void ConnectFour::ConfigureOptions() {
  UI::ClearWindowScreen();
  PrintWelcomeMsg();
  GetVariants();
  GetPlayerConfig();
  GetPlayerDetails();
  GetFirstPlayer();
  GetLevels();
  InitializeGame();
  PrintTopHeader();
  PrintBtmHeader();
  UI::ClearWindowScreen();
  DrawHeader();
}

void ConnectFour::GetLevels() {
  if(kPlayerModeEnum[current_options_.player_mode_] == kSinglePlayer) {
    GetDifficulty();
  } else if(kPlayerModeEnum[current_options_.player_mode_] == kTwoAIPlayers) {
    GetAIPlayerLevels();
  }
}

void ConnectFour::GetAIPlayerLevels() {
  int temp_int;
  std::ostringstream temp_ostream;
  current_options_.simulation_depths_.clear();
  for(auto ID_player : current_options_.players_) {
    UI::GotoNextWindowLine();
    temp_ostream << kIndentScreen << kGetDepthMsg_1
                 << ID_player.second.GetName() << kGetDepthMsg_2 << kMinAIDepth
                 << kAndStr << kMaxAIDepth << kGetDepthMsg_3;
    GetValueFromConsoleUser(temp_int, temp_ostream.str(), [](const int& x) {
      return (x >= kMinAIDepth && x <= kMaxAIDepth);
    });
    current_options_.simulation_depths_[ID_player.first] = temp_int;
    temp_ostream.str("");
  }
}

void ConnectFour::GetVariants() {
  UI::GotoNextWindowLine();
  GetSelectedOptions(kGameVariantsEnum, current_options_.kind_, kChooseKindMsg);
}

void ConnectFour::GetPlayerConfig() {
  UI::GotoNextWindowLine();
  GetSelectedOptions(kPlayerModeEnum, current_options_.player_mode_,
                     kChooseModesMsg);
}

void ConnectFour::GetFirstPlayer() {
  UI::GotoNextWindowLine();
  auto it_player1 = current_options_.players_.find(kFirstPlayerChipId);
  auto it_player2 = current_options_.players_.find(kFirstPlayerChipId + 1);
  GetSelectedOptions({it_player1->second.GetName().c_str(),
                      it_player2->second.GetName().c_str()},
                     current_options_.firstplayer_, "Who is playing first? ");
}

void ConnectFour::GetDifficulty() {
  UI::GotoNextWindowLine();
  GetSelectedOptions(level_names_, current_options_.difficulty_,
                     kGetDifficultyMsg);
}

void ConnectFour::Reset() {
  std::fill(latest_positions_.begin(), latest_positions_.end(),
            model_board_->GetDimensions()[k1stDim]);
  model_board_->Reset();
  board_->ClearBoard();
  b_played_ = false;
  if(kGameVariantsEnum[current_options_.kind_] == kFiveInRowConnectFour) {
    FillForConnectFive();
  }
  last_move_ = kLastMoveBoardEmpty;
}

void ConnectFour::DisplayEndofGame() const {
  auto it = current_options_.players_.find(model_board_->GetCurrentChipId());
  assert(it != current_options_.players_.end());
  std::stringstream stemp;
  stemp << std::endl << kIndentScreen;
  if(model_board_->GetCurrentState() == Core::States::Draw) {
    stemp << kDrawMsg << std::endl;
  } else {
    stemp << kWinnerMsg << it->second.GetName() << std::endl;
  }
  UI::WriteToWindow(stemp);
}

/**
 * @brief Selects a variant of the game and return the number of ships to be
 * connected to win. based on the user selection (current_options_.kind_) the
 * appropriate board object is created
 * @param num_chips_connected output argument to retrieve the number of chips to
 * connect to win.
 */
void ConnectFour::InitializeGameVariant(uint8_t& num_chips_connected) {
  UI::TextPiece chip1(UI::TextPiece::SymbolType(KPly1Symbol),
                      kFirstPlayerChipId);
  UI::TextPiece chip2(UI::TextPiece::SymbolType(KPly2Symbol),
                      kFirstPlayerChipId + 1);
  std::map<int8_t, UI::TextPiece> chips = {{chip1.GetIdentifier(), chip1},
                                           {chip2.GetIdentifier(), chip2}};
  if(kGameVariantsEnum[current_options_.kind_] == kClassicConnectFour) {
    board_.reset(new UI::ConnectXBoard(
        chips, {kClassicBoardNumRows, kClassicBoardNumCols}, kIndentScreen));

  } else if(kGameVariantsEnum[current_options_.kind_] ==
            kFiveInRowConnectFour) {
    board_.reset(new UI::ConnectXBoard(
        chips, {kClassicBoardNumRows, kClassicBoardNumCols + 2},
        kIndentScreen));
    num_chips_connected = kClassicChipsConnected + 1;
  } else if(kGameVariantsEnum[current_options_.kind_] == k3By6ConnectThree) {
    board_.reset(new UI::ConnectXBoard(
        chips, {KClassic3By6Width, KClassic3By6Height}, kIndentScreen));
    num_chips_connected = kClassicChipsConnected - 1;
  } else if(kGameVariantsEnum[current_options_.kind_] == kTenBy7ConnectFour) {
    board_.reset(new UI::ConnectXBoard(
        chips, {kClassicBoardNumRows + 1, kClassicBoardNumCols + 3},
        kIndentScreen));
  } else if(kGameVariantsEnum[current_options_.kind_] == kEightBy8ConnectFour) {
    board_.reset(new UI::ConnectXBoard(
        chips, {kClassicBoardNumRows + 2, kClassicBoardNumCols + 1},
        kIndentScreen));
  }
  margin_board_.reset(
      new UI::MarginBoard(current_options_, board_->GetNumCharLine(),
                          level_names_, chips, board_->GetDimensions()));
}

/**
 * @brief Creates the intelligent_board for the ~AI module.
 * the intelligent_board needs to know which side is the computer on (party1 or
 * party2), the difficulty level, the number of pieces to connect to win, and
 * the board's dimension.
 * @param num_chips_connected  the number of chips to connect to win.
 */
void ConnectFour::InitializeModel(uint8_t num_chips_connected) {
  Core::IntelligentBoard::Party party1(kFirstPlayerChipId,
                                       Core::kLevelExternalPlayer);
  Core::IntelligentBoard::Party party2(kFirstPlayerChipId + 1,
                                       Core::kLevelExternalPlayer);

  if(kPlayerModeEnum[current_options_.player_mode_] == kSinglePlayer) {
    party2.depth_ = kLevelsArray[current_options_.difficulty_];
  } else if(kPlayerModeEnum[current_options_.player_mode_] == kTwoAIPlayers) {
    party1.depth_ = current_options_.simulation_depths_[kFirstPlayerChipId];
    party2.depth_ = current_options_.simulation_depths_[kFirstPlayerChipId + 1];
  }

  if(current_options_.firstplayer_ == 0) {
    model_board_.reset(new Core::IntelligentBoard(
        {party1, party2},
        {board_->GetDimensions()[k1stDim], board_->GetDimensions()[k2ndDim]},
        num_chips_connected));
  } else {
    model_board_.reset(new Core::IntelligentBoard(
        {party2, party1},
        {board_->GetDimensions()[k1stDim], board_->GetDimensions()[k2ndDim]},
        num_chips_connected));
  }
}

// Initializes the game
void ConnectFour::InitializeGame() {
  uint8_t num_chips_connected = kClassicChipsConnected;
  InitializeGameVariant(num_chips_connected);
  InitializeModel(num_chips_connected);
  offset_history_count_ = 0;
  latest_positions_.resize(board_->GetDimensions()[k2ndDim]);
  std::fill(latest_positions_.begin(), latest_positions_.end(),
            board_->GetDimensions()[k1stDim]);
  if(kGameVariantsEnum[current_options_.kind_] == kFiveInRowConnectFour) {
    FillForConnectFive();
  }
}

void ConnectFour::DisplayNameSizeLimits() {
  UI::GotoNextWindowLine();
  std::stringstream stemp;
  stemp << kIndentScreen << kNameMinSize << kLimitNamePlayerMin
        << kCharactersStr << std::endl;
  stemp << kIndentScreen << kNameMaxSize << kLimitNamePlayerMax
        << kCharactersStr << std::endl;
  UI::WriteToWindow(stemp);
  UI::GotoNextWindowLine();
}

// Ask the users for their names depending on the number of players involved
void ConnectFour::GetPlayerDetails() {
  current_options_.players_.clear();
  std::string first_player, second_player;
  if(kPlayerModeEnum[current_options_.player_mode_] == kSinglePlayer) {
    DisplayNameSizeLimits();
    GetValueFromConsoleUser(first_player, kIndentScreen + kNameMsg,
                            [](const std::string& x) {
                              return (x.size() >= kLimitNamePlayerMin &&
                                      x.size() <= kLimitNamePlayerMax);
                            });
    second_player = kAIPlayer1NameMsg;
  } else if(kPlayerModeEnum[current_options_.player_mode_] == kTwoPlayers) {
    DisplayNameSizeLimits();
    GetValueFromConsoleUser(first_player, kIndentScreen + kPlayer1NameMsg,
                            [](const std::string& x) {
                              return (x.size() >= kLimitNamePlayerMin &&
                                      x.size() <= kLimitNamePlayerMax);
                            });
    UI::GotoNextWindowLine();
    GetValueFromConsoleUser(second_player, kIndentScreen + kPlayer2NameMsg,
                            [&first_player](const std::string& x) {
                              return (x.size() >= kLimitNamePlayerMin &&
                                      x.size() <= kLimitNamePlayerMax &&
                                      x != first_player);
                            });
  } else if(kPlayerModeEnum[current_options_.player_mode_] == kTwoAIPlayers) {
    first_player = kAIPlayer1NameMsg;
    second_player = kAIPlayer2NameMsg;
  }

  current_options_.players_.insert(
      std::make_pair(kFirstPlayerChipId, Player(first_player)));
  current_options_.players_.insert(std::make_pair(
      static_cast<int8_t>(kFirstPlayerChipId + 1), Player(second_player)));
}

// This method deals specifically with the ConnectFive variant where the first
// and last columns need to be filled with alternate chips.
void ConnectFour::FillForConnectFive() {
  FillColumn(0, 1);
  FillColumn(board_->GetDimensions()[k2ndDim] - 1, 0);
  FillColumn(0, 0);
  offset_history_count_ = board_->GetDimensions()[k1stDim] * 2;
}

void ConnectFour::FillColumn(std::size_t column, std::size_t num_stop_row) {
  Core::ModelBoard::MoveType current_move(board_->GetDimensions().size());
  current_move[k2ndDim] = column;
  while(latest_positions_[current_move[k2ndDim]] > num_stop_row) {
    current_move[k1stDim] = --latest_positions_[current_move[k2ndDim]];
    model_board_->SetMove(current_move);
    last_move_ = current_move[k2ndDim];
    board_->SetChip(current_move, model_board_->GetCurrentChipId());
  }
}
void ConnectFour::PrepareCursor() {
  board_->PrepareCursor();
}

void ConnectFour::PrintTopHeader() {
  margin_board_->PrintTopHeader();
}

void ConnectFour::PrintBtmHeader() {
  margin_board_->PrintBtmHeader();
}
