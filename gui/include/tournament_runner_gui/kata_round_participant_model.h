#ifndef TOURNAMENT_RUNNER_GUI_KATA_ROUND_PARTICIPANT_MODEL_H
#define TOURNAMENT_RUNNER_GUI_KATA_ROUND_PARTICIPANT_MODEL_H

#include <memory>
#include <vector>
#include <cstdint>

#include <QtCore/QAbstractTableModel>

namespace TournamentRunner
{
class Tournament;
}

namespace TournamentRunnerGUI
{

/**
 * @ingroup GUI
 *
 * @brief The KataRoundParticipantModel class
 * @details The model for the participant table view of each round
 */
class KataRoundParticipantModel :
    public QAbstractTableModel
{
    Q_OBJECT

public:

    /**
     * @brief KataRoundParticipantModel
     * @param kata_round The round the model is for.
     * Depending on this argument participants are visible or not.
     * @param tournament Reference to the Tournament
     * @param startlist The startlist for the round.
     * @param parent Parent Widget
     */
    KataRoundParticipantModel (const uint8_t kata_round,
                               TournamentRunner::Tournament& tournament,
                               std::vector<size_t> startlist,
                               QObject* parent = 0);
    /**
     * @copydoc QAbstractTableModel::rowCount
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
    const uint8_t round_{};
    TournamentRunner::Tournament& tournament_;
    std::vector<size_t> startlist_;

    enum participant_model_column_indices
    {
        index_of_name = 0,
        index_of_score_1,
        index_of_score_2,
        index_of_score_3,
        index_of_score_4,
        index_of_score_5,
        index_of_deduction,
        index_of_overall_score,
        index_last
    };

};

} // namespace TournamentRunnerGUI

#endif // TOURNAMENT_RUNNER_GUI_KATA_ROUND_PARTICIPANT_MODEL_H
