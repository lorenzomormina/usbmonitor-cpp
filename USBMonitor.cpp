#include "USBMonitor.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Target.h"

#include "lmlib/string.h"
#include "lmlib/system.h"

#include <Windows.h>
#include <fstream>
#include <thread>

#include <iostream>

USBMonitor::USBMonitor() {

  settingsPath = "settings.txt";
  loadSettings();
}

void USBMonitor::loadSettings() {
  std::vector<std::string> parts;
  std::string line;
  std::ifstream inputFile(settingsPath);

  if (inputFile.is_open()) {
    while (getline(inputFile, line)) {

      // line is not empty
      if (line.size() > 0) {
        // line is not a comment
        if (line[0] == '#')
          continue;
        // line is case insensitive
        lm::string::toLower(line);
        parts = lm::string::split(line, '=', parts);
        if (parts.size() == 2) {

          lm::string::trim(parts[0]);
          lm::string::trim(parts[1]);

          if (parts[0] == "showconsole")
            showConsole = lm::string::toBool(parts[1]);
          else if (parts[0] == "debugacquisition")
            debugAcquisition = lm::string::toBool(parts[1]);
          else if (parts[0] == "sleeptime")
            sleepTime = std::stoi(parts[1]);
          else if (parts[0] == "logevent")
            logEvent = lm::string::toBool(parts[1]);
          else if (parts[0] == "logtarget")
            logTarget = lm::string::toBool(parts[1]);
          else if (parts[0] == "comparemode") {
            if (parts[1] == "onlypluggedquick")
              compareMode = ONLY_PLUGGED_QUICK;
            else if (parts[1] == "onlypluggedproper")
              compareMode = ONLY_PLUGGED_PROPER;
            else if (parts[1] == "pluggedunpluggedquick")
              compareMode = PLUGGED_UNPLUGGED_QUICK;
            else if (parts[1] == "pluggedunpluggedproper")
              compareMode = PLUGGED_UNPLUGGED_PROPER;
          }
        }
      }
    }
    inputFile.close();
  }

  std::cout << "Settings will be:" << std::endl
            << "ShowConsole = " << (showConsole ? "True" : "False") << std::endl
            << "DebugAcquisition = " << (debugAcquisition ? "True" : "False")
            << std::endl
            << "SleepTime = " << sleepTime << "ms" << std::endl
            << "LogEvent = " << (logEvent ? "True" : "False") << std::endl
            << "LogTarget = " << (logTarget ? "True" : "False") << std::endl
            << "CompareMode = " << comparemode_t_to_str(compareMode)
            << std::endl
            << std::endl;
}

void USBMonitor::run() {
  std::vector<Device> prevDevs, currDevs, targDevsP, targDevsU;
  std::vector<Target> targetList;

  HWND cmd = GetConsoleWindow();
  if (showConsole == 0) {
    ShowWindow(cmd, SW_HIDE);
  } else {
    ShowWindow(cmd, SW_SHOW);
  }

  while (true) {

    // obtain list of plugged usb devices
    if (debugAcquisition == 0) {
      currDevs = ObtainDeviceList();
    } else
      currDevs = ObtainDeviceListDebug();

    // based on setting, compare previous and current list to get the list of
    // usb plugged and eventually usb unplugged.
    switch (compareMode) {

    case ONLY_PLUGGED_PROPER:
      DeviceListCompare(prevDevs, currDevs, targDevsP);
      break;

    case PLUGGED_UNPLUGGED_PROPER:
      DeviceListCompare(prevDevs, currDevs, targDevsP, targDevsU);
      break;

    case ONLY_PLUGGED_QUICK:
      if (currDevs.size() > prevDevs.size()) {
        DeviceListCompare(prevDevs, currDevs, targDevsP);
      } else
        targDevsP.clear();
      break;
    case PLUGGED_UNPLUGGED_QUICK:
      if (currDevs.size() > prevDevs.size()) {
        DeviceListCompare(prevDevs, currDevs, targDevsP);
        targDevsU.clear();
      }

      else if (currDevs.size() < prevDevs.size()) {
        DeviceListCompare(currDevs, prevDevs, targDevsU);
        targDevsP.clear();
      } else {
        targDevsU.clear();
        targDevsP.clear();
      }
      break;
    }

    // log event (plugging and unplugging of usb device)
    logEventList(targDevsP, Target::PLUGGED);
    logEventList(targDevsU, Target::UNPLUGGED);

    // build a list of targets
    targetList.clear();
    ObtainTargetList(targetList, targDevsP, Target::PLUGGED);
    ObtainTargetList(targetList, targDevsU, Target::UNPLUGGED);

    // log the attempt at starting a program
    logTargetList(targetList);

    // attempt to start a program
    ExecuteTargetList(targetList);

    prevDevs = currDevs;

    if (sleepTime > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    } else {
      std::cout << "Please, press ENTER." << std::endl;
      std::cin.ignore();
    }
  }
}

void USBMonitor::logEventList(const std::vector<Device> &tlist,
                              Target::eventmode_t event) {
  std::stringstream message;
  std::tm tn;
  std::string evstr;

  // time now, in tm type struct.
  lm::system::tmTimeNow(tn);

  for (auto &t : tlist) {
    if (event == Target::PLUGGED)
      evstr = "PLUGGED";
    else
      evstr = "UNPLUGGED";

    message << "USB " << evstr << " VID&PID: " << t.getIdAsString() << " at "
            << std::setfill('0') << std::setw(2) << tn.tm_mday << "/"
            << std::setfill('0') << std::setw(2) << tn.tm_mon + 1 << "/"
            << 1900 + tn.tm_year << " " << std::setfill('0') << std::setw(2)
            << tn.tm_hour << ":" << std::setfill('0') << std::setw(2)
            << tn.tm_min << ":" << std::setfill('0') << std::setw(2)
            << tn.tm_sec << ".";
    std::cout << message.str() << std::endl;

    if (logEvent) {
      std::ofstream file("log.txt", std::ios::app);
      if (file.is_open()) {
        file << message.str() << std::endl;
        file.close();
      }
    }

    message.str("");
  }
}

void USBMonitor::logTargetList(const std::vector<Target> &tlist) {
  std::stringstream message;

  for (auto &t : tlist) {
    message << "Trying to start program: " << t.getExe()
            << "; with working directory: " << t.wdir
            << "; eleveted as administrator: " << (t.admin ? "yes" : "no")
            << "; arguments: " << t.args << ".";
    std::cout << message.str() << std::endl;

    if (logTarget) {
      std::ofstream file("log.txt", std::ios::app);
      if (file.is_open()) {
        file << message.str() << std::endl;
        file.close();
      }
    }

    message.str("");
  }
}

std::string USBMonitor::comparemode_t_to_str(comparemode_t m) {
  std::string s;

  switch (m) {
  case ONLY_PLUGGED_QUICK:
    s = "OnlyPluggedQuick";
    break;
  case ONLY_PLUGGED_PROPER:
    s = "OnlyPluggedProper";
    break;
  case PLUGGED_UNPLUGGED_QUICK:
    s = "PluggedUnpluggedQUICK";
    break;
  case PLUGGED_UNPLUGGED_PROPER:
    s = "PluggedUnpluggedProper";
    break;
  }

  return s;
}