#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <QApplication>
#include <QLineEdit>

#include "tournament_runner/karateka.h"
#include "tournament_runner_gui/participant_kata_round_WizardPage.h"

namespace
{

using ::TournamentRunner::Karateka;
using ::TournamentRunnerGUI::ParticipantKataRoundWizardPage;
using ::testing::FloatNear;

const auto float_compare_tolerance = 0.01f;

	
TEST(The_ParticipantKataRoundWizardPage, sets_the_kata_scores_upon_validatePage)
{
    Karateka karateka;

    auto testee = new ParticipantKataRoundWizardPage(karateka);

    QWizard wizard;
    wizard.addPage(testee);

    auto editfields = testee->findChildren<QLineEdit*>();

    for (const auto& editfield : editfields)
    {
        editfield->setText(QString::fromStdString("5.1"));
    }

    const auto no_of_fields = editfields.size();
    editfields.at(no_of_fields-1)->setText(QString::fromStdString("0.5"));

    ASSERT_TRUE(testee->validatePage());

    const auto first_round = 0;

    auto first_round_scores = karateka.get_scores(first_round);
    ASSERT_EQ(6, no_of_fields);
    ASSERT_THAT(first_round_scores.at(0), FloatNear(5.1f, float_compare_tolerance));
    ASSERT_THAT(first_round_scores.at(1), FloatNear(5.1f, float_compare_tolerance));
    ASSERT_THAT(first_round_scores.at(2), FloatNear(5.1f, float_compare_tolerance));
    ASSERT_THAT(first_round_scores.at(3), FloatNear(5.1f, float_compare_tolerance));
    ASSERT_THAT(first_round_scores.at(4), FloatNear(5.1f, float_compare_tolerance));

    ASSERT_THAT(karateka.get_deduction(first_round), FloatNear(0.5f, float_compare_tolerance));
}	

TEST(The_ParticipantKataRoundWizardPage, sets_the_overall_kata_score_upon_validatePage)
{
    Karateka karateka;

    auto testee = new ParticipantKataRoundWizardPage(karateka);

    QWizard wizard;
    wizard.addPage(testee);

    auto editfields = testee->findChildren<QLineEdit*>();

    for (const auto& editfield : editfields)
    {
        editfield->setText(QString::fromStdString("5.1"));
    }

    const auto no_of_fields = editfields.size();
    editfields.at(no_of_fields-1)->setText(QString::fromStdString("0.5"));

    ASSERT_TRUE(testee->validatePage());

    const auto first_round = 0;

    auto first_round_overall_score = karateka.get_overall_score_of_round(first_round);
    ASSERT_THAT(first_round_overall_score, FloatNear((3.0f*5.1f)-0.5f, float_compare_tolerance));
}
	
} // namespace
