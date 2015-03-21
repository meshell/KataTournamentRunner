#ifndef TOURNAMENT_RUNNER_PERSISTENCY_INTERFACE_H_
#define TOURNAMENT_RUNNER_PERSISTENCY_INTERFACE_H_

#include <string>

namespace TournamentRunner
{

class Tournament;

/**
 * @brief The Persistency Interface
 * @details Used to save and load tournament data to/from a file.
 */
class IPersistency
{
public:

    IPersistency () = default;
    IPersistency (const IPersistency&) = default;
    IPersistency (IPersistency&&) = default;
    IPersistency& operator=(const IPersistency&) = default;
    IPersistency& operator=(IPersistency&&) = default;
    virtual ~IPersistency () {}


    /**
     * @brief Save a profile
     * @param tournament_to_save The tournament to save
     * @param to_file Path to the file it should be saved to
     */
    virtual void save_profile (const Tournament& tournament_to_save,
                               const std::string& to_file) const = 0;

    /**
     * @brief Save the profile to the file currently loaded.
     * @param tournament_to_save The tournament to save
     */
    virtual void save_profile (const Tournament& tournament_to_save) const = 0;

    /**
     * @brief Load a profile
     * @param from_file Path to the file it should be loaded from
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
