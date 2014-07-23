#include "tournament_runner/os_helper.h"

namespace TournamentRunner
{
namespace OS
{

std::string get_home_folder ()
{
    auto home_dir = std::string{};

    char* appdata_folder{};
    auto buffer_len = 0U;
    errno_t error = _dupenv_s(&appdata_folder, &buffer_len, "LOCALAPPDATA");
    if (error)
    {
        error = _dupenv_s(&appdata_folder, &buffer_len, "APPDATA");
    }
    if (!error)
    {
        home_dir = appdata_folder;
    }
    free(appdata_folder);
    return home_dir;
}

} //namespace OS
} //namespace TournamentRunner
