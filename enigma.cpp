/* Implementation file of the enigma program */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <vector>

#include "enigma.h"

/*Helper functions*/

/*Function to report errors*/
/*
void error_description (int code, int noRotors, string class_type, CharPtr cl_argument[], int nargument, int nrotor, Reflector &reflector, char message)  {
  switch(code) {
  case INSUFFICIENT_NUMBER_OF_PARAMETERS:
    cerr << "usage: enigma plugboard-file reflector-file (<rotor-file>* rotor-positions)?" << endl;
    break;
  case INVALID_INPUT_CHARACTER:
    cerr << message << " is not a valid input character (input characters must be upper case letters A-Z)!" << endl;
    break;
  case INVALID_INDEX:
    {
      cerr << "Invalid index in " << class_type << " file " << cl_argument[nargument] << endl;
      break;
    }
  case NON_NUMERIC_CHARACTER:
    {
      if (class_type == "plugboard")
	cerr << "Non-numeric character in " << class_type << " file " << cl_argument[nargument] << endl;
      else if (class_type == "rotor position")
	cerr << "Non-numeric character in rotor positions file " << cl_argument[noRotors+3] << endl;
      else if (class_type == "reflector")
	cerr << "Non-numeric character in " << class_type << " file " << cl_argument[nargument] << endl;
      else
	cerr << "Non-numeric character for mapping in " << class_type << " file " << cl_argument[nargument] << endl;
      break;
    }
  case IMPOSSIBLE_PLUGBOARD_CONFIGURATION:
    {
      cerr << "Impossible " << class_type << " configurations in file " << cl_argument[nargument] << endl;
      break;
    }
  case INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS:
    {
      cerr << "Incorrect number of parameters in " << class_type << " file " << cl_argument [nargument] << endl;
    break;
    }
  case INVALID_ROTOR_MAPPING:
    {
      cerr  << endl; //Error message already output it check_config function
      break;
    }
  case NO_ROTOR_STARTING_POSITION:
    {
      cerr << "No starting position for rotor " << nrotor << " in " << class_type << " file: " << cl_argument[nargument] << endl;
     break;
    }
  case INVALID_REFLECTOR_MAPPING:
    {
      cerr << "Invalid " << class_type << " mapping in file " << cl_argument[nargument] << endl; 
      break;
    }
  case INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS:
    {
      if (reflector.get_token_size() % 2 ==1)
	cerr << "Incorrect (odd) number of parameters in " << class_type << " file " << cl_argument[nargument] << endl;
      else
	cerr << "Insufficient number of mappings in reflector file: " << cl_argument [nargument] << endl;
      break;
    }
  case ERROR_OPENING_CONFIGURATION_FILE:
    {
      cerr << "Error opening configuration file. Please check the command line parameters." << endl;
      break;
    }
  }
}
*/


/*Function that checks all enigma setup errors */
void check_enigma_setup (int &nrotor, int cl_arguments, char* argv[], int noRotors, int &error_code, Plugboard &plugboard, Rotor* rotor, Reflector &reflector, vector<int> pos_token)
{
  check_command_line_input(cl_arguments, noRotors, error_code);
  if (error_code > 0)
    return;

 
  plugboard.check_config(argv[1], error_code);
  if (error_code > 0)
      return;

  if (noRotors > 0) //only checks for rotor conditions if rotors exist
    {
      rotor[0].check_rot_positions(noRotors, pos_token, error_code, argv[noRotors+3], nrotor); 
      if (error_code > 0)
	 return;

      for (int c = 0 ;c < noRotors; c++) 
	{
	  rotor[c].check_config(argv[c+3], error_code);
	  if (error_code > 0)
	    return;
	}
    }

  reflector.check_config(argv[2], error_code);
    
}
  



/*Function that initialises the array of rotors*/
/*
void initialize_rotors(int noRotors, CharPtr cl_argument[], int &error_code, vector<int> pos_token, Rotor rotor[])
{
  if (noRotors == 0)
    return;
  else
    {
      for (int c = 0; c <= noRotors; c++)
	rotor[c].init_rotor(cl_argument[c+3], error_code);
      
      //Sets rotor starting position tokens and check their validity
      load_rotor_positions(cl_argument[noRotors+3], pos_token, error_code);
      set_rotor_positions(0, pos_token, rotor, noRotors);
    }
}
*/

/*Function that creates the position tokens for all rotors in the system*/
void load_rotor_positions(CharPtr cl_position, vector<int> &pos_token, int &error_code, Rotor* rotor)  {
  ifstream pos_input;
  if (rotor[0].is_valid(cl_position, error_code) == false)
    return;
  
  pos_input.open (cl_position);
  if (pos_input.fail())
    {
      error_code = 11;
      cerr << "Error opening configuration file." << endl;
      return;
    }
  int n;
  while (pos_input >> n)  {
    pos_token.push_back(n);
  }
  pos_input.close();
}

/*Recursive function that sets the rotor positions for each rotor*/
void set_rotor_positions(int c, vector<int> pos_token, Rotor* rotor, int noRotors, int &error_code)  {
  rotor[c].set_top_position(c, noRotors, pos_token, error_code);
  c++;
  if (c < noRotors)
    set_rotor_positions(c, pos_token, rotor, noRotors, error_code);
  else
    return;
}


void check_message_input (char message, int &error_code)
{
  if (isupper(message) == 0)
    {
      error_code = 2;
      cerr << message << " is not a valid input character (input characters must be upper case letters A-Z)!" << endl;
    }
  else
    error_code = 0;
}
  

void check_command_line_input (int no_arguments, int noRotors, int &error_code)
{
/* Command line input */
  if (((noRotors == 0) && (no_arguments < 3)) || ((noRotors > 0) && (no_arguments < 5)))
    {
      error_code = 1;
      cerr << "usage: enigma plugboard-file reflector-file (<rotor-file>* rotor-positions)?" << endl;
    }
}

/*Member functions*/

void BaseModule::check_numeric_char(CharPtr filename, int &error_code)  {
  ifstream enigmasettings;
  enigmasettings.open(filename);
  istreambuf_iterator<char> eos;
  string temp (istreambuf_iterator<char>(enigmasettings), eos );
  enigmasettings.close();
  string settings = temp;
  //Check non numeric character
  for (unsigned int c=0; c< settings.size(); c++)
    {
      if (((settings[c] <= 57) && (settings[c]>=48)) || (settings[c] == 32) || (settings[c] == '\n'))
	{}
      else
	error_code = 4;
    }
}
   


void BaseModule::load_tokens (CharPtr filename, int& error_code)
{
  ifstream enigmasettings;
  int n;
  if (is_valid(filename, error_code) == false)
    {
      return;	      
    }
  enigmasettings.open(filename);
  while (enigmasettings >> n) {
    token.push_back(n);
    }
  enigmasettings.close();
}


bool BaseModule::is_valid (CharPtr filename, int &error_code)
{
  ifstream inStream;
  inStream.open(filename);
  if (inStream.fail())
    {
      error_code = 11;
      cerr << "Error opening configuration file." << endl; 
      return false;
    }
  inStream.seekg(0, inStream.end);
  int n = inStream.tellg();

  if (n == 0)
    {
      empty = true;
      return false;
    }
  else
     empty = false;

  return true;
}
  


void BaseModule::swap_values (char &current_char)  {

  if (empty == true) //Avoids seg fault if the file was empty
    return;

  for (unsigned int i=0; i <= token.size(); i++)
    {
      if ((token[i]+65 == current_char) && (i%2 == 0)) //compares ASCII values
     	{
	  current_char = token[i+1]+65;
	  return;
	}

      if ((token[i]+65 == current_char) && (i%2 == 1))
	{
	  current_char = token[i-1]+65;
	  return;
     	}

    }
}


bool BaseModule::invalid_index ()  {
  for (unsigned int n=0; n <= token.size()-1  ; n++) {
      if ((token[n] > 25) || (token[n] < 0))
	return true; }  
  return false;
}


void Plugboard::check_config(CharPtr cl_input, int &error_code)   {

  if (empty == true)
    return;
  
  //Check for NON_NUMERIC_CHARACTER
  check_numeric_char(cl_input, error_code);
  if(error_code != 0)
    {
      cerr << "Non-numeric character in plugboard file " << cl_input << endl;
      return;
    }
  //Check for INCORRECT_NUMBER_OF_PB_PARAMETERS
  if (token.size()%2 == 1)
    {
      error_code = 6;
      cerr << "Incorrect number of parameters in plugboard file " << cl_input << endl;
      return;
    }
  //Check for IMPOSSIBLE_PLUGBOARD_CONFIGURATION
  for (unsigned int i=0; i<=token.size()-1; i++)
    {
      for (unsigned int c=0; c<=token.size()-1; c++) {
	if ((token[i] == token[c]) && (c != i)) {
	  error_code = 5;
	  cerr << "Impossible plugboard configurations in file " << cl_input << endl;
	  return;
	}
	
      }  //Impossible plugboard configuration
    }
  //Check for INVALID INDEX
  if (invalid_index() == true)
    {
      error_code = 3;
      cerr << "Invalid index in plugboard file " << cl_input << endl;
      return;
    }
 
}



void Rotor::adjust_up(char& current_char)
{
  if (current_char + top_position > 90) 
    current_char += top_position + -26;
  else
    current_char += top_position;
}



void Rotor::adjust_down(char&current_char)
{
  if (current_char - top_position < 65) 
    current_char = current_char - top_position + 26;
  else  
    current_char = current_char - top_position;
}


void Rotor::rotor_inwards (char& current_char, Rotor* rotor, int noRotors, int a) {
  if (noRotors == 0)
    return;

  adjust_up(current_char);

  swap_values(current_char); 

  adjust_down(current_char);
  
    a--;
    if (a >= 0)
      rotor[a].rotor_inwards(current_char, rotor, noRotors, a);
    else
      return;
}

 

void Rotor::swap_values(char &current_char) {
  letter = current_char -65;
  for(int i = 0; i <= 25; i++)
    {
      if (i == current_char - 65)
	{
	  current_char = token[i] + 65;
	  break;
	}
    } 
}


  
void Rotor::rotor_outwards(char &current_char, Rotor* rotor, int noRotors, int a) {
  if (noRotors == 0)
    return;
  
  adjust_up(current_char);

  for (int i = 0; i<=25; i++)
    {
      if ((token[i] == current_char-65))
	{
	  current_char = i + 65;
	  break;
	}
    }
 
  adjust_down(current_char);
  
  a++;
  if (a < noRotors)
    rotor[a].rotor_outwards(current_char, rotor, noRotors, a);
  
}


  
void Rotor::rotate_up(int i, Rotor* rotor, int noRotors) {

  if (noRotors == 0)
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
      else if ((top_position_meets_notch == true) && (rotor[i].get_top_position() == 25))
	{
	  rotor[i].add_top_position(-25);
	  i--;
	  rotate_up(i, rotor, noRotors); }
      else
	{
	  rotor[i].add_top_position(1);
	  i--;
	  rotor[i].rotate_up(i, rotor, noRotors);
	}
      a++;
    }
  while (rotor[i].get_notch(a) != '\0');
}




void Rotor::check_rot_positions(int noRotors, vector<int> pos_token, int &error_code, CharPtr cl_argument, int nrotor) {
  int vsize = pos_token.size();

  //Check NON NUMERIC CHARACTER
  check_numeric_char(cl_argument, error_code);
  if (error_code != 0)
    {
      cerr << "Non-numeric character in rotor positions file " << cl_argument << endl;
      return;
    }
  //Check for INVALID INDEX
  for (int i=0; i<vsize; i++) {
    if (pos_token[i] > 25 || pos_token[i] < 0)
      {
	cerr << "Invalid index in rotor position file " << cl_argument << endl;
	error_code = 3;  //INVALID INDEX code
	return;
      }
  }
  
  //Check for NO_ROTOR_STARTING_POSITION
  if (noRotors <= vsize)
    return;
  else
    {
      error_code = 8;
      cerr << "No starting position for rotor " << nrotor << " in rotor position file " << cl_argument << endl;
    }
}



void Rotor::check_config (CharPtr cl_input, int &error_code) {
  //Check NON_NUMERIC_CHARACTER
  check_numeric_char(cl_input, error_code);
  if (error_code != 0)
    {
      cerr << "Non-numeric character in rotor file " << cl_input << endl;
      return;
    }
  
  //Check INVALID ROTOR MAPPING (too few inputs)
  if (token.size()<=26)
    {
      error_code = 7;
      cerr << "Not all inputs mapped in rotor file: " << cl_input;
      return;
    }
  

  //Check INVALID_ROTOR_MAPPING (double mapping)
  for (int c=0; c<=25;c++)
    {
      for (int i=0; i<=25; i++)
	{
	  if ((token[i] == token[c]) && (c!=i))
	    {
	      error_code = 7;
	      cerr << "Invalid mapping of input " << i << " to output " << token[i];
	      cerr << " (output " << token[i] << " is already mapped to from input " << c << ")";
	      return;
	    }
	}
    }

  //Check INVALID_INDEX 
  if (invalid_index() == true)
    {
      cerr << "Invalid index in rotor file " << cl_input << endl;
      error_code = 3;
    }
}


void Reflector::check_config(CharPtr cl_input, int &error_code)
{
  //Check NON NUMERIC CHARACTER
  check_numeric_char(cl_input, error_code);
  if (error_code != 0)
    {
      cerr << "Non-numeric character in reflector file " << cl_input << endl;
      return;
    }
  //Check INCORRECT NUMBER OF REFLECTOR PARAMETERS
  if ((token.size() != 26) && (token.size() % 2 == 1))
    {
      error_code = 10;
      cerr << "Incorrect (odd) number of parameters in reflector file " << cl_input << endl;
      return;
    }
  else if ((token.size() != 26) && (token.size() % 2 == 0))
    {
      error_code = 10;
      cerr << "Insufficient number of mappings in reflector file: " << cl_input << endl;
      return;
    }
  else
    return;
   
  //Check INVALID REFLECTOR MAPPING
  for (int c=0; c<=25;c++)  {
    for (int i=0; i<=25; i++) {
      if ((token[i] == token[c]) && (c!=i))
	{
	  error_code = 9;
	  cerr << "Invalid reflector mapping in file " << cl_input << endl;
	  return;
	}
    }
    
  }
  
   
  //Check for INVALID INDEX
  if (invalid_index() == true)
    {
      error_code = 3;
      cerr << "Invalid index in reflector file " << cl_input << endl;
      return;
    }
}

