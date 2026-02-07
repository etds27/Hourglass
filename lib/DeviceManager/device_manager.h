#pragma once
#include "input_monitor.h"
#include "input_interface.h"
#include "hg_central_interface.h"
#include "hg_display_interface.h"
#include "device_state.h"
#include "constants.h"
#include "hg_display_manager.h"
#include "executable_task.h"
#include "device_context.h"
#include "device_runtime.h"
#include "central_callback_handler.h"

/// @brief Manages the overall state and logic of the Hourglass device.
/// This class is the central coordinator, responsible for initializing hardware,
/// managing BLE communication, processing user input, and controlling the display
/// based on the current game state commanded by the central device.
class DeviceManager
{
public:
  /// @brief Constructs a new DeviceManager.
  /// @param displayManager The manager for all display interfaces.
  DeviceManager(DeviceContext* context, DeviceRuntime* runtime);

  /// @brief Destroys the DeviceManager.
  ~DeviceManager();

  /// @brief Initializes the device, including BLE services and display interfaces.
  /// Sets the initial state to AwaitingConnection.
  void start();

  /// @brief The main update loop for the device.
  /// This should be called repeatedly. It processes game state changes,
  /// handles user input, and updates the display.
  void update();

private:

  /// @brief The name of this Hourglass device.
  char *m_deviceName;

  CentralCallbackHandler* m_callbackHandler;


  DeviceRuntime* m_runtime;
  DeviceContext* m_context;

  std::vector<ExecutableTask*> tasks;

  void addTask(ExecutableTask* task);
  void removeTask(size_t index);
};