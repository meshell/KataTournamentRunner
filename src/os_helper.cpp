#include "tournament_runner/os_helper.h"

namespace TournamentRunner
{
namespace OS
{

std::string get_home_folder ()
{
    auto home_dir = std::string{};
    const auto home = getenv("HOME");
    if (home != nullptr)
    {
        home_dir = home;
    }
    return home_dir;
}

} //namespace OS
} //namespace TournamentRunner
