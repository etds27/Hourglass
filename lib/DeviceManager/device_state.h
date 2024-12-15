#pragma once

enum class DeviceState
{
  Off,
  AwaitingConnecion,
  AwaitingGameStart,
  ActiveTurn,
  AwaitingTurn,
  Skipped,
  Paused,
  Debug
};