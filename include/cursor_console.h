//============================================================================
// Author      : Franck Nassé - May 19, 2019
// Version     : v1.0
// Copyright   : Copyright (c) 2019, Franck Nassé. All rights reserved.
// Description : Handle cursor in console
//============================================================================
#ifndef CURSOR_CONSOLE_H_
#define CURSOR_CONSOLE_H_

#include <utility>
#include <sstream>
#include "constants.h"

namespace UI {
void EnterGameMode();
void GetCursorPosition(std::pair<int, int>&);
void GotoNextWindowLine();
void SetCursorPosition(int, int);
void CloseWindow();
void WriteToWindow(const std::string & message);
void WriteToWindow(const std::stringstream & message);
void WriteToWindow(const char & message);
void RefreshWindow();
void ClearWindowCurrentLine();
void ClearWindowCurrentLine(int);
void ClearRemainingWindow();
void ClearWindowScreen();
void GetCharacter(std::string&, int = Constants::kLimitNamePlayerMax);
void FlashBeep(int n);
}  // namespace UI
#endif