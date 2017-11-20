/* Main part of the enigma program */

#include <iostream>
#include <cstdlib>
#include <cstring>

#include "enigma.h"


int main(int argc, char** argv)   {
  
  vector<int> posMapping;
  char message; 
  int noRotors;
  int errorCode=0;
  
  if (argc < 5)
    noRotors = 0;
  else
    noRotors = argc-4;
 

  /*Setting up the enigma machine*/
  Plugboard plugboard (argv[1], errorCode);
  Rotor rotor[noRotors+1];
  initialize_rotors(noRotors, argv, errorCode, posMapping, rotor);
  Reflector reflector(argv[2], errorCode); 
  
  /*Checking enigma setup*/
  check_enigma_setup(argc, argv, noRotors, errorCode, plugboard, rotor, reflector, posMapping);
  if (errorCode > 0)
    return errorCode;
  


  
  cin >> std::ws >> message;
  
  while (!cin.eof())   {
    
    rotor[noRotors-1].rotate_up(noRotors-1, rotor, noRotors);
    
    check_message_input(message, errorCode);
    if (errorCode > 0)
      return errorCode;
    
      
    plugboard.pass_through(message);
    
    rotor[noRotors-1].rotor_inwards(message, rotor, noRotors, noRotors-1); //recursive function
    
    reflector.pass_through(message, 1);
      
    rotor[0].rotor_outwards(message, rotor, noRotors, 0);
      
    plugboard.pass_through(message);
      
    cout << message;

    cin >> std::ws >> message;
  }
  
  return 0;

}
