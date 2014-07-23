#ifndef TOURNAMENT_RUNNER_GUI_KATAROUND_OVERVIEW_FRAME_H
#define TOURNAMENT_RUNNER_GUI_KATAROUND_OVERVIEW_FRAME_H

#include <memory>
#include <vector>

#include <QFrame>

class QWizardPage;

namespace Ui
{
class KataRoundOverviewFrame;
}

namespace TournamentRunner
{
class Tournament;
class Karateka;
}

namespace TournamentRunnerGUI
{

enum Kataround
{
    first_round = 0,
    second_round,
    final_round
};

class KataRoundOverviewFrame:
    public QFrame
{
    Q_OBJECT

public:
    KataRoundOverviewFrame(TournamentRunner::Tournament& tournament,
                           uint8_t round,
                           QWidget* parent = 0);
public slots:
    void wizard_finished_slot (int result);

signals:
    /**
     * @brief Start tournament signal
     * @details Used to start the kata tournament
     */
    void next_kata_tournament (uint8_t round);
    void tournament_finished ();

private slots:
    void on_StartTournamentButton_clicked ();
    void on_NextRoundButton_clicked ();
    void on_FinishButton_clicked ();

private:
    QWizardPage* create_start_tournament_intro_page ();
    QWizardPage* create_participant_kata_run_page (TournamentRunner::Karateka& participant);

    std::shared_ptr<Ui::KataRoundOverviewFrame> ui_{};
    TournamentRunner::Tournament& tournament_;
    std::vector<uint32_t> startlist_;
    uint8_t round_{};

};

} // namespace TournamentRunnerGUI

#endif // TOURNAMENT_RUNNER_GUI_KATAROUND_OVERVIEW_FRAME_H
