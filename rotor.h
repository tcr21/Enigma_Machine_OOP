// TRamenason
#ifndef ROTOR_H
#define ROTOR_H

#include <vector>
#include <map>

using namespace std; 

class Rotor
{
    private:
        // I. DATA MEMBERS

        // Declare configuration files for rotor
        const char* ro_configuration_file; 
        const char* ro_position_configuration_file; 
        
        // Declare rotor mappings
        map <int, int> ro_mapping;
        map <int, int> ro_mapping_back;

        // Declare notches vector
        vector <int> notches_vector; 

        // Declare rotor index (eg. rotor 0 of 3 rotors numbered 0-3, going right to left)
        int rotor_index_right_to_left; 

        // Declare rotor position variable
        int rotor_position; 

        
        // II. HELPER FUNCTIONS FOR ERROR CHECKS (MAPPINGS/ NOTCHES & POSITION)

        // Check 1. Declare check_open_file helper function: opens file and check for any errors opening file
        bool check_open_file(const char* input_configuration_file, ifstream& in_stream); 

        // Check 2. Declare check_file_not_empty helper function: checks file is not empty (0 is an invalid number of parameters)
        bool check_file_not_empty(const char* input_configuration_file, ifstream& in_stream); 
        
        // Check 3. Declare check_count_of_numbers_below_max helper function: Check number of parameters does not exceed the limit of 52. Note: we use invalid rotor mapping error code for this. We wish to return this error prior to other errors if the max number of parameters is exceeded
        bool check_count_of_numbers_below_max(int count_numbers, const char* input_configuration_file); 

        // Check 4. Declare check_numeric_characters helper function: checks that all characters are numeric
        bool check_numeric_characters(char* number_array, int count_numbers, const char* input_configuration_file); 

        // Check 5. Declare check_valid_index helper function: checks that all numbers are a valid index (between 0-25)
        bool check_valid_index(int number, int count_numbers, const char* input_configuration_file); 

        // Check 6. Declare check_number_not_mapped_to_multiple helper function: checks number is not being mapped to multiple numbers
        bool check_number_not_mapped_to_multiple(map<int, int>::iterator iterator_to_number, int number, int count_numbers, const char* input_configuration_file); 

        // Check 7. Declare check_multiple_notches_not_at_same_position helper function: checks file is not trying to put multiple notches at the same position
        bool check_multiple_notches_not_at_same_position(vector <int> notches_vector, int i, int number, int count_numbers, const char* input_configuration_file); 

        // Check 8. Declare check_count_of_numbers_above_min helper function: checks the count of numbers is at least 26
        bool check_count_of_numbers_above_min(int count_numbers, const char* input_configuration_file); 
        
        // Check 9. Declare check_count_of_numbers_above_rotor_index helper function: checks there are enough positions for each rotor
        bool check_count_of_numbers_above_rotor_index(int count_numbers, int initial_rotor_index, const char* input_configuration_file, int count_rotors);

        
        // III. HELPER FUNCTIONS FOR CONFIGURATION (MAPPING/ NOTCHES)

        // Configure step 1. Declare insert_pair_into_mapping helper function: inserts pair into rotor mappings
        void insert_into_mapping(int& key, int& value, map <int, int>& mapping);

        // Configure step 2. Declare insert_into_notches_vector helped function: inserts pair into rotor notches vector
        void insert_into_notches_vector(int& number, vector <int>& vector, int count_numbers);

        
        // IV. HELPER FUNCTIONS FOR CONFIGURATION (POSITION)

        // Configure step 1. Declare select_rotor_position helper function: selects position of current rotor, using the number at the same index in the rotor position file as the rotor index (both going left to right)
        void select_rotor_position(int count_numbers, int initial_rotor_index, int current_number); 


    public: 
        // V. CONFIGURE FUNCTIONS
        
        // Function 1. Declare ro_configure function: checks rotor configuration file for errors, and uses it to configure rotor maps, notches and set rotor configuration file data member
        int ro_configure_mappings_notches(const char* input_configuration_file); 

        // Function 2. Declare ro_configure_position function: checks rotor position configuration file for errors, and uses it to configure starting position of rotor and set rotor number data member
        int ro_configure_position(const char*input_configuration_file, int count_rotors, int initial_rotor_index, int converted_rotor_index); 


        // VI. NOTCH MOVEMENT FUNCTIONS

        // Function 1. Declare notch_is_hit function: determines whether notch has been reached
        bool notch_is_hit();

        // Function 2. Declare rotate function: updates position of rotor when it rotates 'by one'
        void rotate();


        // VII. ENCRYPT FUNCTIONS

        // Function 1. Declare ro_encrypt function: encrypts a number that passes through the rotor
        int ro_encrypt(int input_number); 

        // Function 2. Declare ro_encrypt_back function: encrypts a number that passes through the rotor on the way back
        int ro_encrypt_back(int input_number); 
        
}; 



#endif