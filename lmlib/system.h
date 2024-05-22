#include <string>

namespace lm {
namespace system {

void tmTimeNow(std::tm &_tm);
void spawnProcess(const std::string &path, const std::string &wdir,
                  const bool elevated, const std::string args);
void tmTimeNow(std::tm &_tm);
std::string getFolderFromPath(const std::string &path);
std::string GetProgramPath();
} // namespace system
} // namespace lm