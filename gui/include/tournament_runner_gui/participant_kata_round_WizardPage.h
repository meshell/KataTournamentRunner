#ifndef TOURNAMENT_RUNNER_GUI_PARTICIPANT_KATA_ROUND_WIZARDPAGE_H
#define TOURNAMENT_RUNNER_GUI_PARTICIPANT_KATA_ROUND_WIZARDPAGE_H

#include <QWizardPage>

namespace TournamentRunner
{
class Karateka;
}

namespace TournamentRunnerGUI
{

class ParticipantKataRoundWizardPage:
    public QWizardPage
{
public:
    explicit ParticipantKataRoundWizardPage(TournamentRunner::Karateka& participant,
                                            QWidget* parent = 0);

    bool validatePage () final;

private:
    TournamentRunner::Karateka& participant_;
};

} //namespace TournamentRunnerGUI

#endif // TOURNAMENT_RUNNER_GUI_PARTICIPANT_KATA_ROUND_WIZARDPAGE_H
