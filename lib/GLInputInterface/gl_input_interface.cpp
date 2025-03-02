#include "gl_input_interface.h"

bool glPressValue = false;

GLInputInterface::GLInputInterface()
{
    
}

bool GLInputInterface::getValue()
{
    return glPressValue;
}