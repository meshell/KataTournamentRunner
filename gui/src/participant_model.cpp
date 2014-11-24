#include "tournament_runner_gui/participant_model.h"

#include <QStandardItem>
#include <QPushButton>

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
        return {};
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
    return {};
}


QVariant ParticipantModel::headerData (int section,
                                       Qt::Orientation orientation,
                                       int role) const
{
    const std::map<size_t, std::string> header_data =
    {
        {index_of_name, "Name"},
        {index_of_surname, "Surname"},
        {index_of_date_of_birth, "Date of birth"},
        {index_of_dojo, "Dojo"},
        {index_of_rank, "Rank"}
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

bool ParticipantModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row)
    {
        tournament_.remove_participant(position+row);
    }
    endRemoveRows();
    emit layoutChanged();
    return true;
}

Qt::ItemFlags ParticipantModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::ItemIsEnabled;
    }
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

} // namespace TournamentRunnerGUI
