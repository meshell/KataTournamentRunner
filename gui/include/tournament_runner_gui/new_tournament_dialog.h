#ifndef TOURNAMENT_RUNNER_GUI_NEW_TOURNAMENT_DIALOG_H
#define TOURNAMENT_RUNNER_GUI_NEW_TOURNAMENT_DIALOG_H

#include <memory>
#include <QtWidgets/QDialog>

#include "tournament_runner/tournament.h"

namespace Ui
{
class NewTournamentDialog;
}

namespace TournamentRunnerGUI
{

/**
 * @ingroup GUI
 *
 * @brief The NewTournamentDialog class
 * @details Dialog to enter the tournament data
 */
class NewTournamentDialog :
    public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief default constructor
     * @param parent
     */
    explicit NewTournamentDialog (QWidget* parent = 0);

signals:
    /**
     * @brief New tournament signal
     * @details Used to create a new tournament
     */
    void new_tournament (TournamentRunner::TournamentData tournament);

private slots:
    /**
     * @brief Create Tournament slot
     */
    void on_createButton_accepted ();

private:
    std::shared_ptr<Ui::NewTournamentDialog> ui_{};
};

} // // namespace TournamentRunnerGUI

#endif // TOURNAMENT_RUNNER_GUI_NEW_TOURNAMENT_DIALOG_H
