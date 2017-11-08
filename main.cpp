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
  Plugboard pboard(input1.return_letter(), argv[1]);//Constructor also loads settings and creates tokens
  //Return number of rotors depending on how many arguments are entered in the command line
  Rotor rotor[1000];
  int no_rotors;
  /*Change to up to 5 */
  no_rotors = argc - 2; //5 arguments are entered regardless. The only one that may vary is the number of rotors
  for (int c = 0; c <= no_rotors; c++)
    {
      rotor[c].init_rotor(argv[c+2]);
    }
  
  //Rotor rotor1 (argv[2]);//constructor also creates tokens, sets corresponding tokens and sets notches

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
      for (int i = 0; i <= no_rotors; i++)
	{
	  rotor[i].set_letter(message[i]);
	}

      

      
      cout << message[n];
      n++;
      //Set the new letter on the input switch for the next iteration
      input1.set_letter(n, message);

      
      //
    }
  cout << endl;

  //Developer Checks
  cout << "Plugboard configurations:" << endl;    
  cout << pboard.return_settings();
  cout << endl;

  for (int a = 0; a < no_rotors; a++)
    {
      
      cout << "First two rotor configurations for rotor " << a+1 << ":"  << endl;
      cout << rotor[a].get_token(0) << endl;
      cout << rotor[a].get_token(1) << endl;
      cout << "The first notch is at postition: " << rotor[a].get_notch(0) << endl;
      cout << "The second notch is at position: " << rotor[a].get_notch(1) << endl;
      cout << endl;
    }
      cout << "The number of rotors is " << no_rotors << endl;
      

  return 0;

}
