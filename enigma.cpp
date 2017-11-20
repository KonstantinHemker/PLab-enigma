/* Implementation file of the enigma program */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <vector>

#include "enigma.h"


////////////////////////////////////////////////////////
//                  GENERAL FUNCTIONS                 //
////////////////////////////////////////////////////////


/*Function that checks all enigma setup errors */
void check_enigma_setup (int clArguments, char* argv[], int noRotors, int &errorCode, Plugboard &plugboard, Rotor* rotor, Reflector &reflector, vector<int> posMapping)  {
  check_command_line_input(clArguments, noRotors, errorCode);
  if (errorCode > 0)
    return;

  plugboard.check_config(argv[1], errorCode);
  if (errorCode > 0)
    return;
  
  if (noRotors > 0) {//only checks for rotor conditions if rotors exist

    rotor[0].check_rot_positions(noRotors, posMapping, errorCode, argv[noRotors+3]); 
    if (errorCode > 0)
      return;
    
    for (int c = 0 ;c < noRotors; c++)  {
      rotor[c].check_config(argv[c+3], errorCode);
      if (errorCode > 0)
	return;
    }
  }
  reflector.check_config(argv[2], errorCode);
}
  

/*Function that initialises the array of rotors*/
void initialize_rotors(int noRotors, CharPtr clArgument[], int &errorCode, vector<int> &posMapping, Rotor* rotor) {
  if (noRotors > 0) {
    for (int c = 0; c < noRotors; c++)
      rotor[c].init_rotor(clArgument[c+3], errorCode);
    
    //Sets rotor starting position mappings and check their validity
    load_rotor_positions(clArgument[noRotors+3], posMapping, errorCode, rotor);
    set_rotor_positions(0, posMapping, rotor, noRotors, errorCode);
  }
}


/*Function that creates the position mappings for all rotors in the system*/
void load_rotor_positions(CharPtr cl_position, vector<int> &posMapping, int &errorCode, Rotor* rotor)  {
  ifstream pos_input;
  if (rotor[0].is_valid(cl_position, errorCode) == false)
    return;
  
  pos_input.open (cl_position);
  if (pos_input.fail())  {
    errorCode = ERROR_OPENING_CONFIGURATION_FILE;
    return;
  }
  int n;
  while (pos_input >> n)  {
    posMapping.push_back(n);
  }
  pos_input.close();
}


/*Recursive function that sets the rotor positions for each rotor*/
void set_rotor_positions(int c, vector<int> posMapping, Rotor* rotor, int noRotors, int &errorCode)  {
  rotor[c].set_top_position(c, noRotors, posMapping, errorCode);
  c++;
  if (c < noRotors)
    set_rotor_positions(c, posMapping, rotor, noRotors, errorCode);
}

/*Function that checks message input entered in the command line*/
void check_message_input (char message, int &errorCode)  {
  if (isupper(message) == 0)  {
    errorCode = 2;
    cerr << message << " is not a valid input character (input characters must";
    cerr << " be upper case letters A-Z)!" << endl;
    }
  else
    errorCode = NO_ERROR;
}
  
/*Functions that checks the validity of the arguments entered in the command line*/
void check_command_line_input (int no_arguments, int noRotors, int &errorCode)
{
  if (((noRotors == 0) && (no_arguments < 3)) || ((noRotors > 0) && (no_arguments < 5)))  {
    errorCode = INSUFFICIENT_NUMBER_OF_PARAMETERS;
    cerr << "usage: enigma plugboard-file reflector-file (<rotor-file>* rotor-positions)?" << endl;
    return;
  }

  if (errorCode == 11)  {
    cerr << "Error opening configuration file." << endl;
    return;
  }
}


////////////////////////////////////////////////////////
// MEMBER FUNCTIONS OF ABSTRACT CLASS "BASEMODULE"    //
///////////////////////////////////////////////////////


/*Function that loads the mapping*/
void BaseModule::load_mappings (CharPtr filename, int& errorCode)  {
  if (is_valid(filename, errorCode) == false) {
    return;	      
  }
  ifstream enigmasettings;
  int n;
  enigmasettings.open(filename);
  while (enigmasettings >> n) {
    mapping.push_back(n);
    }
  enigmasettings.close();
}


/*Function that checks whether an input file contains a non-numeric character*/
void BaseModule::check_numeric_char(CharPtr filename, int &errorCode)  {
  ifstream enigmasettings;
  enigmasettings.open(filename);
  istreambuf_iterator<char> eos;
  string temp (istreambuf_iterator<char>(enigmasettings), eos );
  enigmasettings.close();
  string settings = temp; 
  //Check non numeric character
  for (unsigned int c=0; c< settings.size(); c++)  {
    if (((settings[c] <= 57) && (settings[c]>=48)) || (settings[c] == 32) || (settings[c] == '\n'))
      //48 and 57 refer to lowest an highest valid ASCII digits, 32 is the whitespace character
      {}
    else
      errorCode = NON_NUMERIC_CHARACTER;
  }
}
   

/*Function that checks for further validity of the input file stream*/
bool BaseModule::is_valid (CharPtr filename, int &errorCode)   {
  ifstream inStream;
  inStream.open(filename);
  if (inStream.fail())  {
      errorCode = ERROR_OPENING_CONFIGURATION_FILE; 
      return false;
    }
  inStream.seekg(0, inStream.end);
  int n = inStream.tellg();
  inStream.close();
  if (n == 0)  { // checks whether the file is an empty file (avoid potential segmentation fault)
    empty = true;  
      return false;
    }
  else
    empty = false; 

  return true;
}

/*Function that changes the value of the current character to its mapped character*/
void BaseModule::swap_values (char &current_char)  {

  if (empty == true) //Avoids seg fault if the file was empty
    return;

  for (unsigned int i=0; i <= mapping.size(); i++)   {
    if ((mapping[i]+65 == current_char) && (i%2 == 0))   { //+65 converts to ASCII value
      current_char = mapping[i+1]+65;
      return;
	}
    if ((mapping[i]+65 == current_char) && (i%2 == 1))  //same as above
      current_char = mapping[i-1]+65;
    }
}

/*Function that checks for an invalid index number in configuration file*/
bool BaseModule::invalid_index ()  {
  for (unsigned int n=0; n <= mapping.size()-1  ; n++) {
      if ((mapping[n] > 25) || (mapping[n] < 0))
	return true;
  }  
  return false;
}


////////////////////////////////////////////////////////
//        MEMBER FUNCTIONS OF CLASS "PLUGBOARD"       //
////////////////////////////////////////////////////////

/*Constructor for the plugboard*/
Plugboard::Plugboard(CharPtr clArgument, int &errorCode) {

  load_mappings (clArgument, errorCode);
}



/*Function that checks all possible error conditions for the plugboard*/
void Plugboard::check_config(CharPtr clInput, int &errorCode)   {

  if (empty == true)
    return;
  
  //Check for NON_NUMERIC_CHARACTER
  check_numeric_char(clInput, errorCode);
  if(errorCode != 0)  {
      cerr << "Non-numeric character in plugboard file " << clInput << endl;
      return;
    }
  //Check for INCORRECT_NUMBER_OF_PB_PARAMETERS
  if (mapping.size()%2 == 1)  {
      errorCode = INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
      cerr << "Incorrect number of parameters in plugboard file " << clInput << endl;
      return;
    }
  //Check for IMPOSSIBLE_PLUGBOARD_CONFIGURATION
  for (unsigned int i=0; i<=mapping.size()-1; i++)  {
      for (unsigned int c=0; c<=mapping.size()-1; c++) {
	if ((mapping[i] == mapping[c]) && (c != i)) {
	  errorCode = IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
	  cerr << "Impossible plugboard configurations in file " << clInput << endl;
	  return;
	}
      }  //Impossible plugboard configuration
    }
  //Check for INVALID INDEX
  if (invalid_index() == true)  {
      errorCode = INVALID_INDEX;
      cerr << "Invalid index in plugboard file " << clInput << endl;
      return;
  } 
}



////////////////////////////////////////////////////////
//         MEMBER FUNCTIONS OF CLASS "ROTOR"          //
////////////////////////////////////////////////////////

/*Pooling function that initializes loads settings and sets notches of the rotor*/
void Rotor:: init_rotor(CharPtr clArgument, int &errorCode) {
  load_mappings(clArgument, errorCode);
  if (errorCode > 0)
    return;
  set_notch();
}


/*Setter/getter functions for notches*/
void Rotor::set_notch ()  {
  for (unsigned int n=25; n <= mapping.size()-1; n++)
    notch[n-25] = mapping[n+1];
}

int Rotor:: get_notch(int n)  {
  return notch[n];
}

/*Setter/getter/add function for the "top position", i.e. offset*/
void Rotor::set_top_position (int c, int noRotors, vector<int> posMapping, int &errorCode)  {
  if (posMapping.size() > 0)
    top_position = posMapping[c];
}

int Rotor::get_top_position() {
  return top_position;
}

void Rotor::add_top_position(int n) {
  top_position += n;
}


/*Functions that adjust for the current offset of the rotor*/
void Rotor::adjust_up(char& current_char)  {
  if (current_char + top_position > 90) //90 is the ASCII value for 'Z'
    current_char += top_position + -26;
  //adjusts with -26 in case the relative position of the rotor would pass Z in relative terms
  else
    current_char += top_position;
}

void Rotor::adjust_down(char&current_char)  {
  if (current_char - top_position < 65) 
    current_char = current_char - top_position + 26;
  //adjusts with +26 in case the relative position would pass Z in relative terms
  else  
    current_char = current_char - top_position;
}



void Rotor::mapping_inwards (char& current_char, Rotor* rotor, int noRotors, int a) {
  if (noRotors == 0)
    return;

  adjust_up(current_char);

  //Swap values according to mapping going into the reflector
  for(int i = 0; i <= 25; i++)  {
    if (i == current_char - 65)  {
      current_char = mapping[i] + 65;
      break;
    }
  } 

  adjust_down(current_char);
  
  a--;
  if (a >= 0)
    rotor[a].mapping_inwards(current_char, rotor, noRotors, a);
}


void Rotor::mapping_outwards(char &current_char, Rotor* rotor, int noRotors, int a) {
  if (noRotors == 0)
    return;
  
  adjust_up(current_char);

  for (int i = 0; i<=25; i++)  {
    if ((mapping[i] == current_char-65)) {
      current_char = i + 65;
      break;
    }
  }
 
  adjust_down(current_char);
  
  a++;
  if (a < noRotors)
    rotor[a].mapping_outwards(current_char, rotor, noRotors, a);
}




/*Function that checks the configurations of the .rot input file*/
void Rotor::check_config (CharPtr clInput, int &errorCode) {
  //Check NON_NUMERIC_CHARACTER
  check_numeric_char(clInput, errorCode);
  if (errorCode != 0)   {
    cerr << "Non-numeric character for mapping in rotor file " << clInput << endl;
    return;
  }
  
  //Check INVALID ROTOR MAPPING (too few inputs)
  if (mapping.size()<=26)   {
    errorCode = INVALID_ROTOR_MAPPING;
    cerr << "Not all inputs mapped in rotor file: " << clInput << endl;
    return;
  }
  
  
  //Check INVALID_ROTOR_MAPPING (double mapping)
  for (int c=0; c<=25;c++)  {
    for (int i=0; i<=25; i++) 	{
      if ((mapping[i] == mapping[c]) && (c!=i))  {
	errorCode = INVALID_ROTOR_MAPPING;
	cerr << "Invalid mapping of input " << i << " to output " << mapping[i];
	cerr << " (output " << mapping[i] << " is already mapped to from input " << c << ")" << endl;
	return;
      }
    }
  }
  
  //Check INVALID_INDEX 
  if (invalid_index() == true)  {
    cerr << "Invalid index in rotor file " << clInput << endl;
    errorCode = INVALID_INDEX;
  }
}



/*Function that checks the positions of the .pos input file*/
void Rotor::check_rot_positions(int noRotors, vector<int> posMapping, int &errorCode, CharPtr clArgument) {

  int vsize = posMapping.size();

  //Check NON NUMERIC CHARACTER
  check_numeric_char(clArgument, errorCode);
  if (errorCode != 0)  {
    cerr << "Non-numeric character in rotor positions file " << clArgument << endl;
    return;
  }
  //Check for INVALID INDEX
  for (int i=0; i<vsize; i++) {
    if (posMapping[i] > 25 || posMapping[i] < 0)  {
      cerr << "Invalid index in rotor position file " << clArgument << endl;
      errorCode = INVALID_INDEX;  //INVALID INDEX code
      return;
    }
  }
  
  //Check for NO_ROTOR_STARTING_POSITION
  if (noRotors <= vsize)
    return;
  else  {
    errorCode = NO_ROTOR_STARTING_POSITION;
    cerr << "No starting position for rotor 0 in rotor position file: " << clArgument << endl;
  }
}



/*Recursive function that rotates the rotors */
void Rotor::rotate_up(int i, Rotor* rotor, int noRotors) {
  
  if (noRotors == 0) //skip these steps if there are no rotors
    return;
  
  bool top_position_meets_notch;
  int a = 0;
  do 
    {
      if (rotor[i].get_notch(a) == rotor[i].get_top_position()+1) //Checks for notch at the next pos 
	top_position_meets_notch = true;
      else
	top_position_meets_notch = false;
      
      if ((top_position_meets_notch == false) && (rotor[i].get_top_position() == 25))
	rotor[i].add_top_position(-25);
      
      else if ((top_position_meets_notch == false) && (rotor[i].get_top_position() != 25))
	rotor[i].add_top_position(1);
      else if ((top_position_meets_notch == true) && (rotor[i].get_top_position() == 25))  {
	rotor[i].add_top_position(-25);
	i--;
	rotor[i].rotate_up(i, rotor, noRotors); }
      else  {
	rotor[i].add_top_position(1);
	i--;
	rotor[i].rotate_up(i, rotor, noRotors);
      }
      a++;
    }
  while (rotor[i].get_notch(a) != '\0');
}


////////////////////////////////////////////////////////
//     MEMBER FUNCTIONS OF CLASS "REFLECTOR"          //
////////////////////////////////////////////////////////

/*Constructor of the reflector*/
Reflector::Reflector (CharPtr clArguments, int &errorCode)  {
  load_mappings(clArguments, errorCode);
}

/*Function that that checks the configurations in the .rf file*/
void Reflector::check_config(CharPtr clInput, int &errorCode)  {
  //Check NON NUMERIC CHARACTER
  check_numeric_char(clInput, errorCode);
  if (errorCode != 0)  {
    cerr << "Non-numeric character in reflector file " << clInput << endl;
    return;
  }
  //Check INCORRECT NUMBER OF REFLECTOR PARAMETERS
  if ((mapping.size() != 26) && (mapping.size() % 2 == 1))  {
    errorCode = INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
    cerr << "Incorrect (odd) number of parameters in reflector file " << clInput << endl;
    return;
  }
  else if ((mapping.size() != 26) && (mapping.size() % 2 == 0))  {
    errorCode = INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
    cerr << "Insufficient number of mappings in reflector file: " << clInput << endl;
    return;
  }
  
  //Check INVALID REFLECTOR MAPPING
  for (int c=0; c<=25;c++)  {
    for (int i=0; i<=25; i++) {
      if ((mapping[i] == mapping[c]) && (c!=i)) 	{
	errorCode = INVALID_REFLECTOR_MAPPING;
	cerr << "Invalid reflector mapping in file " << clInput << endl;
	return;
      }
    } 
  }
   
  //Check for INVALID INDEX
  if (invalid_index() == true)  {
    errorCode = INVALID_INDEX;
    cerr << "Invalid index in reflector file " << clInput << endl;
    return;
  }
}


