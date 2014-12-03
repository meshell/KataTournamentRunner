#ifndef TOURNAMENT_RUNNER_TOURNAMENT_H_
#define TOURNAMENT_RUNNER_TOURNAMENT_H_

#include <string>
#include <vector>
#include <utility>
#include <cstdint>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

#include "tournament_runner/karateka.h"
#include "tournament_runner/date.h"

/**
 * Namespace of the Tournament Runner core-library
 */
namespace TournamentRunner
{

/**
 * @brief The TournamentData struct holding all tournament relevant informations.
 */
struct TournamentData
{
    /**
     * @brief Set the tournament name
     * @param name tournament name
     * @return Reference to the updated tournament data
     */
    TournamentData& with_name (const std::string& name)
    {
        name_ = name;
        return *this;
    }

    /**
     * @overload
     */
    TournamentData& with_name (std::string&& name) noexcept
    {
        name_ = std::move(name);
        return *this;
    }

    /**
     * @brief Set the tournament date
     * @param date Date the tournament is held as Date
     * @return Reference to the updated tournament data
     */
    TournamentData& on_date (const Date& date)
    {
        date_ = date;
        return *this;
    }

    /**
     * @overload
     */
    TournamentData& on_date (Date&& date) noexcept
    {
        date_ = std::move(date);
        return *this;
    }

    /**
     * @brief Set the tournament date
     * @param date_as_string Date the tournament is held
     *        as ISO 8601 extended format string (CCYY-MM-DD)
     * @return Reference to the updated tournament data
     */
    TournamentData& on_date (const std::string& date_as_string)
    {
        date_ = Date(date_as_string);
        return *this;
    }

    /**
     * @brief Set the location the tournament is held
     * @param address Location
     * @return Reference to the updated tournament data
     */
    TournamentData& at_location (const std::string& address)
    {
        location_ = address;
        return *this;
    }

    /**
     * @overload
     */
    TournamentData& at_location (std::string&& address) noexcept
    {
        location_ = std::move(address);
        return *this;
    }

    std::string name_{};
    Date date_{};
    std::string location_{};
};

/**
 * @brief The Tournament class is the starting point for the tournament management.
 * @remark Serializable
 */
class Tournament
{
public:
    /**
     * @brief Default empty Tournament constructor
     */
    Tournament () = default;

    /**
     * @brief Tournament constructor
     * @param data Tournament data as struct
     */
    explicit Tournament (TournamentData data);

    /**
     * @brief Tournament name
     * @return Tournament name
     */
    std::string name () const;

    /**
     * @brief Return the date the tournament is held as ISO 8601 extended format string (CCYY-MM-DD)
     * @return Tournament date as ISO 8601 extended format string (CCYY-MM-DD)
     */
    std::string date_as_string () const;

    /**
     * @brief Return the date the tournament is held as Date
     * @return Tournament date
     */
    Date date() const;

    /**
     * @brief Tournament location address
     * @return Tournament location address
     */
    std::string location () const;

    /**
     * @brief Update the tournament data
     * @param data New Tournament data
     */
    void update_data (const TournamentData& data);

    /**
     * @brief Register a participant to the tournament
     * @param participant
     */
    void add_participant (const Karateka& participant);

    /**
     * @brief Return the number of registred participants
     * @return The number of registred participants
     */
    size_t number_of_participants () const;

    /**
     * @brief Return the participant with the given startnumber
     * @param start_number Start number given to the karateka
     * @return Karateka with the given Start number
     */
    Karateka& get_participant (size_t start_number);

    /**
     * @brief remove_participant
     * @param start_number
     */
    void remove_participant (size_t start_number);

    /**
     * @brief Return a ranked list of participants. The highest ranked comes first.
     * @return list of the participants ranked by the scores
     */
    std::vector<Karateka> get_ranked_list_of_participants () const;

    using RankAnnotatedParticipant = std::pair<size_t, Karateka>;

    static const auto rank_annotated_participant_rank_idx = 0u;
    static const auto rank_annotated_participant_karateka_idx = 1u;
    /**
     * @brief Return a ranked list of participants anotated with the actual rank.
     * @return list of the (rank, participants) tuples ranked by the scores.
     */
    std::vector<RankAnnotatedParticipant> get_anotated_ranked_list_of_participants() const;

    /**
     * @brief Return the current active round
     * @return current round
     */
    int8_t get_current_round () const;

    /**
     * @brief Start the next kata round
     * @return Return the round number of the started round
     */
    uint8_t start_next_kata_round ();

    /**
     * @brief Return the startlist for the next kata round.
     * @param no_of_participants_for_round Number of participants which can be nominated for the next round
     * @return The startlist for the next kata round.
     * @note The ordering is backwards, i.e. the best scored participant starts last
     */
    std::vector<size_t> get_startlist_for_next_kata_round(size_t no_of_participants_for_round) const;

private:
    void reassign_startnumbers ();
    friend class boost::serialization::access;

    template<class Archive>
    void serialize (Archive& archive,
                    const unsigned int version);

    std::vector<size_t> get_list_of_participants_for_next_kata_round (size_t no_of_participants_for_round) const;

    TournamentData data_{};
    std::vector<Karateka> participants_;
    uint8_t current_kata_round_{};
};


inline std::string Tournament::name () const
{
    return data_.name_;
}

inline std::string Tournament::location () const
{
    return data_.location_;
}

inline void Tournament::update_data(const TournamentData& data)
{
    data_ = data;
}

inline int8_t Tournament::get_current_round() const
{
    return current_kata_round_;
}

template<class Archive>
void Tournament::serialize (Archive& archive,
                            const unsigned int)
{
    archive & current_kata_round_;
    archive & data_.name_;
    archive & data_.location_;
    archive & data_.date_;
    archive & participants_;
}

} // namespace TournamentRunner

#endif // TOURNAMENT_RUNNER_TOURNAMENT_H_
