// Tiphaine Ramenason
#include <iostream>
#include <fstream>
#include <map>
#include <cstring>
#include "errors.h"
#include "reflector.h"

using namespace std; 

// I. FUNCTIONS

// Function 1. Define re_configure function: reflector configuration file for errors, and uses it to configure reflector map and set reflector configuration file data member
int Reflector::re_configure(const char input_configuration_file[100])
{
    /* cout << "\nStarting Reflector configuration... \n"; // Test */

    // Declare input stream: used to access file
    ifstream in_stream; 

    // Declare variables: string for each number in file, numbers converted into integers (2 variables since numbers are in pairs), count of numbers 
    char number_array[10];
    int first_number, second_number, count_numbers = 0; 

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
        return 10; 
    }
   
    while (!in_stream.eof())
    {    
        // Check 3. Check number of parameters does not exceed the limit of 26. Note: we wish to return this error prior to other errors if the max number of parameters is exceeded
        if (! check_count_of_numbers_below_max(count_numbers, input_configuration_file))
        {
            return 10; 
        }
        
        // Check 4. Check that all characters are numeric
        if (! check_numeric_characters(number_array, count_numbers, input_configuration_file))
        {
            return 4; 
        }

        // Convert characters into integers
        second_number = stoi(number_array); 
        /* cout << second_number << ", "; // Test */
        
        // Check 5. Check that all numbers are a valid index (between 0-25)
        if (! check_valid_index(second_number, count_numbers, input_configuration_file))
        {
            return 3; 
        }

        // Check configuration
        if ((count_numbers % 2) == 0) // Number is the first of a pair
        {
            for (auto iterator_to_number = re_mapping.begin(); iterator_to_number != re_mapping.end(); iterator_to_number++)
            {
                // Check 6a. Check number is not being mapped to multiple numbers (first of pair)
                if (! check_number_not_mapped_to_multiple(iterator_to_number, second_number, count_numbers, input_configuration_file))
                {
                    return 9; 
                } 
            }
                    
            // Number is first of a pair so set first number variable
            first_number = second_number; 
        }
        else if ((count_numbers % 2) == 1) // Number is the second of a pair
        {
            // Check 7. Check number is not being mapped to itself (in a pair)
            if (! check_number_not_mapped_to_itself(first_number, second_number, count_numbers, input_configuration_file))
            {
                return 9; 
            }
            
            for (auto iterator_to_number = re_mapping.begin(); iterator_to_number != re_mapping.end(); iterator_to_number++)
            {
                // Check 6b. Check number is not being mapped to multiple numbers (second of pair)
                if (! check_number_not_mapped_to_multiple(iterator_to_number, second_number, count_numbers, input_configuration_file))
                {
                    return 9; 
                }
            }

            // Configure map step 1. Insert pair into reflector mapping 
            insert_pair_into_mapping(first_number, second_number, re_mapping);
        }

        // Move to next number string in file
        in_stream >> ws >> number_array;

        // Increment count of numbers
        count_numbers++;
    }

    // Close file
    in_stream.close(); 

    // Check 8. Check the count of numbers is correct (26 ie 13 pairs)
    if (! check_count_of_numbers_correct(count_numbers, input_configuration_file))
    {
        return 10; 
    }

    /* cout << "Success! All checks in " << input_configuration_file << " have been completed \n"; // Test */
    /* cout << "Success! Reflector mapping has been configured \n"; // Test */

    // Set reflector configuration file data member to input file which has just been checked
    re_configuration_file = input_configuration_file;
    /* cout << "Success! Reflector configuration file has been set to " << re_configuration_file << endl; // Test */

    return 0; 
}
    
// Function 2. Define re_encrypt function: encrypts a number that passes through the reflector
int Reflector::re_encrypt(int input_number)
{
    /* cout << "Reflector has encrypted " << input_number << " to " << re_mapping[input_number] << endl; // Test */
    return re_mapping[input_number]; 
}


// II. HELPER FUNCTIONS FOR ERROR CHECKS

// Note: position as used in error messages starts from 0.

// Check 1. Define check_open_file helper function: opens file and check for any errors opening file
bool Reflector::check_open_file(const char input_configuration_file[100], ifstream& in_stream)
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
bool Reflector::check_file_not_empty(const char input_configuration_file[100], ifstream& in_stream)
{
    if (in_stream.eof())
    {
        cerr << "ERROR. File " << input_configuration_file << " is empty. Exactly 26 parameters (13 pairs) are needed \n"; 
        return false; 
    }
    return true; 
}

// Check 3. Define check_count_of_numbers_below_max helper function: checks number of parameters does not exceed the limit of 26. Note: we wish to return this error prior to other errors if the max number of parameters is exceeded
bool Reflector::check_count_of_numbers_below_max(int count_numbers, const char input_configuration_file[100])
{
    if (count_numbers >= 26)
    {
        cerr << "ERROR. File " << input_configuration_file << " has more than 26 parameters \n"; 
        return false;
    }
    return true; 
}
    
// Check 4. Define check_numeric_characters helper function: checks that all characters are numeric
bool Reflector::check_numeric_characters(char number_array[10], int count_numbers, const char input_configuration_file[100])
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
bool Reflector::check_valid_index(int number, int count_numbers, const char input_configuration_file[100])
{
    if (number < 0 || number > 25)
        {
            cerr << "ERROR. Invalid index: " << number << ", at position " << count_numbers << " in file " << input_configuration_file << ", is not between 0 and 25 \n"; 
            return false; 
        }
    return true; 
}
 
// Check 6. Define check_number_not_mapped_to_multiple helper function: checks number is not being mapped to multiple numbers
bool Reflector::check_number_not_mapped_to_multiple(map<int, int>::iterator iterator_to_number, int number, int count_numbers, const char input_configuration_file[100])
{
    if (iterator_to_number->second == number)
    {
        cerr << "ERROR. Impossible reflector configuration: number " << number << ", at position " << count_numbers << " in file " << input_configuration_file << ", has already been mapped, so " << number << " is being mapped to multiple numbers \n"; 
        return false; 
    }
    return true; 
}

// Check 7. Define check_number_not_mapped_to_itself helper function: checks number is not being mapped to itself
bool Reflector::check_number_not_mapped_to_itself(int first_number, int second_number, int count_numbers, const char input_configuration_file[100])
{
    if (second_number == first_number)
    {
        cerr << "ERROR. Impossible reflector configuration: number " << second_number << ", at position  " << count_numbers << " in file " << input_configuration_file << ", is being mapped to itself \n"; 
        return false;
    }
    return true; 
}

// Check 8. Define check_count_of_numbers_even helper function: checks the count of numbers is even
bool Reflector::check_count_of_numbers_correct(int count_numbers, const char input_configuration_file[100])
{
    if (count_numbers != 26)
    {
        cerr << "ERROR. Incorrect number of reflector parameters in file " << input_configuration_file << ": " << count_numbers << " is not equal to 26 (exactly 13 pairs are needed) \n"; 
        return false; 
    }
    /* cout << "There are " << count_numbers << " numbers in file " << input_configuration_file << endl; // Test */
    return true; 
}            
                

// III. HELPER FUNCTIONS FOR MAPPING CONFIGURATION

// Configure map step 1. define insert_pair_into_mapping helper function: inserts pair into reflector mapping 
void Reflector::insert_pair_into_mapping(int& first_number, int& second_number, map <int, int>& mapping)
{
    pair <int, int> p1(first_number, second_number);
    pair <int, int> p2(second_number, first_number);
    /* cout << p1.first << " " << p1.second << ", "; // Test */
    mapping.insert(p1);
    /* cout << p2.first << " " << p2.second << endl; // Test */
    mapping.insert(p2);
} 