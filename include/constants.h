//============================================================================
// Author      : Franck Nassé - February 05, 2016
// Version     : v1.0
// Copyright   : Copyright (c) 2016, Franck Nassé. All rights reserved.
// Description : 
//============================================================================
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <vector>
#include <cstdint>

namespace Constants {
extern const char kClassicConnectFour[];
extern const char kFiveInRowConnectFour[];
extern const char k3By6ConnectThree[];
extern const char kTenBy7ConnectFour[];
extern const char kEightBy8ConnectFour[]; 
extern const std::vector<const char *> kGameVariantsEnum;
extern const std::size_t KClassic3By6Height;
extern const std::size_t KClassic3By6Width;
extern const std::size_t k1stDim;
extern const std::size_t k2ndDim;
extern const char kSinglePlayer [];
extern const char kTwoPlayers   [];
extern const char kTwoAIPlayers [];
extern const std::vector<const char *> kPlayerModeEnum;
extern const std::string kIndentScreen;
extern const char kColonPoint[];
extern const char k0EasyLevel[];
extern const char k1BeginnerLevel[];
extern const char k2IntermediateLevel[];
extern const char k3AdvancedLevel[];
extern const char kChooseKindMsg[];
extern const char kChooseModesMsg[];
extern const char kAIPlayer1NameMsg[];
extern const char kAIPlayer2NameMsg[];
extern const char kGetDepthMsg[];
extern const char kGetDepthMsg_1[];
extern const char kGetDepthMsg_2[]; 
extern const char kGetDepthMsg_3[];
extern const int kMinAIDepth;
extern const int kMaxAIDepth;  
extern const char kNameMsg[];
extern const char kPlayer1NameMsg[];
extern const char kPlayer2NameMsg[];
extern const char kEnterMoveMsg[];
extern const char kGetDifficultyMsg[];
extern const char kSpaceChar;
extern const char kAndStr[];
extern const char kNameMaxSize[];
extern const char kNameMinSize[];
extern const char kCharactersStr[];
extern const char kQuitGameCmd;
extern const char kNewLine;
extern const char kNullChar;
extern const char kQuitGameCmdStr[];
extern const char kUndoGameCmd;
extern const char kUndoGameCmdStr[];
extern const char kRestartCmd;
extern const char kRestartCmdStr[];
extern const char kYesCmd;
extern const char kNoCmd;
extern const char kZeroChar;
extern const char kEmptyChar;
extern const int8_t kFirstPlayerChipId;
extern const int8_t kWindowLines;
extern const int8_t kWindowColumns;
extern const std::size_t kLimitNamePlayerMin;
extern const std::size_t kLimitNamePlayerMax;
extern const std::size_t kClassicBoardNumRows;
extern const std::size_t kClassicBoardNumCols;
extern const uint8_t kClassicChipsConnected;
extern const char kTok1Star[];
extern const char kTok3Stars[];
extern const char kTok3Bubbles[];
extern const char kOddBubble[];
extern const std::vector<std::string> KPly1Symbol;
extern const std::vector<std::string> KPly2Symbol;
extern const char kLastMoveMsg[];
extern const char kTurnMsg1[];
extern const int kLastMoveBoardEmpty;
extern const std::size_t kAtomicUndoCounter;
extern const char kEndOfGameMsg_1[];
extern const char kEndOfGameMsg_2[];
extern const char kEndOfGameMsg_3[];
extern const char kDifficultyCmd;
extern const char  kDifficultyCmdStr[];
extern const char kDrawMsg[];
extern const char kWinnerMsg[];
extern const char kByeMsg[];
extern const char kPossibleMovesMsg[];
extern const char kMushuStudioMsg[];
extern const char kInstructionsMsg_1[];
extern const char kInstructionsMsg_2[];
extern const char kInstructionsMsg_3[];
extern const char kInstructionsMsg_4[];
extern const char kInstructionsMsg_5[];
extern const char kInstructionsMsg_6[];
extern const char kInstructionsMsg_7[];
extern const char kCurrentLevelMsg[];
extern const char kEmptyNullStr[];
extern const char kLastMoveIndent[];
extern const char kContinueCmd;
extern const char kContinueCmdStr[];
extern const int kDelayHuman;
extern const int kDelayMachine;
extern const char kDepthsAIMsg1[];
extern const char kDepthsAIMsg2[];
extern const int kDelayAIPlayer;
extern const char kLevelsNameError[];
extern const char kOnGoingGameMove[];
extern const char kIDPlayerMap[];
}

#endif //CONSTANTS_H 