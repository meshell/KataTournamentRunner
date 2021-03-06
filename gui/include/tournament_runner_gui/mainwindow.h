#ifndef TOURNAMENT_RUNNER_GUI_MAINWINDOW_H
#define TOURNAMENT_RUNNER_GUI_MAINWINDOW_H

#include <memory>
#include <map>
#include <cstdint>

#include <QtWidgets/QMainWindow>
#include <QFileDialog>

#include "tournament_runner/tournament.h"

class QStackedWidget;

namespace Ui
{
class MainWindow;
}

namespace TournamentRunner
{
class IPersistency;
}

/**
 * Namespace of the Tournament Runner GUI part
 */
namespace TournamentRunnerGUI
{

/**
 * @ingroup GUI
 *
 * @brief The MainWindow class
 * @details Main window containing specific frames, e.g. MainFrame.
 */
class MainWindow :
    public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief
     * @param about_dialog
     * @param parent Parent Widget
     */

    /**
     * @brief Default Constructor
     * @param about_dialog Reference to the About Dialog which opens on request
     * @param persistency_handler Reference to the Persistency class
     * @param tournament reference to the Tournament
     * @param parent Parent Widget
     */
    MainWindow (QDialog& about_dialog,
                TournamentRunner::IPersistency& persistency_handler,
                std::shared_ptr<TournamentRunner::Tournament> tournament,
                QWidget* parent = 0);

public slots:
    /**
    * @brief Slot to open program's about dialog (Menu About)
    */
    void on_actionAbout_triggered ();

    /**
     * @brief Slot to load a tournament (Menu Open)
     */
    void on_actionOpen_triggered ();

    /**
     * @brief Slot to open an new tournament dialog
     */
    void on_actionNewTournament_triggered ();

    /**
    * @brief Slot to save to the default profile
    */
    void on_actionSave_triggered ();

    /**
    * @brief Slot to save the profile to a file
    */
    void on_actionSaveAs_triggered ();

    /**
     * @brief Slot to create a new tournament
     * @param tournament Data for the tournament to create
     */
    void create_tournament_slot (TournamentRunner::TournamentData tournament);

    /**
     * @brief Slot to start a kata round
     * @param round
     */
    void start_kata_round_slot (uint8_t round);

    /**
     * @brief Slot to advance to the next kata round overview
     * @param round
     */
    void next_kata_round_slot(uint8_t round);

    /**
     * @brief Slot to finish the tournament and show the tournament overview
     */
    void tournament_finished_slot ();

private:
    QFileDialog* create_file_dialog (QFileDialog::AcceptMode mode);
    void set_file_dialog_filter (QFileDialog* file_dialog);
    void set_central_widget ();

    QDialog& about_dialog_;
    QStackedWidget* main_widget_{};
    TournamentRunner::IPersistency& persistency_handler_;
    std::shared_ptr<Ui::MainWindow> ui_{};
    std::shared_ptr<TournamentRunner::Tournament> tournament_{};
    std::map<uint8_t, int32_t> round_frame_widget_index;
};

} // namespace TournamentRunnerGUI

#endif // TOURNAMENT_RUNNER_GUI_MAINWINDOW_H
