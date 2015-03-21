#include <memory>

#include <gtest/gtest.h>

#include "tournament_runner/tournament.h"
#include "tournament_runner/karateka.h"

#include "tournament_runner_gui/kata_round_participant_model.h"

namespace
{

using ::TournamentRunner::Tournament;
using ::TournamentRunner::Karateka;
using ::TournamentRunnerGUI::KataRoundParticipantModel;

const auto first_round = 0U;

Karateka create_default_participant()
{
    return Karateka{}.with_name("Arneil")
                     .with_surname("Steve")
                     .with_birthdate("1934-08-29")
                     .from_dojo("London")
                     .with_grade("9th Dan");
}

TEST(The_KataRoundParticipantModel, number_of_rows_should_equal_to_the_size_of_the_startlist)
{
    Tournament tournament{};
    auto participant1 = create_default_participant();

    auto participant2 = create_default_participant();

    tournament.add_participant(participant1);
    tournament.add_participant(participant2);
    const auto expected_number_of_rows = 2;

    const auto next_round_startlist = tournament.get_startlist_for_next_kata_round(expected_number_of_rows);
    std::vector<size_t> startlist;
    startlist.assign(std::begin(next_round_startlist), std::end(next_round_startlist));
    KataRoundParticipantModel testee(first_round, tournament, startlist);

    ASSERT_EQ(expected_number_of_rows, testee.rowCount());
}

TEST(The_KataRoundParticipantModel, has_the_participant_of_the_startlist_in_the_rows)
{
    Tournament tournament{};
    const std::string name{"Arneil"};
    const std::string surname{"Steve"};
    const std::string dojo{"London"};

    auto participant1 = Karateka{}
                        .with_name(name)
                        .with_surname(surname)
                        .with_birthdate("1934-08-29")
                        .from_dojo(dojo)
                        .with_grade("9th Dan");

    const std::string name2{"Oyama"};
    const std::string surname2{"Mas"};
    const std::string dojo2{"Honbu"};

    auto participant2 = Karateka{}
                        .with_name(name2)
                        .with_surname(surname2)
                        .with_birthdate("1923-07-27")
                        .from_dojo(dojo2)
                        .with_grade("10th Dan");
    tournament.add_participant(participant1);
    tournament.add_participant(participant2);

    const auto next_round_startlist = tournament.get_startlist_for_next_kata_round(tournament.number_of_participants());
    std::vector<size_t> startlist;
    startlist.assign(std::begin(next_round_startlist), std::end(next_round_startlist));
    KataRoundParticipantModel testee{first_round, tournament, startlist};

    const auto name_and_dojo_participant_1 = name + ", " + surname + " (" + dojo + ")";
    const auto name_and_dojo_participant_2 = name2 + ", " + surname2 + " (" + dojo2 + ")";

    EXPECT_EQ(name_and_dojo_participant_1, testee.data(testee.index(0, 0)).toString().toStdString());
    EXPECT_EQ(name_and_dojo_participant_2, testee.data(testee.index(1, 0)).toString().toStdString());
}

TEST(The_KataRoundParticipantModel, shows_the_scores_of_the_curent_kata_round)
{
    Tournament tournament{};

    auto participant = create_default_participant();

    const auto score_1 = 5.0F;
    const auto score_2 = 5.1F;
    const auto score_3 = 5.2F;
    const auto score_4 = 5.3F;
    const auto score_5 = 5.4F;
    participant.add_kata_score(score_1);
    participant.add_kata_score(score_2);
    participant.add_kata_score(score_3);
    participant.add_kata_score(score_4);
    participant.add_kata_score(score_5);

    tournament.add_participant(participant);

    const auto next_round_startlist = tournament.get_startlist_for_next_kata_round(tournament.number_of_participants());
    std::vector<size_t> startlist;
    startlist.assign(std::begin(next_round_startlist), std::end(next_round_startlist));

    KataRoundParticipantModel testee(first_round, tournament, startlist);

    EXPECT_EQ(score_1, testee.data(testee.index(0, 1)).toFloat());
    EXPECT_EQ(score_2, testee.data(testee.index(0, 2)).toFloat());
    EXPECT_EQ(score_3, testee.data(testee.index(0, 3)).toFloat());
    EXPECT_EQ(score_4, testee.data(testee.index(0, 4)).toFloat());
    EXPECT_EQ(score_5, testee.data(testee.index(0, 5)).toFloat());
}

TEST(The_KataRoundParticipantModel, shows_the_overall_deduction_of_the_curent_kata_round)
{
    Tournament tournament{};
    auto participant = create_default_participant();

    const auto deduction = 0.5F;
    participant.add_deduction(deduction);

    tournament.add_participant(participant);

    const auto next_round_startlist = tournament.get_startlist_for_next_kata_round(tournament.number_of_participants());
    std::vector<size_t> startlist;
    startlist.assign(std::begin(next_round_startlist), std::end(next_round_startlist));

    KataRoundParticipantModel testee(first_round, tournament, startlist);

    EXPECT_EQ(deduction, testee.data(testee.index(0, 6)).toFloat());
}

TEST(The_KataRoundParticipantModel, shows_the_overall_score_of_the_curent_kata_round)
{
    Tournament tournament{};

    auto participant = create_default_participant();

    const auto score_1 = 5.0F;
    const auto score_2 = 5.1F;
    const auto score_3 = 5.2F;
    const auto score_4 = 5.3F;
    const auto score_5 = 5.4F;
    participant.add_kata_score(score_1);
    participant.add_kata_score(score_2);
    participant.add_kata_score(score_3);
    participant.add_kata_score(score_4);
    participant.add_kata_score(score_5);
    const auto deduction = 0.5F;
    participant.add_deduction(deduction);

    const auto overallscore = participant.get_overall_score_of_current_round();

    tournament.add_participant(participant);

    const auto next_round_startlist = tournament.get_startlist_for_next_kata_round(tournament.number_of_participants());
    std::vector<size_t> startlist;
    startlist.assign(std::begin(next_round_startlist), std::end(next_round_startlist));

    KataRoundParticipantModel testee(first_round, tournament, startlist);

    EXPECT_EQ(overallscore, testee.data(testee.index(0, 7)).toFloat());
}

} // namespace
