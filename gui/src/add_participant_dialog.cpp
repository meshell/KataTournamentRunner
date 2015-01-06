#include "tournament_runner_gui/add_participant_dialog.h"
#include "ui_add_participant_dialog.h"

#include <boost/date_time/gregorian/gregorian.hpp>

#include "tournament_runner/date.h"



namespace TournamentRunnerGUI
{

using ::TournamentRunner::Karateka;
using ::TournamentRunner::Date;

AddParticipantDialog::AddParticipantDialog (QWidget* parent) :
    QDialog{parent},
    ui_{std::make_shared<Ui::AddParticipantDialog>()}
{
    ui_->setupUi(this);

    connect(ui_->addButton, SIGNAL(rejected()), this, SLOT(reject()));

    setWindowTitle(tr("Add a participant"));
}


void AddParticipantDialog::on_addButton_accepted ()
{
    const auto name = ui_->ParticipantNameEdit->text().toStdString();
    const auto surname = ui_->ParticipantSurnameEdit->text().toStdString();

    const auto year = static_cast<boost::gregorian::gregorian_calendar::year_type>(ui_->DateOfBirthEdit->date().year());
    const auto month = static_cast<boost::gregorian::gregorian_calendar::month_type>(ui_->DateOfBirthEdit->date().month());
    const auto day = static_cast<boost::gregorian::gregorian_calendar::day_type>(ui_->DateOfBirthEdit->date().day());

    auto date_of_birth = boost::gregorian::date{year, month, day};
    const auto dojo = ui_->DojoEdit->text().toStdString();
    const auto rank = ui_->RankComboBox->currentText().toStdString();

    const auto participant = Karateka{}
                                .with_name(name)
                                .with_surname(surname)
                                .with_birthdate(Date(date_of_birth))
                                .from_dojo(dojo)
                                .with_rank(rank);


    emit register_participant(participant);
}

} // namespace TournamentManagerGUI
