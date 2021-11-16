// Tiphaine Ramenason  
#ifndef REFLECTOR_H
#define REFLECTOR_H

#include <map>

class Reflector
{
    private:
        // I. DATA MEMBERS

        // Declare configuration file for reflector
        const char* re_configuration_file; 
        
        // Declare reflector mapping
        std::map <int, int> re_mapping;

        // II. HELPER FUNCTIONS FOR ERROR CHECKS

        // Check 1. Declare check_open_file helper function: opens file and check for any errors opening file
        bool check_open_file(const char* input_configuration_file, std::ifstream& in_stream); 

        // Check 2. Declare check_file_not_empty helper function: checks file is not empty (0 is an invalid number of parameters)
        bool check_file_not_empty(const char* input_configuration_file, std::ifstream& in_stream); 

        // Check 3. Declare check_count_of_numbers_below_max helper function: checks number of parameters does not exceed the limit of 26. Note: we wish to return this error prior to other errors if the max number of parameters is exceeded
        bool check_count_of_numbers_below_max(int count_numbers, const char* input_configuration_file); 

        // Check 4. Declare check_numeric_characters helper function: checks that all characters are numeric
        bool check_numeric_characters(char* number_array, int count_numbers, const char* input_configuration_file); 

        // Check 5. Declare check_valid_index helper function: checks that all numbers are a valid index (between 0-25)
        bool check_valid_index(int number, int count_numbers, const char* input_configuration_file); 

        // Check 6. Declare check_number_not_mapped_to_multiple helper function: checks number is not being mapped to multiple numbers
        bool check_number_not_mapped_to_multiple(std::map<int, int>::iterator iterator_to_number, int number, int count_numbers, const char* input_configuration_file); 

        // Check 7. Declare check_number_not_mapped_to_itself helper function: checks number is not being mapped to itself
        bool check_number_not_mapped_to_itself(int first_number, int second_number, int count_numbers, const char* input_configuration_file);

        // Check 8. Declare check_count_of_numbers_correct helper function: checks the count of numbers is correct (26 ie 13 pairs)
        bool check_count_of_numbers_correct(int count_numbers, const char* input_configuration_file); 
        
        
        // III. HELPER FUNCTIONS FOR MAPPING CONFIGURATION

        // Configure map step 1. Declare insert_pair_into_mapping helped function: inserts pair into reflector mapping
        void insert_pair_into_mapping(int& first_number, int& second_number, std::map <int, int>& mapping);
    
    public: 
        // IV. FUNCTIONS
        
        // Function 1. Declare re_configure function: checks reflector configuration file for errors, and uses it to configure reflector map and set reflector configuration file data member
        int re_configure(const char* input_configuration_file); 

        // Function 2. Declare re_encrypt function: encrypts a number that passes through the reflector
        int re_encrypt(int input_number); 
}; 

#endif
