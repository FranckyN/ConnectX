//============================================================================
// Author      : Franck Nassé - February 05, 2016
// Version     : v1.0
// Copyright   : Copyright (c) 2016, Franck Nassé. All rights reserved.
// Description : Contains some utility functions to interact with the user
//============================================================================
#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "constants.h"
#include <type_traits>
#include "cursor_console.h"

/**
 * @brief Display a message to the user and get the input
 * @param temp_value: value entered by user 
 * @param message message displayed to the user
 * @param functor The functor here, is used to check the value is valid
 */
template<class T, typename F>
void GetValueFromConsoleUser(T & temp_value, const std::string& message, 
                             const F& functor) {
  std::stringstream s_stream;
  std::string temp_str;
  do {
    s_stream.clear();
    UI::WriteToWindow(message);
    UI::GetCharacter(temp_str);
    s_stream.str(temp_str);
    s_stream >> temp_value;
  } while(!functor(temp_value) || s_stream.fail());
}
/**
 * @brief This method retrieves an option selected by the user.
 * In this case the user has to choose by typing a number then pressing Enter.
 * @param names vector containing the choices
 * @param num_option output argument holding the value chosen by the user
 * @param message the question to display
 * @return 
 */
template<typename T>
typename std::enable_if<std::is_integral<T>::value>::type 
GetSelectedOptions(const std::vector<const char*>& names, T& num_option, 
                   const std::string& message) {
  std::stringstream stemp;
  stemp << Constants::kIndentScreen << std::endl;
  for (std::size_t i = 1; i <= names.size(); i++) {
    stemp << Constants::kIndentScreen << i << Constants::kColonPoint << 
        names[i - 1] << std::endl;
  }
  stemp << Constants::kIndentScreen << std::endl;
  UI::WriteToWindow(stemp);
  UI::RefreshWindow();
  int temp_int;
  int upper_bound = names.size();
  GetValueFromConsoleUser(temp_int,Constants::kIndentScreen + message,
                          [upper_bound]
                          (const int& x) { 
                            return (x > 0 && x <= upper_bound);
                          });
  num_option = static_cast<T>(temp_int - 1);
}


std::size_t number_digits(std::size_t num_index);

#endif //UTILITIES_H


