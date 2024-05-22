#pragma once
#include "Device.h"

#include <string>
#include <vector>

class Target : public Device {

public:
  enum eventmode_t { PLUGGED, UNPLUGGED };
  eventmode_t str_to_eventmode_t(std::string &e);

protected:
  // A Target is a Device with some additional properties:
  // exe is a path to a program to be executed
  std::string exe;

public:
  eventmode_t event = PLUGGED;
  bool admin = false;
  std::string wdir = "";
  std::string args;

  // create a Target from a Device
  Target(Device d) { d.getId(vid, pid); }

  // default constructor: does nothing.
  Target() {}

  // create a Target from a formatted string which contains all the necessary
  // properties and values. the string format is: id=0000&0000,
  // exe=C:\path\to\program.exe
  bool setup(std::string &t);

  // Targets and Devices can be compared
  friend bool operator==(Target &l, Device &r);

  // getter
  std::string getExe() const { return exe; }

  void setEvent(eventmode_t ev) { event = ev; }
};

// extract the exe from a file for each Device in tdevlist creating a list of
// Targets
void ObtainTargetList(std::vector<Target> &tlist,
                      const std::vector<Device> &tdevlist,
                      Target::eventmode_t ev);

// execute the program in exe for each Target in tlist
void ExecuteTargetList(const std::vector<Target> &tlist);