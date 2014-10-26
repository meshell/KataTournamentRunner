#include "tournament_runner_gui/kataround_overview_frame.h"
#include "ui_kataround_overview_frame.h"

#include <limits>
#include <map>

#include <boost/algorithm/string.hpp>
#include <QLabel>
#include <QListView>
#include <QLineEdit>
#include <QFont>
#include <QHBoxLayout>
#include <QWizardPage>

#include "tournament_runner/tournament.h"
#include "tournament_runner_gui/participant_kata_round_WizardPage.h"
#include "tournament_runner_gui/kata_round_participant_model.h"

namespace TournamentRunnerGUI
{

/**
  Number of elements for the next round startlist
 */
static const std::map<uint8_t, size_t> round_configuration = {
    {first_round, std::numeric_limits<size_t>::max()},
    {second_round, 8U},
    {final_round, 4U}
};


static const std::map<uint8_t, std::string> titel_map = {
    {first_round, "First Round"},
    {second_round, "Second Round"},
    {final_round, "Final Round"}
};

KataRoundOverviewFrame::KataRoundOverviewFrame (TournamentRunner::Tournament& tournament,
                                                uint8_t round,
                                                QWidget* parent) :
    QFrame{parent},
    ui_{std::make_shared<Ui::KataRoundOverviewFrame>()},
    tournament_(tournament),
    round_{round}
{
    ui_->setupUi(this);
    if (round_ == final_round)
    {
        ui_->NextRoundButton->hide();
    }
    else
    {
       ui_->FinishButton->hide();
    }

    ui_->RoundTableView->verticalHeader()->hide();
    const auto label = titel_map.at(round_);
    ui_->RoundLabel->setText(QString(tr(label.c_str())));

    const auto startlist = tournament_.get_startlist_for_next_kata_round(round_configuration.at(round_));
    startlist_.assign(std::begin(startlist), std::end(startlist));

    ui_->RoundTableView->setModel(new KataRoundParticipantModel{round_, tournament_, startlist_, this});

    ui_->RoundTableView->setWordWrap(true);
    ui_->RoundTableView->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
}

void KataRoundOverviewFrame::wizard_finished_slot (int result)
{
    ui_->RoundTableView->setModel(new KataRoundParticipantModel{round_, tournament_, startlist_, this});
    if (result)
    {
        ui_->StartTournamentButton->hide();
        if (round_ == final_round)
        {
            ui_->FinishButton->setEnabled(true);
        }
        else
        {
            ui_->NextRoundButton->setEnabled(true);
        }
    }
}

void KataRoundOverviewFrame::on_StartTournamentButton_clicked ()
{
    const auto wizard = new QWizard{this};
    wizard->addPage(create_start_tournament_intro_page());
    wizard->setWindowTitle(tr(titel_map.at(tournament_.get_current_round()).c_str()));
    for (const auto& startnumber : startlist_)
    {
        auto& participant = tournament_.get_participant(startnumber);
        wizard->addPage(create_participant_kata_run_page(participant));
    }

    connect(wizard, SIGNAL(finished(int)), this, SLOT(wizard_finished_slot(int)));

    wizard->show();
}

void KataRoundOverviewFrame::on_NextRoundButton_clicked()
{
    emit next_kata_tournament(tournament_.get_current_round()+1);
}

void KataRoundOverviewFrame::on_FinishButton_clicked()
{
    emit tournament_finished();
}

QWizardPage* KataRoundOverviewFrame::create_start_tournament_intro_page ()
{
    auto page = new QWizardPage{};
    const auto round = tournament_.get_current_round();
    page->setTitle(tr(titel_map.at(round).c_str()));

    std::string lower_case_titel{titel_map.at(round)};
    boost::algorithm::to_lower(lower_case_titel);

    const std::string label_text{"This wizard will guide you through the "+ lower_case_titel};
    QLabel* label = new QLabel{tr(label_text.c_str())};
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout{};
    layout->addWidget(label);
    page->setLayout(layout);

    return page;
}

QWizardPage* KataRoundOverviewFrame::create_participant_kata_run_page (TournamentRunner::Karateka& participant)
{
    QWizardPage* page = new ParticipantKataRoundWizardPage{participant};

    return page;
}

} // namespace TournamentRunnerGUI
