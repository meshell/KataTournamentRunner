#ifndef TOURNAMENT_RUNNER_GUI_PARTICIPANT_RANKING_MODEL_H
#define TOURNAMENT_RUNNER_GUI_PARTICIPANT_RANKING_MODEL_H

#include <vector>
#include <cstdint>
#include <QtCore/QAbstractTableModel>

#include "tournament_runner/karateka.h"

namespace TournamentRunner
{
class Tournament;
}

namespace TournamentRunnerGUI
{

/**
 * @ingroup GUI
 *
 * @brief The ParticipantModel class
 * @details The model for the participant table view
 */
class ParticipantRankingModel :
    public QAbstractTableModel
{
    Q_OBJECT

public:

    /**
     * @brief ParticipantRankingModel
     * @param tournament Reference to the tournament registrator
     * @param parent Parent widget
     */
    explicit ParticipantRankingModel (TournamentRunner::Tournament& tournament,
                                      QObject* parent = 0);
    /**
     * @brief Return the number of registred tournaments
     */
    int rowCount (const QModelIndex& parent = QModelIndex()) const final;

    int columnCount (const QModelIndex& parent = QModelIndex()) const final;

    QVariant data (const QModelIndex& index,
                   int role = Qt::DisplayRole) const final;

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

    std::vector<std::pair<size_t, TournamentRunner::Karateka>> annotated_ranked_list_;
};

} // namespace TournamentRunnerGUI

#endif // TOURNAMENT_RUNNER_GUI_PARTICIPANT_RANKING_MODEL_H
