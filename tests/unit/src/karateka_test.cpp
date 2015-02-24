#include <sstream>

#include <gtest/gtest.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include "tournament_runner/karateka.h"

using boost::serialization::make_nvp;

namespace
{

TournamentRunner::Karateka a_default_karateka ()
{
    return TournamentRunner::Karateka{}
           .with_name("Norris")
           .with_surname("Chuck")
           .with_birthdate("1940-03-10")
           .from_dojo("Chun Kuk Do")
           .with_rank("2nd Dan");
}

TEST(A_KataPerformer, should_be_able_to_get_five_kata_scores)
{
    auto testee = a_default_karateka();
    const auto score = 5.5F;
    const auto number_of_scores = 5U;
    for (auto i = 1U; i < number_of_scores; ++i )
    {
        ASSERT_NO_THROW(testee.add_kata_score(score));
    }
}

TEST(A_KataPerformer, should_throw_length_error_exception_when_more_than_five_kata_scores_are_given)
{
    auto testee = a_default_karateka();
    const auto score = 5.5F;
    const auto expected_number_of_scores = 5U;
    for (auto i = 1U; i <= expected_number_of_scores; ++i )
    {
        testee.add_kata_score(score);
    }
    ASSERT_THROW(testee.add_kata_score(score), std::length_error);
}

TEST(A_KataPerformer, should_move_to_round_1_when_moving_to_next_round_the_first_time)
{
    auto testee = a_default_karateka();
    const auto second_round = 1U;
    ASSERT_EQ(second_round, testee.next_round());
}

TEST(A_KataPerformer, should_move_to_round_2_when_moving_to_next_round_the_second_time)
{
    auto testee = a_default_karateka();
    const auto third_round = 2U;
    testee.next_round();
    ASSERT_EQ(third_round, testee.next_round());
}

TEST(A_KataPerformer, should_not_be_able_to_move_to_the_fourth_round)
{
    auto testee = a_default_karateka();
    testee.next_round();
    testee.next_round();
    ASSERT_THROW(testee.next_round(), std::length_error);
}

TEST(A_KataPerformer, should_have_five_kata_scores_of_0_if_moving_to_the_next_round_without_adding_scores)
{
    auto testee = a_default_karateka();
    testee.next_round();
    const auto zero_score = 0.0F;
    const auto first_round = 0U;
    EXPECT_FLOAT_EQ(zero_score, testee.get_maximum_score_of_round(first_round));
    EXPECT_FLOAT_EQ(zero_score, testee.get_minimum_score_of_round(first_round));
    ASSERT_FLOAT_EQ(zero_score, testee.get_overall_score_of_round(first_round));
}

TEST(A_KataPerformer, should_have_five_kata_scores_of_0_in_the_second_round_if_moving_to_the_third_round_without_adding_scores)
{
    auto testee = a_default_karateka();
    testee.next_round();
    testee.next_round();
    const auto zero_score = 0.0F;
    const auto second_round = 1U;
    EXPECT_FLOAT_EQ(zero_score, testee.get_maximum_score_of_round(second_round));
    EXPECT_FLOAT_EQ(zero_score, testee.get_minimum_score_of_round(second_round));
    ASSERT_FLOAT_EQ(zero_score, testee.get_overall_score_of_round(second_round));
}


TEST(A_KataPerformer, should_have_the_largest_of_all_scores_of_a_round_as_maximum_score)
{
    auto testee = a_default_karateka();
    const auto score_1 = 5.0F;
    const auto score_2 = 5.1F;
    const auto score_3 = 5.2F;
    const auto score_4 = 5.3F;
    const auto score_5 = 5.4F;
    testee.add_kata_score(score_1);
    testee.add_kata_score(score_2);
    testee.add_kata_score(score_3);
    testee.add_kata_score(score_4);
    testee.add_kata_score(score_5);
    const auto first_round = 0U;
    ASSERT_FLOAT_EQ(score_5, testee.get_maximum_score_of_round(first_round));
}

TEST(A_KataPerformer, should_have_the_smallest_of_all_scores_of_a_round_as_minimum_score)
{
    auto testee = a_default_karateka();
    const auto score_1 = 5.0F;
    const auto score_2 = 5.1F;
    const auto score_3 = 5.2F;
    const auto score_4 = 5.3F;
    const auto score_5 = 5.4F;
    testee.add_kata_score(score_1);
    testee.add_kata_score(score_2);
    testee.add_kata_score(score_3);
    testee.add_kata_score(score_4);
    testee.add_kata_score(score_5);
    const auto first_round = 0;
    ASSERT_FLOAT_EQ(score_1, testee.get_minimum_score_of_round(first_round));
}

TEST(A_KataPerformer, should_have_the_sum_of_all_scores_of_round_except_minimum_and_maximum_as_overall_score_of_a_round)
{
    auto testee = a_default_karateka();
    const auto score_1 = 5.0F;
    const auto score_2 = 5.1F;
    const auto score_3 = 5.2F;
    const auto score_4 = 5.3F;
    const auto score_5 = 5.4F;
    testee.add_kata_score(score_1);
    testee.add_kata_score(score_2);
    testee.add_kata_score(score_3);
    testee.add_kata_score(score_4);
    testee.add_kata_score(score_5);
    const auto first_round = 0;
    const auto total_score = score_2 + score_3 + score_4;
    ASSERT_FLOAT_EQ(total_score, testee.get_overall_score_of_round(first_round));
}

TEST(A_KataPerformer, should_have_0_as_maximum_score_if_no_scores_were_added)
{
    auto testee = a_default_karateka();
    const auto first_round = 0;
    const auto zero_score = 0.0F;
    ASSERT_FLOAT_EQ(zero_score, testee.get_maximum_score_of_round(first_round));
}

TEST(A_KataPerformer, should_have_0_as_minimum_score_if_no_scores_were_added)
{
    auto testee = a_default_karateka();
    const auto first_round = 0;
    const auto zero_score = 0.0F;
    ASSERT_FLOAT_EQ(zero_score, testee.get_minimum_score_of_round(first_round));
}

TEST(A_KataPerformer, should_have_0_as_overall_score_if_no_scores_were_added)
{
    auto testee = a_default_karateka();
    const auto first_round = 0;
    const auto zero_score = 0.0F;
    ASSERT_FLOAT_EQ(zero_score, testee.get_overall_score_of_round(first_round));
}

TEST(A_KataPerformer, should_have_a_deduction_subtracted_from_the_overall_score_of_a_round)
{
    auto testee = a_default_karateka();
    const auto score_1 = 5.0F;
    const auto score_2 = 5.1F;
    const float score_3 = 5.2F;
    const auto score_4 = 5.3F;
    const auto score_5 = 5.4F;
    testee.add_kata_score(score_1);
    testee.add_kata_score(score_2);
    testee.add_kata_score(score_3);
    testee.add_kata_score(score_4);
    testee.add_kata_score(score_5);
    const auto first_round = 0;
    const auto deduction = 1.5F;
    testee.add_deduction(deduction);
    const auto total_score = score_2 + score_3 + score_4 - deduction;
    ASSERT_FLOAT_EQ(total_score, testee.get_overall_score_of_round(first_round));
}

TEST(The_kata_score_sort_function, should_return_greater_if_the_overall_score_is_the_same_but_the_rhs_minimal_score_is_higher)
{
    auto karateka_1 = a_default_karateka();
    const auto score_1 = 5.0F;
    const auto score_2 = 5.1F;
    const auto score_3 = 5.2F;
    const auto score_4 = 5.3F;
    const auto score_5 = 5.4F;
    karateka_1.add_kata_score(score_1);
    karateka_1.add_kata_score(score_2);
    karateka_1.add_kata_score(score_3);
    karateka_1.add_kata_score(score_4);
    karateka_1.add_kata_score(score_5);

    auto karateka_2 = a_default_karateka();
    const auto score_1a = 5.1F;
    karateka_2.add_kata_score(score_1a);
    karateka_2.add_kata_score(score_2);
    karateka_2.add_kata_score(score_3);
    karateka_2.add_kata_score(score_4);
    karateka_2.add_kata_score(score_5);
    ASSERT_TRUE(kata_score_sort_greater(karateka_1, karateka_2));
}

TEST(The_kata_score_sort_function, should_return_greater_if_the_overall_and_minimal_score_are_the_same_but_the_rhs_maximum_score_is_higher)
{
    auto karateka_1 = a_default_karateka();
    const auto score_1 = 5.0F;
    const auto score_2 = 5.1F;
    const auto score_3 = 5.2F;
    const auto score_4 = 5.3F;
    const auto score_5 = 5.4F;
    karateka_1.add_kata_score(score_1);
    karateka_1.add_kata_score(score_2);
    karateka_1.add_kata_score(score_3);
    karateka_1.add_kata_score(score_4);
    karateka_1.add_kata_score(score_5);

    auto karateka_2 = a_default_karateka();
    const auto score_5a = 5.5F;
    karateka_2.add_kata_score(score_1);
    karateka_2.add_kata_score(score_2);
    karateka_2.add_kata_score(score_3);
    karateka_2.add_kata_score(score_4);
    karateka_2.add_kata_score(score_5a);
    ASSERT_TRUE(kata_score_sort_greater(karateka_1, karateka_2));
}

TEST(The_kata_score_sort_function, should_not_return_greater_if_all_scores_are_the_same)
{
    auto karateka_1 = a_default_karateka();
    const auto score_1 = 5.0F;
    const auto score_2 = 5.1F;
    const auto score_3 = 5.2F;
    const auto score_4 = 5.3F;
    const auto score_5 = 5.4F;
    karateka_1.add_kata_score(score_1);
    karateka_1.add_kata_score(score_2);
    karateka_1.add_kata_score(score_3);
    karateka_1.add_kata_score(score_4);
    karateka_1.add_kata_score(score_5);

    auto karateka_2 = a_default_karateka();
    karateka_2.add_kata_score(score_1);
    karateka_2.add_kata_score(score_2);
    karateka_2.add_kata_score(score_3);
    karateka_2.add_kata_score(score_4);
    karateka_2.add_kata_score(score_5);
    ASSERT_FALSE(kata_score_sort_greater(karateka_1, karateka_2));
}

TEST(The_kata_score_is_equal_function, should_return_not_equal_if_the_overall_score_is_higher)
{
    auto karateka_1 = a_default_karateka();
    const auto score_1 = 5.0F;
    const auto score_2 = 5.1F;
    const auto score_3 = 5.2F;
    const auto score_4 = 5.3F;
    const auto score_5 = 5.4F;
    karateka_1.add_kata_score(score_1);
    karateka_1.add_kata_score(score_2);
    karateka_1.add_kata_score(score_3);
    karateka_1.add_kata_score(score_4);
    karateka_1.add_kata_score(score_5);

    auto karateka_2 = a_default_karateka();
    const auto score_2a = 5.2F;
    karateka_2.add_kata_score(score_1);
    karateka_2.add_kata_score(score_2a);
    karateka_2.add_kata_score(score_3);
    karateka_2.add_kata_score(score_4);
    karateka_2.add_kata_score(score_5);
    ASSERT_FALSE(kata_score_is_equal(karateka_1, karateka_2));
}

TEST(The_kata_score_is_equal_function, should_return_not_equal_if_the_overall_score_is_the_same_but_the_minimal_score_is_higher)
{
    auto karateka_1 = a_default_karateka();
    const auto score_1 = 5.0F;
    const auto score_2 = 5.1F;
    const auto score_3 = 5.2F;
    const auto score_4 = 5.3F;
    const auto score_5 = 5.4F;
    karateka_1.add_kata_score(score_1);
    karateka_1.add_kata_score(score_2);
    karateka_1.add_kata_score(score_3);
    karateka_1.add_kata_score(score_4);
    karateka_1.add_kata_score(score_5);

    auto karateka_2 = a_default_karateka();
    const auto score_1a = 5.1F;
    karateka_2.add_kata_score(score_1a);
    karateka_2.add_kata_score(score_2);
    karateka_2.add_kata_score(score_3);
    karateka_2.add_kata_score(score_4);
    karateka_2.add_kata_score(score_5);
    ASSERT_FALSE(kata_score_is_equal(karateka_1, karateka_2));
}

TEST(The_kata_score_is_equal_function, should_return_not_equal_if_the_overall_and_minimal_score_are_the_same_but_the_maximum_score_is_higher)
{
    auto karateka_1 = a_default_karateka();
    const auto score_1 = 5.0F;
    const auto score_2 = 5.1F;
    const auto score_3 = 5.2F;
    const auto score_4 = 5.3F;
    const auto score_5 = 5.4F;
    karateka_1.add_kata_score(score_1);
    karateka_1.add_kata_score(score_2);
    karateka_1.add_kata_score(score_3);
    karateka_1.add_kata_score(score_4);
    karateka_1.add_kata_score(score_5);

    auto karateka_2 = a_default_karateka();
    const auto score_5a = 5.5F;
    karateka_2.add_kata_score(score_1);
    karateka_2.add_kata_score(score_2);
    karateka_2.add_kata_score(score_3);
    karateka_2.add_kata_score(score_4);
    karateka_2.add_kata_score(score_5a);
    ASSERT_FALSE(kata_score_is_equal(karateka_1, karateka_2));
}

TEST(The_kata_score_is_equal_function, should_return_equal_if_all_scores_are_the_same)
{
    auto karateka_1 = a_default_karateka();
    const auto score_1 = 5.0F;
    const auto score_2 = 5.1F;
    const auto score_3 = 5.2F;
    const auto score_4 = 5.3F;
    const auto score_5 = 5.4F;
    karateka_1.add_kata_score(score_1);
    karateka_1.add_kata_score(score_2);
    karateka_1.add_kata_score(score_3);
    karateka_1.add_kata_score(score_4);
    karateka_1.add_kata_score(score_5);

    auto karateka_2 = a_default_karateka();
    karateka_2.add_kata_score(score_1);
    karateka_2.add_kata_score(score_2);
    karateka_2.add_kata_score(score_3);
    karateka_2.add_kata_score(score_4);
    karateka_2.add_kata_score(score_5);
    ASSERT_TRUE(kata_score_is_equal(karateka_1, karateka_2));
}

TEST(A_serialized_Karateka, should_persists_the_data)
{
    auto testee = a_default_karateka();
    const auto start_number = 20U;
    testee.set_startnumber(start_number);

    std::stringbuf buffer{};
    std::ostream output_stream{&buffer};
    boost::archive::xml_oarchive out_archive{output_stream};

    const auto xml_element_name = "Karateka";
    out_archive & make_nvp(xml_element_name, testee);

    auto deserialized_karateka = TournamentRunner::Karateka{};

    std::istream inputStream{&buffer};
    boost::archive::xml_iarchive in_archive{inputStream};

    in_archive & make_nvp(xml_element_name, deserialized_karateka);

    ASSERT_EQ(testee.name(), deserialized_karateka.name());
    ASSERT_EQ(testee.surname(), deserialized_karateka.surname());
    ASSERT_EQ(testee.date_of_birth_as_string(), deserialized_karateka.date_of_birth_as_string());
    ASSERT_EQ(testee.dojo(), deserialized_karateka.dojo());
    ASSERT_EQ(testee.grade(), deserialized_karateka.grade());
    ASSERT_EQ(start_number, deserialized_karateka.get_startnumber());
}

TEST(A_serialized_Karateka, should_persist_the_current_kata_round)
{
    auto testee = a_default_karateka();

    const auto current_round = testee.next_round();

    std::stringbuf buffer{};
    std::ostream output_stream{&buffer};
    boost::archive::xml_oarchive out_archive{output_stream};

    const auto xml_element_name = "Karateka";
    out_archive & make_nvp(xml_element_name, testee);

    auto deserialized_karateka = TournamentRunner::Karateka{};

    std::istream inputStream{&buffer};
    boost::archive::xml_iarchive in_archive{inputStream};

    in_archive & make_nvp(xml_element_name, deserialized_karateka);

    const auto expected_next_round = current_round + 1;

    ASSERT_EQ(expected_next_round, deserialized_karateka.next_round());
}

TEST(A_serialized_Karateka, should_persist_the_kata_scores)
{
    auto testee = a_default_karateka();

    const auto minimum_score = 5.0F;
    const auto score_2 = 5.1F;
    const auto score_3 = 5.2F;
    const auto score_4 = 5.3F;
    const auto maximum_score = 5.4F;
    testee.add_kata_score(minimum_score);
    testee.add_kata_score(score_2);
    testee.add_kata_score(score_3);
    testee.add_kata_score(score_4);
    testee.add_kata_score(maximum_score);

    testee.next_round();

    testee.add_kata_score(minimum_score);
    testee.add_kata_score(minimum_score);
    testee.add_kata_score(minimum_score);
    testee.add_kata_score(maximum_score);
    testee.add_kata_score(maximum_score);

    std::stringbuf buffer{};
    std::ostream output_stream{&buffer};
    boost::archive::xml_oarchive out_archive{output_stream};

    const auto xml_element_name = "Karateka";
    out_archive & make_nvp(xml_element_name, testee);

    auto deserialized_karateka = TournamentRunner::Karateka{};

    std::istream inputStream{&buffer};
    boost::archive::xml_iarchive in_archive{inputStream};

    in_archive & make_nvp(xml_element_name,deserialized_karateka);

    const auto overall_score_first_round = score_2 + score_3 + score_4;

    ASSERT_FLOAT_EQ(minimum_score, deserialized_karateka.get_minimum_score_of_round(0));
    ASSERT_FLOAT_EQ(maximum_score, deserialized_karateka.get_maximum_score_of_round(0));
    ASSERT_FLOAT_EQ(overall_score_first_round, deserialized_karateka.get_overall_score_of_round(0));

    const auto overall_score_second_round = minimum_score + minimum_score + maximum_score;

    ASSERT_FLOAT_EQ(minimum_score, deserialized_karateka.get_minimum_score_of_current_round());
    ASSERT_FLOAT_EQ(maximum_score, deserialized_karateka.get_maximum_score_of_current_round());
    ASSERT_FLOAT_EQ(overall_score_second_round, deserialized_karateka.get_overall_score_of_current_round());
}

TEST(A_serialized_Karateka, should_persist_the_kata_scores_and_deductions)
{
    auto testee = a_default_karateka();

    const auto minimum_score = 5.0F;
    const auto score_2 = 5.1F;
    const auto score_3 = 5.2F;
    const auto score_4 = 5.3F;
    const auto maximum_score = 5.4F;
    testee.add_kata_score(minimum_score);
    testee.add_kata_score(score_2);
    testee.add_kata_score(score_3);
    testee.add_kata_score(score_4);
    testee.add_kata_score(maximum_score);

    const auto deduction_round1 = 0.5F;
    testee.add_deduction(deduction_round1);

    testee.next_round();

    testee.add_kata_score(minimum_score);
    testee.add_kata_score(minimum_score);
    testee.add_kata_score(minimum_score);
    testee.add_kata_score(maximum_score);
    testee.add_kata_score(maximum_score);

    const auto deduction_round2 = 1.0F;
    testee.add_deduction(deduction_round2);

    std::stringbuf buffer{};
    std::ostream output_stream{&buffer};
    boost::archive::xml_oarchive out_archive{output_stream};

    const auto xml_element_name = "Karateka";
    out_archive & make_nvp(xml_element_name, testee);

    auto deserialized_karateka = TournamentRunner::Karateka{};

    std::istream inputStream{&buffer};
    boost::archive::xml_iarchive in_archive{inputStream};

    in_archive & make_nvp(xml_element_name, deserialized_karateka);

    const auto overall_score_first_round = score_2 + score_3 + score_4 - deduction_round1;

    ASSERT_FLOAT_EQ(minimum_score, deserialized_karateka.get_minimum_score_of_round(0));
    ASSERT_FLOAT_EQ(maximum_score, deserialized_karateka.get_maximum_score_of_round(0));
    ASSERT_FLOAT_EQ(overall_score_first_round, deserialized_karateka.get_overall_score_of_round(0));

    const auto overall_score_second_round = minimum_score + minimum_score + maximum_score - deduction_round2;

    ASSERT_FLOAT_EQ(minimum_score, deserialized_karateka.get_minimum_score_of_current_round());
    ASSERT_FLOAT_EQ(maximum_score, deserialized_karateka.get_maximum_score_of_current_round());
    ASSERT_FLOAT_EQ(overall_score_second_round, deserialized_karateka.get_overall_score_of_current_round());
}


} //namespace
