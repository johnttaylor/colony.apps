#include "Storm/Thermostat/Main/Main.h"
#include "Cpl/System/Api.h"
#include "Cpl/Io/Stdio/StdIn.h"
#include "Cpl/Io/Stdio/StdOut.h"


Cpl::Io::Stdio::StdIn   infd_;
Cpl::Io::Stdio::StdOut  outfd_;


int main( int argc, char* const argv[] )
{
    // Initialize Colony
    Cpl::System::Api::initialize();

    // Run the application
    return runTheApplication( infd_, outfd_ );
}
