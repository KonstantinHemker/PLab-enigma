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

void create_rot_position_tokens(CharPtr cl_position, vector<int> &pos_token)  {
  ifstream pos_input;
  pos_input.open (cl_position);
  int n;
  while (pos_input >> n)  {
    pos_token.push_back(n);
  }
  pos_input.close();
}

void set_rotor_positions(int n, vector<int> pos_token, Rotor* rotor, int noRotors)  {
  rotor[n].set_top_position(n, noRotors, pos_token);
  n++;
  if (n < noRotors)
    set_rotor_positions(n, pos_token, rotor, noRotors);
  else
    return;
}
								      

/*Member functions*/

int BaseModule::check_numeric_char(CharPtr filename)  {
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
	return 4;
    }
  return 0;       
}


int BaseModule::create_tokens (CharPtr filename)  {
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


bool BaseModule::invalid_index ()  {
  for (unsigned int n=0; n <= token.size()  ; n++) {
      if ((token[n] > 25) || (token[n] < 0))
      return true; }  
  return false;
}


int Plugboard::check_config(CharPtr cl_input)   {
  if (token.size()%2 == 1)
    return 6; //Incorrect number of plugboard parameters
  for (unsigned int i=0; i<=token.size(); i++)
    {
      for (unsigned int c=0; c<=token.size(); c++) {
	if ((token[i] == token[c]) && (c != i))
	    return 5;
      }  //Impossible plugboard configuration
    }
  //Check for INVALID INDEX
  if (invalid_index() == true)
    return 3; 

  //Check for NON_NUMERIC CHARACTER
  if((check_numeric_char(cl_input)) != 0)
    return check_numeric_char(cl_input);
    
  return 0;
}


void Rotor::rotor_inwards (char& current_char, Rotor* rotor, int noRotors, int a) {
  //Set the letter according to current rotation of rotor
  letter = current_char - 65;
  if (letter + top_position > 25) {
    letter += top_position -26;
    current_char = letter + 65;
  }
    else {
    letter +=top_position;
    current_char = letter + 65;
    }

  //Swap the values according to token mapping
  swap_values(current_char);
    
    a++;
    if (a < noRotors)
      rotor[a].rotor_inwards(current_char, rotor, noRotors, a);
    else
      return;
}

 

void Rotor::swap_values(char &current_char) {
  int i = 0;
  while (i<=25)
    {
      if (i == letter) {
	letter = token[i];
	current_char = letter + 65;
	break;
      }
      i++;
    }
      
  if (token[i] + top_position > 25) {
    letter = (token[i] + top_position) -26;
    current_char = letter + 65;
    return;
  }
  else {
    letter = token[i] + top_position;
    current_char = letter + 65;
    return;
  } 
}

    
  
  
void Rotor::rotor_outwards(char &current_char, Rotor* rotor, int noRotors, int a) {
  //Set letter input for pb
  letter = current_char -65;
  if (letter - top_position < 0) {
    current_char = current_char - top_position + 26;
    letter = current_char - 65;
  }
  else  {
    current_char = current_char - top_position;
    letter = current_char - 65;
  }
  
  for (int i = 0; i<=25; i++)
    {
      if ((token[i] == letter) && (i-top_position >=0))
	{
	  letter = i - top_position;
	  current_char = letter + 65;
	  break;
	}
      else if ((token[i] == letter) && (i-top_position < 0))
	{
	  letter = i - top_position + 26;
	  current_char = letter + 65;
	  break;
	}
    }
  a++;
  if (a < noRotors)
    rotor[a].rotor_outwards(current_char, rotor, noRotors, a);
  
}


  
void Rotor::rotate_up(int i, Rotor* rotor) {
  bool top_position_meets_notch;
  int a = 0;
  do 
    {
      if (rotor[i].get_notch(a) == rotor[i].get_top_position()) 
	top_position_meets_notch = true;
      else
	top_position_meets_notch = false;

      //Further checks
      if ((top_position_meets_notch == false) && (rotor[i].get_top_position() == 25))
	rotor[i].add_top_position(-25);
	  
      else if ((top_position_meets_notch == false) && (rotor[i].get_top_position() != 25))
	 rotor[i].add_top_position(1);
      else if ((top_position_meets_notch == true) && (rotor[i].get_top_position() == 25))
	{
	  rotor[i].add_top_position(-25);
	  i++;
	  rotate_up(i, rotor); }
      else
	{
	  rotor[i].add_top_position(1);
	  i++;
	  rotor[i].rotate_up(i, rotor);
	}
      a++;
    }
  while (rotor[i].get_notch(a) != '\0');
}




int Rotor::check_rot_positions(int noRotors, vector<int> pos_token) {
  int vsize = pos_token.size();
  //Check for invalid index
  for (int i=0; i<vsize; i++) {
    if (pos_token[i] > 25)
      return 3;  //INVALID INDEX code
  }
  
  //Check whether there are sufficient configurations for the number of rotors
  cout << vsize << endl;
  cout << noRotors << endl;
  if (noRotors <= vsize)
    return 0;
  else
    return 8;
}



int Rotor::check_config (CharPtr cl_input) {
  //Check NON_NUMERIC_CHARACTER
  if (check_numeric_char(cl_input) != 0)
    return check_numeric_char(cl_input);
  
  //Check INVALID_ROTOR_MAPPING
  for (int c=0; c<=25;c++)  {
    for (int i=0; i<=25; i++) {
      if ((token[i] == token[c]) && (c!=i))
	  return 7;
    }
  }
  //Check INVALID_INDEX 
  if (invalid_index() == true)
    return 3;
  
  return 0;
}


int Reflector::check_config()  {
  //Check INVALID REFLECTOR MAPPING
  for (int c=0; c<=25;c++)  {
    for (int i=0; i<=25; i++) {
      if ((token[i] == token[c]) && (c!=i))
	  return 9;
    }
  }

  //Check for INVALID INDEX
  if (invalid_index() == true)
    return 3;
   

  //Check INCORRECT NUMBER OF REFLECTOR PARAMETERS
  if (token.size() != 26)
    return 10;
  
    return 0;
}
