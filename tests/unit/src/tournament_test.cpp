#include <string>
#include <sstream>
#include <list>
#include <cstdint>

#include <gtest/gtest.h>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "tournament_runner/tournament.h"
#include "tournament_runner/karateka.h"

using ::TournamentRunner::Tournament;
using ::TournamentRunner::Karateka;
using ::TournamentRunner::TournamentData;
using ::TournamentRunner::Date;

namespace
{

TEST(A_Tournament, should_have_the_name_and_date_as_provided_when_created)
{
    const std::string tournament_name{"Schweizermeisterschaft"};
    const std::string tournament_date_as_string{"2001-01-01"};

    Tournament testee{TournamentData{}
                      .with_name(tournament_name)
                      .on_date(tournament_date_as_string)
                     };

    EXPECT_STREQ(tournament_name.c_str(), testee.name().c_str());
    ASSERT_STREQ(tournament_date_as_string.c_str(), testee.date_as_string().c_str());
}

TEST(A_Tournament, should_return_true_for_date_equality_if_the_date_is_equal)
{
    const Date tournament_date{"2001-01-01"};

    Tournament testee{TournamentData()
                      .with_name("Schweizermeisterschaft")
                      .on_date(tournament_date)
                     };

    ASSERT_EQ(tournament_date, testee.date());
}

TEST(A_Tournament, should_return_false_for_date_equality_if_the_date_is_not_equal)
{

    Tournament testee{TournamentData()
                      .with_name("Schweizermeisterschaft")
                      .on_date("2001-01-01")
                     };


    ASSERT_NE(Date{"2002-12-12"}, testee.date());
}

TEST(A_Tournament, should_have_0_participants_after_creation)
{
    Tournament testee{};
    const auto expected_no_of_participants = 0U;
    ASSERT_EQ(expected_no_of_participants, testee.number_of_participants());
}

TEST(A_Tournament, should_have_one_participant_more_if_a_participant_is_added)
{
    auto participant = Karateka{}
                       .with_name("Oyama")
                       .with_surname("Mas")
                       .with_birthdate("1923-07-27")
                       .from_dojo("Honbu")
                       .with_rank("10th Dan");
    Tournament testee{};
    testee.add_participant(participant);
    const auto expected_no_of_participants = 1U;
    ASSERT_EQ(expected_no_of_participants, testee.number_of_participants());
}

TEST(A_Tournament, should_give_a_ranked_list_with_the_higher_of_two_scores_first)
{
    auto participant1 = Karateka{}
                        .with_name("Oyama")
                        .with_surname("Mas")
                        .with_birthdate("1923-07-27")
                        .from_dojo("Honbu")
                        .with_rank("10th Dan");

    const auto score_1_1 = 5.0F;
    const auto score_1_2 = 5.1F;
    const auto score_1_3 = 5.2F;
    const auto score_1_4 = 5.3F;
    const auto score_1_5 = 5.4F;
    // total = 5.1 + 5.2 + 5.3 = 15.6
    participant1.add_kata_score(score_1_1);
    participant1.add_kata_score(score_1_2);
    participant1.add_kata_score(score_1_3);
    participant1.add_kata_score(score_1_4);
    participant1.add_kata_score(score_1_5);

    auto participant2 = Karateka{}
                        .with_name("Arneil")
                        .with_surname("Steve")
                        .with_birthdate("1934-08-29")
                        .from_dojo("London")
                        .with_rank("9th Dan");

    const auto score_2_1 = 5.1F;
    const auto score_2_2 = 5.2F;
    const auto score_2_3 = 5.3F;
    const auto score_2_4 = 5.4F;
    const auto score_2_5 = 5.5F;
    // total = 5.2 + 5.3 + 5.4 = 15.9
    participant2.add_kata_score(score_2_1);
    participant2.add_kata_score(score_2_2);
    participant2.add_kata_score(score_2_3);
    participant2.add_kata_score(score_2_4);
    participant2.add_kata_score(score_2_5);

    Tournament testee{};
    testee.add_participant(participant1);
    testee.add_participant(participant2);

    auto ranked_list_first_round = testee.get_ranked_list_of_participants();
    const auto first_place = ranked_list_first_round.at(0);
    const auto second_place = ranked_list_first_round.at(1);

    ASSERT_STREQ(participant2.name().c_str(), first_place.name().c_str());
    ASSERT_STREQ(participant1.name().c_str(), second_place.name().c_str());

}

TEST(A_Tournament, should_give_a_ranked_list_with_the_highest_score_first_and_lowest_score_last_for_three_elements)
{
    auto participant_first_place = Karateka{}
                                   .with_name("Arneil")
                                   .with_surname("Steve")
                                   .with_birthdate("1934-08-29")
                                   .from_dojo("London")
                                   .with_rank("9th Dan");

    const auto score_1_1 = 5.1F;
    const auto score_1_2 = 5.2F;
    const auto score_1_3 = 5.3F;
    const auto score_1_4 = 5.4F;
    const auto score_1_5 = 5.5F;
    // total = 5.2 + 5.3 + 5.4 = 15.9
    participant_first_place.add_kata_score(score_1_1);
    participant_first_place.add_kata_score(score_1_2);
    participant_first_place.add_kata_score(score_1_3);
    participant_first_place.add_kata_score(score_1_4);
    participant_first_place.add_kata_score(score_1_5);

    auto participant_second_place = Karateka{}
                                    .with_name("Lundgren")
                                    .with_surname("Dolph")
                                    .with_birthdate("1957-11-03")
                                    .from_dojo("Stockholm ")
                                    .with_rank("3 Dan");

    const auto score_2_1 = 5.1F;
    const auto score_2_2 = 5.2F;
    const auto score_2_3 = 5.3F;
    const auto score_2_4 = 5.3F;
    const auto score_2_5 = 5.5F;
    // total = 5.2 + 5.3 + 5.3 = 15.8
    participant_second_place.add_kata_score(score_2_2);
    participant_second_place.add_kata_score(score_2_1);
    participant_second_place.add_kata_score(score_2_3);
    participant_second_place.add_kata_score(score_2_4);
    participant_second_place.add_kata_score(score_2_5);


    auto participant_third_place = Karateka()
                                   .with_name("Oyama")
                                   .with_surname("Mas")
                                   .with_birthdate("1923-07-27")
                                   .from_dojo("Honbu")
                                   .with_rank("10th Dan");

    const auto score_3_1 = 5.0F;
    const auto score_3_2 = 5.1F;
    const auto score_3_3 = 5.2F;
    const auto score_3_4 = 5.3F;
    const auto score_3_5 = 5.4F;
    // total = 5.1 + 5.2 + 5.3 = 15.6
    participant_third_place.add_kata_score(score_3_1);
    participant_third_place.add_kata_score(score_3_2);
    participant_third_place.add_kata_score(score_3_3);
    participant_third_place.add_kata_score(score_3_4);
    participant_third_place.add_kata_score(score_3_5);


    Tournament testee{};
    testee.add_participant(participant_second_place);
    testee.add_participant(participant_first_place);
    testee.add_participant(participant_third_place);

    auto ranked_list_first_round = testee.get_ranked_list_of_participants();
    const auto first_place = ranked_list_first_round.at(0);
    const auto second_place = ranked_list_first_round.at(1);
    const auto third_place = ranked_list_first_round.at(2);

    ASSERT_STREQ(participant_first_place.name().c_str(), first_place.name().c_str());
    ASSERT_STREQ(participant_second_place.name().c_str(), second_place.name().c_str());
    ASSERT_STREQ(participant_third_place.name().c_str(), third_place.name().c_str());
}

TEST(A_Tournament, should_give_a_ranked_list_with_the_highest_minimal_score_first_if_the_overall_scores_are_the_same_but_the_minimal_scores_are_different)
{
    auto participant1 = Karateka{}
                        .with_name("Oyama")
                        .with_surname("Mas")
                        .with_birthdate("1923-07-27")
                        .from_dojo("Honbu")
                        .with_rank("10th Dan");

    const auto score_1_1 = 5.0F;
    const auto score_1_2 = 5.1F;
    const auto score_1_3 = 5.2F;
    const auto score_1_4 = 5.3F;
    const auto score_1_5 = 5.4F;
    // total = 5.1 + 5.2 + 5.3 = 15.6
    participant1.add_kata_score(score_1_1);
    participant1.add_kata_score(score_1_2);
    participant1.add_kata_score(score_1_3);
    participant1.add_kata_score(score_1_4);
    participant1.add_kata_score(score_1_5);

    auto participant2 = Karateka{}
                        .with_name("Arneil")
                        .with_surname("Steve")
                        .with_birthdate("1934-08-29")
                        .from_dojo("London")
                        .with_rank("9th Dan");

    const auto score_2_1 = 5.1F;
    const auto score_2_2 = 5.1F;
    const auto score_2_3 = 5.2F;
    const auto score_2_4 = 5.3F;
    const auto score_2_5 = 5.5F;
    // total = 5.1 + 5.2 + 5.3 = 15.6
    participant2.add_kata_score(score_2_1);
    participant2.add_kata_score(score_2_2);
    participant2.add_kata_score(score_2_3);
    participant2.add_kata_score(score_2_4);
    participant2.add_kata_score(score_2_5);

    Tournament testee;
    testee.add_participant(participant2);
    testee.add_participant(participant1);

    auto ranked_list_first_round = testee.get_ranked_list_of_participants();
    const auto first_place = ranked_list_first_round.at(0);
    const auto second_place = ranked_list_first_round.at(1);

    ASSERT_STREQ(participant2.name().c_str(), first_place.name().c_str());
    ASSERT_STREQ(participant1.name().c_str(), second_place.name().c_str());
}

TEST(A_Tournament, should_give_a_ranked_list_with_the_highest_maximal_score_first_if_the_overall_and_minimal_scores_are_the_same_but_the_maximal_scores_are_different)
{
    auto participant1 = Karateka{}
                        .with_name("Oyama")
                        .with_surname("Mas")
                        .with_birthdate("1923-07-27")
                        .from_dojo("Honbu")
                        .with_rank("10th Dan");

    const auto score_1_1 = 5.0F;
    const auto score_1_2 = 5.1F;
    const auto score_1_3 = 5.2F;
    const auto score_1_4 = 5.3F;
    const auto score_1_5 = 5.4F;
    // total = 5.1 + 5.2 + 5.3 = 15.6
    participant1.add_kata_score(score_1_1);
    participant1.add_kata_score(score_1_2);
    participant1.add_kata_score(score_1_3);
    participant1.add_kata_score(score_1_4);
    participant1.add_kata_score(score_1_5);

    auto participant2 = Karateka{}
                        .with_name("Arneil")
                        .with_surname("Steve")
                        .with_birthdate("1934-08-29")
                        .from_dojo("London")
                        .with_rank("9th Dan");

    const auto score_2_1 = 5.0F;
    const auto score_2_2 = 5.1F;
    const auto score_2_3 = 5.2F;
    const auto score_2_4 = 5.3F;
    const auto score_2_5 = 5.5F;
    // total = 5.1 + 5.2 + 5.3 = 15.6
    participant2.add_kata_score(score_2_1);
    participant2.add_kata_score(score_2_2);
    participant2.add_kata_score(score_2_3);
    participant2.add_kata_score(score_2_4);
    participant2.add_kata_score(score_2_5);

    Tournament testee;
    testee.add_participant(participant2);
    testee.add_participant(participant1);

    auto ranked_list_first_round = testee.get_ranked_list_of_participants();
    const auto first_place = ranked_list_first_round.at(0);
    const auto second_place = ranked_list_first_round.at(1);

    ASSERT_STREQ(participant2.name().c_str(), first_place.name().c_str());
    ASSERT_STREQ(participant1.name().c_str(), second_place.name().c_str());
}

TEST(A_Tournament, should_start_with_the_first_round)
{
    Tournament testee{};
    const auto expected_next_round = 1U;
    ASSERT_EQ(expected_next_round, testee.start_next_kata_round());
}

TEST(A_Tournament, should_start_the_second_round_after_the_first)
{
    Tournament testee{};
    const auto expected_next_round = 2U;
    auto next_round = testee.start_next_kata_round();
    next_round = testee.start_next_kata_round();
    ASSERT_EQ(expected_next_round, next_round);
}

TEST(A_Tournament, should_have_a_list_of_participants_for_next_round_which_contains_all_participants_when_more_than_the_no_participants_are_qualified)
{
    auto participant_first_place = Karateka{}
                                   .with_name("Arneil")
                                   .with_surname("Steve")
                                   .with_birthdate("1934-08-29")
                                   .from_dojo("London")
                                   .with_rank("9th Dan");

    const auto score_1_1 = 5.1F;
    const auto score_1_2 = 5.2F;
    const auto score_1_3 = 5.3F;
    const auto score_1_4 = 5.4F;
    const auto score_1_5 = 5.5F;
    // total = 5.2 + 5.3 + 5.4 = 15.9
    participant_first_place.add_kata_score(score_1_1);
    participant_first_place.add_kata_score(score_1_2);
    participant_first_place.add_kata_score(score_1_3);
    participant_first_place.add_kata_score(score_1_4);
    participant_first_place.add_kata_score(score_1_5);

    auto participant_second_place = Karateka{}
                                    .with_name("Lundgren")
                                    .with_surname("Dolph")
                                    .with_birthdate("1957-11-03")
                                    .from_dojo("Stockholm")
                                    .with_rank("3rd Dan");


    const auto score_2_1 = 5.1F;
    const auto score_2_2 = 5.2F;
    const auto score_2_3 = 5.3F;
    const auto score_2_4 = 5.3F;
    const auto score_2_5 = 5.5F;
    // total = 5.2 + 5.3 + 5.3 = 15.8
    participant_second_place.add_kata_score(score_2_2);
    participant_second_place.add_kata_score(score_2_1);
    participant_second_place.add_kata_score(score_2_3);
    participant_second_place.add_kata_score(score_2_4);
    participant_second_place.add_kata_score(score_2_5);


    Tournament testee{};
    testee.add_participant(participant_second_place);
    testee.add_participant(participant_first_place);

    auto participant_list_for_second_round = testee.get_startlist_for_next_kata_round(std::numeric_limits<size_t>::max());
    const auto no_second_round_participants = testee.number_of_participants();
    ASSERT_EQ(no_second_round_participants, participant_list_for_second_round.size());
    const auto participant_1 = testee.get_participant(participant_list_for_second_round.at(0));
    const auto participant_2 = testee.get_participant(participant_list_for_second_round.at(1));

    const auto first_place_is_in_list = (participant_first_place.name() == participant_1.name()) ||
                                        (participant_first_place.name() == participant_2.name());
    const auto second_place_is_in_list = (participant_second_place.name() == participant_1.name()) ||
                                         (participant_second_place.name() == participant_2.name());
    ASSERT_TRUE(first_place_is_in_list);
    ASSERT_TRUE(second_place_is_in_list);
}

TEST(A_Tournament, should_have_a_list_of_participants_for_next_round_which_contains_the_two_with_highest_scores_if_two_participants_are_qualified)
{
    auto participant_first_place = Karateka{}
                                   .with_name("Arneil")
                                   .with_surname("Steve")
                                   .with_birthdate("1934-08-29")
                                   .from_dojo("London")
                                   .with_rank("9th Dan");

    const auto score_1_1 = 5.1F;
    const auto score_1_2 = 5.2F;
    const auto score_1_3 = 5.3F;
    const auto score_1_4 = 5.4F;
    const auto score_1_5 = 5.5F;
    // total = 5.2 + 5.3 + 5.4 = 15.9
    participant_first_place.add_kata_score(score_1_1);
    participant_first_place.add_kata_score(score_1_2);
    participant_first_place.add_kata_score(score_1_3);
    participant_first_place.add_kata_score(score_1_4);
    participant_first_place.add_kata_score(score_1_5);

    auto participant_second_place = Karateka{}
                                    .with_name("Lundgren")
                                    .with_surname("Dolph")
                                    .with_birthdate("1957-11-03")
                                    .from_dojo("Stockholm")
                                    .with_rank("3rd Dan");


    const auto score_2_1 = 5.1F;
    const auto score_2_2 = 5.2F;
    const auto score_2_3 = 5.3F;
    const auto score_2_4 = 5.3F;
    const auto score_2_5 = 5.5F;
    // total = 5.2 + 5.3 + 5.3 = 15.8
    participant_second_place.add_kata_score(score_2_2);
    participant_second_place.add_kata_score(score_2_1);
    participant_second_place.add_kata_score(score_2_3);
    participant_second_place.add_kata_score(score_2_4);
    participant_second_place.add_kata_score(score_2_5);


    auto participant_third_place = Karateka{}
                                   .with_name("Oyama")
                                   .with_surname("Mas")
                                   .with_birthdate("1923-07-27")
                                   .from_dojo("Honbu")
                                   .with_rank("10th Dan");

    const auto score_3_1 = 5.0F;
    const auto score_3_2 = 5.1F;
    const auto score_3_3 = 5.2F;
    const auto score_3_4 = 5.3F;
    const auto score_3_5 = 5.4F;
    // total = 5.1 + 5.2 + 5.3 = 15.6
    participant_third_place.add_kata_score(score_3_1);
    participant_third_place.add_kata_score(score_3_2);
    participant_third_place.add_kata_score(score_3_3);
    participant_third_place.add_kata_score(score_3_4);
    participant_third_place.add_kata_score(score_3_5);


    Tournament testee{};
    testee.add_participant(participant_second_place);
    testee.add_participant(participant_first_place);
    testee.add_participant(participant_third_place);

    const auto no_second_round_participants = 2U;
    auto participant_list_for_second_round = testee.get_startlist_for_next_kata_round(no_second_round_participants);

    ASSERT_EQ(no_second_round_participants, participant_list_for_second_round.size());
    const auto participant_1 = testee.get_participant(participant_list_for_second_round.at(0));
    const auto participant_2 = testee.get_participant(participant_list_for_second_round.at(1));

    const auto first_place_is_in_list = (participant_first_place.name() == participant_1.name()) ||
                                        (participant_first_place.name() == participant_2.name());
    const auto second_place_is_in_list = (participant_second_place.name() == participant_1.name()) ||
                                         (participant_second_place.name() == participant_2.name());
    ASSERT_TRUE(first_place_is_in_list);
    ASSERT_TRUE(second_place_is_in_list);
}

TEST(A_Tournament, should_have_a_list_of_participants_for_next_round_ordered_with_best_listed_as_last)
{
    auto participant_first_place = Karateka()
                                   .with_name("Arneil")
                                   .with_surname("Steve")
                                   .with_birthdate("1934-08-29")
                                   .from_dojo("London")
                                   .with_rank("9th Dan");

    const auto score_1_1 = 5.1F;
    const auto score_1_2 = 5.2F;
    const auto score_1_3 = 5.3F;
    const auto score_1_4 = 5.4F;
    const auto score_1_5 = 5.5F;
    // total = 5.2 + 5.3 + 5.4 = 15.9
    participant_first_place.add_kata_score(score_1_1);
    participant_first_place.add_kata_score(score_1_2);
    participant_first_place.add_kata_score(score_1_3);
    participant_first_place.add_kata_score(score_1_4);
    participant_first_place.add_kata_score(score_1_5);

    auto participant_second_place = Karateka()
                                    .with_name("Lundgren")
                                    .with_surname("Dolph")
                                    .with_birthdate("1957-11-03")
                                    .from_dojo("Stockholm")
                                    .with_rank("3rd Dan");

    const auto score_2_1 = 5.1F;
    const auto score_2_2 = 5.2F;
    const auto score_2_3 = 5.3F;
    const auto score_2_4 = 5.3F;
    const auto score_2_5 = 5.5F;
    // total = 5.2 + 5.3 + 5.3 = 15.8
    participant_second_place.add_kata_score(score_2_2);
    participant_second_place.add_kata_score(score_2_1);
    participant_second_place.add_kata_score(score_2_3);
    participant_second_place.add_kata_score(score_2_4);
    participant_second_place.add_kata_score(score_2_5);


    auto participant_third_place = Karateka()
                                   .with_name("Oyama")
                                   .with_surname("Mas")
                                   .with_birthdate("1923-07-27")
                                   .from_dojo("Honbu")
                                   .with_rank("10th Dan");

    const auto score_3_1 = 5.0F;
    const auto score_3_2 = 5.1F;
    const auto score_3_3 = 5.2F;
    const auto score_3_4 = 5.3F;
    const auto score_3_5 = 5.4F;
    // total = 5.1 + 5.2 + 5.3 = 15.6
    participant_third_place.add_kata_score(score_3_1);
    participant_third_place.add_kata_score(score_3_2);
    participant_third_place.add_kata_score(score_3_3);
    participant_third_place.add_kata_score(score_3_4);
    participant_third_place.add_kata_score(score_3_5);


    Tournament testee{};
    testee.add_participant(participant_second_place);
    testee.add_participant(participant_first_place);
    testee.add_participant(participant_third_place);

    const auto no_second_round_participants = 2U;
    auto participant_list_for_second_round = testee.get_startlist_for_next_kata_round(no_second_round_participants);

    const auto second_place = testee.get_participant(participant_list_for_second_round.at(0));
    const auto first_place = testee.get_participant(participant_list_for_second_round.at(1));

    ASSERT_STREQ(participant_first_place.name().c_str(), first_place.name().c_str());
    ASSERT_STREQ(participant_second_place.name().c_str(), second_place.name().c_str());
}

TEST(A_Tournament, should_have_a_list_of_participants_for_the_next_round_containing_three_if_two_are_second_and_two_qualified)
{
    auto participant_first_place = Karateka{}
                                   .with_name("Arneil")
                                   .with_surname("Steve")
                                   .with_birthdate("1934-08-29")
                                   .from_dojo("London")
                                   .with_rank("9th Dan");

    const auto score_1_1 = 5.1F;
    const auto score_1_2 = 5.2F;
    const auto score_1_3 = 5.3F;
    const auto score_1_4 = 5.4F;
    const auto score_1_5 = 5.5F;
    // total = 5.2 + 5.3 + 5.4 = 15.9
    participant_first_place.add_kata_score(score_1_1);
    participant_first_place.add_kata_score(score_1_2);
    participant_first_place.add_kata_score(score_1_3);
    participant_first_place.add_kata_score(score_1_4);
    participant_first_place.add_kata_score(score_1_5);

    auto participant_second_place = Karateka{}
                                    .with_name("Lundgren")
                                    .with_surname("Dolph")
                                    .with_birthdate("1957-11-03")
                                    .from_dojo("Stockholm")
                                    .with_rank("3rd Dan");

    const auto score_2_1 = 5.1F;
    const auto score_2_2 = 5.2F;
    const auto score_2_3 = 5.3F;
    const auto score_2_4 = 5.3F;
    const auto score_2_5 = 5.5F;
    // total = 5.2 + 5.3 + 5.3 = 15.8
    participant_second_place.add_kata_score(score_2_2);
    participant_second_place.add_kata_score(score_2_1);
    participant_second_place.add_kata_score(score_2_3);
    participant_second_place.add_kata_score(score_2_4);
    participant_second_place.add_kata_score(score_2_5);


    auto participant_second_place_2 = Karateka{}
                                      .with_name("Oyama")
                                      .with_surname("Mas")
                                      .with_birthdate("1923-07-27")
                                      .from_dojo("Honbu")
                                      .with_rank("10th Dan");
    // total = 5.2 + 5.3 + 5.3 = 15.8
    participant_second_place_2.add_kata_score(score_2_1);
    participant_second_place_2.add_kata_score(score_2_3);
    participant_second_place_2.add_kata_score(score_2_2);
    participant_second_place_2.add_kata_score(score_2_5);
    participant_second_place_2.add_kata_score(score_2_4);

    Tournament testee;
    testee.add_participant(participant_first_place);
    testee.add_participant(participant_second_place);
    testee.add_participant(participant_second_place_2);

    const auto no_second_round_participants = 2U;
    const auto participant_list_for_second_round = testee.get_startlist_for_next_kata_round(no_second_round_participants);
    const size_t expected_startlist_size = 3U;

    ASSERT_EQ(expected_startlist_size, participant_list_for_second_round.size());
}

TEST(A_Tournament, should_have_a_list_of_participants_for_the_next_round_containing_four_if_three_are_second_and_two_qualified)
{
    auto participant_first_place = Karateka{}
                                   .with_name("Arneil")
                                   .with_surname("Steve")
                                   .with_birthdate("1934-08-29")
                                   .from_dojo("London")
                                   .with_rank("9th Dan");

    const auto score_1_1 = 5.1F;
    const auto score_1_2 = 5.2F;
    const auto score_1_3 = 5.3F;
    const auto score_1_4 = 5.4F;
    const auto score_1_5 = 5.5F;
    // total = 5.2 + 5.3 + 5.4 = 15.9
    participant_first_place.add_kata_score(score_1_1);
    participant_first_place.add_kata_score(score_1_2);
    participant_first_place.add_kata_score(score_1_3);
    participant_first_place.add_kata_score(score_1_4);
    participant_first_place.add_kata_score(score_1_5);

    auto participant_second_place = Karateka{}
                                    .with_name("Lundgren")
                                    .with_surname("Dolph")
                                    .with_birthdate("1957-11-03")
                                    .from_dojo("Stockholm")
                                    .with_rank("3rd Dan");

    const auto score_2_1 = 5.1F;
    const auto score_2_2 = 5.2F;
    const auto score_2_3 = 5.3F;
    const auto score_2_4 = 5.3F;
    const auto score_2_5 = 5.5F;
    // total = 5.2 + 5.3 + 5.3 = 15.8
    participant_second_place.add_kata_score(score_2_2);
    participant_second_place.add_kata_score(score_2_1);
    participant_second_place.add_kata_score(score_2_3);
    participant_second_place.add_kata_score(score_2_4);
    participant_second_place.add_kata_score(score_2_5);


    auto participant_second_place_2 = Karateka()
                                      .with_name("Oyama")
                                      .with_surname("Mas")
                                      .with_birthdate("1923-07-27")
                                      .from_dojo("Honbu")
                                      .with_rank("10th Dan");

    // total = 5.2 + 5.3 + 5.3 = 15.8
    participant_second_place_2.add_kata_score(score_2_1);
    participant_second_place_2.add_kata_score(score_2_3);
    participant_second_place_2.add_kata_score(score_2_2);
    participant_second_place_2.add_kata_score(score_2_5);
    participant_second_place_2.add_kata_score(score_2_4);

    auto participant_second_place_3 = Karateka()
                                      .with_name("Lowe")
                                      .with_surname("Bobby")
                                      .with_birthdate("1933-08-30")
                                      .from_dojo("Hawaii")
                                      .with_rank("9th Dan");

    // total = 5.2 + 5.3 + 5.3 = 15.8
    participant_second_place_3.add_kata_score(score_2_4);
    participant_second_place_3.add_kata_score(score_2_1);
    participant_second_place_3.add_kata_score(score_2_3);
    participant_second_place_3.add_kata_score(score_2_2);
    participant_second_place_3.add_kata_score(score_2_5);



    Tournament testee{};
    testee.add_participant(participant_second_place_3);
    testee.add_participant(participant_second_place);
    testee.add_participant(participant_first_place);
    testee.add_participant(participant_second_place_2);

    const auto no_second_round_participants = 2U;
    const auto participant_list_for_second_round = testee.get_startlist_for_next_kata_round(no_second_round_participants);
    const auto expected_startlist_size = 4U;

    ASSERT_EQ(expected_startlist_size, participant_list_for_second_round.size());
}

TEST(A_serialized_Tournament, should_persist_the_tournament_data)
{
    std::stringbuf buffer{};
    std::ostream output_stream{&buffer};
    boost::archive::text_oarchive out_archive{output_stream};


    std::string tournament_name{"Schweizermeisterschaft"};
    std::string tournament_date_as_string{"2001-01-01"};
    TournamentRunner::Date tournament_date{tournament_date_as_string};

    std::string tournament_location{"Kriens"};

    Tournament testee{TournamentData{}
                      .with_name(tournament_name)
                      .on_date(tournament_date)
                      .at_location(tournament_location)
                     };

    out_archive & testee;

    std::istream inputStream{&buffer};

    boost::archive::text_iarchive in_archive{inputStream};

    Tournament deserialized_tournament{};

    in_archive & deserialized_tournament;

    EXPECT_EQ(tournament_name, deserialized_tournament.name());
    EXPECT_EQ(tournament_date, deserialized_tournament.date());
    EXPECT_EQ(tournament_location, deserialized_tournament.location());
}

TEST(A_serialized_Tournament, should_persist_the_next_round)
{
    std::stringbuf buffer{};
    std::ostream output_stream{&buffer};
    boost::archive::text_oarchive out_archive{output_stream};

    Tournament testee{TournamentData()
                      .with_name("Schweizermeisterschaft")
                      .on_date("2001-01-01")
                      .at_location("Kriens")
                     };

    auto next_round = testee.start_next_kata_round();
    next_round = testee.start_next_kata_round();

    out_archive & testee;

    std::istream inputStream{&buffer};

    boost::archive::text_iarchive in_archive{inputStream};

    Tournament deserialized_tournament{};

    in_archive & deserialized_tournament;

    const auto expected_next_round = next_round + 1;

    ASSERT_EQ(expected_next_round, deserialized_tournament.start_next_kata_round());
}

TEST(A_serialized_Tournament, should_persist_the_participant_list)
{
    std::stringbuf buffer{};
    std::ostream output_stream{&buffer};
    boost::archive::text_oarchive out_archive{output_stream};

    Tournament testee{};

    auto participant1 = Karateka{}
                        .with_name("Oyama")
                        .with_surname("Mas")
                        .with_birthdate("1923-07-27")
                        .from_dojo("Honbu")
                        .with_rank("10th Dan");

    testee.add_participant(participant1);

    auto participant2 = Karateka{}
                        .with_name("Arneil")
                        .with_surname("Steve")
                        .with_birthdate("1934-08-29")
                        .from_dojo("London")
                        .with_rank("9th Dan");

    testee.add_participant(participant2);

    out_archive & testee;

    std::istream inputStream{&buffer};

    boost::archive::text_iarchive in_archive{inputStream};

    Tournament deserialized_tournament{};

    in_archive& deserialized_tournament;

    const auto expected_number_of_participants = 2U;

    ASSERT_EQ(expected_number_of_participants, deserialized_tournament.number_of_participants());

    EXPECT_EQ(testee.get_participant(0).name(), deserialized_tournament.get_participant(0).name());
    EXPECT_EQ(testee.get_participant(0).surname(), deserialized_tournament.get_participant(0).surname());
    EXPECT_EQ(testee.get_participant(0).date_of_birth_as_string(), deserialized_tournament.get_participant(0).date_of_birth_as_string());
    EXPECT_EQ(testee.get_participant(0).dojo(), deserialized_tournament.get_participant(0).dojo());
    EXPECT_EQ(testee.get_participant(0).rank(), deserialized_tournament.get_participant(0).rank());

    EXPECT_EQ(testee.get_participant(1).name(), deserialized_tournament.get_participant(1).name());
    EXPECT_EQ(testee.get_participant(1).surname(), deserialized_tournament.get_participant(1).surname());
    EXPECT_EQ(testee.get_participant(1).date_of_birth_as_string(), deserialized_tournament.get_participant(1).date_of_birth_as_string());
    EXPECT_EQ(testee.get_participant(1).dojo(), deserialized_tournament.get_participant(1).dojo());
    EXPECT_EQ(testee.get_participant(1).rank(), deserialized_tournament.get_participant(1).rank());
}

TEST(A_Tournament, should_all_participants_move_to_the_next_round_when_starting_next_round)
{
    Tournament testee{};

    auto participant1 = Karateka{}
                        .with_name("Oyama")
                        .with_surname("Mas");

    testee.add_participant(participant1);

    auto participant2 = Karateka{}
                        .with_name("Arneil")
                        .with_surname("Steve");

    testee.add_participant(participant2);

    const auto expected_next_round = testee.start_next_kata_round() + 1;
    ASSERT_EQ(expected_next_round, testee.get_participant(0).next_round());
    ASSERT_EQ(expected_next_round, testee.get_participant(1).next_round());
}

TEST(A_Tournament, should_have_the_participant_which_performed_the_most_katas_on_top_of_the_ranked_list)
{
    auto participant1 = Karateka{}
                        .with_name("Oyama")
                        .with_surname("Mas");

    const auto score_1_1 = 5.0F;
    const auto score_1_2 = 5.0F;
    const auto score_1_3 = 5.0F;
    const auto score_1_4 = 5.0F;
    const auto score_1_5 = 5.0F;

    participant1.add_kata_score(score_1_1);
    participant1.add_kata_score(score_1_2);
    participant1.add_kata_score(score_1_3);
    participant1.add_kata_score(score_1_4);
    participant1.add_kata_score(score_1_5);

    participant1.next_round();
    participant1.add_kata_score(score_1_1);
    participant1.add_kata_score(score_1_2);
    participant1.add_kata_score(score_1_3);
    participant1.add_kata_score(score_1_4);
    participant1.add_kata_score(score_1_5);

    auto participant2 = Karateka{}
                        .with_name("Arneil")
                        .with_surname("Steve");

    const auto score_2_1 = 5.1F;
    const auto score_2_2 = 5.1F;
    const auto score_2_3 = 5.1F;
    const auto score_2_4 = 5.1F;
    const auto score_2_5 = 5.1F;

    participant2.add_kata_score(score_2_1);
    participant2.add_kata_score(score_2_2);
    participant2.add_kata_score(score_2_3);
    participant2.add_kata_score(score_2_4);
    participant2.add_kata_score(score_2_5);

    Tournament testee{};
    testee.add_participant(participant1);
    testee.add_participant(participant2);

    auto ranked_list_first_round = testee.get_ranked_list_of_participants();
    const auto first_place = ranked_list_first_round.at(0);
    const auto second_place = ranked_list_first_round.at(1);

    ASSERT_STREQ(participant1.name().c_str(), first_place.name().c_str());
    ASSERT_STREQ(participant2.name().c_str(), second_place.name().c_str());
}

TEST(A_Tournament, should_have_the_karateka_with_the_best_scores_on_rank_first_and_other_second)
{
    auto participant1 = Karateka{}
                        .with_name("Oyama")
                        .with_surname("Mas");

    const auto score_1 = 5.0F;
    const auto score_2 = 5.0F;
    const auto score_3 = 5.0F;
    const auto score_4 = 5.0F;
    const auto score_5 = 5.0F;

    participant1.add_kata_score(score_1);
    participant1.add_kata_score(score_2);
    participant1.add_kata_score(score_3);
    participant1.add_kata_score(score_4);
    participant1.add_kata_score(score_5);

    auto participant2 = Karateka{}
                        .with_name("Arneil")
                        .with_surname("Steve");

    const auto score_1a = 5.0F;
    const auto score_2a = 5.2F;
    const auto score_3a = 5.3F;
    const auto score_4a = 5.3F;
    const auto score_5a = 5.1F;

    participant2.add_kata_score(score_1a);
    participant2.add_kata_score(score_2a);
    participant2.add_kata_score(score_3a);
    participant2.add_kata_score(score_4a);
    participant2.add_kata_score(score_5a);

    Tournament testee{};
    testee.add_participant(participant1);
    testee.add_participant(participant2);


    const auto ranked_list = testee.get_anotated_ranked_list_of_participants();

    ASSERT_EQ(1U, std::get<0>(ranked_list.at(0)));
    ASSERT_STREQ(participant2.name().c_str(), std::get<1>(ranked_list.at(0)).name().c_str());
    ASSERT_EQ(2U, std::get<0>(ranked_list.at(1)));
    ASSERT_STREQ(participant1.name().c_str(), std::get<1>(ranked_list.at(1)).name().c_str());
}

TEST(A_Tournament, should_return_a_empty_ranklist_if_the_tournament_has_no_participants)
{
    Tournament testee{};
    const auto ranked_list = testee.get_anotated_ranked_list_of_participants();

    ASSERT_TRUE(ranked_list.empty());
}


TEST(A_Tournament, should_have_the_participant_which_performed_the_most_katas_ranked_first)
{
    auto participant1 = Karateka{}
                        .with_name("Oyama")
                        .with_surname("Mas");

    const auto score_1 = 5.0F;


    participant1.add_kata_score(score_1);
    participant1.add_kata_score(score_1);
    participant1.add_kata_score(score_1);
    participant1.add_kata_score(score_1);
    participant1.add_kata_score(score_1);

    participant1.next_round();
    participant1.add_kata_score(score_1);
    participant1.add_kata_score(score_1);
    participant1.add_kata_score(score_1);
    participant1.add_kata_score(score_1);
    participant1.add_kata_score(score_1);

    auto participant2 = Karateka{}
                        .with_name("Arneil")
                        .with_surname("Steve");

    const auto score_2 = 5.1F;

    participant2.add_kata_score(score_2);
    participant2.add_kata_score(score_2);
    participant2.add_kata_score(score_2);
    participant2.add_kata_score(score_2);
    participant2.add_kata_score(score_2);

    Tournament testee{};
    testee.add_participant(participant1);
    testee.add_participant(participant2);

    const auto ranked_list = testee.get_anotated_ranked_list_of_participants();

    ASSERT_EQ(1U, std::get<0>(ranked_list.at(0)));
    ASSERT_STREQ(participant1.name().c_str(), std::get<1>(ranked_list.at(0)).name().c_str());
    ASSERT_EQ(2U, std::get<0>(ranked_list.at(1)));
    ASSERT_STREQ(participant2.name().c_str(), std::get<1>(ranked_list.at(1)).name().c_str());
}

TEST(A_Tournament, should_have_karatekas_with_the_same_scores_on_the_last_round_on_the_same_rank)
{
    auto participant1 = Karateka{}
                        .with_name("Oyama")
                        .with_surname("Mas");

    const auto score_1 = 5.1F;

    participant1.add_kata_score(score_1);
    participant1.add_kata_score(score_1);
    participant1.add_kata_score(score_1);
    participant1.add_kata_score(score_1);
    participant1.add_kata_score(score_1);

    auto participant2 = Karateka{}
                        .with_name("Arneil")
                        .with_surname("Steve");

    const auto score_2 = 5.3F;

    participant2.add_kata_score(score_2);
    participant2.add_kata_score(score_2);
    participant2.add_kata_score(score_2);
    participant2.add_kata_score(score_2);
    participant2.add_kata_score(score_2);

    participant1.next_round();
    participant2.next_round();
    const auto score_3 = 5.0F;

    participant1.add_kata_score(score_3);
    participant1.add_kata_score(score_3);
    participant1.add_kata_score(score_3);
    participant1.add_kata_score(score_3);
    participant1.add_kata_score(score_3);

    participant2.add_kata_score(score_3);
    participant2.add_kata_score(score_3);
    participant2.add_kata_score(score_3);
    participant2.add_kata_score(score_3);
    participant2.add_kata_score(score_3);

    Tournament testee{};
    testee.add_participant(participant1);
    testee.add_participant(participant2);


    const auto ranked_list = testee.get_anotated_ranked_list_of_participants();
    ASSERT_EQ(1U, std::get<0>(ranked_list.at(0)));
    ASSERT_EQ(1U, std::get<0>(ranked_list.at(1)));
    ASSERT_NE(std::get<1>(ranked_list.at(0)).name(), std::get<1>(ranked_list.at(1)).name());
}

TEST(A_Tournament, should_have_third_karateka_on_rank_3_if_two_karatka_are_on_first_rank)
{
    auto participant1 = Karateka{}
                        .with_name("Oyama")
                        .with_surname("Mas");

    const auto score_1 = 5.0F;

    participant1.add_kata_score(score_1);
    participant1.add_kata_score(score_1);
    participant1.add_kata_score(score_1);
    participant1.add_kata_score(score_1);
    participant1.add_kata_score(score_1);

    auto participant2 = Karateka{}
                        .with_name("Arneil")
                        .with_surname("Steve");

    participant2.add_kata_score(score_1);
    participant2.add_kata_score(score_1);
    participant2.add_kata_score(score_1);
    participant2.add_kata_score(score_1);
    participant2.add_kata_score(score_1);

    auto participant3 = Karateka{}
                        .with_name("Oyama")
                        .with_surname("Shigeru");

    participant3.add_kata_score(score_1);
    participant3.add_kata_score(score_1);
    participant3.add_kata_score(score_1);
    participant3.add_kata_score(score_1);

    const auto score_2 = 4.9F;
    participant3.add_kata_score(score_2);

    Tournament testee{};
    testee.add_participant(participant1);
    testee.add_participant(participant2);
    testee.add_participant(participant3);


    const auto ranked_list = testee.get_anotated_ranked_list_of_participants();
    ASSERT_EQ(1U, std::get<0>(ranked_list.at(0)));
    ASSERT_EQ(1U, std::get<0>(ranked_list.at(1)));

    ASSERT_EQ(3U, std::get<0>(ranked_list.at(2)));
    ASSERT_STREQ(participant3.name().c_str(), std::get<1>(ranked_list.at(2)).name().c_str());
}


TEST(A_Tournament, should_remove_participant_and_reassign_startnumbers_when_removing_a_participant)
{
    auto participant1 = Karateka{}
                       .with_name("Oyama")
                       .with_surname("Mas");

    auto participant2 = Karateka{}
                        .with_name("Arneil")
                        .with_surname("Steve");

    Tournament testee{};
    testee.add_participant(participant1);
    testee.add_participant(participant2);
    const auto no_of_participants_before_delete = 2u;
    EXPECT_EQ(no_of_participants_before_delete, testee.number_of_participants());

    const auto startnumber_participant1 = participant1.get_startnumber();
    testee.remove_participant(startnumber_participant1);
    const auto no_of_participants_after_delete = 1u;
    EXPECT_EQ(no_of_participants_after_delete, testee.number_of_participants());

    const auto new_participant1 = testee.get_participant(startnumber_participant1);
    ASSERT_EQ(participant2.name(), new_participant1.name());
}

TEST(A_Tournament, should_throw_an_out_of_range_exception_when_trying_to_remove_a_non_existing_participant)
{
    auto participant = Karateka{}
                       .with_name("Oyama")
                       .with_surname("Mas");

    Tournament testee{};
    testee.add_participant(participant);

    const auto nonexisting_startnumber = 3u;
    ASSERT_THROW(testee.remove_participant(nonexisting_startnumber), std::out_of_range);
}

} //namespace
