#ifndef ENIGMA_H
#define ENIGMA_H

#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

/* Error codes  */
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

class Plugboard;
class Rotor;
class Reflector;


/*Function that checks the enigma setup*/
//Precondition: All elements of the engima machine have been defined as subclasses of the abstract class BaseModule
//Postcondition: Function sequentially calls the check functions for the command line input, the plugboard,
//rotor and reflector configurations and passes the error code into these functions which is set according to
//the error condition. 
void check_enigma_setup (int clArguments, char* argv[], int noRotors, int &errorCode, Plugboard &plugboard, Rotor* rotor, Reflector &reflector, vector<int> posMapping);

/*Function that initialises the rotors*/
//Precondition: At least one instance of class Rotor was created
//Postcondition: Initializes the rotors by calling the functions that load their settings and set their
//starting position and notches. 
void initialize_rotors(int noRotors, CharPtr clArgument[], int &errorCode, vector<int> &posMapping, Rotor* rotor);

/*Function that creates all rotor position mappings */
//Precondition: At least one instance of class Rotor was created
//Postcondition: Reads the rotor positions for all rotors created from the file specified in the command
//line argument into the "posMapping" integer vector
void load_rotor_positions(CharPtr cl_position, vector<int> &posMapping, int &errorCode, Rotor* rotor);

/*Function that sets the rotor positions across all rotors*/
//Precondition: An instance of class rotor was created and the posMapping vector carries the position values
//required for the vector
//Postcondition: The top position of each instance of the rotor class is set to the corresponding value in the
//.pos file. Note that this function uses recursion to set the top position of however many rotors are created
void set_rotor_positions(int c, vector<int> posMapping, Rotor* rotor, int noRotors, int &errorCode);

/*Function which checks the input by the program user*/
//Precondition: 
//Postcodition: 
void check_message_input (char message, int &errorCode);

/*Function that checks the command line input of the user*/
void check_command_line_input(int no_arguments, int noRotors, int &errorCode); 


/*Class definitions*/
//This is an abstract class that carries certain attributes and operations that will be performed by all
//subclasses, such as loading the mapping and checking for some common potential error conditions (e.g. invalid
//index). In order to access the members of the abstract class, all attributes in this class are declared as
//protected.
class BaseModule  {
 protected:
  string settings;
  vector<int> mapping;
  char letter;
  bool empty;
 public:
  void check_numeric_char (CharPtr filename, int &errorCode);
  bool invalid_index ();
  void load_mappings (CharPtr filename, int &errorCode);
  int get_mapping_size() {
    return mapping.size(); }   
  void swap_values (char &current_char); //represents the wiring
  bool is_valid(CharPtr filename, int &errorCode);
  virtual void check_config(CharPtr clInput, int &errorCode) = 0;
};

/*Class definition of plugboard as a child class of BaseModule*/

class Plugboard : public BaseModule {
 public:
  Plugboard (CharPtr clArgument, int &errorCode) {
     load_mappings (clArgument, errorCode); }
   void pass_through(char &message)  {
     set_letter(message);
     swap_values(message); }
   void set_letter (char l) {
     letter = l; }
   void check_config(CharPtr clInput, int &errorCode);
};



/*Class definition of reflector as a child class of BaseModule*/
class Reflector: public BaseModule  {
 public:
  Reflector(CharPtr clArguments, int &errorCode)  {
      load_mappings(clArguments, errorCode); }
  void pass_through(char &message, int n)  {
    letter = message;
    swap_values(message); }
  char get_letter() {
    return letter; }
  void check_config(CharPtr clInput, int &errorCode);    
};


/*Class definition for the rotors.*/
//Note that this class deliberately has no constructor, because we create an array of rotors to give the
//enigma setup more flexibility.
class Rotor : public BaseModule  {
 private:
  int top_position;
  vector<int> rotor_positions;
  int notch[26];
 public:
  void init_rotor(CharPtr clArgument, int &errorCode) {
    load_mappings(clArgument, errorCode);
    if (errorCode > 0)
      return;
    set_notch(); }    
  void set_notch ()  {
    for (unsigned int n=25; n <= mapping.size()-1; n++)
      notch[n-25] = mapping[n+1]; }
   int get_notch(int n)  {
     return notch[n]; }
   void rotor_inwards(char &current_char, Rotor* rotor, int noRotors, int a);
   void rotor_outwards(char &current_char, Rotor* rotor, int noRotors, int a);
   void set_top_position (int c, int noRotors, vector<int> posMapping, int &errorCode)  {
     if (posMapping.size() > 0)
      top_position = posMapping[c]; }
   void add_top_position(int n) {
     top_position += n; }
   int get_top_position() {
     return top_position;  }
   void swap_values(char &current_char);
   void check_config(CharPtr clInput, int &errorCode);
   void check_rot_positions(int noRotors, vector<int> posMapping, int &errorCode, CharPtr clArgument);
   void rotate_up(int i, Rotor* rotor, int noRotors);
   void adjust_up (char &current_char);
   void adjust_down (char &current_char);
};

#endif
