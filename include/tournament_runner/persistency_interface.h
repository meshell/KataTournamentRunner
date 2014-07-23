#ifndef TOURNAMENT_RUNNER_PERSISTENCY_INTERFACE_H_
#define TOURNAMENT_RUNNER_PERSISTENCY_INTERFACE_H_

#include <string>

namespace TournamentRunner
{

class Tournament;

/**
 * @brief The Persistency Interface
 */
class IPersistency
{
public:

    virtual ~IPersistency() {};

    /**
     * @brief Save a profile
     * @param The tournament to save
     * @param Path to the file it should be saved to
     */
    virtual void save_profile (const Tournament& tournament_to_save,
                               const std::string& to_file) = 0;

    /**
     * @brief Save the profile to the file currently loaded.
     * @param The tournament to save
     */
    virtual void save_profile (const Tournament& tournament_to_save) = 0;

    /**
     * @brief Load a profile
     * @param Path to the file it should be saved to
     * @return The tournament loaded
     */
    virtual Tournament load_profile (const std::string& from_file) = 0;

    /**
     * @brief Load the default profile
     * @return The tournament loaded
     */
    virtual Tournament load_profile () = 0;

};

} //namespace TournamentRunner


#endif // TOURNAMENT_RUNNER_PERSISTENCY_INTERFACE_H_
