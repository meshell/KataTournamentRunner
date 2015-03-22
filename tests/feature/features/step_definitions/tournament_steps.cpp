#include <string>
#include <stdexcept>
#include <cstdint>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cucumber-cpp/defs.hpp>

#include "tournament_runner/tournament.h"
#include "feature_test_contexts.h"

using cucumber::ScenarioScope;
using ::TournamentRunner::TournamentData;
using ::TournamentRunner::Tournament;
using ::TournamentRunner::Date;
using ::TournamentRunner::Karateka;

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::FloatNear;

namespace FeatureTests
{

const auto float_compare_tolerance = 0.01F;


static TournamentRunner::Karateka create_default_participant ()
{
    return TournamentRunner::Karateka{}
                                 .with_name("Norris")
                                 .with_surname("Chuck")
                                 .with_birthdate("1940-03-10")
                                 .from_dojo("Chun Kuk Do")
                                 .with_grade("3nd Dan");
}

GIVEN("^no tournament is open$")
{
    ScenarioScope<TournamentContext> context;
    context->tournament = nullptr;
}

GIVEN("^a Tournament is open$")
{
    ScenarioScope<TournamentContext> context;
    context->tournament = std::make_shared<Tournament>(
                              TournamentData{}
                              .with_name("SM")
                              .on_date(Date{"2014-07-26"})
                              );
}

GIVEN("^the following Tournament is open:$")
{
    TABLE_PARAM(tournament_param);
    ScenarioScope<TournamentContext> context;

    const auto& tournament_table = tournament_param.hashes();

    ASSERT_THAT(tournament_table.size(), Eq(1U));
    for (const auto& table_row : tournament_table)
    {
        auto tournament = TournamentData{}
                                    .with_name(table_row.at("name"))
                                    .on_date(table_row.at("date"));

        try
        {
            tournament.at_location(table_row.at("location"));
        }
        catch (const std::out_of_range& )
        {
            // it is ok if not all elements are listed in the feature file table
        }
        context->tournament = std::make_shared<TournamentRunner::Tournament>(tournament);
    }
}

GIVEN("^the Tournament has the following participants:$")
{
    ScenarioScope<TournamentContext> context;
    TABLE_PARAM(person_param);
    const auto& persons_table = person_param.hashes();
    for (const auto& table_row : persons_table)
    {

        auto attendee = Karateka{}
                        .with_name(table_row.at("name"))
                        .with_surname(table_row.at("surname"));


        try
        {
            attendee.with_birthdate(table_row.at("date of birth"));
            attendee.from_dojo(table_row.at("dojo"));
            attendee.with_grade(table_row.at("belt"));
        }
        catch (const std::exception&)
        {
            // do nothing we expected that not all columns are available
        }

        try
        {
            const auto score1 = std::stof(table_row.at("score 1-1"));
            attendee.add_kata_score(score1);
            const auto score2 = std::stof(table_row.at("score 1-2"));
            attendee.add_kata_score(score2);
            const auto score3 = std::stof(table_row.at("score 1-3"));
            attendee.add_kata_score(score3);
            const auto score4 = std::stof(table_row.at("score 1-4"));
            attendee.add_kata_score(score4);
            const auto score5 = std::stof(table_row.at("score 1-5"));
            attendee.add_kata_score(score5);

            attendee.next_round();

            const auto score21 = std::stof(table_row.at("score 2-1"));
            attendee.add_kata_score(score21);
            const auto score22 = std::stof(table_row.at("score 2-2"));
            attendee.add_kata_score(score22);
            const auto score23 = std::stof(table_row.at("score 2-3"));
            attendee.add_kata_score(score23);
            const auto score24 = std::stof(table_row.at("score 2-4"));
            attendee.add_kata_score(score24);
            const auto score25 = std::stof(table_row.at("score 2-5"));
            attendee.add_kata_score(score25);
        }
        catch (const std::exception&)
        {
            // do nothing we expected that not all columns are available
        }

        context->tournament->add_participant(attendee);
    }
}

GIVEN("^a participant has performed a kata$")
{
    ScenarioScope<TournamentContext> context;
    context->tournament->add_participant(create_default_participant());
}

GIVEN("^the following participants have performed a kata:$")
{
    TABLE_PARAM(person_param);
    const auto& persons_table = person_param.hashes();

    ScenarioScope<TournamentContext> context;
    for (const auto& table_row : persons_table)
    {
        auto attendee = TournamentRunner::Karateka{}
                        .with_name(table_row.at("name"))
                        .with_surname(table_row.at("surname"))
                        .with_birthdate(table_row.at("date of birth"))
                        .from_dojo(table_row.at("dojo"))
                        .with_grade(table_row.at("belt"));

        const auto score1 = std::stof(table_row.at("score 1"));
        attendee.add_kata_score(score1);
        const auto score2 = std::stof(table_row.at("score 2"));
        attendee.add_kata_score(score2);
        const auto score3 = std::stof(table_row.at("score 3"));
        attendee.add_kata_score(score3);
        const auto score4 = std::stof(table_row.at("score 4"));
        attendee.add_kata_score(score4);
        const auto score5 = std::stof(table_row.at("score 5"));
        attendee.add_kata_score(score5);
        context->tournament->add_participant(attendee);
    }
}

GIVEN("^the following participants have performed katas:$")
{
    TABLE_PARAM(person_param);
    const auto& persons_table = person_param.hashes();

    ScenarioScope<TournamentContext> context;
    for (const auto& table_row : persons_table)
    {
        auto attendee = TournamentRunner::Karateka{}
                        .with_name(table_row.at("name"))
                        .with_surname(table_row.at("surname"));

        attendee.add_kata_score(std::stof(table_row.at("1-1")));
        attendee.add_kata_score(std::stof(table_row.at("1-2")));
        attendee.add_kata_score(std::stof(table_row.at("1-3")));
        attendee.add_kata_score(std::stof(table_row.at("1-4")));
        attendee.add_kata_score(std::stof(table_row.at("1-5")));

        if(!table_row.at("2-1").empty())
        {
            attendee.next_round();
            attendee.add_kata_score(std::stof(table_row.at("2-1")));
            attendee.add_kata_score(std::stof(table_row.at("2-2")));
            attendee.add_kata_score(std::stof(table_row.at("2-3")));
            attendee.add_kata_score(std::stof(table_row.at("2-4")));
            attendee.add_kata_score(std::stof(table_row.at("2-5")));
        }
        if(!table_row.at("3-1").empty())
        {
            attendee.next_round();
            attendee.add_kata_score(std::stof(table_row.at("3-1")));
            attendee.add_kata_score(std::stof(table_row.at("3-2")));
            attendee.add_kata_score(std::stof(table_row.at("3-3")));
            attendee.add_kata_score(std::stof(table_row.at("3-4")));
            attendee.add_kata_score(std::stof(table_row.at("3-5")));
        }
        context->tournament->add_participant(attendee);
    }
}

GIVEN("^the following participants performed a kata in the 2nd round:$")
{
    TABLE_PARAM(person_param);
    const auto& persons_table = person_param.hashes();

    ScenarioScope<TournamentContext> context;
    context->participant_startlist = context->tournament->get_startlist_for_next_kata_round(persons_table.size());

    context->tournament->start_next_kata_round();

    for (const auto& table_row : persons_table)
    {
        const auto start_number = std::stoul(table_row.at("start No"));

        ASSERT_THAT(context->participant_startlist.front(), Eq(start_number));

        auto& participant_second_round = context->tournament->get_participant(context->participant_startlist.front());

        const auto score1 = std::stof(table_row.at("score 1"));
        participant_second_round.add_kata_score(score1);
        const auto score2 = std::stof(table_row.at("score 2"));
        participant_second_round.add_kata_score(score2);
        const auto score3 = std::stof(table_row.at("score 3"));
        participant_second_round.add_kata_score(score3);
        const auto score4 = std::stof(table_row.at("score 4"));
        participant_second_round.add_kata_score(score4);
        const auto score5 = std::stof(table_row.at("score 5"));
        participant_second_round.add_kata_score(score5);
        context->participant_startlist.erase(std::begin(context->participant_startlist));
    }
}

GIVEN("^the best (\\d+) participants are nominated for the next round$")
{
    ScenarioScope<TournamentContext> context;
    REGEX_PARAM(size_t, number_of_nominated_participants);
    context->number_of_participants_of_next_round = number_of_nominated_participants ;
}

WHEN("^I create the following Tournament:")
{
    TABLE_PARAM(tournament_params);
    ScenarioScope<TournamentContext> context;

    const auto& tournament_data_table = tournament_params.hashes();

    ASSERT_THAT(tournament_data_table.size(), Eq(1U));

    for (const auto& table_row : tournament_data_table)
    {
        auto tournament = TournamentData{}
                                    .with_name(table_row.at("name"))
                                    .on_date(table_row.at("date"));

        try
        {
            tournament.at_location(table_row.at("location"));
        }
        catch (const std::out_of_range& )
        {
            // it is ok if not all elements are listed in the feature file table
        }
        context->tournament = std::make_shared<Tournament>(tournament);
    }
}

WHEN("^I add the following participants to the tournament:$")
{
    ScenarioScope<TournamentContext> context;
    TABLE_PARAM(person_param);
    const auto& persons_table = person_param.hashes();
    for (const auto& table_row  : persons_table)
    {

        auto attendee = Karateka{}
                        .with_name(table_row.at("name"))
                        .with_surname(table_row.at("surname"))
                        .with_birthdate(table_row.at("date of birth"))
                        .from_dojo(table_row.at("dojo"))
                        .with_grade(table_row.at("belt"));

        context->tournament->add_participant(attendee);
    }

}

WHEN("^a referee gives score ([0-9.]+)$")
{
    REGEX_PARAM(float, kata_score);

    ScenarioScope<TournamentContext> context;
    const auto first_participant_index = 0U;
    context->tournament->get_participant(first_participant_index).add_kata_score(kata_score);
}

WHEN("^the overall deduction is ([0-9.]+)$")
{
    REGEX_PARAM(float, deduction);

    ScenarioScope<TournamentContext> context;
    const auto first_participant_index = 0U;
    context->tournament->get_participant(first_participant_index).add_deduction(deduction);
}

WHEN("^the next round starts$")
{
    ScenarioScope<TournamentContext> context;
    context->participant_startlist = context->tournament->get_startlist_for_next_kata_round(context->number_of_participants_of_next_round);
    context->tournament->start_next_kata_round();
}

WHEN("^the ranks for a round are caclulated$")
{
    ScenarioScope<TournamentContext> context;
    context->participant_list = context->tournament->get_ranked_list_of_participants();
}

THEN("^the Tournament should have the following participants:$")
{
    ScenarioScope<TournamentContext> context;
    TABLE_PARAM(person_param);
    const auto& persons_table = person_param.hashes();
    auto participant_index = 0U;
    for (const auto& table_row : persons_table)
    {
        auto attendee = Karateka()
                        .with_name(table_row.at("name"))
                        .with_surname(table_row.at("surname"));
        try
        {
            attendee.with_birthdate(table_row.at("date of birth"));
            attendee.from_dojo(table_row.at("dojo"));
            attendee.with_grade(table_row.at("belt"));
        }
        catch (const std::exception&)
        {
            // do nothing we expected that not all columns are available
        }

        auto tournament_participant = context->tournament->get_participant(participant_index++);

        ASSERT_THAT(attendee.name(), StrEq(tournament_participant.name()));
        ASSERT_THAT(attendee.surname(), StrEq(tournament_participant.surname()));

        try
        {
            const auto score1 = std::stof(table_row.at("score 1-1"));
            const auto score2 = std::stof(table_row.at("score 1-2"));
            const auto score3 = std::stof(table_row.at("score 1-3"));
            const auto score4 = std::stof(table_row.at("score 1-4"));
            const auto score5 = std::stof(table_row.at("score 1-5"));

            std::vector<float> scores_round_1 = tournament_participant.get_scores(0);

            ASSERT_THAT(scores_round_1.back(), FloatNear(score5, float_compare_tolerance));
            scores_round_1.pop_back();
            ASSERT_THAT(scores_round_1.back(), FloatNear(score4, float_compare_tolerance));
            scores_round_1.pop_back();
            ASSERT_THAT(scores_round_1.back(), FloatNear(score3, float_compare_tolerance));
            scores_round_1.pop_back();
            ASSERT_THAT(scores_round_1.back(), FloatNear(score2, float_compare_tolerance));
            scores_round_1.pop_back();
            ASSERT_THAT(scores_round_1.back(), FloatNear(score1, float_compare_tolerance));
            scores_round_1.pop_back();

            const auto score21 = std::stof(table_row.at("score 2-1"));
            const auto score22 = std::stof(table_row.at("score 2-2"));
            const auto score23 = std::stof(table_row.at("score 2-3"));
            const auto score24 = std::stof(table_row.at("score 2-4"));
            const auto score25 = std::stof(table_row.at("score 2-5"));

            auto scores_round_2 = tournament_participant.get_scores(1);

            ASSERT_THAT(scores_round_2.back(), FloatNear(score25, float_compare_tolerance));
            scores_round_2.pop_back();
            ASSERT_THAT(scores_round_2.back(), FloatNear(score24, float_compare_tolerance));
            scores_round_2.pop_back();
            ASSERT_THAT(scores_round_2.back(), FloatNear(score23, float_compare_tolerance));
            scores_round_2.pop_back();
            ASSERT_THAT(scores_round_2.back(), FloatNear(score22, float_compare_tolerance));
            scores_round_2.pop_back();
            ASSERT_THAT(scores_round_2.back(), FloatNear(score21, float_compare_tolerance));
            scores_round_2.pop_back();

        }
        catch (const std::exception&)
        {
            // do nothing because we expected that score columns are not always available
        }
    }
}

THEN("^the sum of points given should be ([0-9.]+)$")
{
    REGEX_PARAM(float, kata_overall_score);

    ScenarioScope<TournamentContext> context;
    const auto first_participant_index = 0U;
    auto participant = context->tournament->get_participant(first_participant_index);
    ASSERT_NEAR(kata_overall_score, participant.get_overall_score_of_round(context->kata_round), float_compare_tolerance);
}

THEN("^the minimum score canceled should be ([0-9.]+)$")
{
    REGEX_PARAM(float, kata_minimum_score);

    ScenarioScope<TournamentContext> context;
    const auto first_participant_index = 0U;
    auto participant = context->tournament->get_participant(first_participant_index);
    ASSERT_NEAR(kata_minimum_score, participant.get_minimum_score_of_round(context->kata_round), float_compare_tolerance);
}

THEN("^the maximum score canceled should be ([0-9.]+)$")
{
    REGEX_PARAM(float, kata_maximum_score);

    ScenarioScope<TournamentContext> context;
    const auto first_participant_index = 0U;
    auto participant = context->tournament->get_participant(first_participant_index);
    ASSERT_NEAR(kata_maximum_score, participant.get_maximum_score_of_round(context->kata_round), float_compare_tolerance);
}

THEN("^the ranked list should be the following:$")
{
    TABLE_PARAM(person_param);
    const auto& persons_table = person_param.hashes();

    ScenarioScope<TournamentContext> context;
    for (const auto& table_row : persons_table)
    {
        const auto name = std::string(table_row.at("name"));
        const auto surname = std::string(table_row.at("surname"));
        const auto sum = std::stof(table_row.at("sum"));
        const auto maximum = std::stof(table_row.at("maximum"));
        const auto minimum = std::stof(table_row.at("minimum"));
        auto participant = context->participant_list.front();
        ASSERT_THAT(participant.name(), StrEq(name));
        ASSERT_THAT(participant.surname(), StrEq(surname));
        ASSERT_THAT(participant.get_overall_score_of_current_round(), FloatNear(sum, float_compare_tolerance));
        ASSERT_THAT(participant.get_maximum_score_of_current_round(), FloatNear(maximum, float_compare_tolerance));
        ASSERT_THAT(participant.get_minimum_score_of_current_round(), FloatNear(minimum, float_compare_tolerance));
        context->participant_list.erase(std::begin(context->participant_list));
    }
}

THEN("^the list of participants for the round is the following:$")
{
    TABLE_PARAM(person_param);
    const auto& persons_table = person_param.hashes();
    ScenarioScope<TournamentContext> context;
    auto position = 1U;
    for (const auto& table_row : persons_table)
    {
        const auto start_position = std::stoul(table_row.at("start position"));
        const auto name = std::string(table_row.at("name"));
        const auto surname = std::string(table_row.at("surname"));
        ASSERT_THAT(position, Eq(start_position));

        const auto start_no = std::stoul(table_row.at("start No"));
        const auto start_number = context->participant_startlist.front();
        context->participant_startlist.erase(std::begin(context->participant_startlist));
        ASSERT_THAT(start_number, Eq(start_no));

        const auto participant = context->tournament->get_participant(start_number);
        ASSERT_THAT(participant.name(), StrEq(name));
        ASSERT_THAT(participant.surname(), StrEq(surname));

        position++;
    }
}

THEN("^the list contains (\\d+) participants$")
{
    REGEX_PARAM(size_t, number_of_starting_participants);
    ScenarioScope<TournamentContext> context;
    ASSERT_THAT(context->participant_startlist.size(), Eq(number_of_starting_participants));
}

THEN("^the following tournament is opened:$")
{
    TABLE_PARAM(tournament_params);
    ScenarioScope<TournamentContext> context;
    ASSERT_TRUE(context->tournament != nullptr);

    const auto& tournament_data_table = tournament_params.hashes();
    auto expected_table_size = 1U;

    ASSERT_EQ(expected_table_size, tournament_data_table.size());

    for (const auto& table_row : tournament_data_table)
    {
        ASSERT_EQ(table_row.at("name"), context->tournament->name());
        ASSERT_EQ(table_row.at("date"), context->tournament->date_as_string());
        auto location = std::string{""};
        try
        {
            location = table_row.at("location");
            ASSERT_EQ(location, context->tournament->location());
        }
        catch (const std::out_of_range& )
        {
            // it is ok if not all elements are listed in the feature file table
        }
    }
}

THEN("^the number of participants for the tournament should be (\\d+)$")
{
    REGEX_PARAM(size_t, expected_number_of_participants);
    ScenarioScope<TournamentContext> context;
    ASSERT_THAT(context->tournament->number_of_participants(), Eq(expected_number_of_participants));

}

THEN("^the rankedlist is the following:$")
{
    ScenarioScope<TournamentContext> context;
    auto rankedlist = context->tournament->get_anotated_ranked_list_of_participants();
    TABLE_PARAM(rankedlist_params);
    const auto& rankedlist_table = rankedlist_params.hashes();

    for (const auto& table_row : rankedlist_table)
    {
        const auto predicate = [&table_row] (const Tournament::RankAnnotatedParticipant& element)
            {
                const auto rank = std::get<0>(element);
                const auto participant = std::get<1>(element);
                return ((std::stoul(table_row.at("rank")) == rank) &&
                        (table_row.at("name") == participant.name()) &&
                        (table_row.at("surname") == participant.surname()));

            };
        const auto found = std::count_if(std::begin(rankedlist),
                                         std::end(rankedlist),
                                         predicate);
        ASSERT_TRUE(found == 1);
    }
}

} // namespace FeatureTests
