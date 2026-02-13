#include "device_manager.h"
#include "device_context.h"
#include "device_runtime.h"
#include "device_state.h"
#include "executable_task.h"
#include "sleep_processor.h"
#include "display_processor.h"
#include "hg_central_processor.h"
#include "input_processor.h"
#include "runtime_processor.h"
#include "logger.h"

#ifndef SIMULATOR
#include <EEPROM.h>
#include "ble_interface.h"
#include "button_input_interface.h"
#include "device_config.h"
#else
#include "gl_input_interface.h"
#include "gl_ring_interface.h"
#include "simulator_central_interface.h"
#include "simulator_tools.h"
#endif

#include <cstring>
#include <vector>

namespace
{
  const LogString loggerTag = "DeviceManager";
}

DeviceManager::DeviceManager(DeviceContext *context, DeviceRuntime *runtime)
{
  logger.info(loggerTag, ": Initializing Device Manager");
  logger.info(loggerTag, ": Device name: ", runtime->deviceName);

#ifndef SIMULATOR
  m_callbackHandler = new CentralCallbackHandler();
  m_callbackHandler->registerCallbacks(context, runtime);
#endif

  m_context = context;
  m_runtime = runtime;
}

DeviceManager::~DeviceManager() = default;

void DeviceManager::start()
{
  logger.info(loggerTag, ": Starting the device manager");
  /// 1. Central Interface Polling Processor
  /// 2. Sleep Processor (Must run after polling to determine if we should sleep)
  /// 3. Input Processor (This reads the inputs generated from the input monitors and performs actions based on them)
  /// 4. Display Processor (This updates the display based on the current device state and any other relevant data)
  /// 5. Notification (This handles any notifications that need to be sent to the central or other components)
  /// 6. Context and runtime cleanup / metric updates
  logger.info(loggerTag, ": Adding central processor task");
  addTask(m_context->centralProcessor);
  logger.info(loggerTag, ": Adding sleep processor task");
  addTask(m_context->sleepProcessor);
  logger.info(loggerTag, ": Adding input processor task");
  addTask(m_context->inputProcessor);
  logger.info(loggerTag, ": Adding display processor task");
  addTask(m_context->displayProcessor);
  // addTask(m_context->notificationProcessor);
  logger.info(loggerTag, ": Adding runtime processor task");
}

void DeviceManager::addTask(ExecutableTask *task)
{
  tasks.push_back(task);
  task->start(m_context, m_runtime);
  logger.info(loggerTag, ": Task count: ", tasks.size());
}

void DeviceManager::removeTask(size_t index)
{
  logger.info(loggerTag, ": Removing task at index ", index);
  ExecutableTask *task = tasks[index];
  task->cleanup(m_context, m_runtime);
  delete task;
  tasks.erase(tasks.begin() + index);
  logger.info(loggerTag, ": Task count: ", tasks.size());
}

void DeviceManager::update()
{
  /// The main processing loop for the device
  /// This will run all of the registered executable tasks
  /// and update the device state accordingly
  /// This will also remove any completed tasks from the list
  /// The static tasks for the device are performed in the following order:
  /// 1. Central Interface Polling Processor
  /// 2. Sleep Processor (Must run after polling to determine if we should sleep)
  /// 3. Input Monitor (This checks the actual input lines for updates and then publishes them to the context)
  /// 4. Input Processor (This reads the inputs generated from the input monitors and performs actions based on them)
  /// 5. Display Processor (This updates the display based on the current device state and any other relevant data)
  /// 6. Notification Processor (This handles any notifications that need to be sent to the central or other components)
  /// 7. Context and runtime cleanup / metric updates
  size_t i = 0;

  // Run the existing list of known tasks and remove any that are completed
  while (i < tasks.size())
  {
    ExecutableTask *task = tasks[i];
    if (task->update(m_context, m_runtime))
    {
      i++;
    }
    else
    {
      removeTask(i);
    }
  }

  // Add any new tasks that were generated from the existing tasks to the main task list and start them
  while (!m_runtime->pendingTasks.empty())
  {
    ExecutableTask* newTask;
    if (m_runtime->pendingTasks.dequeue(newTask))
    {
      addTask(newTask);
      if (newTask->update(m_context, m_runtime)) {
        logger.info(loggerTag, ": New task added from runtime pending tasks queue");
      }
      else
      {
        logger.info(loggerTag, ": New task from runtime pending tasks queue completed immediately");
        removeTask(tasks.size() - 1);
      }
    }
  }

  // Manually run the runtime processor task's update function to ensure it runs after all other tasks have been updated and cleaned up
  m_context->runtimeProcessor->update(m_context, m_runtime);
}
