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


bool Date::operator== (const Date& other) const
{
    return (this->date_ == other.date_);
}

bool Date::operator!= (const Date& other) const
{
    return !(*this == other);
}


boost::gregorian::date Date::date_from_string (const std::string& date_as_string)
{
    boost::gregorian::date date{};
    auto exception_catched = false;
    try
    {
        date = boost::gregorian::from_simple_string(date_as_string);
    }
    catch (const std::exception& )
    {
        exception_catched = true;
    }
    if (exception_catched)
    {
        exception_catched = false;
        try
        {
            date = boost::gregorian::from_uk_string(date_as_string);
        }
        catch (const std::exception& )
        {
            exception_catched = true;
        }
    }
    if (exception_catched)
    {
        exception_catched = false;
        try
        {
            date = boost::gregorian::from_us_string(date_as_string);
        }
        catch (const std::exception& )
        {
            exception_catched = true;
        }
    }
    if (exception_catched)
    {
        date = boost::gregorian::from_undelimited_string(date_as_string);
    }
    return date;
}

std::ostream& operator<<(std::ostream& os,
                         const Date& date)
{
    return os << date.to_iso_extended_string();
}

} // namespace TournamentRunner


