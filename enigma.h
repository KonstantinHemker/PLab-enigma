#ifndef ENIGMA_H
#define ENIGMA_H

#include <cstring>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

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


class InputSwitch;
class Plugboard;
class Rotor;


/*Helper function declarations */
bool check_no_parameters (int numberArguments);

/*Function that carries the error descriptions*/
const char* error_description (int code);

/*Function that checks the message input*/
int check_message(char message[]);



/*Class definitions*/

class BaseModule  {
 protected:
  string settings;
  vector<int> token;
  char letter;
 public:
  int check_numeric_char(char* filename);
  int create_tokens (char* filename);
  int get_token (int n)  {
    return token[n]; }
  void swap_values (char &current_char); //represents the wiring
  bool invalid_index ();
};



class InputSwitch {
 private:
  char letter;
 public:
  InputSwitch (char &l) : letter(l) {}
  void set_letter (int pos, char array[]) {
    letter = array[pos];}
  char return_letter()  {
    return letter; }
};


 class Plugboard : public BaseModule {
 private:
   char letter;
 public:
   Plugboard (char l, char* cl_argument) : letter(l) {
     create_tokens(cl_argument);
  }
  void set_letter (char l)  {
    letter = l; }
  int check_config(char* cl_input);
  char return_letter() {
    return letter; }
  };

class Reflector: public BaseModule {
 private:
  char letter;
 public:
  Reflector (char l, char* cl_argument) : letter(l) {
    create_tokens(cl_argument); }
  char get_letter() {
    return letter; }
  int check_config();
};


class Rotor : public BaseModule {
 private:
  int number; //refers to the number of the rotor (from right to left on the configuration)
  int top_position;
  int relative_position;
  char letter;
  vector<int> corr_token;
  vector<int> rotor_positions;
  int notch[26];
 public:
  void init_rotor(char* cl_argument)
    {
    create_tokens(cl_argument);
    set_notch();
    relative_position = top_position;
  }    
  void set_notch () {
    for (unsigned int n=25; n<=token.size()-1; n++)
      notch[n-25] = token[n+1];  }
  int get_notch(int n) {
    return notch[n]; }
  void rotor_inwards(char &current_char, Rotor* rotor, int noRotors, int a);
  void rotor_outwards(char &current_char, Rotor* rotor, int noRotors, int a);
  void load_top_position (int n, int nrotors) {
    top_position = rotor_positions[nrotors - (n+1)];  }
  void add_top_position(int n) {
    top_position += n; }
  int get_top_position() {
    return top_position; }
  void swap_values(char &current_char);
  int check_config(char* cl_input);
  int create_rot_position_tokens(char* cl_position);
  int check_rot_positions(int noRotors);
  void rotate_up(int i, Rotor* rotor);
  void reset_relative_position()  {
    relative_position = letter; }
  
};




#endif
