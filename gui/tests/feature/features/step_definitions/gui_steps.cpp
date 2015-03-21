#include <memory>
#include <map>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cucumber-cpp/defs.hpp>
#include <boost/filesystem.hpp>
#include <QApplication>
#include <QtTest/QTest>
#include <QWindow>
#include <QAction>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QStackedWidget>

#include "tournament_runner/date.h"
#include "tournament_runner/tournament.h"
#include "tournament_runner/persistency.h"
#include "tournament_runner_gui/about_dialog.h"
#include "tournament_runner_gui/mainwindow.h"
#include "tournament_runner_gui/new_tournament_dialog.h"
#include "tournament_runner_gui/kataround_overview_frame.h"

using cucumber::ScenarioScope;
using ::TournamentRunnerGUI::AboutDialog;
using ::TournamentRunnerGUI::KataRoundOverviewFrame;
using ::TournamentRunnerGUI::MainWindow;
using ::TournamentRunner::Tournament;
using ::TournamentRunner::TournamentData;
using ::TournamentRunner::Persistency;
using ::TournamentRunner::Date;
using ::TournamentRunner::Karateka;

using ::testing::StartsWith;
using ::testing::NotNull;
using ::testing::Eq;
using ::testing::StrEq;
using ::testing::FloatEq;


namespace UIFeatureTests
{

static const std::map<std::string, std::string> action_map
{
    {"About", "actionAbout"}
    ,{"New Tournament", "actionNewTournament"}
    ,{"Save", "actionSave"}
    ,{"Save As", "actionSaveAs"}
    ,{"Open", "actionOpen"}
    ,{"Open Tournament", "actionOpen_Tournament"}
    ,{"Exit", "actionExit"}
};

static const std::map<std::string, std::string> dialog_map
{
    {"About", "AboutDialog"}
    ,{"New Tournament", "NewTournamentDialog"}
    ,{"Edit Tournament", "EditTournamentDialog"}
    ,{"Add Participant", "AddParticipantDialog"}
    ,{"Edit Participant", "EditParticipantDialog"}
};

static const std::map<std::string, std::string> button_map
{
    {"Add Participant", "AddParticipantButton"}
    ,{"Edit Tournament", "EditTournamentButton"}
    ,{"Start", "StartTournamentButton"}
};

static const std::map<std::string, Qt::Key> key_map
{
    {"Delete", Qt::Key_Delete}
    ,{"Ctrl", Qt::Key_Meta}
    ,{"Alt", Qt::Key_Alt}
    ,{"F1", Qt::Key_F1}
    ,{"Enter", Qt::Key_Enter}
    ,{"S", Qt::Key_S}
    ,{"N", Qt::Key_N}
};

static const std::map<std::string, Qt::KeyboardModifiers> modifier_key_map
{
    {"Ctrl", Qt::ControlModifier}
    ,{"Ctrl+Shift", Qt::ControlModifier | Qt::ShiftModifier}
    ,{"Alt", Qt::AltModifier}
};

static QAction* get_action (QMainWindow& mainwindow,
                     std::string which_action)
{
    return mainwindow.findChild<QAction*>(action_map.at(which_action).c_str());
}

static QPushButton* get_button (QMainWindow& mainwindow,
                         std::string which_button)
{
    return mainwindow.findChild<QPushButton*>(button_map.at(which_button).c_str());
}

static QDialog* get_dialog (QMainWindow& mainwindow,
                     std::string which_dialog)
{
    return mainwindow.findChild<QDialog*>(dialog_map.at(which_dialog).c_str());
}

static int qapplication_argc{};
static const auto test_profile_file = boost::filesystem::temp_directory_path() / "test_profile.ktm";

struct gui_context
{
    std::shared_ptr<QApplication> application{std::make_shared<QApplication>(qapplication_argc, nullptr)};
    AboutDialog about_dialog{QString{"0"}, QString{"0"}, QString{"0"}};
    Persistency persistency{test_profile_file.string()};
    boost::filesystem::path profile_file{test_profile_file};
    std::shared_ptr<Tournament> tournament{};
    std::shared_ptr<QMainWindow> mainwindow{std::make_shared<MainWindow>(about_dialog, persistency, tournament)};
    QWidget* current_widget{};
    QDialog* current_dialog{};
};

struct start_frame_context
{
    KataRoundOverviewFrame* startframe{};
};

BEFORE()
{
    ScenarioScope<gui_context> context;
}

AFTER()
{
    if (boost::filesystem::exists(test_profile_file))
    {
        boost::filesystem::remove(test_profile_file);
    }
    ScenarioScope<gui_context> context;
    if (boost::filesystem::exists(context->profile_file))
    {
        boost::filesystem::remove(context->profile_file);
    }
    while (context->application->hasPendingEvents())
    {
        context->application->processEvents();
    }
    context->application->exit();
}

GIVEN("^a tournaments is open$")
{
    ScenarioScope<gui_context> context;
    auto default_tournament = TournamentData{}
                              .with_name("SM")
                              .on_date("2014-07-26")
                              .at_location("Kriens");

    context->tournament = std::make_shared<Tournament>(default_tournament);
    context->mainwindow = std::make_shared<MainWindow>(context->about_dialog, context->persistency, context->tournament);
}

GIVEN("^the following Tournament is open:$")
{
    ScenarioScope<gui_context> context;
    auto tournament = TournamentData{};

    TABLE_PARAM(tournament_param);
    const auto& tournament_table = tournament_param.hashes();
    for (const auto& table_row : tournament_table)
    {
        tournament.with_name(std::string(table_row.at("name")));
        tournament.on_date(std::string(table_row.at("date")));

        tournament.at_location(std::string(table_row.at("location")));
    }

    context->tournament = std::make_shared<Tournament>(tournament);
    context->mainwindow = std::make_shared<MainWindow>(context->about_dialog, context->persistency, context->tournament);
    context->mainwindow->repaint();
}

GIVEN("^the Tournament has the following participants:$")
{
    ScenarioScope<gui_context> context;
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
            attendee.with_grade(table_row.at("rank"));
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
    context->mainwindow->repaint();
}

GIVEN("^the mainpage is shown$")
{
    // nothing to do here
}

GIVEN("^the following participants have performed a kata:$")
{
    TABLE_PARAM(person_param);
    const table_hashes_type& persons_table = person_param.hashes();

    ScenarioScope<gui_context> context;
    for (const auto& table_row : persons_table)
    {
        auto attendee = TournamentRunner::Karateka{}
                        .with_name(table_row.at("name"))
                        .with_surname(table_row.at("surname"));

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
    context->mainwindow->repaint();

}

GIVEN("^the following participants have performed katas:$")
{
    TABLE_PARAM(person_param);
    const auto& persons_table = person_param.hashes();

    ScenarioScope<gui_context> context;
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

WHEN("^I restart the application$")
{
    ScenarioScope<gui_context> context;
    try
    {
        context->tournament = std::make_shared<Tournament>(context->persistency.load_profile());
    }
    catch (const std::ios_base::failure& )
    {
        context->tournament = std::make_shared<Tournament>();
    }
    context->mainwindow = std::make_shared<MainWindow>(context->about_dialog, context->persistency, context->tournament);
}

WHEN("^I choose the \"([^\"]*)\" menu entry$")
{
    REGEX_PARAM(std::string, action_string);
    ScenarioScope<gui_context> context;

    auto action = get_action(*(context->mainwindow), action_string);

    ASSERT_THAT(action, NotNull());

    action->trigger();
}

WHEN("^I press the \"([^\"]*)\" button$")
{
    REGEX_PARAM(std::string, button_string);
    ScenarioScope<gui_context> context;

    auto* button = get_button(*(context->mainwindow), button_string);

    ASSERT_THAT(button, NotNull());

    button->click();
}

WHEN("^save the profile to \"([^\"]*)\"$")
{
    REGEX_PARAM(std::string, file_path);
    ScenarioScope<gui_context> context;
    context->profile_file = file_path;

    auto* file_dialog = context->mainwindow->findChild<QFileDialog*>();
    ASSERT_THAT(file_dialog, NotNull());
    file_dialog->fileSelected(QString::fromStdString(file_path));
    file_dialog->close();
}

WHEN("^I load the profile \"([^\"]*)\"$")
{
    REGEX_PARAM(std::string, file_path);
    ScenarioScope<gui_context> context;
    context->profile_file = file_path;
    auto* open_action = get_action(*(context->mainwindow), "Open");
    ASSERT_THAT(open_action, NotNull());
    open_action->trigger();

    auto* file_dialog = context->mainwindow->findChild<QFileDialog*>();
    ASSERT_THAT(file_dialog, NotNull());
    file_dialog->fileSelected(QString::fromStdString(file_path));
    file_dialog->close();
}

WHEN("^the startpage for the second round is shown$")
{
    ScenarioScope<gui_context> context;
    ScenarioScope<start_frame_context> startpage_context;
    startpage_context->startframe = new KataRoundOverviewFrame(*(context->tournament), 1);
    context->tournament->start_next_kata_round();
}

WHEN("I enter the \"([^\"]*)\" dialog")
{
    REGEX_PARAM(std::string, dialog_name);
    ScenarioScope<gui_context> context;

    auto* dialog = get_dialog(*(context->mainwindow), dialog_name);

    ASSERT_THAT(dialog, NotNull());

    context->current_dialog = dialog;
}

WHEN("^I enter the following tournament data:$")
{
    ScenarioScope<gui_context> context;

    TABLE_PARAM(tournament_param);
    const auto& tournament_table = tournament_param.hashes();
    for (const auto& table_row : tournament_table)
    {
        auto* tournament_name_edit = context->mainwindow->findChild<QLineEdit*>("TournamentNameEdit");
        if (tournament_name_edit != nullptr)
        {
            const auto name = std::string(table_row.at("name"));
            tournament_name_edit->setText(QString::fromStdString(name));
        }

        auto* tournament_date_edit = context->mainwindow->findChild<QDateEdit*>("TournamentDateEdit");
        if (tournament_date_edit != nullptr)
        {
            const Date date(std::string(table_row.at("date")));
            tournament_date_edit->setDate(QDate(date.year(), date.month(), date.day()));
        }

        auto* tournament_location_edit = context->mainwindow->findChild<QTextEdit*>("TournamentLocationEdit");
        if (tournament_location_edit != nullptr)
        {
            const auto location = std::string(table_row.at("location"));
            tournament_location_edit->setText(QString::fromStdString(location));
        }
    }
}

WHEN("^I enter the following participant data:$")
{
    ScenarioScope<gui_context> context;

    TABLE_PARAM(participant_param);
    const auto& participant_table = participant_param.hashes();
    for (const auto& table_row : participant_table)
    {
        auto* participant_name_edit = context->mainwindow->findChild<QLineEdit*>("ParticipantNameEdit");
        if (participant_name_edit != nullptr)
        {
            const auto name = std::string(table_row.at("name"));
            participant_name_edit->setText(QString::fromStdString(name));
        }

        auto* participant_surname_edit = context->mainwindow->findChild<QLineEdit*>("ParticipantSurnameEdit");
        if (participant_surname_edit != nullptr)
        {
            const auto surname = std::string(table_row.at("surname"));
            participant_surname_edit->setText(QString::fromStdString(surname));
        }

        auto* date_of_birth_edit = context->mainwindow->findChild<QDateEdit*>("DateOfBirthEdit");
        if (date_of_birth_edit != nullptr)
        {
            const Date date(std::string(table_row.at("date of birth")));
            date_of_birth_edit->setDate(QDate(date.year(), date.month(), date.day()));
        }

        auto* dojo_edit = context->mainwindow->findChild<QLineEdit*>("DojoEdit");
        if (dojo_edit != nullptr)
        {
            const auto dojo = std::string(table_row.at("dojo"));
            dojo_edit->setText(QString::fromStdString(dojo));
        }

        auto* rank_edit = context->mainwindow->findChild<QComboBox*>("RankComboBox");
        if (rank_edit != nullptr)
        {
            const auto rank = std::string(table_row.at("rank"));
            auto index = rank_edit->findText(QString::fromStdString(rank));
            rank_edit->setCurrentIndex(index);
        }

    }
}

WHEN("^I select the (\\d+)(?:nd|st|rd|th) row$")
{
    ScenarioScope<gui_context> context;
    REGEX_PARAM(int, row);

    auto* participantTable = context->mainwindow->findChild<QTableView*>("ParticipantTableView");
    participantTable->selectRow(row-1);
    context->current_widget = participantTable;
}

WHEN("^I doubleclick the (\\d+)(?:nd|st|rd|th) row$") {
    ScenarioScope<gui_context> context;
    REGEX_PARAM(int, row);

    auto* participantTable = context->mainwindow->findChild<QTableView*>("ParticipantTableView");

    participantTable->selectRow(row-1);

    const auto xPos = participantTable->columnViewportPosition(2) + 5;
    const auto yPos = participantTable->rowViewportPosition(row-1) + 10;

    auto* viewport = participantTable->viewport();

    //QTest::mouseClick(viewport, Qt::LeftButton, NULL, QPoint(xPos, yPos));
    QTest::mouseDClick(viewport, Qt::LeftButton, NULL, QPoint(xPos, yPos));
}

WHEN("^I press the \"([^\"]*)\" key$")
{
    ScenarioScope<gui_context> context;
    REGEX_PARAM(std::string, key);

    if(context->current_dialog)
    {
        QTest::keyClick(context->current_dialog, key_map.at(key));
    }
    else if(context->current_widget)
    {
        QTest::keyClick(context->current_widget, key_map.at(key));
    }
    else
    {
        context->mainwindow->show();
        QTest::keyClick(context->mainwindow.get(), key_map.at(key));
    }
}

WHEN("^I press the \"([^\"]*)\" and the \"([^\"]*)\" key together$")
{
    ScenarioScope<gui_context> context;
    REGEX_PARAM(std::string, modifier_key);
    REGEX_PARAM(std::string, key);

    context->mainwindow->show();
    QTest::keyClick(context->mainwindow.get(), key_map.at(key), modifier_key_map.at(modifier_key), 50);
}

WHEN("^I press the \"([^\"]*)\" the \"([^\"]*)\" and the \"([^\"]*)\" key together$")
{
    ScenarioScope<gui_context> context;
    REGEX_PARAM(std::string, first_modifier_key);
    REGEX_PARAM(std::string, second_modifier_key);
    REGEX_PARAM(std::string, key);

    const auto modifier_key = first_modifier_key + "+" + second_modifier_key;

    context->mainwindow->show();
    QTest::keyClick(context->mainwindow.get(), key_map.at(key), modifier_key_map.at(modifier_key), 50);
}

THEN("^the \"About\" dialog is shown$")
{
    ScenarioScope<gui_context> context;
    ASSERT_TRUE(QTest::qWaitForWindowExposed(&(context->about_dialog)));
}

THEN("^the mainscreen shows the following tournament:$")
{
    ScenarioScope<gui_context> context;
    context->mainwindow->repaint();

    TABLE_PARAM(tournament_param);
    const auto& tournament_table = tournament_param.hashes();
    for (const auto& table_row : tournament_table)
    {
        const auto* tournament_name = context->mainwindow->findChild<QLabel*>("TournamentName");
        EXPECT_THAT(tournament_name, NotNull());
        if (tournament_name != nullptr)
        {
            EXPECT_THAT(tournament_name->text().toStdString(), StrEq(std::string(table_row.at("name"))));
        }

        const auto* tournament_date = context->mainwindow->findChild<QLabel*>("TournamentDate");
        EXPECT_THAT(tournament_date, NotNull());
        if (tournament_date != nullptr)
        {
            EXPECT_THAT(tournament_date->text().toStdString(), StrEq(std::string(table_row.at("date"))));
        }

        const auto* tournament_location = context->mainwindow->findChild<QLabel*>("TournamentLocation");
        EXPECT_THAT(tournament_location, NotNull());
        if (tournament_location != nullptr)
        {
            EXPECT_THAT(tournament_location->text().toStdString(), StrEq(std::string(table_row.at("location"))));
        }
    }
}

THEN("^the participant list contains the following participants:$")
{
    ScenarioScope<gui_context> context;

    TABLE_PARAM(participant_param);
    const auto& participant_table = participant_param.hashes();
    auto index = 0;
    for (const auto& table_row : participant_table)
    {
        const auto* participantTable = context->mainwindow->findChild<QTableView*>("ParticipantTableView");

        auto* model = participantTable->model();
        const auto name = model->data(model->index(index, 0)).toString().toStdString();
        EXPECT_THAT(name, StrEq(std::string(table_row.at("name"))));

        const auto surname = model->data(model->index(index, 1)).toString().toStdString();
        EXPECT_THAT(surname, StrEq(std::string(table_row.at("surname"))));

        const auto date_of_birth = model->data(model->index(index, 2)).toString().toStdString();
        EXPECT_THAT(date_of_birth, StrEq(std::string(table_row.at("date of birth"))));

        const auto dojo = model->data(model->index(index, 3)).toString().toStdString();
        EXPECT_THAT(dojo, StrEq(std::string(table_row.at("dojo"))));

        const auto rank = model->data(model->index(index, 4)).toString().toStdString();
        EXPECT_THAT(rank, StrEq(std::string(table_row.at("rank"))));

        index++;
    }
}

THEN("^the file \"([^\"]*)\" exists$")
{
    REGEX_PARAM(std::string, file_path);
    const boost::filesystem::path file_to_check{file_path};
    ASSERT_TRUE(boost::filesystem::exists(file_to_check));
}

THEN("^a startpage for the first round is shown$")
{
   ScenarioScope<gui_context> context;
   const auto central_widget = dynamic_cast<QStackedWidget*>(context->mainwindow->centralWidget());
   ASSERT_THAT(central_widget, NotNull());
   const auto central_frame = central_widget->currentWidget();
   ScenarioScope<start_frame_context> frame_context;
   frame_context->startframe = dynamic_cast<KataRoundOverviewFrame*>(central_frame);
   ASSERT_THAT(frame_context->startframe, NotNull());
}

THEN("^the following partipants are shown on the startpage:$")
{
    ScenarioScope<start_frame_context> context;

    ASSERT_THAT(context->startframe, NotNull());
    const auto* participantTable = context->startframe->findChild<QTableView*>("RoundTableView");
    ASSERT_THAT(participantTable, NotNull());
    auto model = participantTable->model();

    TABLE_PARAM(participant_param);
    const auto& participant_table = participant_param.hashes();
    for (const auto& table_row : participant_table)
    {
        auto index = std::stoi(table_row.at("start position"));
        const auto name = model->data(model->index(index-1, 0)).toString().toStdString();
        ASSERT_THAT(name, StartsWith(std::string(table_row.at("name"))));
    }
}

THEN("^the following table is shown on the finalpage:$")
{
    ScenarioScope<gui_context> context;
    auto* mainwindow = dynamic_cast<MainWindow*>(context->mainwindow.get());
    ASSERT_THAT(mainwindow, NotNull());
    mainwindow->tournament_finished_slot();

    const auto* finalframe = context->mainwindow->findChild<QFrame*>("FinalFrame");
    ASSERT_THAT(finalframe, NotNull());
    const auto* participantTable = finalframe->findChild<QTableView*>("ParticipantTableView");
    ASSERT_THAT(participantTable, NotNull());
    auto model = participantTable->model();
    auto index = 0;

    TABLE_PARAM(participant_param);
    const auto& participant_table = participant_param.hashes();
    for (const auto& table_row : participant_table)
    {
        const auto rank = model->data(model->index(index, 0)).toInt();
        EXPECT_THAT(rank, Eq(std::stoi(table_row.at("rank"))));
        const auto name = model->data(model->index(index, 1)).toString().toStdString();
        EXPECT_THAT(name, StartsWith(std::string(table_row.at("name"))));
        const auto overall_1 = model->data(model->index(index, 2)).toFloat();
        EXPECT_THAT(overall_1, FloatEq(std::stof(table_row.at("overall 1")))) << name;
        const auto min_1 = model->data(model->index(index, 3)).toFloat();
        EXPECT_THAT(min_1, FloatEq(std::stof(table_row.at("min 1")))) << name;
        const auto max_1 = model->data(model->index(index, 4)).toFloat();
        EXPECT_THAT(max_1, FloatEq(std::stof(table_row.at("max 1")))) << name;
        if (!table_row.at("overall 2").empty())
        {
            const auto overall_2 = model->data(model->index(index, 5)).toFloat();
            EXPECT_THAT(overall_2, FloatEq(std::stof(table_row.at("overall 2")))) << name;
            const auto min_2 = model->data(model->index(index, 6)).toFloat();
            EXPECT_THAT(min_2, FloatEq(std::stof(table_row.at("min 2")))) << name;
            const auto max_2 = model->data(model->index(index, 7)).toFloat();
            EXPECT_THAT(max_2, FloatEq(std::stof(table_row.at("max 2")))) << name;
            if (!table_row.at("overall 3").empty())
            {
                const auto overall_3 = model->data(model->index(index, 8)).toFloat();
                EXPECT_THAT(overall_3, FloatEq(std::stof(table_row.at("overall 3")))) << name;
                const auto min_3 = model->data(model->index(index, 9)).toFloat();
                EXPECT_THAT(min_3, FloatEq(std::stof(table_row.at("min 3")))) << name;
                const auto max_3 = model->data(model->index(index, 10)).toFloat();
                EXPECT_THAT(max_3, FloatEq(std::stof(table_row.at("max 3")))) << name;
            }
        }
        index++;
    }
}


} // namespace UIFeatureTests
