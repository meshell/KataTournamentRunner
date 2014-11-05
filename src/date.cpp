#include "tournament_runner/date.h"


namespace TournamentRunner
{

Date::Date (const std::string& date_as_string):
    date_{std::move(date_from_string(date_as_string))}
{

}

Date::Date (boost::gregorian::date date):
    date_{std::move(date)}
{
}

bool Date::is_valid() const
{
    return !date_.is_special();
}

std::string Date::to_iso_extended_string () const
{
    return boost::gregorian::to_iso_extended_string(date_);
}

int32_t Date::year () const
{
    return date_.year();
}

int32_t Date::month () const
{
    return date_.month();
}

int32_t Date::day () const
{
    return date_.day();
}

boost::gregorian::date Date::date_from_string (const std::string& date_as_string)
{
    boost::gregorian::date date{boost::gregorian::not_a_date_time};

    using date_from_string_func = std::function<boost::gregorian::date(std::string)>;
    const std::vector<date_from_string_func> from_string_functions =
    {
        &boost::gregorian::from_simple_string,
        &boost::gregorian::from_uk_string,
        &boost::gregorian::from_us_string,
        &boost::gregorian::from_undelimited_string
    };

    for (const auto& func : from_string_functions)
    {
        try
        {
            date = func(date_as_string);
            return date;
        }
        catch (const std::exception& )
        {

        }
    }
    return date;
}

bool operator== (const Date& lhs, const Date& rhs)
{
    return (lhs.date_ == rhs.date_);
}

bool operator!= (const Date& lhs, const Date& rhs)
{
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os,
                         const Date& date)
{
    return os << date.to_iso_extended_string();
}

} // namespace TournamentRunner


