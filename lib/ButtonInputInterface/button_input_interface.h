#include "input_interface.h"
#include <stdint.h>

#ifndef SIMULATOR

class ButtonInputInterface: public InputInterface {
    public:
        ButtonInputInterface(uint8_t pin);
        virtual bool getValue();
    
    private:
        uint8_t m_pin;
};

#endif