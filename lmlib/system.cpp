#include "system.h"
#include "string.h"

#include <Windows.h>
#include <chrono>

#include <shellapi.h>

void lm::system::spawnProcess(const std::string &path, const std::string &wdir,
                              const bool elevated, const std::string args) {

  SHELLEXECUTEINFO ShExecInfo;

  TCHAR *path_buf = lm::string::c_str(path);

  TCHAR *wdir_buf;
  if (wdir == "")
    wdir_buf = NULL;
  else {
    wdir_buf = lm::string::c_str(wdir);
  }

  LPCSTR params;
  if (args == "")
    params = NULL;
  else {
    params = lm::string::c_str(args);
  }

  ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
  ShExecInfo.fMask = NULL;
  ShExecInfo.hwnd = NULL;
  if (elevated == false)
    ShExecInfo.lpVerb = NULL;
  else
    ShExecInfo.lpVerb = "runas";
  ShExecInfo.lpFile = path_buf;
  ShExecInfo.lpParameters = params;
  ShExecInfo.lpDirectory = wdir_buf;
  ShExecInfo.nShow = SW_MAXIMIZE;
  ShExecInfo.hInstApp = NULL;

  ShellExecuteEx(&ShExecInfo);
}

void lm::system::tmTimeNow(std::tm &_tm) {
  time_t time_now =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  localtime_s(&_tm, &time_now);
}

std::string lm::system::getFolderFromPath(const std::string &path) {
  size_t pos = path.find_last_of("\\/");
  return (std::string::npos == pos) ? "" : path.substr(0, pos);
}

std::string lm::system::GetProgramPath() {

  std::string ret;
  char buf[MAX_PATH];
  GetModuleFileName(nullptr, buf, MAX_PATH);
  ret = std::string(buf);

  return ret;
}
