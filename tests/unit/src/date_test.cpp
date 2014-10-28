#include <functional>
#include <sstream>

#include <gmock/gmock.h>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "tournament_runner/date.h"


using ::TournamentRunner::Date;

using ::testing::Eq;

namespace
{

TEST(A_Date, should_throw_an_exception_when_created_from_a_string_with_invalid_day)
{
    ASSERT_ANY_THROW(Date{"2013-02-29"});
}

TEST(A_Date, should_throw_an_exception_when_created_from_a_string_with_invalid_month)
{
    ASSERT_ANY_THROW(Date{"2013-13-20"});
}

TEST(A_Date, should_throw_an_exception_when_created_from_a_string_with_invalid_year)
{
    ASSERT_ANY_THROW(Date{"13-13-20"});
}

TEST(A_Date, should_throw_an_exception_when_created_from_a_string_with_empty_date)
{
    ASSERT_ANY_THROW(Date{""});
}

TEST(A_Date, should_not_throw_an_exception_when_created_from_a_string_with_uk_format_date)
{
    ASSERT_NO_THROW(Date{"23-07-2013"});
}

TEST(A_Date, should_not_throw_an_exception_when_created_date_from_a_string_with_us_format_date)
{
    ASSERT_NO_THROW(Date{"07-23-2013"});
}

TEST(A_Date, should_not_throw_an_exception_when_created_from_a_string_with_iso_type_date)
{
    ASSERT_NO_THROW(Date{"20130125"});
}

TEST(A_Date, should_not_throw_an_exception_when_created_from_a_string_with_iso_extended_type_date)
{
    ASSERT_NO_THROW(Date{"2013-October-09"});
}

TEST(A_Date, should_not_throw_an_exception_when_created_from_a_string_with_iso_extended_type_date_with_short_month)
{
    ASSERT_NO_THROW(Date{"2013-Oct-09"});
}

TEST(A_Date, should_not_throw_an_exception_when_created_from_a_string_with_iso_extended_type_date_with_wrong_month)
{
    ASSERT_ANY_THROW(Date{"2013-Set-09"});
}

TEST(A_Date, should_be_serializable_and_deserializable)
{
    std::stringbuf buffer{};
    std::ostream output_stream{&buffer};
    boost::archive::text_oarchive out_archive{output_stream};

    auto testee = Date{"2013-01-01"};


    out_archive & testee;

    std::istream inputStream{&buffer};

    boost::archive::text_iarchive in_archive{inputStream};

    auto deserialized_date = Date{};

    in_archive & deserialized_date;

    ASSERT_THAT(deserialized_date, Eq(testee));
}

} //namespace