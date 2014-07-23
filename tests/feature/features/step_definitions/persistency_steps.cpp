#include <string>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cucumber-cpp/defs.hpp>
#include <boost/filesystem.hpp>

#include "feature_test_contexts.h"
#include "tournament_runner/tournament.h"
#include "tournament_runner/karateka.h"

using cucumber::ScenarioScope;

namespace FeatureTests
{

AFTER()
{
    ScenarioScope<PersistencyContext> persistency_context;
    if (!persistency_context->path_to_profile.empty())
    {
        const auto profile_file = boost::filesystem::path{persistency_context->path_to_profile};
        boost::filesystem::remove(profile_file);
    }
}

WHEN("^I restart the application$")
{
    ScenarioScope<TournamentContext> context;
    context->tournament = nullptr;
}

WHEN("^I save the profile to \"([^\"]*)\"$")
{
    REGEX_PARAM(std::string, path_to_profile_file);

    ScenarioScope<TournamentContext> tournament_context;
    ScenarioScope<PersistencyContext> persistency_context;

    auto tournament = *(tournament_context->tournament);

    persistency_context->path_to_profile = path_to_profile_file;
    persistency_context->persistency.save_profile(tournament, path_to_profile_file);
}


WHEN("^I open the profile \"([^\"]*)\"$")
{
    REGEX_PARAM(std::string, path_to_profile_file);

    ScenarioScope<TournamentContext> tournament_context;
    ScenarioScope<PersistencyContext> persistency_context;

    tournament_context->tournament = std::make_shared<TournamentRunner::Tournament>(persistency_context->persistency.load_profile(path_to_profile_file));
}

} // namespace FeatureTests
