#include "tournament_runner/os_helper.h"

namespace TournamentRunner
{
namespace OS
{

std::string get_home_folder ()
{
    const auto home = getenv("HOME");
    if (home)
    {
        return std::string{home};
    }
    return {};
}

} //namespace OS
} //namespace TournamentRunner
