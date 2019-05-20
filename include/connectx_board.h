//============================================================================
// Author      : Franck Nassé - February 05, 2016
// Version     : v1.0
// Copyright   : Copyright (c) 2016, Franck Nassé. All rights reserved.
// Description : 
//============================================================================
#ifndef CONNECTX_BOARD_H_
#define CONNECTX_BOARD_H_

#include "text_board.h"
#include <list>

namespace UI {
 /*A ConnectX board is currently a two-dimensional grid, usually with 6 rows and
 * 7 columns.
 * A given position is identified by their coordinates in a 
 * std::vector<std::size_t>
 * (x, y), because it was decided that the design should not assume a 2D space 
 * layout but instead take into account more dimensions (3D, 4D, etc.).
 * 
 * Coordinate System:
 * In the grid below the chip X, located in the second column and third row, has
 * the following coordinates:(2, 1)
 *   
 *   |_0_|_1_|_2_|_3_|_4_|_5_|_6_|
 * 0 |___|___|___|___|___|___|___|
 * 1 |___|___|___|___|___|___|___|
 * 2 |___|_X_|___|___|___|___|___|
 * 3 |___|___|___|___|___|___|___|
 * 4 |___|___|___|___|___|___|___|
 * 5 |___|___|___|___|___|___|___|
 *
 */
class ConnectXBoard : public TextBoard {
 public:
  ConnectXBoard(const std::map<int8_t, TextPiece>& chips, 
                const std::vector<std::size_t>& dimensions,
                const std::string& line_indent);
  ~ConnectXBoard() {}
  void SetConnectedChips(const std::list<std::vector<std::size_t> >& positions,
                         int8_t ship_id);
  void UnSetConnectedChips(
      const std::list<std::vector<std::size_t> >& positions,
      int8_t ship_id);
  void ClearBoard();
  
 private:
  void PrintColumnNumber();
  void ChangeBackgroundCell(const std::vector<std::size_t>& position,
                            int8_t piece_ID, char replaced_char, char new_char);
};
}
#endif  //CONNECTX_BOARD_H_