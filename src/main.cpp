/*
 * main.cpp
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 01/17/2013
 */

#include "application.h"

int main(int argc, const char **argv)
{
    julia::Application application(argc, argv);
    application.run();
    
    return 0;
}
