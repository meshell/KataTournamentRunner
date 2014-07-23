#ifndef PERSISTENCY_MOCK_H_
#define PERSISTENCY_MOCK_H_

#include <gmock/gmock.h>
#include "tournament_runner/persistency_interface.h"
#include "tournament_runner/tournament.h"

namespace
{

/**
 * @brief The Persistency Interface
 */
class PersistencyMock:
    public TournamentRunner::IPersistency
{
public:
    MOCK_METHOD2(save_profile, void (const TournamentRunner::Tournament& tournament_to_save,
                                     const std::string& to_file));

    MOCK_METHOD1(save_profile, void (const TournamentRunner::Tournament& tournament_to_save));

    MOCK_METHOD1(load_profile, TournamentRunner::Tournament (const std::string& from_file));

    MOCK_METHOD0(load_profile, TournamentRunner::Tournament ());
};

} // namespace

#endif // PERSISTENCY_MOCK_H_
