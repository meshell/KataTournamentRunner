#ifndef TOURNAMENT_RUNNER_OS_HELPER_H_
#define TOURNAMENT_RUNNER_OS_HELPER_H_

#include <string>

namespace TournamentRunner
{
namespace OS
{

/**
 * @brief Return homefolder as defined in the environment variable
 * @return the homefolder path as string
 */
std::string get_home_folder ();

} //namespace OS
} //namespace TournamentRunner

#endif //TOURNAMENT_RUNNER_OS_HELPER_H_
