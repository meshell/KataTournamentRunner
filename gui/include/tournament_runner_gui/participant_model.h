#ifndef TOURNAMENT_RUNNER_GUI_PARTICIPANT_MODEL_H
#define TOURNAMENT_RUNNER_GUI_PARTICIPANT_MODEL_H

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
class ParticipantModel :
    public QAbstractTableModel
{
    Q_OBJECT

public:

    /**
     * @brief ParticipantModel
     * @param tournament Reference to the tournament registrator
     * @param parent Parent widget
     */
    explicit ParticipantModel (TournamentRunner::Tournament& tournament,
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
        index_of_name = 0,
        index_of_surname,
        index_of_date_of_birth,
        index_of_dojo,
        index_of_rank,
        index_last
    };

    TournamentRunner::Tournament& tournament_;


};

} // namespace TournamentRunnerGUI

#endif // TOURNAMENT_RUNNER_GUI_PARTICIPANT_MODEL_H
