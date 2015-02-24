#include "tournament_runner/persistency.h"

#include <fstream>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/filesystem.hpp>

#include "tournament_runner/tournament.h"

namespace bf = boost::filesystem;
namespace ba = boost::archive;

using boost::serialization::make_nvp;

namespace TournamentRunner
{

static const auto serialization_xml_element_name = "Tournament";


Persistency::Persistency (std::string default_path):
    current_profile_file_{std::move(default_path)}
{
}

void Persistency::save_profile (const Tournament& tournament_to_save,
                                const std::string& to_file) const
{
    std::ofstream output_file{};
    output_file.exceptions(std::ifstream::failbit);

    const auto output_file_path = bf::path{to_file};
    const auto parent_directory = bf::path{output_file_path.parent_path()};

    if (!exists(parent_directory))
    {
        bf::create_directories(parent_directory);
    }

    output_file.open(to_file);

    ba::xml_oarchive output_archive{output_file};

    output_archive & make_nvp(serialization_xml_element_name, tournament_to_save);
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

    ba::xml_iarchive input_archive{input_file};

    input_archive & make_nvp(serialization_xml_element_name, tournament);

    return tournament;
}

} //namespace TournamentRunner
