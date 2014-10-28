#ifndef TOURNAMENT_RUNNER_GUI_FINALFRAME_H
#define TOURNAMENT_RUNNER_GUI_FINALFRAME_H

#include <memory>

#include <QFrame>

#include "tournament_runner/tournament.h"

class QWizardPage;

namespace Ui
{
class FinalFrame;
}

namespace TournamentRunnerGUI
{

/**
 * @ingroup GUI
 *
 * @brief The FinalFrame class
 * @details Shows the Tournament overview (ranked list) after the final round was executed
 */
class FinalFrame:
    public QFrame
{
    Q_OBJECT

public:
    /**
     * @brief Default Constructor
     * @param tournament Refernce to the tournament
     * @param parent Parent Widget
     */
    explicit FinalFrame(TournamentRunner::Tournament& tournament,
                        QWidget* parent = 0);
private:
    std::shared_ptr<Ui::FinalFrame> ui_{};
    TournamentRunner::Tournament& tournament_;
};

} // namespace TournamentRunnerGUI

#endif // TOURNAMENT_RUNNER_GUI_FINALFRAME_H
