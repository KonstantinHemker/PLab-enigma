/* Main part of the enigma program */

#include <iostream>
#include <cstdlib>
#include <cstring>

#include "enigma.h"

using namespace std;

int main(int argc, char** argv)
{
  vector<int> pos_token;
  char message; 
  int no_rotors;
  int error_code=0;
 


  if (argc < 5)
    no_rotors = 0;
  else
    no_rotors = argc-4;


  Plugboard plugboard (argv[1]);

  Rotor rotor[no_rotors+1];
  if (no_rotors > 0)
    {
      for (int c = 0; c <= no_rotors; c++)
	rotor[c].init_rotor(argv[c+3]);
    
      //Sets rotor starting position tokens and check their validity
      create_rot_position_tokens(argv[no_rotors+3], pos_token);
      set_rotor_positions(0, pos_token, rotor, no_rotors); 

      
      /*Checking initial settings of the rotor*/
      //Check validity of rotor positions
      error_code = rotor[0].check_rot_positions(no_rotors, pos_token);
      if (error_code != 0){
	cerr << error_description(error_code) << endl;
	return error_code;
      }
    
      //Check rotor settings;
      int error_code;
      for (int c=0;c<no_rotors;c++) {
	if (rotor[c].check_config(argv[c+3]) !=0)
	  {
	    error_code = rotor[c].check_config(argv[c+3]);
	    cerr << error_description(error_code) << endl;
	    return error_code;
	  }
      }
    }
  
  //Initializing the reflector
  Reflector reflector(argv[2]);  /**MARKER**/
  //Check reflector
  
  error_code = reflector.check_config();
  if (error_code != 0)
    {
      cerr << error_description(error_code) << endl;
      return error_code;
    }


 cout << "Notches Start of testing:" << endl;
  cout << "Rotor 1: " << rotor[0].get_notch(0) << endl;
  cout << "Rotor 2: " << rotor[1].get_notch(0) << endl;
  cout << "Position at start: " << endl;
  cout << "Rotor 1: " << rotor[0].get_top_position() << endl;
   cout << "Rotor 2: " << rotor[1].get_top_position() << endl;
  
  
  cin >> std::ws >> message;

  while (!cin.eof()) 
    {

      error_code = check_user_input(message, argc, no_rotors);
      if (error_code > 0)
	{
	  cerr << error_description(error_code) << endl;
	  return error_code;
	}
      
      
      plugboard.pass_through(message);
     
        //Check plugboard configurations
      error_code = plugboard.check_config(argv[1]);
      if (error_code != 0)
	{  
	  cerr << error_description(error_code) << endl;
	  return error_code;
	}
      
      
      rotor[no_rotors-1].rotor_inwards(message, rotor, no_rotors, no_rotors-1); //recursive function
      
     
      reflector.pass_through(message, 1);
      
      
      rotor[0].rotor_outwards(message, rotor, no_rotors, 0);
      
      rotor[no_rotors-1].rotate_up(no_rotors-1, rotor, no_rotors);

      plugboard.pass_through(message);
      
      cout << message;

      cin >> std::ws >> message;

      //Set the new letter on the input switch for the next iteration 
      
    }
  




  cout << endl;
  cout << "Position at start: " << endl;
  cout << "Rotor 1: " << rotor[0].get_top_position() << endl;
  cout << "Rotor 2: " << rotor[1].get_top_position() << endl;









  
  /*Developer Checks
  cout << "Plugboard - first six tokens configurations:" << endl;    
  for (unsigned int i = 0; i < 6; i++)
    {
      cout << plugboard.get_token(i) << " "; }
      
	  
  cout << endl;

  for (int a = 0; a < no_rotors; a++)
    {
     
    
 
      cout << "First two rotor configurations for rotor " << a+1 << ":"  << endl;
      cout << rotor[a].get_token(0) << endl;
      cout << rotor[a].get_token(1) << endl;
      cout << "The first notch is at postition: " << rotor[a].get_notch(0) << endl;
      cout << "The second notch is at position: " << rotor[a].get_notch(1) << endl;
      cout << "After encrypting, the top position of rotor" << a+1 << " is " << rotor[a].get_top_position() << endl;
      cout << endl;
    }
      cout << "The number of rotors is " << no_rotors << endl;

      */

      return 0;

}
