// TRamenason
#ifndef PLUGBOARD_H
#define PLUGBOARD_H

#include <map>

using namespace std; 

class Plugboard
{
    private:
        // I. DATA MEMBERS

        // Declare configuration file for plugboard
        const char* p_configuration_file; 
        
        // Declare plugboard mapping
        map <int, int> p_mapping;

        
        // II. HELPER FUNCTIONS FOR ERROR CHECKS

        // Check 1. Declare check_open_file helper function: opens file and check for any errors opening file
        bool check_open_file(const char* input_configuration_file, ifstream& in_stream); 

        // Check 2. Declare check_count_of_numbers_below_max helper function: checks number of parameters does not exceed the limit of 26. Note: we wish to return this error prior to other errors if the max number of parameters is exceeded
        bool check_count_of_numbers_below_max(int count_numbers, const char* input_configuration_file); 

        // Check 3. Declare check_numeric_characters helper function: checks that all characters are numeric
        bool check_numeric_characters(char* number_array, int count_numbers, const char* input_configuration_file); 

        // Check 4. Declare check_valid_index helper function: checks that all numbers are a valid index (between 0-25)
        bool check_valid_index(int number, int count_numbers, const char* input_configuration_file); 

        // Check 5. Declare check_number_not_mapped_to_multiple helper function: checks number is not being mapped to multiple numbers
        bool check_number_not_mapped_to_multiple(map<int, int>::iterator iterator_to_number, int number, int count_numbers, const char* input_configuration_file); 

        // Check 6. Declare check_number_not_mapped_to_itself helper function: checks number is not being mapped to itself
        bool check_number_not_mapped_to_itself(int first_number, int second_number, int count_numbers, const char* input_configuration_file);

        // Check 7. Declare check_count_of_numbers_even helper function: checks the count of numbers is even
        bool check_count_of_numbers_even(int count_numbers, const char* input_configuration_file); 
        
        
        // III. HELPER FUNCTIONS FOR MAPPING CONFIGURATION

        // Configure map step 1. Declare insert_pair_into_mapping helped function: inserts pair into plugboard mapping
        void insert_pair_into_mapping(int& first_number, int& second_number, map <int, int>& mapping);

        // Configure map step 2. Declare insert_remaining_numbers_into_mapping helper function: insert numbers excluded from file (numbers that simply map to themselves) into mapping
        void insert_remaining_numbers_into_mapping(map <int, int>& mapping);

    
    public: 
        // IV. FUNCTIONS
        
        // Function 1. Declare p_configure function: checks plugboard configuration file for errors, and uses it to configure plugboard map and set plugboard configuration file data member
        int p_configure(const char* input_configuration_file); 

        // Function 2. Declare p_encrypt function: encrypts a number that passes through the plugboard
        int p_encrypt(int input_number); 
}; 

#endif