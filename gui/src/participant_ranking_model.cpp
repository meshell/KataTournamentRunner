#include "tournament_runner_gui/participant_ranking_model.h"

#include <QStandardItem>

#include "tournament_runner/tournament.h"

namespace TournamentRunnerGUI
{

using ::TournamentRunner::Karateka;

ParticipantRankingModel::ParticipantRankingModel (TournamentRunner::Tournament& tournament,
                                                  QObject* parent) :
    QAbstractTableModel{parent}
{
    annotated_ranked_list_ = tournament.get_anotated_ranked_list_of_participants();
}


int ParticipantRankingModel::rowCount (const QModelIndex&) const
{
    return annotated_ranked_list_.size();
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
        return QVariant{};
    }

    TournamentRunner::Karateka participant = std::get<1>(annotated_ranked_list_.at(index.row()));

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
            case index_of_rank:
            {
                return std::get<0>(annotated_ranked_list_.at(index.row()));
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
                return participant.get_overall_score_of_round(0);
            }
            case index_min_score_1:
            {
                return participant.get_minimum_score_of_round(0);
            }
            case index_max_score_1:
            {
                return participant.get_maximum_score_of_round(0);
            }
            case index_overall_score_2:
            {
                return participant.get_overall_score_of_round(1);
            }
            case index_min_score_2:
            {
                return participant.get_minimum_score_of_round(1);
            }
            case index_max_score_2:
            {
                return participant.get_maximum_score_of_round(1);
            }
            case index_overall_score_3:
            {
                return participant.get_overall_score_of_round(2);
            }
            case index_min_score_3:
            {
                return participant.get_minimum_score_of_round(2);
            }
            case index_max_score_3:
            {
                return participant.get_maximum_score_of_round(2);
            }
            default:
            {
            }
        }
    }
    return QVariant();
}

QVariant ParticipantRankingModel::headerData (int section,
                                       Qt::Orientation,
                                       int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (section)
        {
            case index_of_rank:
            {
                return tr("Rank");
            }
            case index_of_name:
            {
                return tr("Name, Surname (Dojo)");
            }
            case index_overall_score_1:
            {
                return tr("1th round\noverall");
            }
            case index_min_score_1:
            {
                return tr("1th round\nmin");
            }
            case index_max_score_1:
            {
                return tr("1th round\nmax");
            }
            case index_overall_score_2:
            {
                return tr("2nd round\noverall");
            }
            case index_min_score_2:
            {
                return tr("2nd round\nmin");
            }
            case index_max_score_2:
            {
                return tr("2nd round\nmax");
            }
            case index_overall_score_3:
            {
                return tr("3rd round\noverall");
            }
            case index_min_score_3:
            {
                return tr("3rd round\nmin");
            }
            case index_max_score_3:
            {
                return tr("3rd round\nmax");
            }
            default:
            {
            }
        }
    }
    return QVariant{};
}


} // namespace TournamentRunnerGUI
