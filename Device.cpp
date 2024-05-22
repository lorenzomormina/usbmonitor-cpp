#include "Device.h"

#include <fstream>
#include <iomanip>
#include <sstream>

#include <Windows.h>
#include <initguid.h>
#include <setupapi.h>
#include <usbiodef.h>

#include "lmlib/container.h"
#include "lmlib/string.h"

bool operator==(const Device &l, const Device &r) {
  return (l.vid == r.getVid() && l.pid == r.getPid());
}

std::string Device::getIdAsString(char separator) const {
  std::stringstream id;
  std::string ret;

  id << // padding with 0; 4 digits; base hex <-- number
      std::setfill('0') << std::setw(4) << std::hex << vid << separator
     << std::setfill('0') << std::setw(4) << std::hex << pid;

  ret = id.str();
  return lm::string::toUpper(ret);
}

bool Device::setIdFromString(const std::string &id, char separator) {
  std::vector<std::string> idparts;
  bool ok1, ok2;

  lm::string::split(id, separator, idparts);
  if (idparts.size() == 2) {

    vid = lm::string::toUShort(idparts[0], &ok1, 16);
    pid = lm::string::toUShort(idparts[1], &ok2, 16);
    if (ok1 && ok2) {
      return true;
    }
  }
  return false;
}

std::vector<Device> ObtainDeviceList() {
  std::vector<Device> devlist;
  std::string id;
  Device elem;

  HDEVINFO deviceInfoSet;
  TCHAR buffer[64];
  DWORD memberIndex;
  const GUID *guidDev =
      static_cast<const GUID *>(&GUID_DEVINTERFACE_USB_DEVICE);

  deviceInfoSet = SetupDiGetClassDevs(guidDev, nullptr, nullptr,
                                      DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

  memberIndex = 0;
  while (true) {
    SP_DEVINFO_DATA deviceInfoData;
    ZeroMemory(&deviceInfoData, sizeof(SP_DEVINFO_DATA));
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    if (SetupDiEnumDeviceInfo(deviceInfoSet, memberIndex, &deviceInfoData) ==
        FALSE) {
      if (GetLastError() == ERROR_NO_MORE_ITEMS) {
        break;
      }
    }
    DWORD nSize = 0;
    SetupDiGetDeviceInstanceId(deviceInfoSet, &deviceInfoData, buffer,
                               sizeof(buffer), &nSize);
    buffer[nSize] = '\0';
    memberIndex++;

    id = std::string(buffer);

    elem.setId(lm::string::toUShort(id.substr(8, 4), nullptr, 16),
               lm::string::toUShort(id.substr(17, 4), nullptr, 16));

    devlist.push_back(elem);
  }

  if (deviceInfoSet) {
    SetupDiDestroyDeviceInfoList(deviceInfoSet);
  }

  return devlist;
}

std::vector<Device> ObtainDeviceListDebug() {
  std::vector<Device> devlist;
  std::vector<std::string> id;
  std::string line;
  Device elem;

  std::ifstream inputFile("debugusb.txt");

  if (inputFile.is_open()) {
    while (getline(inputFile, line)) {
      if (elem.setIdFromString(line, '&'))
        devlist.push_back(elem);
    }
    inputFile.close();
  }

  return devlist;
}

void DeviceListCompare(const std::vector<Device> &older,
                       const std::vector<Device> &newer,
                       std::vector<Device> &added) {
  added.clear();
  for (auto &i : newer) {
    if (!lm::container::contains(older, i))
      added.push_back(i);
  }
}

void DeviceListCompare(const std::vector<Device> &older,
                       const std::vector<Device> &newer,
                       std::vector<Device> &added,
                       std::vector<Device> &removed) {
  added.clear();
  for (auto &i : newer) {
    if (!lm::container::contains(older, i))
      added.push_back(i);
  }

  removed.clear();
  for (auto &i : older) {
    if (!lm::container::contains(newer, i))
      removed.push_back(i);
  }
}