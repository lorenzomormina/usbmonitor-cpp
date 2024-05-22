#pragma once

#include <string>
#include <vector>

class Device {

protected:
  // A Device is an object with a Vendor Id and a Product Id
  uint16_t vid, pid;

public:
  // Devices can be compared: two Devices are equal if both vid and pid are
  // equal this funcion is defined as a friend and not as a method otherwise
  // DeviceListCompare won't work.
  friend bool operator==(const Device &l, const Device &r);

  // obtain a string which contains vid and pid separated by a character
  // it's hardcoded that vid and pid are four digits in hex base
  std::string getIdAsString(char separator = '&') const;

  // set vid and pid from a string which contains those ids separated by a
  // character
  bool setIdFromString(const std::string &id, char separator = '&');

  // setters and getters
  void getId(uint16_t &v, uint16_t &p) const {
    v = vid;
    p = pid;
  }
  uint16_t getVid() const { return vid; }
  uint16_t getPid() const { return pid; }
  void setId(const uint16_t v, const uint16_t p) {
    vid = v;
    pid = p;
  }
};

// obtain a list of plugged usb devices
std::vector<Device> ObtainDeviceList();

// used for debug purposes, obtain a list of devices from a text file
std::vector<Device> ObtainDeviceListDebug();

// compare older with newer and puts the mathematical set difference in added
void DeviceListCompare(const std::vector<Device> &older,
                       const std::vector<Device> &newer,
                       std::vector<Device> &added);

void DeviceListCompare(const std::vector<Device> &older,
                       const std::vector<Device> &newer,
                       std::vector<Device> &added,
                       std::vector<Device> &removed);