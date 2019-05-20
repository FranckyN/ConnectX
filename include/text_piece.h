//============================================================================
// Author      : Franck Nassé - February 05, 2016
// Version     : v1.0
// Copyright   : Copyright (c) 2016, Franck Nassé. All rights reserved.
// Description : TextPiece
//============================================================================
#ifndef CONNECTFOUR_TEXT_PIECE_H_
#define CONNECTFOUR_TEXT_PIECE_H_

#include <string>
#include <vector>
#include <cstdint>

namespace UI
{
/**
 * @class TextPiece
 * @brief This class represents a textual representation of a chip aimed to be displayed in a console.
  A chip is represented by a symbol and an integer identifier
 */
class TextPiece {
    public:
  //each element of SymbolType is a line on the console
  typedef std::vector<std::string> SymbolType;
  
  TextPiece() {};
  
  TextPiece(const SymbolType& symbols, int8_t identifier)
    : symbols_(symbols), identifier_(identifier) {}
  
  const SymbolType& GetSymbol() const { return symbols_; }
  int8_t GetIdentifier() const { return identifier_; }
  
 private:
  SymbolType symbols_;
  int8_t   identifier_;
};
}
#endif //CONNECTFOUR_TEXT_PIECE_H_