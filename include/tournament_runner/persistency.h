#ifndef TOURNAMENT_RUNNER_PERSISTENCY_H_
#define TOURNAMENT_RUNNER_PERSISTENCY_H_

#include <string>

#include "tournament_runner/persistency_interface.h"

namespace TournamentRunner
{

class Tournament;

/**
 * @brief The Persistency class
 * @details Handles loading and saving tournament data (profiles).
 * A tournament is serialized and saved to a file.
 */
class Persistency :
    public IPersistency
{
public:
    /**
     * @brief Default Constructor
    */
    Persistency () = default;

    /**
     * @brief Constructor with default path
     * @param default_path The default path to profile files.
     */
    explicit Persistency (std::string default_path);

    /**
     * @copydoc IPersistency::save_profile(const Tournament&, std::string)
     * @exception std::ios_base::failure Thrown if the profile can not be saved.
    */
    void save_profile (const Tournament& tournament_to_save,
                       const std::string& to_file) const final;
    /**
     * @copydoc IPersistency::save_profile(const Tournament&)
     * @exception std::ios_base::failure Thrown if the profile can not be saved.
     */
    void save_profile (const Tournament& tournament_to_save) const final;

    /**
     * @copydoc IPersistency::load_profile(std::string)
     * @exception std::ios_base::failure Thrown if the profile can not be loaded,
     * e.g. the file doesn't exist
     */
    Tournament load_profile (const std::string& from_file) final;

    /**
     * @copydoc IPersistency::load_profile()
     * @exception std::ios_base::failure Thrown if the default profile can not be loaded,
     * e.g. the file doesn't exist or the default file ist not set. \see Persistency(std::string)
     */
    Tournament load_profile () final;

private:
    std::string current_profile_file_{};

};

} //namespace TournamentRunner


#endif // TOURNAMENT_RUNNER_PERSISTENCY_H_
