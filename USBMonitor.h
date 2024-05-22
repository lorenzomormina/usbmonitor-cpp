#pragma once

#include "Target.h"
#include <string>
#include <vector>

class USBMonitor {

public:
  enum comparemode_t {
    ONLY_PLUGGED_QUICK,
    ONLY_PLUGGED_PROPER,
    PLUGGED_UNPLUGGED_QUICK,
    PLUGGED_UNPLUGGED_PROPER
  };
  std::string comparemode_t_to_str(comparemode_t m);

protected:
  bool showConsole = true;
  bool debugAcquisition = false;
  unsigned int sleepTime = 1000;
  bool logEvent = true;
  bool logTarget = false;
  comparemode_t compareMode = ONLY_PLUGGED_QUICK;

  std::string settingsPath;

  void loadSettings();

public:
  USBMonitor();
  [[noreturn]] void run();
  void logEventList(const std::vector<Device> &tlist,
                    Target::eventmode_t event);
  void logTargetList(const std::vector<Target> &tlist);
};
