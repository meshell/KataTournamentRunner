#include "tournament_runner_gui/edit_tournament_dialog.h"
#include "ui_edit_tournament_dialog.h"

using ::TournamentRunner::TournamentData;
using ::TournamentRunner::Date;

namespace TournamentRunnerGUI
{


EditTournamentDialog::EditTournamentDialog (TournamentRunner::Tournament& tournament,
                                            QWidget* parent) :
    QDialog{parent},
    ui_{std::make_shared<Ui::EditTournamentDialog>()}
{
    ui_->setupUi(this);

    connect(ui_->updateButton, SIGNAL(rejected()), this, SLOT(reject()));

    ui_->TournamentNameEdit->setText(QString::fromStdString(tournament.name()));
    ui_->TournamentDateEdit->setDate(QDate(tournament.date().year(),
                                           tournament.date().month(),
                                           tournament.date().day()));
    ui_->TournamentLocationEdit->setText(QString::fromStdString(tournament.location()));

    setWindowTitle(tr("Edit tournament"));
}


void EditTournamentDialog::on_updateButton_accepted ()
{
    const boost::gregorian::date date(ui_->TournamentDateEdit->date().year(),
                                      ui_->TournamentDateEdit->date().month(),
                                      ui_->TournamentDateEdit->date().day());

    const auto tournament = TournamentData{}
                                .with_name(ui_->TournamentNameEdit->text().toStdString())
                                .on_date(Date{date})
                                .at_location(ui_->TournamentLocationEdit->document()->toPlainText().toStdString());

    emit update_tournament(tournament);
}

} // namespace TournamentRunnerGUI
