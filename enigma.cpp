/* Implementation file of the enigma program */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include "enigma.h"

using namespace std;

/*Helper functions*/
bool check_no_parameters (int numberArguments)  {
  if (numberArguments != 7)
    return false;
  else
    return true;
}

/*Member functions*/
Plugboard::Plugboard(char l, char set[])  {
  letter = l;
  ifstream plugsettings;
  plugsettings.open("set[]");
  for (int n = 0; (!plugsettings.eof()); n++)
    plugsettings.get(settings[n]);
  plugsettings.close();
};

  
