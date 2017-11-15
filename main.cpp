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
  int no_rotors = argc-4;
  int error_code=0;
  int plugboard_wirings;
  
   //Initialise the InputSwitch
  //InputSwitch input1(message); 

  Plugboard plugboard;
  plugboard_wirings = plugboard.load_tokens(argv[1]);

  Rotor rotor[no_rotors+1];
  for (int c = 0; c <= no_rotors; c++)
    rotor[c].init_rotor(argv[c+3]);
    
  //Sets rotor starting position tokens and check their validity
  create_rot_position_tokens(argv[no_rotors+3], pos_token);
  set_rotor_positions(0, pos_token, rotor, no_rotors); 
  

  /*Checking initial settings of the rotor*/
  //Check validity of rotor positions
  int check_pos = rotor[0].check_rot_positions(no_rotors, pos_token);
  if (check_pos != 0){
    cout << error_description(check_pos) << endl;
    return check_pos;
  }
    
  //Check rotor settings;
  int check_rot = 0;
  for (int c=0;c<no_rotors;c++) {
    if (rotor[c].check_config(argv[c+3]) !=0)
      {
	check_rot = rotor[c].check_config(argv[c+3]);
	cout << error_description(check_rot) << endl;
	return check_rot; }
  }

  //Initializing the reflector
  Reflector reflector(argv[2]);  /**MARKER**/
  //Check reflector
  int check_rf;
  check_rf = reflector.check_config();
  if (check_rf != 0)
    {
      cout << error_description(check_rf) << endl;
      return check_rf;
    }

  
  /* 
  cout  << "The starting position of the rotor 1 is " << rotor[0].get_top_position() << endl;
  cout << "The starting position of rotor 2 is " << rotor[1].get_top_position() << endl;;
  */
  cin >> std::ws >> message;

  while (!cin.eof()) 
    {

      error_code = check_user_input(message, argc);
      if (error_code > 0)
	{
	  cout << error_description(error_code) << endl;
	  return error_code;
	}
      
      //input1.set_letter(message);
      
      
      plugboard.pass_through(message, plugboard_wirings);
     
        //Check plugboard configurations
      int check_pb;
      check_pb = plugboard.check_config(argv[1]);
      if (check_pb != 0)
	{  
	  cout << error_description(check_pb) << endl;
	  return check_pb;
	}
      
      //int i = 0;
      rotor[0].rotor_inwards(message, rotor, no_rotors, 0); //recursive function
      
     
      reflector.pass_through(message, 1);
      
      
      rotor[no_rotors-1].rotor_outwards(message, rotor, no_rotors, no_rotors-1);
      
      rotor[0].rotate_up(0, rotor);

      plugboard.pass_through(message, plugboard_wirings);
      
      cout << message;

      cin >> std::ws >> message;

      //Set the new letter on the input switch for the next iteration 
      
    }
  
  cout << endl;

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
