#include <chrono>
#include <iostream>

#include <Deliberation/Core/Log.h>
#include <Deliberation/Deliberation.h>

using namespace deliberation;

int main(int argc, char * argv[])
{
    deliberation::init();

    Log("MAIN", "Hallo {}, {} times", "Welt", 2);

    return 0;
}
