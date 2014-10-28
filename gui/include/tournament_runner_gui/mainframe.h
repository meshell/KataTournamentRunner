#ifndef TOURNAMENT_RUNNER_GUI_MAINFRAME_H
#define TOURNAMENT_RUNNER_GUI_MAINFRAME_H

#include <memory>

#include <QFrame>

#include "tournament_runner/tournament.h"
#include "tournament_runner/karateka.h"

class QWizardPage;

namespace Ui
{
class MainFrame;
}

namespace TournamentRunnerGUI
{

/**
 * @ingroup GUI
 *
 * @brief The MainFrame class shows the tournament overview before the any round is executed.
 */
class MainFrame:
    public QFrame
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     * @param tournament Reference to the tournament
     * @param parent Parent Widget
     */
    explicit MainFrame(TournamentRunner::Tournament& tournament,
                       QWidget* parent = 0);

public slots:
    /**
     * @brief Slot to register the participant to the tournament
     * @param participant The karateka to add to the tournament
     */
    void register_participant_slot (TournamentRunner::Karateka participant);

    /**
     * @brief Slot to update the tournament data
     * @param tournament_data The new data of the tournament
     */
    void update_tournament_slot (TournamentRunner::TournamentData tournament_data);

signals:
    /**
     * @brief Start tournament signal
     * @details Used to start the kata tournament
     */
    void start_kata_tournament (uint8_t round);

private slots:
    /**
    * @brief Slot to open program's add participant dialog
    */
    void on_AddParticipantButton_clicked ();

    /**
    * @brief Slot to open the tournament edit dialog
    */
    void on_EditTournamentButton_clicked ();

    /**
    * @brief Slot to start the tournament (switch to first Kata Round Overview page)
    */
    void on_StartTournamentButton_clicked ();

private:
    std::shared_ptr<Ui::MainFrame> ui_{};
    TournamentRunner::Tournament& tournament_;


};

} // namespace TournamentRunnerGUI

#endif // TOURNAMENT_RUNNER_GUI_MAINFRAME_H
