#include "input_interface.h"
#include <stdint.h>

class GLInputInterface: public InputInterface {
    public:
        GLInputInterface();
        virtual bool getValue();
};