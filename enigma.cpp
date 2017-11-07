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
const char* error_description (int code)  {
  switch(code) {
  case INSUFFICIENT_NUMBER_OF_PARAMETERS:
    return "Error: Insufficient number of parameters";
    break;
  case INVALID_INPUT_CHARACTER:
    return "Error: Invalid input character";
    break;
  case INVALID_INDEX:
    return "Error: Invalid index";
     break;
  case NON_NUMERIC_CHARACTER:
    return "Error: Non numeric character";
     break;
  case IMPOSSIBLE_PLUGBOARD_CONFIGURATION:
    return "Error: Impossible plugboard configuration";
     break;
  case INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS:
    return "Error: Incorrect number of plugboard parameters";
    break;
  case INVALID_ROTOR_MAPPING:
    return "Error: Invalid rotor mapping";
     break;
  case NO_ROTOR_STARTING_POSITION:
    return "Error: No rotor starting position";
     break;
  case INVALID_REFLECTOR_MAPPING:
    return "Error: Invalid reflector mapping";
     break;
  case INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS:
    return "Error: Incorrect number of reflector parameters";
     break;
  case ERROR_OPENING_CONFIGURATION_FILE:
    return "Error: Error opening configuration file";
     break;
  }
  return "No error in machine setup detected";
}

/*Helper function to check the number of parameters entered in the command line*/
bool check_no_parameters (int numberArguments)  {
  if (numberArguments != 7)
    return false;
  else
    return true;
}
/*End of function definition*/

/*Helper functiont that converts a settings into the corresponding ASCII value*/
int convert_to_ASCII (int vec)  {
  int ASCII;
  ASCII = vec+65;
  return ASCII;
}
/*End of function definition*/

/*Function that checks the message input*/
int check_message(char message[])  {
  for (int n=0; message[n]!= '\0'; n++)
    {if (isupper(message[n]) == 0)
	return 2; //Code for invalid input character
    }
  return 0;
}


/*Member functions*/

void BaseModule::load_settings(char* filename)  {
  ifstream plugsettings;
  plugsettings.open(filename);
  istreambuf_iterator<char> eos;
  string temp (istreambuf_iterator<char>(plugsettings), eos );
  plugsettings.close();
  settings = temp;
}




int BaseModule::create_tokens (const string &s)  {
  istringstream is(s);
  int n;
  while (is >> n) {
    token.push_back(n);
  }
  return token.size();
}



void BaseModule::swap_values (char &current_char)  {

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

int Plugboard::check_config()   {
  if (token.size()%2 == 1)
    return 6; //Incorrect number of plugboard parameters
  for (unsigned int i=0; i<=token.size(); i++)
    {
      for (unsigned int c=0; c<=token.size(); c++) {
	if ((token[i] == token[c]) && (c != i))
	    return 5;
      }  //Impossible plugboard configuration
      if ((token[i] > 25) || (token[i] < 0))
	  return 3; //Invalid index
      //Check non numeric character
      for (unsigned int c=0; c< settings.size()-1; c++)
	{
	  if (((settings[c] <= 57) && (settings[c]>=48)) || (settings[c] == 32) || (settings[c] == '\0'))
	    {}
	  else
	    return 4;
	}
    }
  return 0;
}


