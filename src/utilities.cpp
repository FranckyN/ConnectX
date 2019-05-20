//============================================================================
// Author      : Franck Nassé - February 05, 2016
// Version     : v1.0
// Copyright   : Copyright (c) 2016, Franck Nassé. All rights reserved.
// Description : Implementation of some of the methods declared in utilities.h
//============================================================================
#ifdef __cplusplus__
#include <cstdlib>
#else
#include <stdlib.h>
#endif
#include "utilities.h"

// returns the number of digits
std::size_t number_digits(std::size_t num_index) {
  std::size_t num_digits = 0;
  if(num_index == 0) {
    num_digits = 1;
  } else {
    while(num_index) {
      num_index /= 10;
      num_digits++;
    }
  }
  return num_digits;
}