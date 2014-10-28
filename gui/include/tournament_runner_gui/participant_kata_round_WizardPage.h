#ifndef TOURNAMENT_RUNNER_GUI_PARTICIPANT_KATA_ROUND_WIZARDPAGE_H
#define TOURNAMENT_RUNNER_GUI_PARTICIPANT_KATA_ROUND_WIZARDPAGE_H

#include <QWizardPage>

namespace TournamentRunner
{
class Karateka;
}

namespace TournamentRunnerGUI
{

/**
 * @ingroup GUI
 *
 * @brief The ParticipantKataRoundWizardPage class
 * @details A Wizard page to enter the kata scores of a participant
 */
class ParticipantKataRoundWizardPage:
    public QWizardPage
{
public:
    /**
     * @brief Default Constructor
     * @param participant Reference to the participant to manipulate with the wizard (add scores)
     * @param parent Parent Widget
     */
    explicit ParticipantKataRoundWizardPage(TournamentRunner::Karateka& participant,
                                            QWidget* parent = 0);

    /**
     * @copydoc QWizardPage::validatePage()
     * @details Updates the scores on the participant.
     * @return true
     */
    bool validatePage () final;

private:
    TournamentRunner::Karateka& participant_;
};

} //namespace TournamentRunnerGUI

#endif // TOURNAMENT_RUNNER_GUI_PARTICIPANT_KATA_ROUND_WIZARDPAGE_H
