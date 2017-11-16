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
int check_no_parameters (int numberArguments, int noRotors);

/*Function that carries the error descriptions*/
const char* error_description (int code);

/*Function that checks the message input*/
int check_message(char message);

/*Function that creates all rotor position tokens */
void create_rot_position_tokens(CharPtr cl_position, vector<int> &pos_token);

/*Function that sets the rotor positions across all rotors*/
void set_rotor_positions(int n, vector<int> pos_token, Rotor* rotor, int noRotors);

/*Function which checks the input by the program user*/
void check_message_input (char message, int &error_code);

/*Function that checks the command line input of the user*/
void check_command_line_input(int no_arguments, int noRotors, int &error_code); 


/*Class definitions*/

class BaseModule  {
 protected:
  string settings;
  vector<int> token;
  char letter;
  bool empty;
 public:
  void check_numeric_char (CharPtr filename, int& error_code);
  int load_tokens (CharPtr filename);
  int get_token (int n)  {
    return token[n]; }
  void swap_values (char &current_char); //represents the wiring
  bool invalid_index ();
  bool is_valid (CharPtr filename);
};


class Plugboard : public BaseModule {
 private:
   char letter;
 public:
   Plugboard (CharPtr cl_argument)
     {
       load_tokens (cl_argument);
     }
   void pass_through(char &message)
   {
     set_letter(message);
     swap_values(message);
   }
   void set_letter (char l)
   {
     letter = l;
   }
   void check_config(CharPtr cl_input, int &error_code);
   
   char return_letter() {
     return letter; }
};



class Reflector: public BaseModule {
 private:
  char letter;
 public:
  Reflector(CharPtr cl_arguments) {
    load_tokens(cl_arguments); }
  void pass_through(char &message, int n) {
    letter = message;
    swap_values(message);
  }
  Reflector (char l, CharPtr cl_argument) : letter(l) {
    load_tokens(cl_argument); }
  char get_letter() {
    return letter; }
  void check_config(CharPtr cl_input, int &error_code);
};


class Rotor : public BaseModule {
 private:
  int top_position;
  char letter;
  vector<int> rotor_positions;
  int notch[26];
 public:
  void init_rotor(CharPtr cl_argument)
    {
    load_tokens(cl_argument);
    set_notch();
    }    
  void set_notch () {
    for (unsigned int n=25; n<=token.size()-1; n++)
      notch[n-25] = token[n+1];  }
  int get_notch(int n) {
    return notch[n]; }
  void rotor_inwards(char &current_char, Rotor* rotor, int noRotors, int a);
  void rotor_outwards(char &current_char, Rotor* rotor, int noRotors, int a);
  void set_top_position (int n, int noRotors, vector<int> pos_token) {
    top_position = pos_token[noRotors - (n+1)];
  }
  void add_top_position(int n) {
    top_position += n; }
  int get_top_position() {
    return top_position; }
  void swap_values(char &current_char);
  int check_config(CharPtr cl_input, int &error_code);
  int check_rot_positions(int noRotors, vector<int> pos_token);
  void rotate_up(int i, Rotor* rotor, int noRotors);
};




#endif
