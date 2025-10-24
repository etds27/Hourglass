#pragma once

#include "input_interface.h"
#include "device_state.h"
#include "device_config.h"
#include <cstring>

class MockInputInterface : public InputInterface
{
public:
    bool m_value = false;

    void setValue(bool value);
    bool getValue() override;
};