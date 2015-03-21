#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <QPushButton>
#include <QWizard>

#include <tournament_runner/tournament.h>
#include <tournament_runner/karateka.h>
#include <tournament_runner_gui/kataround_overview_frame.h>

namespace
{

using ::TournamentRunner::Tournament;
using ::TournamentRunner::Karateka;
using ::TournamentRunnerGUI::KataRoundOverviewFrame;

using ::testing::NotNull;
using ::testing::IsNull;

	
TEST(The_Kata_Round_overview_page, shows_a_Wizard_upon_StartButton_click_with_a_start_page_and_page_per_participant)
{
    auto participant1 = Karateka{}.with_name("Arneil")
                                  .with_surname("Steve");

    auto participant2 = Karateka{}.with_name("Oyama")
                                  .with_surname("Mas");


    auto tournament = Tournament{};
    tournament.add_participant(participant1);
    tournament.add_participant(participant2);

    KataRoundOverviewFrame testee{tournament, TournamentRunnerGUI::first_round};

    auto* start_button = testee.findChild<QPushButton*>("StartTournamentButton");

    auto* wizard = testee.findChild<QWizard*>();
    ASSERT_THAT(wizard, IsNull());

    start_button->click();

    wizard = testee.findChild<QWizard*>();
    ASSERT_THAT(wizard, NotNull());
    auto page_ids = wizard->pageIds();
    const auto expected_no_of_wizard_pages = tournament.number_of_participants()+1;
    ASSERT_EQ(expected_no_of_wizard_pages, page_ids.size());
}	

TEST(The_Kata_Round_overview_page, enables_the_next_button_when_the_wizard_finished_successfully)
{
    auto tournament = Tournament{};
    KataRoundOverviewFrame testee{tournament, TournamentRunnerGUI::first_round};

    auto* next_button = testee.findChild<QPushButton*>("NextRoundButton");
    auto* finish_button = testee.findChild<QPushButton*>("FinishButton");

    ASSERT_FALSE(next_button->isEnabled());
    ASSERT_TRUE(finish_button->isHidden());

    testee.wizard_finished_slot(1);

    ASSERT_TRUE(next_button->isEnabled());
}

TEST(The_Kata_Round_overview_page_of_the_final_round, enables_the_finish_button_when_the_wizard_finished_successfully)
{
    auto tournament = Tournament{};
    KataRoundOverviewFrame testee{tournament, TournamentRunnerGUI::final_round};

    auto* next_button = testee.findChild<QPushButton*>("NextRoundButton");
    auto* finish_button = testee.findChild<QPushButton*>("FinishButton");

    ASSERT_FALSE(finish_button->isEnabled());
    ASSERT_TRUE(next_button->isHidden());

    testee.wizard_finished_slot(1);

    ASSERT_TRUE(finish_button->isEnabled());
}

} // namespace
