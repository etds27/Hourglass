#include "mock_input_interface.h"

bool MockInputInterface::getValue()
{
    return m_value;
}

void MockInputInterface::setValue(bool value) { 
    m_value = value; 
}
