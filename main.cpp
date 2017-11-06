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

  InputSwitch input1(message[n]);
  Plugboard pboard(input1.return_letter());//Constructor also initialises the settings
  pboard.get_tokens(pboard.return_settings());

  
  while (message[n] != '\0')
    {
     
      //Check plugboard configurations
      int check;
      check = pboard.check_config();
      if (check != 0)
	{  
	  cout << error_description(check) << endl;
	  return 0;
	}
      

      //Passes in the new letter of the input class into the plugboard      
      pboard.set_letter(input1.return_letter()); 
      //swap values
      pboard.swap_values(message[n]);


      
      cout << message[n];
      n++;
      //Set the new letter on the input switch for the next iteration
      input1.set_letter(n, message);

      
      //
    }
  cout << endl;

  //Developer Checks
  cout << "First six tokens:" << endl;
  cout << pboard.return_token(0) << endl;
      cout << pboard.return_token(1) << endl;
      cout << pboard.return_token(2) << endl;
      cout << pboard.return_token(3) << endl;
      cout << pboard.return_token(4) << endl;
      cout << pboard.return_token(5) << endl;

      cout << "Plugboard configurations:" << endl;    
      cout << pboard.return_settings();
            cout << endl;
  

  


  return 0;

}
