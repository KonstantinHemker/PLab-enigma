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
	  return 0;
	}

  InputSwitch input1(message[n]);
  Plugboard pboard(input1.return_letter());//Constructor also initialises the settings
  pboard.load_settings(argv[1]);
  pboard.create_tokens(pboard.return_settings());
  //Return number of rotors

  Rotor rotor1;
  rotor1.load_settings(argv[2]); //reads position into token
  rotor1.create_tokens(rotor1.return_settings());
  rotor1.set_curr_token();
  rotor1.set_notch();
  //Initlialise rotors

  
  while (message[n] != '\0')
    {
     
      //Check plugboard configurations
      int check_pb;
      check_pb = pboard.check_config();
      if (check_pb != 0)
	{  
	  cout << error_description(check_pb) << endl;
	  return 0;
	}
      

      //Passes in the new letter of the input class into the plugboard      
      pboard.set_letter(input1.return_letter()); 
      //swap values
      pboard.swap_values(message[n]);

      //Message reaches the rotor
      

      
      cout << message[n];
      n++;
      //Set the new letter on the input switch for the next iteration
      input1.set_letter(n, message);

      
      //
    }
  cout << endl;

  //Developer Checks
  cout << "First pb six tokens:" << endl;
  cout << pboard.get_token(0) << endl;
      cout << pboard.get_token(1) << endl;
      cout << pboard.get_token(2) << endl;
      cout << pboard.get_token(3) << endl;
      cout << pboard.get_token(4) << endl;
      cout << pboard.get_token(5) << endl;

      cout << "Plugboard configurations:" << endl;    
      cout << pboard.return_settings();
      cout << endl;
      cout << "Rotor configurations" << endl;
      cout << rotor1.get_token(0) << endl;
      cout << rotor1.get_token(1) << endl;
      cout << "The notch is at postition: " << rotor1.get_notch(0) << endl;
  


  return 0;

}
