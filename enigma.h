#ifndef ENIGMA_H
#define ENIGMA_H

/* error codes  */
#define INSUFFICIENT_NUMBER_OF_PARAMETERS		1
#define INVALID_INPUT_CHARACTER				2
#define INVALID_INDEX					3
#define NON_NUMERIC_CHARACTER				4
#define IMPOSSIBLE_PLUGBOARD_CONFIGURATION		5
#define INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS	6
#define INVALID_ROTOR_MAPPING				7
#define NO_ROTOR_STARTING_POSITION			8
#define INVALID_REFLECTOR_MAPPING			9
#define INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS	10
#define ERROR_OPENING_CONFIGURATION_FILE		11

#define NO_ERROR					0

typedef char* CharPtr;


/*Helper function declarations */
bool check_no_parameters (int numberArguments);


/*Class definitions*/

class InputSwitch {
 private:
  CharPtr letter;
 public:
  InputSwitch (char &l) : letter(&l) {}
  char return_letter()  {
    return *letter; }
};


class Plugboard  {
 private:
  char letter;
  char settings[512];
 public:
  Plugboard (char l, char set[]);
  //void swap_values;
  //void get_settings;
};
  






#endif
