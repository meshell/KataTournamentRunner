#include "tournament_runner_gui/participant_kata_round_WizardPage.h"

#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDoubleValidator>

#include "tournament_runner/karateka.h"

namespace TournamentRunnerGUI
{

ParticipantKataRoundWizardPage::ParticipantKataRoundWizardPage(TournamentRunner::Karateka& participant,
                                                               QWidget* parent /* = 0 */) :
    QWizardPage(parent),
    participant_(participant)
{
    const auto name = participant_.name() +
                      ", " +
                      participant_.surname() +
                      " ("+
                      participant_.dojo() +
                      ")";
    setTitle(QString::fromStdString(name));
    setSubTitle(tr("Please fill all scores."));

    // TODO make this configurable
    const auto bottom_score = 6.0F;
    const auto top_score = 8.0F;
    const int decimals = 1;
    auto* score_validator = new QDoubleValidator{bottom_score, top_score, decimals};
    score_validator->setNotation(QDoubleValidator::StandardNotation);

    QLabel* score_1_Label = new QLabel("Score 1:");
    QLineEdit* score_1_LineEdit = new QLineEdit;
    score_1_LineEdit->setValidator(score_validator);
    score_1_Label->setBuddy(score_1_LineEdit);

    QLabel* score_2_Label = new QLabel("Score 2:");
    QLineEdit* score_2_LineEdit = new QLineEdit;
    score_2_LineEdit->setValidator(score_validator);
    score_2_Label->setBuddy(score_2_LineEdit);

    QLabel* score_3_Label = new QLabel("Score 3:");
    QLineEdit* score_3_LineEdit = new QLineEdit;
    score_3_LineEdit->setValidator(score_validator);
    score_3_Label->setBuddy(score_3_LineEdit);

    QLabel* score_4_Label = new QLabel("Score 4:");
    QLineEdit* score_4_LineEdit = new QLineEdit;
    score_4_LineEdit->setValidator(score_validator);
    score_4_Label->setBuddy(score_4_LineEdit);

    QLabel* score_5_Label = new QLabel("Score 5:");
    QLineEdit* score_5_LineEdit = new QLineEdit;
    score_5_LineEdit->setValidator(score_validator);
    score_5_Label->setBuddy(score_5_LineEdit);

    QLabel* deduction_Label = new QLabel("Overall Deduction:");
    QLineEdit* deduction_LineEdit = new QLineEdit;
    deduction_LineEdit->setValidator(score_validator);
    deduction_Label->setBuddy(deduction_LineEdit);

    const auto no = QString::fromStdString(std::to_string(participant_.get_startnumber()));
    registerField(no+"-score1*", score_1_LineEdit);
    registerField(no+"-score2*", score_2_LineEdit);
    registerField(no+"-score3*", score_3_LineEdit);
    registerField(no+"-score4*", score_4_LineEdit);
    registerField(no+"-score5*", score_5_LineEdit);
    registerField(no+"-deduction", deduction_LineEdit);


    auto* scores_layout = new QHBoxLayout();
    scores_layout->addWidget(score_1_Label);
    scores_layout->addWidget(score_1_LineEdit);
    scores_layout->addWidget(score_2_Label);
    scores_layout->addWidget(score_2_LineEdit);
    scores_layout->addWidget(score_3_Label);
    scores_layout->addWidget(score_3_LineEdit);
    scores_layout->addWidget(score_4_Label);
    scores_layout->addWidget(score_4_LineEdit);
    scores_layout->addWidget(score_5_Label);
    scores_layout->addWidget(score_5_LineEdit);

    auto* deduction_layout = new QHBoxLayout();
    deduction_layout->addWidget(deduction_Label);
    deduction_layout->addWidget(deduction_LineEdit);
    deduction_layout->addStretch();

    auto* main_layout = new QVBoxLayout();
    main_layout->addLayout(scores_layout);
    main_layout->addLayout(deduction_layout);
    setLayout(main_layout);
    setCommitPage(true);
}

bool ParticipantKataRoundWizardPage::validatePage ()
{
    const auto no = QString::fromStdString(std::to_string(participant_.get_startnumber()));
    const auto score1 = field(no+"-score1").toFloat();
    participant_.add_kata_score(score1);
    const auto score2 = field(no+"-score2").toFloat();
    participant_.add_kata_score(score2);
    const auto score3 = field(no+"-score3").toFloat();
    participant_.add_kata_score(score3);
    const auto score4 = field(no+"-score4").toFloat();
    participant_.add_kata_score(score4);
    const auto score5 = field(no+"-score5").toFloat();
    participant_.add_kata_score(score5);
    const auto deduction = field(no+"-deduction").toFloat();
    participant_.add_deduction(deduction);
    return true;
}

} // namespace TournamentRunnerGUI
