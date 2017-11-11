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

int BaseModule::check_alpha_char(char* filename)  {
  ifstream enigmasettings;
  enigmasettings.open(filename);
  istreambuf_iterator<char> eos;
  string temp (istreambuf_iterator<char>(enigmasettings), eos );
  enigmasettings.close();
  string settings = temp;
  //Check non numeric character
  /*
       for (unsigned int c=0; c< settings.size(); c++)
      	{
        if (((settings[c] <= 57) && (settings[c]>=48)) || (settings[c] == 32) || (settings[c] == '\0'))
          {}
        else
          return 4;
      	}
  */
       return 0;
        
}


int BaseModule::create_tokens (char* filename)  {
  ifstream enigmasettings;
  enigmasettings.open(filename);
  int n;
  while (enigmasettings >> n) {
    token.push_back(n);
  }
  enigmasettings.close();
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

int Plugboard::check_config(char* cl_input)   {
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
    }
  //Check for non-numeric character
  /*
  if((check_alpha_char(cl_input)) != 0)
    return check_alpha_char(cl_input);
  */
  return 0;
}

void Rotor::set_letter(char &current_char) {
  letter = current_char-65;
  if ((letter + top_position) > 25)
    { current_char = (letter  + top_position + 65) -26; //-26 accounts for rotor finishing one circumference
      letter = current_char -65;
    }
  else
    {
      current_char = letter + top_position + 65;
      letter = current_char -65;
    }
}

void Rotor::swap_values(char &current_char) {
  //current_char = token[letter];

  for (int i = 0; i<=25; i++)
    {
      if (i == letter)
	current_char = token[i] + 65;
	}
}

void rotate_up(int i, Rotor* rotor, vector<int> v) {
  bool top_position_meets_notch;
  int a = 0;
  do 
    {
      if (rotor[i].get_notch(a) == rotor[i].get_top_position()) 
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
	  i++;
	  rotate_up(i, rotor, v); }
      else
	{
	  rotor[i].add_top_position(1);
	  i++;
	  rotate_up(i, rotor, v);
	}
      a++;
    }
  while (rotor[i].get_notch(a) != '\0');
}


  //rotate the top position of the rotor up by one
  //check notch of the rotor
  //if notch of this rotor should move up as well, we should


void create_rot_position_tokens(char* cl_position, vector<int> &rot_positions)  {
  ifstream pos_input;
  pos_input.open (cl_position);
  int n;
  while (pos_input >> n)  {
    rot_positions.push_back(n);
  }
  pos_input.close();
}

int Rotor::check_config (char* cl_input) {
  //Check INVALID_ROTOR_MAPPING
  for (int c=0; c<=25;c++)  {
    for (int i=0; i<=25; i++) {
      if ((token[i] == token[c]) && (c!=i))
	  return 7;
    }
  }
  return 0;
  //Check Invalid Index Code
  for (int c=0; token[c] != '\0'; c++)
    if (token[c] > 25)
      return 3;
  //Check non_numeric character
  if (check_alpha_char(cl_input) != 0)
    return check_alpha_char(cl_input);
}
