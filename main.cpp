/* Main part of the enigma program */

#include <iostream>
#include <cstdlib>
#include <cstring>

#include "enigma.h"


int main(int argc, char** argv)
{
  vector<int> pos_mapping;
  char message; 
  int no_rotors;
  int error_code=0;
 
  if (argc < 5)
    no_rotors = 0;
  else
    no_rotors = argc-4;
 

  /*Setting up the enigma machine*/
  Plugboard plugboard (argv[1], error_code);
  Rotor rotor[no_rotors+1];
  initialize_rotors(no_rotors, argv, error_code, pos_mapping, rotor);
  Reflector reflector(argv[2], error_code); 
  
  /*Checking enigma setup*/
  check_enigma_setup(argc, argv, no_rotors, error_code, plugboard, rotor, reflector, pos_mapping);
  if (error_code > 0)
      return error_code;



  
  cin >> std::ws >> message;

  while (!cin.eof()) 
    {
      rotor[no_rotors-1].rotate_up(no_rotors-1, rotor, no_rotors);
      
      check_message_input(message, error_code);
      if (error_code > 0)
	  return error_code;
      
      
      plugboard.pass_through(message);

      rotor[no_rotors-1].rotor_inwards(message, rotor, no_rotors, no_rotors-1); //recursive function
     
      reflector.pass_through(message, 1);
      
      rotor[0].rotor_outwards(message, rotor, no_rotors, 0);
      
      plugboard.pass_through(message);
      
      cout << message;

      cin >> std::ws >> message;
    }

return 0;

}
