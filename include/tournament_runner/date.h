#ifndef TOURNAMENT_RUNNER_DATE_H_
#define TOURNAMENT_RUNNER_DATE_H_

#include <cstdint>
#include <ostream>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>

namespace TournamentRunner
{

/**
 * @brief The Date class
 */
class Date
{
public:
    /**
     * @brief Default empty Date constructor
     * @details Sets the date to the current date
     */
    Date () = default;
    /**
     * @brief Constructor from date as string
     * @param date_as_string Date as string
     */
    explicit Date (const std::string& date_as_string);

    /**
     * @brief Constructor from boost gregorian date
     * @param date Date as boost gregorian date
     */
    explicit Date (boost::gregorian::date date);

    /**
     * @brief Return date as extended iso string
     * @return Date as iso string
     */
    std::string to_iso_extended_string () const;

    /**
     * @brief Return the year
     * @return The year as integer
     */
    int32_t year () const;
    /**
     * @brief Return the month
     * @return The month as integer
     */
    int32_t month () const;
    /**
     * @brief Return the day of month
     * @return The day of month as integer
     */
    int32_t day () const;

    bool operator== (const Date& other) const;
    bool operator!= (const Date& other) const;

    Date& operator= (const Date& rhs) = default;

private:
    static boost::gregorian::date date_from_string (const std::string& date_as_string);

    friend class boost::serialization::access;

    /**
     * @brief Serialization save function (boost::serialization)
     */
    template<class Archive>
    void save (Archive& archive,
               const unsigned int version) const;

    /**
     * @brief Serialization load function (boost::serialization)
     */
    template<class Archive>
    void load (Archive& archive,
               const unsigned int version);

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    boost::gregorian::date date_{boost::gregorian::day_clock::local_day()};
};

std::ostream& operator<<(std::ostream& os,
                         const Date& date);

template<class Archive>
void Date::save (Archive& archive,
                 const unsigned int version) const
{
    const auto date_as_string = boost::gregorian::to_iso_extended_string(date_);
    archive & date_as_string;
}

template<class Archive>
void Date::load (Archive& archive,
                 const unsigned int version)
{
    auto date_as_string = std::string{};
    archive & date_as_string;
    date_ = boost::gregorian::from_simple_string(date_as_string);
}

} // namespace TournamentRunner

#endif //TOURNAMENT_RUNNER_DATE_H_
