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
//Precondition: Messages are entered by the user in a loop statement in main
//Postcodition: Checks whether each character is an upper case character from A-Z
void check_message_input (char message, int &errorCode);

/*Function that checks the command line input of the user*/
//Precondition: The enigma program is launched by the user in the command line
//Postcondition: Checks the command line input for validity with regards to number of arguments and
//existence of files entered in the command line
//This looks at two cases: 1) When there are no rotor configurations entered
//(i.e. a minimum of three arguments) or 2) when there are 
void check_command_line_input(int no_arguments, int noRotors, int &errorCode); 


//////////////////////////////////////////////////////////
//                  CLASS DEFINITIONS                   //
//////////////////////////////////////////////////////////


//This is an abstract class that carries certain attributes and operations that will be performed by all
//subclasses, such as loading the mapping and checking for some common potential error conditions (e.g. invalid
//index). In order to access the members of the abstract class, all attributes in this class are declared as
//protected.
class BaseModule  {
 protected:
  string settings;
  vector<int> mapping;
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
//Only novel features of this class compared to the abstract class are that
//it has different configurations to check
class Plugboard : public BaseModule {
 public:
  Plugboard (CharPtr clArgument, int &errorCode);
  void pass_through(char &message);
  void check_config(CharPtr clInput, int &errorCode);
};



/*Class definition for the rotors.*/
//Note that this class deliberately has no constructor, because we create an array of rotors to give the
//enigma setup more flexibility.
class Rotor : public BaseModule  {
 private:
  int top_position; //refers to the current offset from the "null position" in absolute terms
  vector<int> rotor_positions; //holds the rotor offset from the "null position" (0 at top) for all rotors
  int notch[26]; //holds the notches speified in the .rot position
 public:

  /*Function that initializes loads settings and sets notches of the rotor*/
  void init_rotor(CharPtr clArgument, int &errorCode);  

  /*Setter and getter functions for notches*/
  void set_notch (); 
  int get_notch(int n);

  /*Functions that adjusts up the current letter according to the offset*/
  //Required to adjust "up" if we are going into the rotor (entering "relative position mode")
  //Required to adjust "down" if we are going out of the rotor to "tell" the current
  //character where it is in space (i.e. in absolute position terms)
  void adjust_up (char &current_char);
  void adjust_down (char &current_char);

  /*Setter/getter/add function for the "top position", i.e. offset*/
  void set_top_position (int c, int noRotors, vector<int> posMapping, int &errorCode);
  int get_top_position();
  void add_top_position(int n);


  /*Recursive functions that process the letter coming before and after the reflector through all rotors */
  //Before the reflector, the letter written at each position in the .rot file
  //is mapped to the number of the position it is at (i.e. the number at the position 0
  //in the file +65 (in ASCII terms) represents the mapping of 0+65 ('A'). 
  void mapping_inwards(char &current_char, Rotor* rotor, int noRotors, int a);
  
  //After the reflector, the letter mapping is flipped. That is, the letter coming
  //is mapped to whatever position it can be found on in the .rot file. 
  void mapping_outwards(char &current_char, Rotor* rotor, int noRotors, int a);

  /*Function that checks the configurations of the .rot input file*/
  //Checks for the following error conditions of the .rot file corresponding to the instance of this rotor:
  //(1) Non-numeric character, (2) Invalid rotor mapping (3) Invalid index
  void check_config(CharPtr clInput, int &errorCode);

  /*Function that checks the positions of the .pos input file*/
  //Checks for the following error conditions of the .pos file
  //(1) Non-numeric character (2) Invalid Index (3) No rotor starting position
  void check_rot_positions(int noRotors, vector<int> posMapping, int &errorCode, CharPtr clArgument);

  /*Recursive function that rotates the rotors */
  //This will shift the rightmost rotor in every case the function is invoked and checks whether
  //there are notches meeting the top position of the rotor one to its left. If the top position
  //of the rotor to the left meets a notch of the rotor in question, the rotor to the left rotates
  //by one position as well.
  //The function recursively keeps checking the condition whether any notch of any rotor meets the top
  //position of a rotor to its left and rotates them if this is the case
  void rotate_up(int i, Rotor* rotor, int noRotors);
};

#endif


/*Class definition of reflector as a child class of BaseModule*/

class Reflector: public BaseModule  {
 public:
  /*Reflector that refers to member functions of the abstract parent class*/
  Reflector(CharPtr clArguments, int &errorCode);

  /*Function that processes the letter for the reflector*/
  //This function just refers to the swap_values function in the abstract base class
  //void pass_through(char &message, int n);

  
  void check_config(CharPtr clInput, int &errorCode);    
};

