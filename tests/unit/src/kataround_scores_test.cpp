#include <gtest/gtest.h>
#include <sstream>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include "tournament_runner/kataround_scores.h"

namespace
{

using TournamentRunner::KataRoundScores;

TEST(The_KataRoundScores, should_initially_have_five_kata_scores_of_0)
{
    TournamentRunner::KataRoundScores testee{};
    const auto zero_score = 0.0F;
    EXPECT_FLOAT_EQ(zero_score, testee.get_maximum_score());
    EXPECT_FLOAT_EQ(zero_score, testee.get_minimum_score());
    ASSERT_FLOAT_EQ(zero_score, testee.get_overall_score());
}

TEST(The_KataRoundScores, should_be_able_to_add_five_scores)
{
    TournamentRunner::KataRoundScores testee{};
    const auto score = 5.5F;
    const auto number_of_scores = 5U;
    for (auto i = 1U; i < number_of_scores; ++i )
    {
        ASSERT_NO_THROW(testee.add_kata_score(score));
    }
}

TEST(The_KataRoundScores, should_throw_length_error_exception_when_more_than_five_kata_scores_are_added)
{
    TournamentRunner::KataRoundScores testee{};
    const auto score = 5.5F;
    const auto expected_number_of_scores = 5U;
    for (auto i = 1U; i <= expected_number_of_scores; ++i )
    {
        testee.add_kata_score(score);
    }
    ASSERT_THROW(testee.add_kata_score(score), std::length_error);
}

TEST(The_KataRoundScores, should_have_the_largest_of_all_scores_as_maximum_score)
{
    TournamentRunner::KataRoundScores testee{};
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
    ASSERT_FLOAT_EQ(score_5, testee.get_maximum_score());
}

TEST(The_KataRoundScores, should_have_the_smallest_of_all_scores_minimum_score)
{
    TournamentRunner::KataRoundScores testee{};
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
    ASSERT_FLOAT_EQ(score_1, testee.get_minimum_score());
}

TEST(The_KataRoundScores, should_have_the_sum_of_all_scores_except_minimum_and_maximum_as_overall_score)
{
    TournamentRunner::KataRoundScores testee{};
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
    const auto total_score = score_2 + score_3 + score_4;
    ASSERT_FLOAT_EQ(total_score, testee.get_overall_score());
}

TEST(The_KataRoundScores, should_have_0_as_maximum_score_if_no_scores_were_added)
{
    TournamentRunner::KataRoundScores testee{};
    const auto zero_score = 0.0F;
    ASSERT_FLOAT_EQ(zero_score, testee.get_maximum_score());
}

TEST(The_KataRoundScores, should_have_0_as_minimum_score_if_no_scores_were_added)
{
    TournamentRunner::KataRoundScores testee{};
    const auto zero_score = 0.0F;
    ASSERT_FLOAT_EQ(zero_score, testee.get_minimum_score());
}

TEST(The_KataRoundScores, should_have_0_as_overall_score_if_no_scores_were_added)
{
    TournamentRunner::KataRoundScores testee{};
    const auto zero_score = 0.0F;
    ASSERT_FLOAT_EQ(zero_score, testee.get_overall_score());
}

TEST(The_KataRoundScores, should_have_a_deduction_subtracted_from_the_overall_score)
{
    TournamentRunner::KataRoundScores testee{};
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
    const auto deduction = 1.5F;
    testee.add_deduction(deduction);
    const auto total_score = score_2 + score_3 + score_4 - deduction;
    ASSERT_FLOAT_EQ(total_score, testee.get_overall_score());
}

TEST(The_KataRoundScores, should_have_the_largest_of_all_scores_as_maximum_score_if_not_all_scores_added)
{
    TournamentRunner::KataRoundScores testee{};
    const auto score_1 = 5.0F;
    const auto score_2 = 5.1F;
    const auto score_3 = 5.2F;
    testee.add_kata_score(score_1);
    testee.add_kata_score(score_2);
    testee.add_kata_score(score_3);
    ASSERT_FLOAT_EQ(score_3, testee.get_maximum_score());
}

TEST(The_KataRoundScores, should_have_the_smallest_of_all_scores_minimum_score_if_not_all_scores_added)
{
    TournamentRunner::KataRoundScores testee{};
    const auto score_1 = 5.0F;
    const auto score_2 = 5.1F;
    const auto score_3 = 5.2F;
    testee.add_kata_score(score_1);
    testee.add_kata_score(score_2);
    testee.add_kata_score(score_3);
    ASSERT_FLOAT_EQ(score_1, testee.get_minimum_score());
}

TEST(The_KataRoundScores, should_have_the_sum_of_all_scores_except_minimum_and_maximum_as_overall_score_if_not_all_scores_added)
{
    TournamentRunner::KataRoundScores testee{};
    const auto score_1 = 5.0F;
    const auto score_2 = 5.1F;
    const auto score_3 = 5.1F;
    const auto score_4 = 5.2F;
    testee.add_kata_score(score_1);
    testee.add_kata_score(score_2);
    testee.add_kata_score(score_3);
    testee.add_kata_score(score_4);
    ASSERT_FLOAT_EQ(score_2 + score_3, testee.get_overall_score());
}

TEST(A_serialized_KataRoundScores, should_persist_the_kata_scores)
{
    TournamentRunner::KataRoundScores testee{};

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

    std::stringbuf buffer{};
    std::ostream output_stream{&buffer};
    boost::archive::xml_oarchive out_archive{output_stream};

    const auto xml_element_name = "Scores";
    out_archive & make_nvp(xml_element_name, testee);

    auto deserialized_kataroundscores = TournamentRunner::KataRoundScores{};

    std::istream inputStream{&buffer};
    boost::archive::xml_iarchive in_archive{inputStream};

    in_archive & make_nvp(xml_element_name, deserialized_kataroundscores);

    const auto overall_score = score_2 + score_3 + score_4;

    ASSERT_FLOAT_EQ(minimum_score, deserialized_kataroundscores.get_minimum_score());
    ASSERT_FLOAT_EQ(maximum_score, deserialized_kataroundscores.get_maximum_score());
    ASSERT_FLOAT_EQ(overall_score, deserialized_kataroundscores.get_overall_score());

}

} //namespace
