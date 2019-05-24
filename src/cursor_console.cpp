// Author      : Franck Nassé - May 19, 2019
// Version     : v1.0
// Copyright   : Copyright (c) 2019, Franck Nassé. All rights reserved.
// Description : Manages Ncurses interaction
//============================================================================
#include <assert.h>
#include <curses.h>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include "constants.h"
#include "cursor_console.h"

using namespace Constants;

namespace UI {

// x is the column, y is the row. The origin (0,0) is top-left.
void SetCursorPosition(int x, int y) {
  move(y, x);
}

void GetCursorPosition(std::pair<int, int>& cursor) {
  refresh();
  getsyx(cursor.second, cursor.first);
}

void ClearWindowCurrentLine() {
  clrtoeol();
}

void FlashBeep(int n) {
  while(n--) {
    if(OK == beep()) {
      flash();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}

void Beep(int n) {
  while(n--) {
    beep();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}

void ClearWindowCurrentLine(int n) {
  while(n--) {
    ClearWindowCurrentLine();
    GotoNextWindowLine();
  }
}

void ClearRemainingWindow() {
  clrtobot();
}

void ClearWindowScreen() {
  wclear(stdscr);
}

void EnterGameMode() {
  initscr();
  resize_term(kWindowLines, kWindowColumns);
  scrollok(stdscr, false);
  SetCursorPosition(0, 0);
  RefreshWindow();
}

void CloseWindow() {
  endwin();
}

void WriteToWindow(const std::stringstream& message) {
  std::string line;
  std::stringstream local(message.str());
  while(std::getline(local, line, kNewLine)) {
    printw("%s\n", line.c_str());
  }
}

void WriteToWindow(const std::string& message) {
  printw("%s", message.c_str());
}

void WriteToWindow(const char& message) {
  addch(message);
}

void RefreshWindow() {
  refresh();
}

void GotoNextWindowLine() {
  addch(kNewLine);
}

void GetCharacter(std::string& input, int n) {
  assert(n > 0);
  keypad(stdscr, TRUE);
  char buffer_input[16];
  memset(buffer_input, kZeroChar, 16);
  getnstr(buffer_input, n);
  input = std::string(buffer_input);
}

}  // namespace UI
