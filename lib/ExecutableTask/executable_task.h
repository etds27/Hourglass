#pragma once

struct DeviceRuntime; // Forward declaration to avoid circular dependency
struct DeviceContext; // Forward declaration to avoid circular dependency

struct ExecutableTask {
    virtual ~ExecutableTask() {}

    /// @brief Start the task and complete any preparation needed
    virtual void start(DeviceContext* context, DeviceRuntime* runtime) {};

    /// @brief Update the task's internal state
    /// @return true if the task is complete and should be removed, false otherwise
    virtual bool update(DeviceContext* context, DeviceRuntime* runtime) = 0;

    /// @brief Cleanup any resources used by the task
    virtual void cleanup(DeviceContext* context, DeviceRuntime* runtime) {};
};