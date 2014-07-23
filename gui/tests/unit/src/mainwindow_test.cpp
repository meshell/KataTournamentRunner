#include <gmock/gmock.h>

#include <boost/filesystem.hpp>

#include <QtWidgets>
#include <QLabel>

#include "tournament_runner/persistency.h"
#include "tournament_runner/tournament.h"
#include "tournament_runner/os_helper.h"
#include "tournament_runner_gui/mainwindow.h"
#include "tournament_runner_gui/mainframe.h"
#include "tournament_runner_gui/finalframe.h"
#include "tournament_runner_gui/kataround_overview_frame.h"

#include "persistency_mock.h"

namespace
{

using ::TournamentRunnerGUI::MainWindow;
using ::TournamentRunnerGUI::MainFrame;
using ::TournamentRunnerGUI::KataRoundOverviewFrame;
using ::TournamentRunnerGUI::FinalFrame;
using boost::filesystem::path;

using ::testing::Ref;
using ::testing::Eq;
using ::testing::Return;
using ::testing::NotNull;

class The_MainWindow :
    public ::testing::Test
{
public:
    void SetUp() override;
    void TearDown() override;

protected:
    path path_to_profile_;
};

void The_MainWindow::SetUp()
{

    auto default_profile_path = TournamentRunner::OS::get_home_folder();
    if (default_profile_path.empty())
    {
        default_profile_path = "./";
    }

    path_to_profile_ = path{default_profile_path};
    path_to_profile_ /= path{".ktm"};
    path_to_profile_ /= path{"default.ktm"};
}

void The_MainWindow::TearDown()
{
    if (boost::filesystem::exists(path_to_profile_))
    {
        boost::filesystem::remove_all(path_to_profile_.parent_path());
    }
}

TEST_F(The_MainWindow, creates_StackedWidget_with_a_tournament_overview_upon_tournament_creation)
{
    QDialog about_dialog;
    const std::string tournament_name{"SM"};
    const std::string tournament_date{"2014-07-01"};
    const std::string tournament_location{"Kriens"};
    const auto tournament = TournamentRunner::TournamentData{}
                                             .with_name(tournament_name)
                                             .on_date(tournament_date)
                                             .at_location(tournament_location);
    PersistencyMock persistency_handler;

    MainWindow testee{about_dialog, persistency_handler, nullptr};

    testee.create_tournament_slot(tournament);

    const auto* central_widget = dynamic_cast<QStackedWidget*>(testee.centralWidget());
    ASSERT_THAT(central_widget, NotNull());
    ASSERT_THAT(dynamic_cast<MainFrame*>(central_widget->currentWidget()), NotNull());
}

TEST_F(The_MainWindow, shows_the_tournament_overview_upon_tournament_creation)
{
    QDialog about_dialog;
    const std::string tournament_name{"SM"};
    const std::string tournament_date{"2014-07-01"};
    const std::string tournament_location{"Kriens"};
    const auto tournament = TournamentRunner::TournamentData{}
                                             .with_name(tournament_name)
                                             .on_date(tournament_date)
                                             .at_location(tournament_location);
    PersistencyMock persistency_handler;

    MainWindow testee{about_dialog, persistency_handler, nullptr};

    testee.create_tournament_slot(tournament);

    const auto tournament_name_label = testee.findChild<QLabel*>("TournamentName");
    ASSERT_THAT(tournament_name_label, NotNull());
    EXPECT_EQ(tournament_name, tournament_name_label->text().toStdString());

    const auto tournament_date_label = testee.findChild<QLabel*>("TournamentDate");
    ASSERT_THAT(tournament_date_label, NotNull());
    EXPECT_EQ(tournament_date, tournament_date_label->text().toStdString());

    const auto tournament_location_label = testee.findChild<QLabel*>("TournamentLocation");
    ASSERT_THAT(tournament_location_label, NotNull());
    EXPECT_EQ(tournament_location, tournament_location_label->text().toStdString());
}

TEST_F(The_MainWindow, should_save_a_profile_to_a_default_profile_when_save_action_is_triggered)
{
    QDialog about_dialog;
    std::shared_ptr<TournamentRunner::Tournament> tournament = std::make_shared<TournamentRunner::Tournament>();
    ::testing::NiceMock<PersistencyMock> persistency_handler;


    EXPECT_CALL(persistency_handler, save_profile(Ref(*tournament)));

    MainWindow testee{about_dialog, persistency_handler, tournament};

    testee.on_actionSave_triggered();
}

TEST_F(The_MainWindow, should_create_a_default_profile_upon_save_if_none_exists)
{
    QDialog about_dialog;
    std::shared_ptr<TournamentRunner::Tournament> tournament = std::make_shared<TournamentRunner::Tournament>();
    TournamentRunner::Persistency persistency_handler{path_to_profile_.string()};

    ASSERT_FALSE(boost::filesystem::exists(path_to_profile_));

    MainWindow testee{about_dialog, persistency_handler, tournament};

    testee.on_actionSave_triggered();

    ASSERT_TRUE(boost::filesystem::exists(path_to_profile_));
}

TEST_F(The_MainWindow, should_show_the_tournament_saved_in_the_default_profile_upon_startup)
{
    QDialog about_dialog;
    const std::string tournament_name{"SM"};
    const std::string tournament_date{"2014-07-01"};
    const std::string tournament_location{"Kriens"};
    TournamentRunner::Tournament tournament {TournamentRunner::TournamentData{}
                                             .with_name(tournament_name)
                                             .on_date(tournament_date)
                                             .at_location(tournament_location)
                                            };
    PersistencyMock persistency_handler;

    MainWindow testee{about_dialog, persistency_handler, std::make_shared<TournamentRunner::Tournament>(tournament)};

    const auto name = testee.findChild<QLabel*>("TournamentName");
    ASSERT_TRUE(name != nullptr);
    EXPECT_EQ(tournament_name, name->text().toStdString());

    const auto date = testee.findChild<QLabel*>("TournamentDate");
    ASSERT_TRUE(date != nullptr);
    EXPECT_EQ(tournament_date, date->text().toStdString());

    const auto location = testee.findChild<QLabel*>("TournamentLocation");
    ASSERT_TRUE(date != nullptr);
    EXPECT_EQ(tournament_location, location->text().toStdString());

}

TEST_F(The_MainWindow, adds_a_kata_round_overview_page_upon_tournament_start)
{
    QDialog about_dialog;
    const std::string tournament_name{"SM"};
    const std::string tournament_date{"2014-07-01"};
    const std::string tournament_location{"Kriens"};
    const auto tournament = std::make_shared<TournamentRunner::Tournament>(TournamentRunner::TournamentData{}
                                             .with_name(tournament_name)
                                             .on_date(tournament_date)
                                             .at_location(tournament_location)
                                            );
    PersistencyMock persistency_handler;

    MainWindow testee{about_dialog, persistency_handler, tournament};


    testee.start_kata_round_slot(0);

    const auto* central_widget = dynamic_cast<QStackedWidget*>(testee.centralWidget());
    ASSERT_THAT(central_widget, NotNull());
    ASSERT_THAT(dynamic_cast<KataRoundOverviewFrame*>(central_widget->currentWidget()), NotNull());
}

TEST_F(The_MainWindow, advances_the_tournament_to_the_next_round_upon_next_round_start)
{
    QDialog about_dialog;
    const std::string tournament_name{"SM"};
    const std::string tournament_date{"2014-07-01"};
    const std::string tournament_location{"Kriens"};
    const auto tournament = std::make_shared<TournamentRunner::Tournament>(TournamentRunner::TournamentData{}
                                             .with_name(tournament_name)
                                             .on_date(tournament_date)
                                             .at_location(tournament_location)
                                            );
    PersistencyMock persistency_handler;

    MainWindow testee{about_dialog, persistency_handler, tournament};

    const auto initial_round = tournament->get_current_round();

    testee.next_kata_round_slot(1);

    ASSERT_EQ(initial_round+1, tournament->get_current_round());

}
TEST_F(The_MainWindow, changes_to_a_final_tournament_overview_when_tournament_finishs)
{
    QDialog about_dialog;
    const std::string tournament_name{"SM"};
    const std::string tournament_date{"2014-07-01"};
    const std::string tournament_location{"Kriens"};
    const auto tournament = std::make_shared<TournamentRunner::Tournament>(TournamentRunner::TournamentData{}
                                             .with_name(tournament_name)
                                             .on_date(tournament_date)
                                             .at_location(tournament_location)
                                            );
    PersistencyMock persistency_handler;

    MainWindow testee{about_dialog, persistency_handler, tournament};

    // create a new frame
    testee.start_kata_round_slot(0);
    testee.tournament_finished_slot();

    const auto* central_widget = dynamic_cast<QStackedWidget*>(testee.centralWidget());
    ASSERT_THAT(central_widget, NotNull());
    ASSERT_THAT(dynamic_cast<FinalFrame*>(central_widget->currentWidget()), NotNull());

}

} // namespace
