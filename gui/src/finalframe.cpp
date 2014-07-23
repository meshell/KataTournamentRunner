#include "tournament_runner_gui/finalframe.h"
#include "ui_finalframe.h"

#include <QLabel>

#include "tournament_runner_gui/participant_ranking_model.h"

namespace TournamentRunnerGUI
{

FinalFrame::FinalFrame (TournamentRunner::Tournament& tournament,
                      QWidget* parent) :
    QFrame{parent},
    ui_{std::make_shared<Ui::FinalFrame>()},
    tournament_(tournament)
{
    ui_->setupUi(this);
    ui_->TournamentName->setText(QString::fromStdString(tournament_.name()));
    ui_->TournamentDate->setText(QString::fromStdString(tournament_.date_as_string()));
    ui_->TournamentLocation->setText(QString::fromStdString(tournament_.location()));

    ui_->ParticipantTableView->verticalHeader()->hide();

    ui_->ParticipantTableView->setModel(new ParticipantRankingModel(tournament_, this));

    ui_->ParticipantTableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    ui_->ParticipantTableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
}


} // namespace TournamentRunnerGUI
