// Tiphaine Ramenason  
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <new>
#include "enigma.h"
#include "errors.h"
#include "plugboard.h"
#include "reflector.h"
#include "rotor.h"

using namespace std; 

// I. FUNCTIONS

// Function 1. Define e_configure function: configures the enigma machine including plugboard, reflector and rotor, returning any relevant error codes as the configuration occurs
int Enigma::e_configure(int count_arguments, char** string_array_arguments)
{
    int error_returned;

    // Check command line arguments, set configuration files for each object and set count of rotors
    error_returned = check_config_command_line_arguments(count_arguments, string_array_arguments); 
    if (error_returned)
    {
        return error_returned;
    }

    // Check & configure plugboard
    error_returned = plugboard.p_configure(plugboard_configuration_file);
    if (error_returned)
    {
        return error_returned;
    }

    // Check & configure reflector
    error_returned = reflector.re_configure(reflector_configuration_file);
    if (error_returned)
    {
        return error_returned;
    }

    // Check & configure rotor. 
    if (count_rotors != 0)
    {   
        error_returned = check_config_rotors();
        if (error_returned)
        {
        return error_returned;
        }
    }
    // PLEASE NOTE: if there are no rotors, the rotor position file will be ignored and its contents will not be checked for errors
    /* if (count_rotors == 0)
    {
        cout << "Please note there are " << count_rotors << " rotors, so the rotor position file will be ignored and its contents will not be checked for errors \n"; // Test
    } */

    // Confirm enigma has been successfully configured
    enigma_is_configured = true; 
    /* cout << "\nSuccess! Enigma has been configured. \n"; // Test */ 

    return 0; 
}

// Function 2. Define e_encrypt_message function: takes in the user's input message and for each character, checks it is valid (returns error if not), converts it to an integer, encrypts it, converts back to a character and outputs it
int Enigma::e_encrypt_message()
{
    // Ensure encrypt function cannot be used unless enigma is configured prior
    if (!enigma_is_configured)
    {
        cerr << "ERROR. Enigma has not been configured so cannot encrypt. \n"; 
        return 1;
    } 

    char input_letter, encrypted_letter; 
    int input_number, encrypted_number; 
    vector <char> input_message = {};
    vector <char> encrypted_message = {};

    /* // Ask user for input
    cout << "\nWelcome to Enigma! Please input the message you wish to encrypt. The encrypted message will appear on the screen. Once this is done, press ctrl+D to close the standard input stream, get a recap, and return to command line. \n"; // Test */
    
    // Check each character
    cin >> ws >> input_letter;
    while (!cin.eof())
    {
        // Check characters are valid
        if (! check_input_character(input_letter))
        {
            return 2; 
        }

        // Put into input vector in case wish to echo user input upon conclusion (when cin stream closed by user), for testing purposes
        input_message.push_back(input_letter); 

        // Encrypt & print to screen
        input_number = input_letter - 65; 
        encrypted_number = encrypt_number(input_number);
        encrypted_letter = encrypted_number + 65; 
        cout << encrypted_letter; 

        // Put into encrypted vector in case wish to repeat encrypted version of message upon conclusion (when cin stream closed by user), for testing purposes
        encrypted_message.push_back(encrypted_letter); 

        // Move to next character
        cin >> ws >> input_letter; 
    }
    cout << endl; 
    /* // Echo user input and result once again, when user closes cin stream // Test
    cout << "\n \nTo recap, your message: \n"; 
    int input_message_length = input_message.size(); 
    for (int i = 0; i < input_message_length; i++) 
    {
        cout << input_message[i];
    } 
    cout << endl << "was encrypted to: \n"; 
    int encrypted_message_length = encrypted_message.size(); 
    for (int i = 0; i < encrypted_message_length; i++) 
    {
        cout << encrypted_message[i];
    } 
    cout << "\nThanks for using enigma today! \n \n"; */

    return 0;   
}


// II. HELPER FUNCTIONS FOR ERROR CHECKS & CONFIGURATION

// Check & config 1. Define check_config_command_line_arguments helper function: checks for errors in command line arguments, sets the input file for each machine part, and sets count of rotors
int Enigma::check_config_command_line_arguments(int count_arguments, char** string_array_arguments)
{
    int check_plugboard_arg = 0, check_reflector_arg = 0, check_rotor_arg = 0,check_rotor_position_arg = 0; 

    // Go through each command line argument
    for (int i = 1; i < count_arguments; i++)
    {
        int length_argument = strlen(string_array_arguments[i]); 
        // Plugboard argument
        if (! strncmp(".pb", string_array_arguments[i] + (length_argument - 3), 3))
        {
            check_plugboard_arg++; 
            strcpy(plugboard_configuration_file, string_array_arguments[i]); 
        }
        // Reflector argument
        if (! strncmp(".rf", string_array_arguments[i] + (length_argument - 3), 3))
        {
            check_reflector_arg++; 
            strcpy(reflector_configuration_file, string_array_arguments[i]); 
        }
        // Rotor argument
        if (! strncmp(".rot", string_array_arguments[i] + (length_argument - 4), 4))
        {
            check_rotor_arg++; 
            rotor_configuration_files.push_back(string_array_arguments[i]); 
        }

        // Rotor position argument
        if (! strncmp(".pos", string_array_arguments[i] + (length_argument - 4), 4))
        {
            check_rotor_position_arg++; 
            strcpy(rotor_position_configuration_file, string_array_arguments[i]); 
        }
    }
    
    // Check one argument per machine part except for rotor
    // Plugboard argument
    if (check_plugboard_arg != 1)
    {
        cerr << "ERROR. The number of plugboard arguments is incorrect. Please also check your reflector, rotor and rotor position arguments. \n"; 
        return 1; 
    }
    // Reflector argument
    if (check_reflector_arg != 1)
    {
        cerr << "ERROR. The number of reflector arguments is incorrect. Please also check your rotor and rotor position arguments. \n"; 
        return 1;
    }
    
    // Rotor argument: there can be any number of rotors, including 0, so no check for number of rotor arguments here
    /* cout << "Note: you have included " << check_rotor_arg << " rotor(s). \n"; // Test */

    // Rotor position argument: in the case of no rotors, program should still expect last file to specify rotor positions, so check included here
    if (check_rotor_position_arg != 1)
    {
        cerr << "ERROR. The number of rotor position arguments is incorrect. \n"; 
        return 1; 
    }

    /* cout << "\nSuccess! Number of command line arguments is valid. \n"; // Test */ 

    // Set count of rotors data member
    count_rotors = rotor_configuration_files.size(); 
    /* cout << "Note: to confirm, you have included " << count_rotors << " rotor(s). \n"; // Test */

    return 0; 
}

// Check & config 2. Define check_config_rotors helper function: for each rotor configuration file, checks for errors and creates a new rotor
int Enigma::check_config_rotors()
{
    int error_returned = 0, initial_rotor_index = 0; 

    // Go through vector of rotor configuration files (note count_rotors has previously been set to the size of that vector). For each: create a rotor, configure mappings/ notches, configure position, and put into enigma's vector of rotors
    for (int i = 0; i < count_rotors; i++)
    {
        // Create new rotor
        Rotor* current_rotor_ptr = new Rotor;

        // Configure mappings/ notches
        error_returned = current_rotor_ptr->ro_configure_mappings_notches(rotor_configuration_files[i]); 
        if (error_returned)
        {
        return (error_returned);
        }

        // Configure position
        error_returned = current_rotor_ptr->ro_configure_position(rotor_position_configuration_file, count_rotors, initial_rotor_index, (count_rotors - (initial_rotor_index + 1))); 
        if (error_returned)
        {
        return (error_returned);
        }

        // Put into vector of rotors
        vector_rotors.push_back(current_rotor_ptr);
        /* cout << "Success! Rotor of initial index " << initial_rotor_index << " has been added to enigma's vector of rotors. \n"; // Test */

        // Increment rotor index
        initial_rotor_index++; 
    }

    return 0; 
}


// Check & config 3. Declare check_input_characters helper function: checks that an input character is valid (A-Z)
bool Enigma::check_input_character(char letter)
{
    int number = letter;
    if (number < 65 || number > 90)
    {
        cerr << " ...ERROR. You have input an invalid character: " << letter << ". Please also check your other characters, and ensure you only include capital letters from A-Z. \n"; 
        return false;
    }
    return true;
}


// III. HELPER FUNCTIONS FOR MOVING THROUGH ROTORS

// Rotors 1. Declare move_through_rotors helper function: passes the integer from right-most to left-most rotor 
int Enigma::move_through_rotors(int input_number)
{
    int encrypted_number = input_number; 
    bool notch_was_hit = false; 

    // Move through rotors right to left and for each: rotate as required, and encrypt number
    /* cout << "Moving from rotors right to left... \n"; // Test */
    for (int i = count_rotors - 1; i >= 0; i--)
    {
        // If right most rotor, rotate for every input number
        if (i == (count_rotors - 1))
        {
            /* cout << "Rotor of initial index " << i << " so always rotates \n"; // Test */
            vector_rotors[i]->rotate();
        }
        // If not right most rotor, only rotate if notch was hit by previous rotor
        else
        {
            /* cout << "Rotor of initial index " << i << " so only rotates if notch was hit by previous rotor \n"; // Test */
            if (notch_was_hit)
            {
                /* cout << "Notch was hit by previous rotor so rotor of initial index " << i << " rotates \n"; // Test */
                vector_rotors[i]->rotate();
            }
        }

        // Encrypt
        /* cout << "Rotor of initial index " << i << " has encrypted " << encrypted_number << "."; // Test */
        encrypted_number = vector_rotors[i]->ro_encrypt(encrypted_number); 
        /* cout << ". To confirm, " << encrypted_number << endl; // Test */

        // Check if notch was hit by current rotor, so next rotor 'knows' it needs to rotate when go back through loop
        notch_was_hit = vector_rotors[i]->notch_is_hit(); 
        /* cout << "Notch was hit: " << notch_was_hit << ". If 1 (true), next rotor (if there is one) will rotate \n"; // Test */
    }
    return encrypted_number; 
}

// Rotors 2. Declare move_back_through_rotors helper function: passes the integer back from left-most to right-most rotor
int Enigma::move_back_through_rotors(int input_number)
{
    int encrypted_number = input_number;
    // Move through rotors left to right and for each: encrypt (no rotation on the way back)
    for (int i = 0; i < count_rotors; i++)
    {
        /* cout << "On way back, rotor of initial index " << i << " has encrypted " << encrypted_number << " to "; // Test */
        encrypted_number = vector_rotors[i]->ro_encrypt_back(encrypted_number); 
        /* cout << encrypted_number << endl; // Test */
    }
    return encrypted_number; 
}


// IV. HELPER FUNCTION FOR ENCRYPTING

// Encrypt 1. Define encrypt_number helper function: encrypts the integer by passing it through the plugboard, rotors, reflector, back through rotors, and back through plugboard 
int Enigma::encrypt_number(int input_number)
{
    int encrypted_number; 

    encrypted_number = plugboard.p_encrypt(input_number); 

    if (count_rotors != 0)
    {
        encrypted_number = move_through_rotors(encrypted_number);
    }

    encrypted_number = reflector.re_encrypt(encrypted_number); 

    if (count_rotors != 0)
    {
        encrypted_number = move_back_through_rotors(encrypted_number); 
    }

    encrypted_number = plugboard.p_encrypt(encrypted_number);

    /* cout << "Enigma has encrypted " << input_number << " to " << encrypted_number << endl; // Test */ 
    return encrypted_number; 
}


// V. DESTRUCTOR
// Destructor 1. Declare destructor: ensures rotors created on the heap with 'new' operator are deleted
Enigma::~Enigma()
{
    if (! vector_rotors.empty())
    {
        for (int i = 0; i < count_rotors; i++)
        {
        delete vector_rotors[i]; 
        }
    }
}

