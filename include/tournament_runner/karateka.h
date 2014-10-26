#ifndef TOURNAMENT_RUNNER_KARATEKA_H_
#define TOURNAMENT_RUNNER_KARATEKA_H_

#include <string>
#include <vector>
#include <cstdint>
#include <array>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/array.hpp>

#include "tournament_runner/date.h"
#include "tournament_runner/kata_performer_interface.h"

// TODO: replace with std::array as soon as serialization is supported
using boost::array;

namespace TournamentRunner
{

/**
 * @brief Number of kata scores per round (5)
 */
const uint8_t number_of_kata_scores_per_round = 5;
/**
 * @brief Max number of kata rounds (3)
 */
const uint8_t max_number_of_kata_rounds = 3;

/**
 * @brief The Karateka class
 * @details A karateka is a person practicing karate and participating at a tournament.
 * Serializable
 */
class Karateka :
    public IKataPerformer
{
public:

    Karateka& with_name (const std::string& name);
    Karateka& with_name (std::string&& name) noexcept;
    Karateka& with_surname (const std::string& surname);
    Karateka& with_surname (std::string&& surname) noexcept;
    Karateka& with_birthdate (const Date& date_of_birth);
    Karateka& with_birthdate (Date&& date_of_birth) noexcept;
    Karateka& with_birthdate (const std::string& date_of_birth);
    Karateka& from_dojo (const std::string& dojo);
    Karateka& from_dojo (std::string&& dojo) noexcept;
    Karateka& with_rank (const std::string& rank);
    Karateka& with_rank (std::string&& rank) noexcept;

    /**
     * @brief set_startnumber
     * @param start_number
     */
    void set_startnumber (size_t start_number);

    /**
     * @brief Return the startnumber
     * @return startnumber
     */
    size_t get_startnumber() const;

    /**
     * @copydoc IKataPerformer::add_kata_score
     * @throws std::length_error exception if more than number_of_kata_scores_per_round scores are added
     * @return Number of kata scores added already for current round
     */
    uint32_t add_kata_score (float score) final;

    /**
     * @copydoc IKataPerformer::add_deduction
     * @throws std::length_error exception if more than number_of_kata_scores_per_round scores are added
     */
    void add_deduction (float deduction) final;

    /**
     * @copydoc IKataPerformer::next_round
     * @throws std::length_error exception if we try to move to a round larger than max_number_of_kata_rounds
     */
    uint8_t next_round () final;

    /**
     * @copydoc IKataPerformer::get_maximum_score_of_round
     */
    float get_maximum_score_of_round (uint8_t round) const final;

    /**
     * @copydoc IKataPerformer::get_minimum_score_of_round
     */
    float get_minimum_score_of_round (uint8_t round) const final;

    /**
     * @copydoc IKataPerformer::get_overall_score_of_round
     */
    float get_overall_score_of_round (uint8_t round) const final;

    /**
     * @copydoc IKataPerformer::get_maximum_score_of_current_round
     */
    float get_maximum_score_of_current_round () const final;

    /**
     * @copydoc IKataPerformer::get_minimum_score_of_current_round
     */
    float get_minimum_score_of_current_round () const final;

    /**
     * @copydoc IKataPerformer::get_overall_score_of_current_round
     */
    float get_overall_score_of_current_round () const final;

    /**
     * @brief Return all scores of a round
     * @param round Which round ?
     * @return All scores of round round
     */
    std::vector<float> get_scores (uint8_t round) const;

    float get_deduction (uint8_t round) const;

    /**
     * @brief Return the name of the Karateka
     * @return The karateka's name
    */
    std::string name () const;
    /**
     * @brief Return the surname of the Karateka
     * @return The karateka's surname
    */
    std::string surname () const;

    /**
     * @brief Return the date of birth of the Karateka
     * @return The karateka's date of birth as string
     */
    std::string date_of_birth_as_string () const;

    /**
     * @brief Return a date of birth string of the Karateka
     * @return The karateka's date of birth
     */
    Date date_of_birth () const;

    /**
     * @brief Return the dojo the Karateka belongs to
     * @return The dojo the Karateka belongs to
     */
    std::string dojo () const;
    /**
     * @brief Return the rank of the Karateka
     * @return The karateka's rank
     */
    std::string rank () const;

private:

    friend class boost::serialization::access;

    /**
     * @brief Serialization function (boost::serialization)
     */
    template<class Archive>
    void serialize (Archive& archive, const unsigned int version);

    std::string name_{};
    std::string surname_{};
    Date date_of_birth_{};
    std::string dojo_{};
    std::string rank_{};
    array<std::vector<float>, max_number_of_kata_rounds> kata_scores_;
    array<float, max_number_of_kata_rounds> deductions_;
    size_t start_number_{};
    uint8_t kata_round_{};
};

/**
 * @brief Karateka score compare method for std::sort algorithm
 * @details Rules are:
 * @li highest overall kata score first.
 * @li if the same the higher minimal score comes first
 * @li if this is also the same the higher maximal score comes first
 * @param lhs
 * @param rhs
 * @retval ​true if the lhs is less than the rhs.
 * @retval false otherwise
 */
bool kata_score_sort_greater(const IKataPerformer& lhs,
                             const IKataPerformer& rhs);

/**
 * @brief kata_score_is_equal
 * @param lhs
 * @param rhs
 * @return
 */
bool kata_score_is_equal(const IKataPerformer& lhs,
                         const IKataPerformer& rhs);



inline void Karateka::set_startnumber (size_t start_number)
{
    start_number_ = start_number;
}

inline size_t Karateka::get_startnumber () const
{
    return start_number_;
}

inline std::string Karateka::name () const
{
    return name_;
}

inline std::string Karateka::surname () const
{
    return surname_;
}

inline std::string Karateka::date_of_birth_as_string () const
{
    return date_of_birth_.to_iso_extended_string();
}

inline Date Karateka::date_of_birth() const
{
    return date_of_birth_;
}

inline std::string Karateka::dojo () const
{
    return dojo_;
}

inline std::string Karateka::rank () const
{
    return rank_;
}

template<class Archive>
void Karateka::serialize (Archive& archive,
                          const unsigned int)
{
    archive & name_;
    archive & surname_;
    archive & date_of_birth_;
    archive & dojo_;
    archive & rank_;
    archive & kata_scores_;
    archive & deductions_;
    archive & start_number_;
    archive & kata_round_;
}

} //namespace TournamentRunner

#endif // TOURNAMENT_RUNNER_KARATEKA_H_
