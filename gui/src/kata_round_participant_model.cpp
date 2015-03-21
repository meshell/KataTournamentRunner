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
  return static_cast<int>(startlist_.size());
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
        return {};
    }

    const auto startnumber = startlist_.at(static_cast<size_t>(index.row()));
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
                if (scores.size() == TournamentRunner::number_of_kata_scores_per_round)
                {
                    const auto score_idx = static_cast<size_t>(index.column()-index_of_score_1);
                    return scores.at(score_idx);
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
    return {};
}




QVariant KataRoundParticipantModel::headerData (int section,
                                       Qt::Orientation orientation,
                                       int role) const
{
    const std::map<int, std::string> header_data =
    {
        {index_of_name, "Name (Dojo)"},
        {index_of_score_1, "score 1"},
        {index_of_score_2, "score 2"},
        {index_of_score_3, "score 3"},
        {index_of_score_4, "score 4"},
        {index_of_score_5, "score 5"},
        {index_of_overall_score, "overall"},
        {index_of_deduction, "deduction"}
    };

    if ((role == Qt::DisplayRole) && (orientation == Qt::Horizontal))
    {
        const auto header = header_data.find(section);
        if(header != std::end(header_data))
        {
            return tr((header->second).c_str());
        }

    }
    return {};
}


} // namespace TournamentRunnerGUI
