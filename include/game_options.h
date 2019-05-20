//============================================================================
// Author      : Franck Nassé - February 05, 2019
// Version     : v1.0
// Copyright   : Copyright (c) 2019, Franck Nassé. All rights reserved.
// Description : GameOption structrure
//============================================================================
#ifndef GAME_OPTIONS_H
#define GAME_OPTIONS_H

#include <cstddef>
#include <map>
#include "player.h"

  struct GameOptions {
    std::size_t kind_;
    std::size_t player_mode_;
    std::map<int8_t, Player> players_;
    uint8_t difficulty_;
    std::map<int8_t, int8_t> simulation_depths_;
    int8_t firstplayer_;
  };

#endif // GAME_OPTIONS_H
