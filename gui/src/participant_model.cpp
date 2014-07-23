#include "tournament_runner_gui/participant_model.h"

#include <QStandardItem>

#include "tournament_runner/tournament.h"
#include "tournament_runner/karateka.h"

namespace TournamentRunnerGUI
{

using ::TournamentRunner::Tournament;

ParticipantModel::ParticipantModel (TournamentRunner::Tournament& tournament,
                                    QObject* parent) :
    QAbstractTableModel{parent},
    tournament_(tournament)
{
}


int ParticipantModel::rowCount (const QModelIndex&) const
{
    return tournament_.number_of_participants();
}

int ParticipantModel::columnCount (const QModelIndex&) const
{
    return index_last;
}

QVariant ParticipantModel::data (const QModelIndex& index,
                                 int role) const
{
    if (!index.isValid() ||
        index.row() >= static_cast<int>(tournament_.number_of_participants()) ||
        index.row() < 0)
    {
        return QVariant{};
    }

    auto participant = tournament_.get_participant(index.row());

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
            case index_of_name:
            {
                return QString::fromStdString(participant.name());
            }
            case index_of_surname:
            {
                return QString::fromStdString(participant.surname());
            }
            case index_of_date_of_birth:
            {
                return QString::fromStdString(participant.date_of_birth_as_string());
            }
            case index_of_dojo:
            {
                return QString::fromStdString(participant.dojo());
            }
            case index_of_rank:
            {
                return QString::fromStdString(participant.rank());
            }
            default:
            {
            }
        }
    }
    return QVariant();
}

QVariant ParticipantModel::headerData (int section,
                                       Qt::Orientation,
                                       int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (section)
        {
            case index_of_name:
            {
                return tr("Name");
            }
            case index_of_surname:
            {
                return tr("Surname");
            }
            case index_of_date_of_birth:
            {
                return tr("Date of birth");
            }
            case index_of_dojo:
            {
                return tr("Dojo");
            }
            case index_of_rank:
            {
                return tr("Rank");
            }
            default:
            {

            }
        }
    }
    return QVariant{};
}


} // namespace TournamentRunnerGUI
