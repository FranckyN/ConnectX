//============================================================================
// Author      : Franck Nassé - February 05, 2016
// Version     : v1.0
// Copyright   : Copyright (c) 2016, Franck Nassé. All rights reserved.
// Description : Player class
//============================================================================
#ifndef CONNECTFOUR_PLAYER_H_
#define CONNECTFOUR_PLAYER_H_

#include <string>
#include <cstdint>

/**
 * @class Player
 * @author Franck Nassé
 * @file player.h
 * @brief This class represents a player in the game.
 * A player is identified by a name.
 */
class Player {
 public:
  explicit Player(const std::string& name) : name_(name) {}
  std::string GetName() const { return name_; }
 private:
  std::string name_;
};
#endif // CONNECTFOUR_PLAYER_H_