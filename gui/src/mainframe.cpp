#include "tournament_runner_gui/mainframe.h"
#include "ui_mainframe.h"

#include <QLabel>

#include "tournament_runner_gui/add_participant_dialog.h"
#include "tournament_runner_gui/participant_model.h"
#include "tournament_runner_gui/edit_tournament_dialog.h"

namespace TournamentRunnerGUI
{

MainFrame::MainFrame (TournamentRunner::Tournament& tournament,
                      QWidget* parent) :
    QFrame{parent},
    ui_{std::make_shared<Ui::MainFrame>()},
    tournament_(tournament)
{
    ui_->setupUi(this);
    ui_->TournamentName->setText(QString::fromStdString(tournament_.name()));
    ui_->TournamentDate->setText(QString::fromStdString(tournament_.date_as_string()));
    ui_->TournamentLocation->setText(QString::fromStdString(tournament_.location()));

    ui_->ParticipantTableView->verticalHeader()->hide();

    ui_->ParticipantTableView->setModel(new ParticipantModel(tournament_, this));

    ui_->ParticipantTableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    ui_->ParticipantTableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    if (tournament_.get_current_round() != 0)
    {
        ui_->StartTournamentButton->hide();
    }
}

void MainFrame::on_AddParticipantButton_clicked ()
{
    auto* add_participant_dialog = new AddParticipantDialog(this);
    connect(add_participant_dialog, SIGNAL(register_participant(TournamentRunner::Karateka)),
            this, SLOT(register_participant_slot(TournamentRunner::Karateka)));
    add_participant_dialog->show();
}

void MainFrame::on_EditTournamentButton_clicked ()
{
    auto* edit_tournament_dialog = new EditTournamentDialog(tournament_, this);
    connect(edit_tournament_dialog, SIGNAL(update_tournament(TournamentRunner::TournamentData)),
            this, SLOT(update_tournament_slot(TournamentRunner::TournamentData)));
    edit_tournament_dialog->show();
}

void MainFrame::register_participant_slot (TournamentRunner::Karateka participant)
{
    tournament_.add_participant(participant);
    ui_->ParticipantTableView->setModel(new ParticipantModel(tournament_, this));
}

void MainFrame::update_tournament_slot (TournamentRunner::TournamentData tournament_data)
{
    tournament_.update_data(tournament_data);
    ui_->TournamentName->setText(QString::fromStdString(tournament_.name()));
    ui_->TournamentDate->setText(QString::fromStdString(tournament_.date_as_string()));
    ui_->TournamentLocation->setText(QString::fromStdString(tournament_.location()));
}

void MainFrame::on_StartTournamentButton_clicked ()
{
    emit start_kata_tournament(0);
}

} // namespace TournamentRunnerGUI
