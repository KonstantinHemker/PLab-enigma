/* Implementation file of the enigma program */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <vector>

#include "enigma.h"


/*Helper functions*/

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

//vector<std::string> readFileToArray



/*Member functions*/
Plugboard::Plugboard(char l) : letter (l)  {
  ifstream plugsettings;
  plugsettings.open("plugboards/I.pb");
  istreambuf_iterator<char> eos;
  string temp (istreambuf_iterator<char>(plugsettings), eos );
  plugsettings.close();
  settings = temp;
};



int Plugboard::get_tokens (const string &s)  {
  istringstream is(s);
  int n;
  while (is >> n) {
    token.push_back(n);
  }
  return token.size();
}



void Plugboard::swap_values (char &current_char)  {
  
  for (int i=0; i <= token.size(); i++)
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


