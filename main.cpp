//Konstantin Hemker
//Object oriented design and programming
//Assessed exercise 2

/*
The following main program walks the programmer/reader through the individual steps 
in the enigma machine. The setup can be roughly divided into four parts. 
(1) Set up the enigma elements/classes (plugboard, rotors and reflector) 
according to the configurations specified in the files entered in the command line. 
(2) Check the configurations of this setup for potential input. The different error
codes and conditions are specified in enigma.h 
(3) Read in the message from command line, shift up the rotor before the processing
 circuit is cloded, i.e. < input.txt file and check for its validity
(4) If the message is valid, the letter is processed by the enigma in the below order
and returns the processed value. After finishing, the next letter is read through the 
system

                      &              &             &
=========  <------ ROTOR[0]<------ROTOR[1]<-----ROTOR[2]<----- ========   
         |                                                    |         |
REFLECTOR|&                                                  &|PLUGBOARD| < cin                 
         |                                                    |         |
========= -------> ROTOR[0]------>ROTOR[2]----->ROTOR[2]-----> =========
                      &              &             &
Note (1): In our model, the number of rotors could be scaled up depending on 
the number of command line inputs. This could have been a useful feature for 
the Germans back then, so let's give credit to Bjarne Stoustrup for not being 
born 40 years earlier.  

Note (2): At each '&', there is a wiring process happening (i.e. letter passed through the system
is mapped) to the letter it is mapped to. */


/* Main part of the enigma program */

#include <iostream>
#include <cstdlib>
#include <cstring>

#include "enigma.h"


int main(int argc, char** argv)   {
  
  vector<int> posMapping; //holds the position mapping for all instances of rotors (hence not in class)
  char message; 
  int noRotors; 
  int errorCode=0;

  /*Specifying the number of rotors*/
  if (argc < 4) //with no rotors, the minimum number of arguments to be entered would be 3 
    noRotors = 0;
  else
    noRotors = argc-4; //remaining arguments minus the 3 other arguments and .pos file
 

  
  /*Part(1) Setting up the enigma machine*/
  Plugboard plugboard (argv[1], errorCode);
  Rotor rotor[noRotors+1];
  initialize_rotors(noRotors, argv, errorCode, posMapping, rotor); //initializes entire array
  Reflector reflector(argv[2], errorCode); 
  
  /*Part(2) Checking enigma setup*/
  check_enigma_setup(argc, argv, noRotors, errorCode, plugboard, rotor, reflector, posMapping);
  if (errorCode > 0)
    return errorCode;

  /*Part(3) Read from command line and check the rotor*/ 
  cin >> std::ws >> message;
  
  while (!cin.eof())   {
    
    rotor[noRotors-1].rotate_up(noRotors-1, rotor, noRotors);
    
    check_message_input(message, errorCode);
    if (errorCode > 0)
      return errorCode;
    
    /*Part(4) Process the message and print out the processed message*/  
    plugboard.swap_values(message);
    
    rotor[noRotors-1].mapping_inwards(message, rotor, noRotors, noRotors-1); //recursive function (applies to all rotors)
    
    reflector.swap_values(message);
      
    rotor[0].mapping_outwards(message, rotor, noRotors, 0); //recursive function (applies to all rotors)
      
    plugboard.swap_values(message);
      
    cout << message;

    cin >> std::ws >> message;
  }
  
  return 0;

}
