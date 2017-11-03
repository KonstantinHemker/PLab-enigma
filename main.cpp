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

  char message[512]; 
  //What word is entered in the input switch? (character by character)
  cout << "Enter the message you would like to encode/decode: " << endl;
  cin.getline (message, 511);

  
  //get the input for the input switch
  for (int n=0; message[n] != '\0'; n++)
    {
      InputSwitch input1(message[n]);
      //Initialises the letter from the input switch using its constructor

      //Pass the value to the Plugboard

      Plugboard pboard(input1.return_letter(), argv[1]); // reads in first argument from terminal
      //created a character array in class Plugboard that carries its settings
      //swap values


      //
    }
  cout << endl;
  
  

  


  return 0;

}
