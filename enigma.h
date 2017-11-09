#ifndef ENIGMA_H
#define ENIGMA_H

#include <cstring>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

/* error codes  */
#define INSUFFICIENT_NUMBER_OF_PARAMETERS		12
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

/*Function that loads rotor positions*/
void create_rot_position_tokens(char* cl_position, vector<int> &rot_positions);



/*Function that rotates the rotors*/
void rotate_up(int i, Rotor* rotor, vector<int> v);


/*Class definitions*/

class BaseModule  {
 protected:
  string settings;
  vector<int> token;
 public:
  //void load_settings(char* filename);
  string return_settings() {
    return settings;}
  int create_tokens (char* filename);
  int get_token (int n)  {
    return token[n]; }
  void swap_values (char &current_char); //represents the wiring
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
    //load_settings(cl_argument);
    create_tokens(cl_argument);
  }
  void set_letter (char l)  {
    letter = l; }
  int check_config();
  char return_letter() {
    return letter; }
  //void get_settings;
};


class Rotor : public BaseModule {
 private:
  int top_position;
  char letter;
  vector<int> corr_token;
  int notch[26];
 public:
  void init_rotor(char* cl_argument)
    {
      //load_settings(cl_argument);
    create_tokens(cl_argument);
    set_corr_token();
    set_notch();
  }    
  void set_notch () {
    for (unsigned int n=25; n<=token.size()-1; n++)
      notch[n-25] = token[n+1];  }
  int get_notch(int n) {
    return notch[n]; }
  int get_corr_token(int n) {
    return corr_token[n]; }
  void set_corr_token()  {
    for (int n=0; n<=25; n++)
      corr_token.push_back(n); }
  void set_letter(char &current_char);
  char get_letter() {
    return letter; }
  void load_top_position (int n, vector<int> &positions, int nrotors) {
    top_position = positions[nrotors - n];  }
  void add_top_position(int n) {
    top_position += n; }
  int get_top_position() {
    return top_position; }
  void swap_values(char &current_char);
};




#endif
