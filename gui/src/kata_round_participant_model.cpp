#include "tournament_runner_gui/kata_round_participant_model.h"

#include <QStandardItem>

#include "tournament_runner/tournament.h"
#include "tournament_runner/karateka.h"

namespace TournamentRunnerGUI
{

using ::TournamentRunner::Tournament;

KataRoundParticipantModel::KataRoundParticipantModel (const uint8_t kata_round,
                                                      TournamentRunner::Tournament& tournament,
                                                      std::vector<size_t> startlist,
                                                      QObject* parent) :
    QAbstractTableModel{parent},
    round_{kata_round},
    tournament_(tournament),
    startlist_{startlist}
{
}


int KataRoundParticipantModel::rowCount (const QModelIndex&) const
{
  return startlist_.size();
}

int KataRoundParticipantModel::columnCount (const QModelIndex&) const
{
    return index_last;
}

QVariant KataRoundParticipantModel::data (const QModelIndex& index,
                                          int role) const
{
    if (!index.isValid() ||
        (tournament_.get_current_round() < round_) ||
        (index.row() >= static_cast<int>(tournament_.number_of_participants())) ||
        (index.row() < 0))
    {
        return QVariant{};
    }

    const auto startnumber = startlist_.at(index.row());
    const auto& participant = tournament_.get_participant(startnumber);

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
            case index_of_name:
            {
                const auto name = participant.name() +
                                  ", " +
                                  participant.surname() +
                                  " (" +
                                  participant.dojo() +
                                  ")";
                return QString::fromStdString(name);
            }
            case index_of_score_1:
            case index_of_score_2:
            case index_of_score_3:
            case index_of_score_4:
            case index_of_score_5:
            {
                const auto scores = participant.get_scores(round_);
                if (scores.size() == 5)
                {
                    return scores.at(index.column()-index_of_score_1);
                }
            }
            case index_of_deduction:
            {
                return participant.get_deduction(round_);
            }
            case index_of_overall_score:
            {
                return participant.get_overall_score_of_round(round_);
            }
            default:
            {

            }
        }
    }
    return QVariant();
}

QVariant KataRoundParticipantModel::headerData (int section,
                                                Qt::Orientation,
                                                int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (section)
        {
            case index_of_name:
                return tr("Name (Dojo)");
            case index_of_score_1:
                return tr("score 1");
            case index_of_score_2:
                return tr("score 2");
            case index_of_score_3:
                return tr("score 3");
            case index_of_score_4:
                return tr("score 4");
            case index_of_score_5:
                return tr("score 5");
            case index_of_deduction:
                return tr("deduction");
            case index_of_overall_score:
                return tr("overall");
            default:
                return QVariant{};
        }
    }
    return QVariant{};
}


} // namespace TournamentRunnerGUI
