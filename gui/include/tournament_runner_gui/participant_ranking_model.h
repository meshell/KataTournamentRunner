#ifndef TOURNAMENT_RUNNER_GUI_PARTICIPANT_RANKING_MODEL_H
#define TOURNAMENT_RUNNER_GUI_PARTICIPANT_RANKING_MODEL_H

#include <vector>
#include <cstdint>
#include <QtCore/QAbstractTableModel>

#include "tournament_runner/karateka.h"
#include "tournament_runner/tournament.h"

using TournamentRunner::Tournament;

namespace TournamentRunnerGUI
{

/**
 * @ingroup GUI
 *
 * @brief The ParticipantRankingModel
 * @details The model for the ranked list of participants
 */
class ParticipantRankingModel :
    public QAbstractTableModel
{
    Q_OBJECT

public:

    /**
     * @brief ParticipantRankingModel
     * @param tournament Reference to the tournament
     * @param parent Parent Widget
     */
    explicit ParticipantRankingModel (TournamentRunner::Tournament& tournament,
                                      QObject* parent = 0);
    /**
     * @copydoc QAbstractTableModel::rowCount
     * @brief Return the number of registred participant
     */
    int rowCount (const QModelIndex& parent = QModelIndex()) const final;

    /**
     * @copydoc QAbstractTableModel::columnCount
    */
    int columnCount (const QModelIndex& parent = QModelIndex()) const final;

    /**
     * @copydoc QAbstractTableModel::data
    */
    QVariant data (const QModelIndex& index,
                   int role = Qt::DisplayRole) const final;

    /**
     * @copydoc QAbstractTableModel::headerData
    */
    QVariant headerData (int section,
                         Qt::Orientation orientation,
                         int role = Qt::DisplayRole ) const  final;

private:
    enum participant_model_column_indices
    {
        index_of_rank = 0,
        index_of_name,
        index_overall_score_1,
        index_min_score_1,
        index_max_score_1,
        index_overall_score_2,
        index_min_score_2,
        index_max_score_2,
        index_overall_score_3,
        index_min_score_3,
        index_max_score_3,
        index_last
    };

    std::vector<Tournament::RankAnnotatedParticipant> annotated_ranked_list_;
};

} // namespace TournamentRunnerGUI

#endif // TOURNAMENT_RUNNER_GUI_PARTICIPANT_RANKING_MODEL_H
