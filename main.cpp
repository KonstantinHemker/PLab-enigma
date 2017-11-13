/* Main part of the enigma program */

#include <iostream>
#include <cstdlib>
#include <cstring>

#include "enigma.h"

using namespace std;

int main(int argc, char** argv)
{
  //Check terminal input
  //if (check_no_parameters(argc) == false)
  // return 1;
  int n = 0;
  
    
  char message[512]; 
  //What word is entered in the input switch? (character by character)
  cout << "Enter the message you would like to encode/decode (uppercase letters only): " << endl;

  cin.getline (message, 511);

  //Check the message input
  int check_input;
  check_input = check_message(message);
   if (check_input != 0)
	{  
	  cout << error_description(check_input) << endl;
	  return check_input;
	}

  InputSwitch input1(message[n]);
  Plugboard pboard(input1.return_letter(), argv[1]);//Constructor also loads settings and creates tokens
  //Return number of rotors depending on how many arguments are entered in the command line
  int no_rotors;
  /*Change to up to 5 */
  no_rotors = argc - 4; //5 arguments are entered regardless. The only one that may vary is the number of rotors
    
  Rotor rotor[no_rotors+1];
  
  for (int c = 0; c <= no_rotors; c++)
    {
      rotor[c].init_rotor(argv[c+3]);
    }
  
  //Sets rotor starting position tokens and check their validity
  rotor[0].create_rot_position_tokens(argv[no_rotors+3]);
  //Check validity of rotor positions
  int check_pos = rotor[0].check_rot_positions(no_rotors);
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

  
  for (int i = 0; i <no_rotors; i++)
    {
      rotor[i].load_top_position(i, no_rotors);
    }

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
          
      //Use the corr_token to pass the input backwards through the rotor
      //(negative forloop)
      
      rotor[0].rotor_outwards(message[n], rotor, no_rotors, 0);
      

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
