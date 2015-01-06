#include "tournament_runner/persistency.h"

#include <fstream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/filesystem.hpp>

#include "tournament_runner/tournament.h"

using boost::filesystem::exists;

namespace TournamentRunner
{

Persistency::Persistency (std::string default_path):
    current_profile_file_{std::move(default_path)}
{
}

void Persistency::save_profile (const Tournament& tournament_to_save,
                                const std::string& to_file) const
{
    std::ofstream output_file{};
    output_file.exceptions(std::ifstream::failbit);

    const auto output_file_path = boost::filesystem::path{to_file};
    const auto parent_directory = boost::filesystem::path{output_file_path.parent_path()};

    if (!exists(parent_directory))
    {
        boost::filesystem::create_directories(parent_directory);
    }

    output_file.open(to_file);

    boost::archive::text_oarchive output_archive{output_file};

    output_archive & tournament_to_save;
}

void Persistency::save_profile (const Tournament& tournament_to_save) const
{
    save_profile(tournament_to_save, current_profile_file_);
}

Tournament Persistency::load_profile (const std::string& from_file)
{
    current_profile_file_ = from_file;

    return load_profile();
}

Tournament Persistency::load_profile ()
{
    std::ifstream input_file{};
    input_file.exceptions(std::ifstream::failbit);
    input_file.open(current_profile_file_);

    auto tournament = Tournament{};

    boost::archive::text_iarchive input_archive{input_file};

    input_archive & tournament;

    return tournament;
}

} //namespace TournamentRunner
