#ifndef FEATURE_TEST_CONTEXTS_H_
#define FEATURE_TEST_CONTEXTS_H_

#include <cstdint>
#include <string>
#include <list>
#include <memory>
#include <cucumber-cpp/defs.hpp>

#include "tournament_runner/tournament.h"
#include "tournament_runner/persistency.h"


using cucumber::ScenarioScope;

namespace FeatureTests
{

struct TournamentContext
{
    std::shared_ptr<TournamentRunner::Tournament> tournament
    {
        std::make_shared<TournamentRunner::Tournament>(
            TournamentRunner::TournamentData()
            .with_name("WM")
            .on_date("2013-07-20")
        )
    };
    uint8_t kata_round {0};
    size_t number_of_participants_of_next_round {0};
    std::vector<TournamentRunner::Karateka> participant_list;
    std::vector<size_t> participant_startlist;
};

struct PersistencyContext
{
    TournamentRunner::Persistency persistency;
    std::string path_to_profile;
};


} // namespace FeatureTests

#endif // FEATURE_TEST_CONTEXTS_H_
