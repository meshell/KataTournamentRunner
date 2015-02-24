#include <functional>
#include <sstream>

#include <gmock/gmock.h>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>

#include "tournament_runner/date.h"

using boost::serialization::make_nvp;

using ::TournamentRunner::Date;

using ::testing::Eq;

namespace
{

TEST(A_Date, returns_a_invalid_date_when_created_from_a_string_with_invalid_day)
{
    const auto testee = Date{"2013-02-29"};
    ASSERT_FALSE(testee.is_valid());
}

TEST(A_Date, returns_a_invalid_date_when_created_from_a_string_with_invalid_month)
{
    const auto testee = Date{"2013-13-20"};
    ASSERT_FALSE(testee.is_valid());
}

TEST(A_Date, returns_a_invalid_date_when_created_from_a_string_with_invalid_year)
{
    const auto testee = Date{"13-13-20"};
    ASSERT_FALSE(testee.is_valid());
}

TEST(A_Date, returns_a_invalid_date_when_created_from_a_string_with_empty_date)
{
    const auto testee = Date{""};
    ASSERT_FALSE(testee.is_valid());
}

TEST(A_Date, returns_a_valid_date_when_created_from_a_string_with_uk_format_date)
{
    const auto testee = Date{"23-07-2013"};
    ASSERT_TRUE(testee.is_valid());
}

TEST(A_Date, returns_a_valid_date_when_created_date_from_a_string_with_us_format_date)
{
    const auto testee = Date{"07-23-2013"};
    ASSERT_TRUE(testee.is_valid());
}

TEST(A_Date, returns_a_valid_date_when_created_from_a_string_with_iso_type_date)
{
    const auto testee = Date{"20130125"};
    ASSERT_TRUE(testee.is_valid());
}

TEST(A_Date, returns_a_valid_date_when_created_from_a_string_with_iso_extended_type_date)
{
    const auto testee = Date{"2013-October-09"};
    ASSERT_TRUE(testee.is_valid());
}

TEST(A_Date, returns_a_valid_date_when_created_from_a_string_with_iso_extended_type_date_with_short_month)
{
    const auto testee = Date{"2013-Oct-09"};
    ASSERT_TRUE(testee.is_valid());
}

TEST(A_Date, returns_a_invalid_date_when_created_from_a_string_with_iso_extended_type_date_with_wrong_month)
{
    const auto testee = Date{"2013-Set-09"};
    ASSERT_FALSE(testee.is_valid());
}

TEST(A_Date, should_be_serializable_and_deserializable)
{
    std::stringbuf buffer{};
    std::ostream output_stream{&buffer};
    boost::archive::xml_oarchive out_archive{output_stream};

    const auto testee = Date{"2013-01-01"};

    const auto xml_element_name = "Date";
    out_archive & make_nvp(xml_element_name, testee);

    std::istream inputStream{&buffer};

    boost::archive::xml_iarchive in_archive{inputStream};

    auto deserialized_date = Date{};

    in_archive & make_nvp(xml_element_name, deserialized_date);

    ASSERT_THAT(deserialized_date, Eq(testee));
}

} //namespace
