#include "Target.h"

#include <fstream>
#include <string>
#include <vector>

#include "lmlib/container.h"
#include "lmlib/string.h"
#include "lmlib/system.h"

bool Target::setup(std::string &t) {
  std::vector<std::string> parts, subparts;
  bool bid = false, bexe = false;
  bool bexecutable = false;

  if (t.size() > 0)
    if (t[0] == '#')
      return false;

  lm::string::smartSplit(t, ',', parts);
  // foreach p=value
  for (auto &s : parts) {

    //{p, value}
    lm::string::splitInTwo(s, '=', subparts);
    // redundant!
    if (subparts.size() == 2) {

      subparts[0] = lm::string::toLower(lm::string::trim(subparts[0]));
      subparts[1] = lm::string::trim(subparts[1]);
      if (subparts[0] == "id") {
        bid = setIdFromString(subparts[1]);

      } else if (subparts[0] == "exe") {
        exe = subparts[1];
        bexe = true;
      } else if (subparts[0] == "event") {
        event = str_to_eventmode_t(subparts[1]);
      } else if (subparts[0] == "admin") {
        admin = lm::string::toBool(subparts[1]);
      } else if (subparts[0] == "wdir") {
        if (subparts[1] == "[usbmonitor]") {
          // get argv[0] and extract folder path
          wdir = lm::system::getFolderFromPath(lm::system::GetProgramPath());
        } else if (subparts[1] == "[executable]") {
          bexecutable = true;
        } else if (subparts[1] == "[default]") {
          wdir = "";
        } else
          wdir = subparts[1];
      }

      else if (subparts[0] == "args") {

        args = subparts[1];
      }
    }

  } // end foreach

  if (bexecutable && bexe) {
    // extract folder path of exe, but exe may not be set yet... this step must
    // be done after the loop
    wdir = lm::system::getFolderFromPath(exe);
  }

  if (bid && bexe)
    return true;
  return false;
}

bool operator==(Target &l, Device &r) {
  return (l.vid == r.getVid() && l.pid == r.getPid());
}

void ObtainTargetList(std::vector<Target> &tlist,
                      const std::vector<Device> &tdevlist,
                      Target::eventmode_t ev) {
  Target elem;
  std::string line;

  std::ifstream inputFile("target.txt");

  if (inputFile.is_open()) {
    while (getline(inputFile, line)) {
      if (elem.setup(line)) {
        if (lm::container::contains(tdevlist, Device(elem)) && ev == elem.event)
          tlist.push_back(elem);
      }
    }
    inputFile.close();
  }
}

void ExecuteTargetList(const std::vector<Target> &tlist) {
  for (auto &t : tlist) {
    lm::system::spawnProcess(t.getExe(), t.wdir, t.admin, t.args);
  }
}

Target::eventmode_t Target::str_to_eventmode_t(std::string &e) {
  if (lm::string::toLower(e) == "unplugged")
    return eventmode_t::UNPLUGGED;
  else
    return eventmode_t::PLUGGED;
}