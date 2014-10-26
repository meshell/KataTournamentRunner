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
 * @brief The ParticipantModel class
 * @details The model for the participant table view
 */
class KataRoundParticipantModel :
    public QAbstractTableModel
{
    Q_OBJECT

public:

    /**
     * @brief KataRoundParticipantModel
     * @param kata_round
     * @param tournament
     * @param parent
     */
    KataRoundParticipantModel (const uint8_t kata_round,
                               TournamentRunner::Tournament& tournament,
                               std::vector<size_t> startlist,
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
