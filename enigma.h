#ifndef ENIGMA_H
#define ENIGMA_H

#include <cstring>
#include <string>
#include <vector>
#include <sstream>


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

using namespace std;

class InputSwitch;
class Plugboard;
class Rotor;
class Reflector;

/*Helper function declarations */
int check_no_parameters (int numberArguments, int noRotors);

/*Function that checks the enigma setup*/
void check_enigma_setup (int &n, int cl_arguments, char* argv[], int noRotors, int &error_code, string &class_type, Plugboard &plugboard, Rotor* rotor, Reflector &reflector, vector<int> pos_token);


/*Function that carries the error descriptions*/
void error_description (int code, string class_type, CharPtr cl_argument[], int n, Reflector &reflector);

/*Function that checks the message input*/
int check_message(char message);

//void initialize_rotors(int noRotors, CharPtr cl_argument[], int &error_code, vector<int> &pos_token, Rotor rotor[]);

/*Function that creates all rotor position tokens */
void load_rotor_positions(CharPtr cl_position, vector<int> &pos_token, int &error_code);

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
  void check_numeric_char (CharPtr filename, int &error_code);
  void load_tokens (CharPtr filename, int &error_code);
  int get_token (int n)  {
    return token[n]; }
  int get_token_size() {
    return token.size(); }   
  void swap_values (char &current_char); //represents the wiring
  bool invalid_index ();
  bool is_valid(CharPtr filename, int &error_code);
          
};


class Plugboard : public BaseModule {
 private:
   char letter;
 public:
   Plugboard (CharPtr cl_argument, int &error_code)
     {
       load_tokens (cl_argument, error_code);
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



class Reflector: public BaseModule
{
 private:
  char letter;
 public:
  Reflector(CharPtr cl_arguments, int &error_code)
    {
      load_tokens(cl_arguments, error_code);
    }
  void pass_through(char &message, int n)
  {
    letter = message;
    swap_values(message);
  }
  char get_letter()
  {
    return letter;
  }
  void check_config(CharPtr cl_input, int &error_code);
 
      
};


class Rotor : public BaseModule
{
 private:
  int top_position;
  char letter;
  vector<int> rotor_positions;
  int notch[26];
 public:
   void init_rotor(CharPtr cl_argument, int &error_code)
    {
      load_tokens(cl_argument, error_code);
      if (error_code > 0)
	return;
      set_notch();
    }    
  void set_notch ()
  {
    for (unsigned int n=25; n <= token.size()-1; n++)
      notch[n-25] = token[n+1];
  }
  int get_notch(int n) {
    return notch[n]; }
  void rotor_inwards(char &current_char, Rotor* rotor, int noRotors, int a);
  void rotor_outwards(char &current_char, Rotor* rotor, int noRotors, int a);
  void set_top_position (int n, int noRotors, vector<int> pos_token) {
    top_position = pos_token[n];
  }
  void add_top_position(int n)
  {
    top_position += n;
  }
  int get_top_position()
  {
    return top_position;
  }
  void swap_values(char &current_char);
  void check_config(CharPtr cl_input, int &error_code);
  void check_rot_positions(int noRotors, vector<int> pos_token, int &error_code, CharPtr cl_argument);
  void rotate_up(int i, Rotor* rotor, int noRotors);
  void adjust_up (char &current_char);
  void adjust_down (char &current_char);
};




#endif
