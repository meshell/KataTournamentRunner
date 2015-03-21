#include "tournament_runner_gui/participant_ranking_model.h"

#include <map>
#include <QStandardItem>

#include "tournament_runner/tournament.h"

namespace TournamentRunnerGUI
{

using ::TournamentRunner::Karateka;
using ::TournamentRunner::Tournament;

ParticipantRankingModel::ParticipantRankingModel (Tournament& tournament,
                                                  QObject* parent) :
    QAbstractTableModel{parent}
{
    annotated_ranked_list_ = tournament.get_anotated_ranked_list_of_participants();
}


int ParticipantRankingModel::rowCount (const QModelIndex&) const
{
    return static_cast<int>(annotated_ranked_list_.size());
}

int ParticipantRankingModel::columnCount (const QModelIndex&) const
{
    return index_last;
}

QVariant ParticipantRankingModel::data (const QModelIndex& index,
                                        int role) const
{
    if (!index.isValid() ||
         index.row() >= static_cast<int>(annotated_ranked_list_.size()) ||
         index.row() < 0)
    {
        return {};
    }

    const auto participant_idx = static_cast<size_t>(index.row());
    Karateka participant = std::get<Tournament::rank_annotated_participant_karateka_idx>(annotated_ranked_list_.at(participant_idx));

    const auto first_round = 0;
    const auto second_round = 1;
    const auto third_round = 2;

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
            case index_of_rank:
            {
                return QVariant::fromValue(std::get<Tournament::rank_annotated_participant_rank_idx>(annotated_ranked_list_.at(participant_idx)));
            }
            case index_of_name:
            {
                const auto name = participant.name() +
                                  ",\n" +
                                  participant.surname() +
                                  "\n(" +
                                  participant.dojo() +
                                  ")";
                return QString::fromStdString(name);
            }
            case index_overall_score_1:
            {
                return participant.get_overall_score_of_round(first_round);
            }
            case index_min_score_1:
            {
                return participant.get_minimum_score_of_round(first_round);
            }
            case index_max_score_1:
            {
                return participant.get_maximum_score_of_round(first_round);
            }
            case index_overall_score_2:
            {
                return participant.get_overall_score_of_round(second_round);
            }
            case index_min_score_2:
            {
                return participant.get_minimum_score_of_round(second_round);
            }
            case index_max_score_2:
            {
                return participant.get_maximum_score_of_round(second_round);
            }
            case index_overall_score_3:
            {
                return participant.get_overall_score_of_round(third_round);
            }
            case index_min_score_3:
            {
                return participant.get_minimum_score_of_round(third_round);
            }
            case index_max_score_3:
            {
                return participant.get_maximum_score_of_round(third_round);
            }
            default:
            {
            }
        }
    }
    return {};
}

QVariant ParticipantRankingModel::headerData (int section,
                                       Qt::Orientation orientation,
                                       int role) const
{
    const std::map<int, std::string> header_data =
    {
        {index_of_rank, "Rank"},
        {index_of_name, "Name,\nSurname\n(Dojo)"},
        {index_overall_score_1, "1th round\noverall"},
        {index_min_score_1, "1th round\nmin"},
        {index_max_score_1, "1th round\nmax"},
        {index_overall_score_2, "2nd round\noverall"},
        {index_min_score_2, "2nd round\nmin"},
        {index_max_score_2, "2nd round\nmax"},
        {index_overall_score_3, "3rd round\noverall"},
        {index_min_score_3, "3rd round\nmin"},
        {index_max_score_3, "3rd round\nmax"}
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
