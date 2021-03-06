#include <QDateTime>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "tournament_runner_gui/new_tournament_dialog.h"
#include "ui_new_tournament_dialog.h"

using ::TournamentRunner::TournamentData;
using ::TournamentRunner::Date;

namespace TournamentRunnerGUI
{


NewTournamentDialog::NewTournamentDialog (QWidget* parent) :
    QDialog{parent},
    ui_{std::make_shared<Ui::NewTournamentDialog>()}
{
    ui_->setupUi(this);

    connect(ui_->createButton, SIGNAL(rejected()), this, SLOT(reject()));

    ui_->TournamentDateEdit->setDate(QDate::currentDate());

    setWindowTitle(tr("Create a tournament"));
}


void NewTournamentDialog::on_createButton_accepted ()
{

    TournamentData tournament = TournamentData{}
                                .with_name(ui_->TournamentNameEdit->text().toStdString())
                                .on_date(ui_->TournamentDateEdit->text().toStdString())
                                .at_location(ui_->TournamentLocationEdit->document()->toPlainText().toStdString());

    emit new_tournament(tournament);
}

} // namespace TournamentRunnerGUI
