/** @file constants.h */

#ifndef CONSTANS_H
#define CONSTANS_H


/** @brief Enum that takessymbolizes the states of the Smart Security Box (mainly used in main.ino). */
enum State {IDLE = 0, ENTERING_PASSWORD = 1, ACCESS_DENIED = 2, WRONG_PASSWORD = 3, CORRECT_PASSWORD = 4, BOX_OPEN = 5};


#endif
