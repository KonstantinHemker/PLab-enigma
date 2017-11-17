/* Main part of the enigma program */

#include <iostream>
#include <cstdlib>
#include <cstring>

#include "enigma.h"


int main(int argc, char** argv)
{
  vector<int> pos_token;
  char message; 
  int no_rotors;
  int error_code=0;
  string class_type;
  int n;
 
  if (argc < 5)
    no_rotors = 0;
  else
    no_rotors = argc-4;
 

  Plugboard plugboard (argv[1], error_code);

  
    //Top part goes in constructor
  Rotor rotor[no_rotors+1];
  if (no_rotors > 0)
    {
      //initialize_rotors(no_rotors, argv, error_code, pos_token, rotor);
      
      for (int c = 0; c <= no_rotors; c++)
	rotor[c].init_rotor(argv[c+3], error_code);
    
      //Sets rotor starting position tokens and check their validity
      load_rotor_positions(argv[no_rotors+3], pos_token, error_code);
      set_rotor_positions(0, pos_token, rotor, no_rotors);
      
    }
  
    
  //Initializing the reflector
  Reflector reflector(argv[2], error_code); 

  

  check_enigma_setup(n, argc, argv, no_rotors, error_code, class_type, plugboard, rotor, reflector, pos_token);
  if (error_code > 0)
    {
      error_description(error_code, class_type, argv, n, reflector);

      return error_code;
    }

  
   cin >> std::ws >> message;

  while (!cin.eof()) 
    {

      rotor[no_rotors-1].rotate_up(no_rotors-1, rotor, no_rotors);
      
      check_message_input(message, error_code);
      if (error_code > 0)
	{
	  error_description(error_code, class_type, argv, n, reflector);
	  return error_code;
	}
      
      
      plugboard.pass_through(message);
          
      
      rotor[no_rotors-1].rotor_inwards(message, rotor, no_rotors, no_rotors-1); //recursive function
      
     
      reflector.pass_through(message, 1);
      
      
      rotor[0].rotor_outwards(message, rotor, no_rotors, 0);
      
      
      plugboard.pass_through(message);
      
      cout << message;

      cin >> std::ws >> message;

      //Set the new letter on the input switch for the next iteration 
      
    }

return 0;

}
