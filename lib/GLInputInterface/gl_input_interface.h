#pragma once
#include "input_interface.h"
#include <stdint.h>


extern bool glPressValue;

class GLInputInterface: public InputInterface {
    public:
        GLInputInterface();
        virtual bool getValue();
};