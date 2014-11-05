#ifndef TOURNAMENT_RUNNER_GUI_EDIT_TOURNAMENT_DIALOG_H
#define TOURNAMENT_RUNNER_GUI_EDIT_TOURNAMENT_DIALOG_H

#include <memory>

#include <QtWidgets/QDialog>

#include "tournament_runner/tournament.h"

namespace Ui
{
class EditTournamentDialog;
}

namespace TournamentRunnerGUI
{

/**
 * @ingroup GUI
 *
 * @brief The NewTournamentDialog class
 * @details Dialog to change the tournament data
 */
class EditTournamentDialog :
    public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief default constructor
     * @param parent
     */
    /**
     * @brief Default Constructor
     * @param tournament Reference to the Tournament
     * @param parent Parent Widget
     */
    explicit EditTournamentDialog (TournamentRunner::Tournament& tournament,
                                   QWidget* parent = 0);

signals:
    /**
     * @brief Edit tournament signal
     * @details Used to edit the tournament
     */
    void update_tournament (TournamentRunner::TournamentData tournament);

private slots:
    /**
     * @brief Update Tournament slot
     */
    void on_updateButton_accepted ();

private:
    std::shared_ptr<Ui::EditTournamentDialog> ui_{};
};

} // // namespace TournamentRunnerGUI

#endif // TOURNAMENT_RUNNER_GUI_EDIT_TOURNAMENT_DIALOG_H
