#include "tournament_runner/os_helper.h"

#include <cstdlib>

namespace TournamentRunner
{
namespace OS
{

std::string get_home_folder ()
{
    const auto home = std::getenv("HOME");
    if (home)
    {
        return std::string{home};
    }
    return {};
}

} //namespace OS
} //namespace TournamentRunner
