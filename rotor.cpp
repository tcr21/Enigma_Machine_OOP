// Tiphaine Ramenason 
#include <iostream>
#include <fstream>
#include <map>
#include <cstring>
#include "errors.h"
#include "rotor.h"

using namespace std; 

// I. CONFIGURE FUNCTIONS

// Function 1. Declare ro_configure_mappings_notches function: checks rotor configuration file for errors, and uses it to configure rotor maps, notches, and set rotor configuration file data member
int Rotor::ro_configure_mappings_notches(const char* input_configuration_file)
{
    /* cout << "\nStarting Rotor configuration... \n"; // Test */ 

    // Declare input stream: used to access file
    ifstream in_stream; 

    // Declare variables: string for each number in file, number converted into integers, count of numbers 
    char number_array[10];
    int current_number, count_numbers = 0; 

    // Check 1. Open file and check for any errors opening file
    if (! check_open_file(input_configuration_file, in_stream))
    {
        return 11;
    } 

    // Read file and conduct checks on each number as and when it is read, returning error codes as and when the file is read
    in_stream >> ws >> number_array;

    // Check 2. Check file is not empty (0 is an invalid number of parameters)
    if (! check_file_not_empty(input_configuration_file, in_stream))
    {
        return 7; 
    }
   
    while (!in_stream.eof())
    {    
        // Check 3. Check number of parameters does not exceed the limit of 52. Note: we use invalid rotor mapping error code for this. We wish to return this error prior to other errors if the max number of parameters is exceeded
        if (! check_count_of_numbers_below_max(count_numbers, input_configuration_file))
        {
            return 7; 
        }
        
        // Check 4. Check that all characters are numeric
        if (! check_numeric_characters(number_array, count_numbers, input_configuration_file))
        {
            return 4; 
        }

        // Convert characters into integers
        current_number = stoi(number_array);
        /* cout << current_number << " "; // Test */
        
        // Check 5. Check that all numbers are a valid index (between 0-25)
        if (! check_valid_index(current_number, count_numbers, input_configuration_file))
        {
            return 3; 
        }

        // Check configuration
        if (count_numbers < 26) // Number is part of rotor mapping
        {
            for (auto iterator_to_number = ro_mapping.begin(); iterator_to_number != ro_mapping.end(); iterator_to_number++)
            {
                // Check 6. Check number is not being mapped to multiple numbers
                if (! check_number_not_mapped_to_multiple(iterator_to_number, current_number, count_numbers, input_configuration_file))
                {
                    return 7; 
                }
            }
            // Configure step 1. Insert into rotor mappings
            /* cout << "Mappings: "; // Test */
            insert_into_mapping(count_numbers, current_number, ro_mapping);
            /* cout << ", "; // Test */
            insert_into_mapping(current_number, count_numbers, ro_mapping_back);
            /* cout << " | "; // Test */
        }
        else // Number is part of notches
        {
            int notches_vector_length = notches_vector.size();
            for (int i = 0; i < notches_vector_length; i++)
            {
                // Check 7. Check file is not trying to put multiple notches at the same position
                if (! check_multiple_notches_not_at_same_position(notches_vector, i, current_number, count_numbers, input_configuration_file))
                {
                    return 7; 
                }
            }
            // Configure step 2. Insert into notches vector
            insert_into_notches_vector(current_number, notches_vector, count_numbers);
        }

        // Move to next number string in file
        in_stream >> ws >> number_array;

        // Increment count of numbers
        count_numbers++;
    }

    // Close file
    in_stream.close(); 

    // Check 8. Check the count of numbers is at least 26
    if (! check_count_of_numbers_above_min(count_numbers, input_configuration_file))
    {
        return 7; 
    }

    /* cout << "Success! All checks in " << input_configuration_file << " have been completed \n"; // Test
    cout << "Success! Rotor mapping and notches have been configured \n"; // Test */

    // Set rotor configuration file data member to input file which has just been checked
    ro_configuration_file = input_configuration_file;
    /* cout << "Success! Rotor configuration file has been set to " << ro_configuration_file << endl; // Test */

    return 0; 
}

// Function 2. Declare ro_configure_position function: checks rotor position configuration file for errors, and uses it to configure starting position of rotor and set rotor number data member
int Rotor::ro_configure_position(const char*input_configuration_file, int count_rotors, int initial_rotor_index, int converted_rotor_index)
{
    /* cout << "\nStarting Rotor position configuration... \n"; // Test */

    // Declare input stream: used to access file
    ifstream in_stream; 

    // Declare variables: string for each number in file, number converted into integers, count of numbers 
    char number_array[10];
    int current_number, count_numbers = 0; 

    // Check 1. Open file and check for any errors opening file
    if (! check_open_file(input_configuration_file, in_stream))
    {
        return 11;
    } 

    // Read file and conduct checks on each number as and when it is read, returning error codes as and when the file is read
    in_stream >> ws >> number_array;
   
    while (!in_stream.eof())
    {    
        // Check 2. Check that all characters are numeric
        if (! check_numeric_characters(number_array, count_numbers, input_configuration_file))
        {
            return 4; 
        }

        // Convert characters into integers
        current_number = stoi(number_array);
        /* cout << current_number << ", "; // Test */
        
        // Check 3. Check that all numbers are a valid index (between 0-25)
        if (! check_valid_index(current_number, count_numbers, input_configuration_file))
        {
            return 3; 
        }

        // Configure step 1. Select position of current rotor, using the number at the same index in the file as the rotor index (both going left to right)
        select_rotor_position(count_numbers, initial_rotor_index, current_number); 

        // Move to next number string in file
        in_stream >> ws >> number_array;

        // Increment count of numbers
        count_numbers++;
    }

    // Close file
    in_stream.close(); 

    // Check 4. Check there are enough positions for each rotor
    if (! check_count_of_numbers_above_rotor_index(count_numbers, initial_rotor_index, input_configuration_file, count_rotors))
    {
        return 8; 
    }

    // PLEASE NOTE: if there are more positions in the file than there are rotors, the extra positions are simply ignored and the configuration proceeds
    if (count_numbers > count_rotors)
    {
        /* cout << "Please note that file " << input_configuration_file << " contains " << count_numbers << " starting positions, yet there are only " << count_rotors << " rotors. The extra starting positions in the file are simply being discarded and configuration is proceeding normally \n"; // Test */
    }

    // Set rotor position configuration file data member to input file which has just been checked
    ro_position_configuration_file = input_configuration_file;
    /* cout << "Success! Rotor of initial index " << initial_rotor_index << " has had its position configuration file set to " << ro_position_configuration_file << endl; // Test */

    // Set rotor index (right to left) data member to converted rotor index (ie right to left, not left to right)
    rotor_index_right_to_left = converted_rotor_index; 
    /* cout << "Success! Rotor of initial index " << initial_rotor_index << " has had its converted index (right to left) set to " << rotor_index_right_to_left << endl; // Test */

    return 0;
}


// II. NOTCH MOVEMENT FUNCTIONS

// Function 1. Define notch_is_hit function: determines whether notch has been reached
bool Rotor::notch_is_hit()
{
    int count_notches = notches_vector.size(); 
    for (int i = 0; i < count_notches; i++)
    {
        if (rotor_position == notches_vector[i])
        {
            /* cout << "Rotor of converted index " << rotor_index_right_to_left << " has a notch that has been hit at rotor position " << rotor_position << endl; // Test */
            return true; 
        }
    }
    /* cout << "Rotor of converted index " << rotor_index_right_to_left << " does not have a notch at rotor position " << rotor_position << endl; // Test */
    return false; 
}

// Function 2. Define rotate function: updates position of rotor when it rotates 'by one'
void Rotor::rotate()
{
    // If index 25 (Z) on rim reaches the 'absolute' top, rotation means index 25 (Z) on rim 'drops' to the bottom, and index 0 (A) on rim finds itself at the top given circular nature
    if (rotor_position == 25)
    {
        rotor_position = 0; 
    }
    else
    {
        rotor_position++; 
    }
    /* cout << "Rotor of converted index " << rotor_index_right_to_left << " has rotated. Updated rotor position is " << rotor_position << endl; // Test */
}


// III. ENCRYPT FUNCTIONS

// Function 1. Declare ro_encrypt function: encrypts a number that passes through the rotor
int Rotor::ro_encrypt(int input_number)
{
    int encrypted_number; 

    // Adjust number at input considering rotor position (needs to be relative to absolute frame) & encrypt
    if ((input_number + rotor_position) >= 26)
    {
        encrypted_number = ro_mapping[input_number + rotor_position - 26]; 
    }
    else
    {
        encrypted_number = ro_mapping[input_number + rotor_position];
    }

    // Adjust number at output considering rotor position (needs to be relative to absolute frame before moving on to next machine component)
    if ((encrypted_number - rotor_position) < 0)
    {
        encrypted_number = encrypted_number - rotor_position + 26; 
    }
    else
    {
        encrypted_number = encrypted_number - rotor_position;
    }

    /* cout << "On way forward, " << input_number << " was encrypted to " << encrypted_number << endl; // Test */
    return encrypted_number; 
}

// Function 2. Declare ro_encrypt_back function: encrypts a number that passes through the rotor on the way back
int Rotor::ro_encrypt_back(int input_number)
{
    int encrypted_number; 

    if ((input_number + rotor_position) >= 26)
    {
        encrypted_number = ro_mapping_back[input_number + rotor_position - 26]; 
    }
    else
    {
        encrypted_number = ro_mapping_back[input_number + rotor_position];
    }

    // Adjust number at output considering rotor position (needs to be relative to absolute frame before moving on to next machine component)
    if ((encrypted_number - rotor_position) < 0)
    {
        encrypted_number = encrypted_number - rotor_position + 26; 
    }
    else
    {
        encrypted_number = encrypted_number - rotor_position;
    }

    /* cout << "On way back, " << input_number << " was encrypted to " << encrypted_number << endl; // Test */
    return encrypted_number; 
}


// IV. HELPER FUNCTIONS FOR ERROR CHECKS

// Check 1. Definecheck_open_file helper function: opens file and check for any errors opening file
bool Rotor::check_open_file(const char* input_configuration_file, ifstream& in_stream)
{
    in_stream.open(input_configuration_file); 
    if (in_stream.fail())
    {
        cerr << "ERROR. File " << input_configuration_file << " could not be opened \n"; 
        return false; 
    }
    return true; 
}

// Check 2. Define check_file_not_empty helper function: checks file is not empty (0 is an invalid number of parameters)
bool Rotor::check_file_not_empty(const char input_configuration_file[100], ifstream& in_stream)
{
    if (in_stream.eof())
    {
        cerr << "ERROR. File " << input_configuration_file << " is empty. At least 26 parameters are needed \n"; 
        return false; 
    }
    return true; 
}

// Check 3. Define check_count_of_numbers_below_max helper function: Check number of parameters does not exceed the limit of 52. Note: we use invalid rotor mapping error code for this. We wish to return this error prior to other errors if the max number of parameters is exceeded
bool Rotor::check_count_of_numbers_below_max(int count_numbers, const char input_configuration_file[100])
{
    if (count_numbers >= 52)
    {
        cerr << "ERROR. File " << input_configuration_file << " has more than 52 parameters. Only 26 parameters for rotor mapping and 26 notches maximum allowed \n"; 
        return false;
    }
    return true; 
}

// Check 4. Define check_numeric_characters helper function: checks that all characters are numeric
bool Rotor::check_numeric_characters(char number_array[10], int count_numbers, const char input_configuration_file[100])
{
    for (int i = 0; i < static_cast<int>(strlen(number_array)); i++)
    {
        if (!isdigit(number_array[i]))
        {
            cerr << "ERROR. Non numeric character: " << number_array[i] << ", at position " << count_numbers << " in file " << input_configuration_file << ", is not a numeric character \n"; 
            return false; 
        }
    }
    return true; 
}

// Check 5. Define check_valid_index helper function: checks that all numbers are a valid index (between 0-25)
bool Rotor::check_valid_index(int number, int count_numbers, const char input_configuration_file[100])
{
    if (number < 0 || number > 25)
        {
            cerr << "ERROR. Invalid index: " << number << ", at position " << count_numbers << " in file " << input_configuration_file << ", is not between 0 and 25 \n"; 
            return false; 
        }
    return true; 
}

// Check 6. Define check_number_not_mapped_to_multiple helper function: checks number is not being mapped to multiple numbers
bool Rotor::check_number_not_mapped_to_multiple(map<int, int>::iterator iterator_to_number, int number, int count_numbers, const char input_configuration_file[100])
{
    if (iterator_to_number->second == number)
    {
        cerr << "ERROR. Impossible rotor configuration: number " << number << ", at position " << count_numbers << " in file " << input_configuration_file << ", has already been mapped, so " << number << " is being mapped to multiple numbers \n"; 
        return false; 
    }
    return true; 
}

// Check 7. Define check_multiple_notches_not_at_same_position helper function: checks file is not trying to put multiple notches at the same position
bool Rotor::check_multiple_notches_not_at_same_position(vector <int> notches_vector, int i, int number, int count_numbers, const char input_configuration_file[100])
{
    if (notches_vector[i] == number)
    {
        cerr << "ERROR. Impossible rotor configuration: number " << number << ", at position " << count_numbers << " in file " << input_configuration_file << ", already has a notch on it, so multiple notches are being put on the same number. \n"; 
        return false; 
    }
    return true; 
}

// Check 8. Define check_count_of_numbers_above_min helper function: checks the count of numbers is at least 26
bool Rotor::check_count_of_numbers_above_min(int count_numbers, const char input_configuration_file[100])
{
    if (count_numbers < 26)
    {
        cerr << "ERROR. Incorrect number of rotor parameters in file " << input_configuration_file << ": " << count_numbers << " is below minimum of 26 parameters required \n"; 
        return false; 
    }
    /* cout << "There are " << count_numbers << " numbers in file " << input_configuration_file << endl; // Test */
    return true; 
}

// Check 9. Declare check_count_of_numbers_above_rotor_index helper function: checks there are enough positions for each rotor
bool Rotor::check_count_of_numbers_above_rotor_index(int count_numbers, int initial_rotor_index, const char input_configuration_file[100], int count_rotors)
{
    if (count_numbers <= initial_rotor_index)
    {
        cerr << "ERROR. Insufficient rotor starting positions: rotor of initial (0 based) index " << initial_rotor_index << " does not have a starting position. File " << input_configuration_file << " only has " << count_numbers << " position parameter(s), yet there are " << count_rotors << " rotors \n"; 
        return false; 
    }
    return true; 
}


// V. HELPER FUNCTIONS FOR CONFIGURATION (MAPPING/NOTCHES)

// Configure step 1. Declare insert_pair_into_mapping helped function: inserts pair into rotor mappings
void Rotor::insert_into_mapping(int& key, int& value, map <int, int>& mapping)
{
    pair <int, int> p1(key, value);
    /* cout << p1.first << " " << p1.second; // Test */
    mapping.insert(p1);
} 

// Configure step 2. Declare insert_into_notches_vector helped function: inserts pair into rotor notches vector
void Rotor::insert_into_notches_vector(int& number, vector <int>& vector, int count_numbers)
{
    /* cout << "Notch: " << number << ", at position " << count_numbers << "| "; // Test */ 
    notches_vector.push_back(number); 
}


// VI. HELPER FUNCTIONS FOR CONFIGURATION (POSITION)

// Configure step 1. Define select_rotor_position helper function: selects position of current rotor, using the number at the same index in the rotor position file as the rotor index (both going left to right)
void Rotor::select_rotor_position(int count_numbers, int initial_rotor_index, int current_number)
{
    if (count_numbers == initial_rotor_index)
    {
        rotor_position = current_number;
        /* cout << "Success! Rotor of initial index " << initial_rotor_index << " has had its position set to " << rotor_position << " as number of same index " << count_numbers << " in the rotor position file is " << current_number << endl; // Test */
    }
}


