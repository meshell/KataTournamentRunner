#include <map>

#include <gtest/gtest.h>

#include "tournament_runner/tournament.h"
#include "tournament_runner/karateka.h"
#include "tournament_runner_gui/participant_model.h"

namespace
{

using ::TournamentRunner::Tournament;
using ::TournamentRunner::Karateka;
using ::TournamentRunnerGUI::ParticipantModel;

TEST(The_ParticipantModel, number_of_colums_should_equal_the_number_of_data_fields_of_a_karateka)
{
    const auto expected_number_of_columns = 5;
    Tournament tournament;
    ParticipantModel testee{tournament};

    ASSERT_EQ(expected_number_of_columns, testee.columnCount());
}

TEST(The_ParticipantModel, number_of_rows_should_equal_0_if_no_participant_is_registred)
{
    const auto expected_number_of_rows = 0;
    Tournament tournament;
    ParticipantModel testee{tournament};
    ASSERT_EQ(expected_number_of_rows, testee.rowCount());
}

TEST(The_ParticipantModel, number_of_rows_should_equal_the_number_of_registred_participant)
{
    Tournament tournament;
    ParticipantModel testee{tournament};

    auto participant1 = Karateka{}
                        .with_name("Arneil")
                        .with_surname("Steve")
                        .with_birthdate("1934-08-29")
                        .from_dojo("London")
                        .with_rank("9th Dan");

    auto participant2 = Karateka{}
                        .with_name("Oyama")
                        .with_surname("Mas")
                        .with_birthdate("1923-07-27")
                        .from_dojo("Honbu")
                        .with_rank("10th Dan");
    tournament.add_participant(participant1);
    tournament.add_participant(participant2);
    const auto expected_number_of_rows = 2;

    ASSERT_EQ(expected_number_of_rows, testee.rowCount());
}

TEST(The_ParticipantModel, has_the_registred_participant_in_the_first_row)
{
    Tournament tournament;
    ParticipantModel testee{tournament};
    const std::string name{"Arneil"};
    const std::string surname{"Steve"};
    const std::string dojo{"London"};
    const std::string rank{"9th Dan"};

    auto participant1 = Karateka{}
                        .with_name(name)
                        .with_surname(surname)
                        .with_birthdate("1934-08-29")
                        .from_dojo(dojo)
                        .with_rank(rank);

    tournament.add_participant(participant1);

    std::map<std::string, std::string> header_to_data_map =
    {
        {"Name", name},
        {"Surname", surname},
        {"Date of birth", "1934-08-29"},
        {"Dojo", dojo},
        {"Rank", rank}
    };

    testee.headerData(0, Qt::Horizontal).toString().toStdString();

    for (auto column = 0U; column < 5U; ++column)
    {
        const auto header = testee.headerData(column, Qt::Horizontal).toString().toStdString();
        EXPECT_EQ(header_to_data_map.at(header),
              testee.data(testee.index(0, column)).toString().toStdString()) << "Header: " << header;
    }
}

} // namespace
