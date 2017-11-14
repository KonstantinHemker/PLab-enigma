/* Main part of the enigma program */

#include <iostream>
#include <cstdlib>
#include <cstring>

#include "enigma.h"

using namespace std;

int main(int argc, char** argv)
{
  int n = 0;
  vector<int> pos_token;
  char message[1024]; 
  int no_rotors = argc-4;
  int error_code=0;
  
  //Remove this line for testing?
  cout << "Enter the message you would like to encode/decode (uppercase letters only): " << endl;

  cin.getline (message, 1023);

  error_code = check_user_input(message, argc);
  if (error_code > 0) {
    cout << error_description(error_code) << endl;
    return error_code;
  }
  
   //Initialise the InputSwitch
  InputSwitch input1(message[n]);
  Plugboard pboard(input1.return_letter(), argv[1]);//Constructor loads settings and creates tokens
  Rotor rotor[no_rotors+1];
  for (int c = 0; c <= no_rotors; c++)
    {
      rotor[c].init_rotor(argv[c+3]);
    }
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

  
  
  //rotor[0].load_top_position(0, rotor, no_rotors);//recursive function
    

  //Initializing the reflector
  Reflector reflector(message[n], argv[2]);
  //Check reflector
  int check_rf;
  check_rf = reflector.check_config();
  if (check_rf != 0)
    {
      cout << error_description(check_rf) << endl;
      return check_rf;
    }

  
  
  cout  << "The starting position of the rotor 1 is " << rotor[0].get_top_position() << endl;
  cout << "The starting position of rotor 2 is " << rotor[1].get_top_position() << endl;;
   
  while (message[n] != '\0')
    {
      //Passes in the new letter of the input class into the plugboard      
      pboard.set_letter(input1.return_letter()); 
      //swap values
      pboard.swap_values(message[n]);

        //Check plugboard configurations
      int check_pb;
      check_pb = pboard.check_config(argv[1]);
      if (check_pb != 0)
	{  
	  cout << error_description(check_pb) << endl;
	  return 0;
	}
       
      //int i = 0;
      rotor[0].rotor_inwards(message[n], rotor, no_rotors, 0); //recursive function
      
      
      Reflector reflector(message[n], argv[2]);
      reflector.swap_values(message[n]);
      
      
      rotor[no_rotors-1].rotor_outwards(message[n], rotor, no_rotors, no_rotors-1);
      
      rotor[0].rotate_up(0, rotor);

      pboard.set_letter(message[n]);
      pboard.swap_values(message[n]);
      
      cout << message[n];

            
      n++;
      //Set the new letter on the input switch for the next iteration
      input1.set_letter(n, message);  
      
    }
  cout << endl;

  //Developer Checks
  cout << "Plugboard - first six tokens configurations:" << endl;    
  for (unsigned int i = 0; i < 6; i++)
    {
      cout << pboard.get_token(i) << " "; }
      
	  
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
  return 0;

}
