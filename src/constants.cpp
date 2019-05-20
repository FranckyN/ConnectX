//============================================================================
// Author      : Franck Nassé - February 05, 2016
// Version     : v1.0
// Copyright   : Copyright (c) 2016, Franck Nassé. All rights reserved.
// Description :
//============================================================================
#include "constants.h"

namespace Constants {
extern const char kClassicConnectFour[] = "6-by-7 Connect"
                                          "Four";
extern const char kTenBy7ConnectFour[] = "7-by-10 Connect"
                                          "Four";
extern const char kEightBy8ConnectFour[] = "8-by-8 Connect"
                                          "Four";
extern const char kFiveInRowConnectFour[] = "Five-in-a-Row";
extern const char k3By6ConnectThree[] = "3-by-6 Connect "
                                        "Three";
extern const std::vector<const char *> kGameVariantsEnum = {kClassicConnectFour, k3By6ConnectThree,
                                                            kFiveInRowConnectFour, kTenBy7ConnectFour,
                                                            kEightBy8ConnectFour};
extern const std::size_t KClassic3By6Height = 6;
extern const std::size_t KClassic3By6Width = 3;
extern const std::size_t k1stDim = 0;
extern const std::size_t k2ndDim = 1;
extern const char kSinglePlayer[] = "Single Player";
extern const char kTwoPlayers[] = "Two Players";
extern const char kTwoAIPlayers[] = "Two Simulated"
                                    " Players";
extern const std::vector<const char *> kPlayerModeEnum = {kSinglePlayer, kTwoPlayers, kTwoAIPlayers};
extern const char kColonPoint[] = ": ";
extern const char k0EasyLevel[] = "Beginner";
extern const char k1BeginnerLevel[] = "Intermediate";
extern const char k2IntermediateLevel[] = "Advanced";
extern const char k3AdvancedLevel[] = "Hard Level";
extern const char kChooseKindMsg[] = "Choose"
                                     " between one of"
                                     " the Connect Four"
                                     " variants listed"
                                     " above: ";
extern const char kChooseModesMsg[] = "Choose"
                                      " between one"
                                      " of the options"
                                      " listed above: ";
extern const char kNameMsg[] = "Enter your "
                               "username:";
extern const char kPlayer1NameMsg[] = "Enter first player"
                                      " name: ";
extern const char kPlayer2NameMsg[] = "Enter second player"
                                      " name: ";
extern const char kNameMaxSize[] = "Name's maximum"
                                   " length is ";
extern const char kNameMinSize[] = "Name's minimum"
                                   " length is ";
extern const char kCharactersStr[] = " characters ";
extern const char kEnterMoveMsg[] = "Type a column"
                                    " number and"
                                    " press Enter: ";
extern const char kAIPlayer1NameMsg[] = "Mushu";
extern const char kAIPlayer2NameMsg[] = "Pippin";
extern const char kGetDepthMsg_1[] = "Enter thinking"
                                     " depth of ";
extern const char kGetDepthMsg_2[] = " (value between ";
extern const char kGetDepthMsg_3[] = "): ";
extern const int kMinAIDepth = 1;
extern const int kMaxAIDepth = 11;
extern const char kGetDifficultyMsg[] = "Select the level "
                                        "of difficulty from "
                                        "the levels listed "
                                        "above: ";
extern const char kEmptyChar = ' ';
extern const char kAndStr[] = " and ";
extern const std::string kIndentScreen(20, kEmptyChar);
extern const char kQuitGameCmd = 'q';
extern const char kNewLine = '\n';
extern const char kNullChar = '\0';
extern const char kQuitGameCmdStr[] = {kQuitGameCmd, kNullChar};
extern const char kUndoGameCmd = 'u';
extern const char kUndoGameCmdStr[] = {kUndoGameCmd, kNullChar};
extern const char kRestartCmd = 'r';
extern const char kRestartCmdStr[] = {kRestartCmd, kNullChar};
extern const char kDifficultyCmd = 'd';
extern const char kDifficultyCmdStr[] = {kDifficultyCmd, kNullChar};
extern const char kYesCmd = 'y';
extern const char kNoCmd = 'n';
extern const char kContinueCmd = 'c';
extern const char kContinueCmdStr[] = {kContinueCmd, kNullChar};
extern const char kZeroChar = '0';
extern const int8_t kFirstPlayerChipId = 1;
extern const std::size_t kLimitNamePlayerMin = 3;
extern const std::size_t kLimitNamePlayerMax = 15;
extern const std::size_t kClassicBoardNumRows = 6;
extern const std::size_t kClassicBoardNumCols = 7;
extern const uint8_t kClassicChipsConnected = 4;
extern const int kChunkPrint = 4095;
extern const int8_t kWindowLines = 120;
extern const int8_t kWindowColumns = 120;
extern const char kTokSec[] = "       ";
extern const char kTok1Star[] = " *   * ";
extern const char kTok3Stars[] = "  ***  ";
extern const char kEvenBubbles[] = "  ooo  ";
extern const char kOddBubble[] = " o   o ";
extern const std::vector<std::string> KPly1Symbol = {kTok1Star, kTok3Stars, kTok1Star};
extern const std::vector<std::string> KPly2Symbol = {kEvenBubbles, kOddBubble, kEvenBubbles};
extern const char kLastMoveMsg[] = "Opponent's "
                                   "last move: ";
extern const char kTurnMsg1[] = "'s turn";
extern const int kLastMoveBoardEmpty = -999;
extern const std::size_t kAtomicUndoCounter = 2;
extern const char kEndOfGameMsg_1[] = "Quit or Continue "
                                      "(type '";
extern const char kEndOfGameMsg_2[] = "' or '";
extern const char kEndOfGameMsg_3[] = "') ? ";
extern const char kDrawMsg[] = "Draw ";
extern const char kWinnerMsg[] = "The winner is ";
extern const char kByeMsg[] = "Thank you for "
                              "playing ConnectX, "
                              "see you next time.";
extern const char kPossibleMovesMsg[] = "Possible moves: ";
extern const char kMushuStudioMsg[] = "ConnectX";
extern const char kInstructionsMsg_1[] = "Connect the symbols"
                                         " and win.";
extern const char kInstructionsMsg_2[] = "At anytime during"
                                         " the game, "
                                         "you can press '";
extern const char kInstructionsMsg_3[] = "' to quit, '";
extern const char kInstructionsMsg_4[] = "' to undo or '";
extern const char kInstructionsMsg_5[] = "' to restart.";
extern const char kInstructionsMsg_6[] = "At the end of a"
                                         " game you"
                                         " can press '";
extern const char kInstructionsMsg_7[] = "' to change "
                                         "the difficulty"
                                         " level.";
extern const char kCurrentLevelMsg[] = "Current Level: ";
extern const int kDelayHuman = 10;
extern const int kDelayMachine = 80;
extern const char kEmptyNullStr[] = "";
extern const char kLastMoveIndent[] = " <-----> ";
extern const char kDepthsAIMsg1[] = " Depths: ";
extern const char kDepthsAIMsg2[] = " Vs ";
extern const int kDelayAIPlayer = 500;
extern const char kLevelsNameError[] = "The number of "
                                       "levels of "
                                       "difficulty must be "
                                       "equal to the number"
                                       " of thinking "
                                       "depths specified in"
                                       " kLevelsArray.";
extern const char kOnGoingGameMove[] = "The game must"
                                       " be ongoing to"
                                       " process a move.";
extern const char kIDPlayerMap[] = "All players "
                                   "identifier must be "
                                   "stored in the "
                                   "std::map "
                                   "current_options_."
                                   "players_.";
}  // namespace Constants
