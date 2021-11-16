// Tiphaine Ramenason  
#ifndef ENIGMA_H
#define ENIGMA_H

#include <vector>
#include "plugboard.h"
#include "reflector.h"
#include "rotor.h"

class Enigma
{
    private:
        // I. CONFIGURATION FILES
        char plugboard_configuration_file[100];
        char reflector_configuration_file[100];
        std::vector <char*> rotor_configuration_files;
        char rotor_position_configuration_file[100];
        bool enigma_is_configured = false;
        

        // II. MACHINE PARTS
        Plugboard plugboard;
        int count_rotors = 0;
        std::vector <Rotor*> vector_rotors; 
        Reflector reflector; 


        // III. HELPER FUNCTIONS FOR ERROR CHECKS & CONFIGURATION

        // Check & config 1. Declare check_config_command_line_arguments helper function: checks for errors in command line arguments, sets the input file for each machine part and sets count of rotors
        int check_config_command_line_arguments(int count_arguments, char** string_array_arguments); 

        // Check & config 2. Declare check_config_rotors helper function: for each rotor configuration file, checks for errors and creates a new rotor
        int check_config_rotors(); 

        // Check & config 3. Declare check_input_characters helper function: checks that an input character is valid (A-Z)
        bool check_input_character(char letter); 
        
        
        // IV. HELPER FUNCTIONS FOR MOVING THROUGH ROTORS

        // Rotors 1. Declare move_through_rotors helper function: passes the integer from right-most to left-most rotor 
        int move_through_rotors(int input_number); 

        // Rotors 2. Declare move_back_through_rotors helper function: passes the integer back from left-most to right-most rotor
        int move_back_through_rotors(int input_number);

        
        // V. HELPER FUNCTION FOR ENCRYPTING

        // Encrypt 1. Declare encrypt_number helper function: encrypts the integer by passing it through the plugboard, rotors, reflector, back through rotors, and back through plugboard
        int encrypt_number(int input_number); 


    public: 
        // VI. FUNCTIONS

        // Function 1. Declare e_configure function: configures the enigma machine including plugboard, reflector and rotor, returning any relevant error codes as the configuration occurs
        int e_configure(int count_arguments, char** string_array_arguments); 

        // Function 2. Declare e_encrypt_message function: takes in the user's input message and for each character, checks it is valid (returns error if not), converts it to an integer, encrypts it, converts back to a character and outputs it
        int e_encrypt_message(); 


        // VII. DESTRUCTOR 
        
        // Destructor 1. Declare destructor: ensures rotors created on the heap with 'new' operator are deleted
        ~Enigma(); 

}; 

#endif
