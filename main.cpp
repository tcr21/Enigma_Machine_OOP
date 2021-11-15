// TRamenason
#include "enigma.h"

using namespace std; 

int main(int argc, char** argv)
{

    // Create an enigma machine
    Enigma enigma; 

    // Configure machine 
    int error_returned = enigma.e_configure(argc, argv); 
    if (error_returned) // If error code is not equal to 0 ie. if an error is returned while configuring, exit program with relevant error code
    {
        return (error_returned);
    }

    // Encrypt message
    enigma.e_encrypt_message();

    return 0;
}