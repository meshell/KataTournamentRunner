#ifndef TOURNAMENT_RUNNER_GUI_ADD_PARTICIPANT_DIALOG_H
#define TOURNAMENT_RUNNER_GUI_ADD_PARTICIPANT_DIALOG_H

#include <memory>

#include <QtWidgets/QDialog>

#include "tournament_runner/karateka.h"

namespace Ui
{
class AddParticipantDialog;
}

namespace TournamentRunnerGUI
{

/**
 * @ingroup GUI
 *
 * @brief The AddParticipantDialog class
 * @details Dialog to enter tournament participant data
 */
class AddParticipantDialog :
    public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief default constructor
     * @param parent
     */
    explicit AddParticipantDialog (QWidget* parent = 0);

signals:
    /**
     * @brief Register participant signal
     * @details Used to add a participant to the tournament
     */
    void register_participant (TournamentRunner::Karateka);

private slots:
    /**
     * @brief Add Participant slot
     */
    void on_addButton_accepted ();

private:
    std::shared_ptr<Ui::AddParticipantDialog> ui_{};
};

} // namespace TournamentRunnerGUI

#endif // TOURNAMENT_RUNNER_GUI_ADD_PARTICIPANT_DIALOG_H
