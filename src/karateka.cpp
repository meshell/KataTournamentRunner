#include "tournament_runner/karateka.h"

#include <cmath>

namespace TournamentRunner
{

Karateka& Karateka::with_name (const std::string& karateka_name)
{
    name_ = karateka_name;
    return *this;
}

Karateka& Karateka::with_name (std::string&& karateka_name) noexcept
{
    name_ = std::move(karateka_name);
    return *this;
}

Karateka& Karateka::with_surname (const std::string& karateka_surname)
{
    surname_ = karateka_surname;
    return *this;
}

Karateka& Karateka::with_surname (std::string&& karateka_surname) noexcept
{
    surname_ = std::move(karateka_surname);
    return *this;
}

Karateka& Karateka::with_birthdate (const Date& birthdate)
{
    date_of_birth_ = birthdate;
    return *this;
}

Karateka& Karateka::with_birthdate (Date&& birthdate) noexcept
{
    date_of_birth_ = std::move(birthdate);
    return *this;
}

Karateka& Karateka::with_birthdate (const std::string& birthdate_as_string)
{
    date_of_birth_ = Date(birthdate_as_string);
    return *this;
}

Karateka& Karateka::from_dojo (const std::string& dojo_name)
{
    dojo_ = dojo_name;
    return *this;
}

Karateka& Karateka::from_dojo (std::string&& dojo_name) noexcept
{
    dojo_ = std::move(dojo_name);
    return *this;
}

Karateka& Karateka::with_grade (const std::string& grade_as_string)
{
    grade_ = grade_as_string;
    return *this;
}

Karateka& Karateka::with_grade (std::string&& grade_as_string) noexcept
{
    grade_ = std::move(grade_as_string);
    return *this;
}

void Karateka::add_kata_score(float score)
{
    kata_scores_[current_kata_round_].add_kata_score(score);
}

void Karateka::add_deduction (float deduction)
{
    kata_scores_[current_kata_round_].add_deduction(deduction);
}

uint8_t Karateka::next_round()
{
    if (current_kata_round_+1 == max_number_of_kata_rounds)
    {
        throw std::length_error{"max number of kata rounds reached"};
    }
    return ++current_kata_round_;
}


float Karateka::get_maximum_score_of_round (uint8_t round) const
{
    return kata_scores_[round].get_maximum_score();
}

float Karateka::get_minimum_score_of_round (uint8_t round) const
{
    return kata_scores_[round].get_minimum_score();
}

float Karateka::get_overall_score_of_round (uint8_t round) const
{
    return kata_scores_[round].get_overall_score();
}

float Karateka::get_maximum_score_of_current_round () const
{
    return kata_scores_[current_kata_round_].get_maximum_score();
}

float Karateka::get_minimum_score_of_current_round () const
{
    return kata_scores_[current_kata_round_].get_minimum_score();
}

float Karateka::get_overall_score_of_current_round () const
{
    return kata_scores_[current_kata_round_].get_overall_score();
}

KataRoundScores::ScoreVectorType Karateka::get_scores(uint8_t round) const
{
    return kata_scores_[round].get_scores();
}

float Karateka::get_deduction (uint8_t round) const
{
    return kata_scores_[round].get_deduction();
}

static int compare_scores_for_round (const IKataPerformer& lhs,
                                  const IKataPerformer& rhs,
                                  uint8_t round)
{
    const auto lhs_score = lhs.get_overall_score_of_round(round);
    const auto rhs_score = rhs.get_overall_score_of_round(round);
    const auto tolerance = 0.01F;
    auto comparision_result = false;
    if (std::fabs(lhs_score - rhs_score) < tolerance)
    {
        const auto lhs_min_score = lhs.get_minimum_score_of_round(round);
        const auto rhs_min_score = rhs.get_minimum_score_of_round(round);
        if (std::fabs(lhs_min_score - rhs_min_score) < tolerance)
        {
            const auto lhs_max_score = lhs.get_maximum_score_of_round(round);
            const auto rhs_max_score = rhs.get_maximum_score_of_round(round);
            if (std::fabs(lhs_max_score - rhs_max_score) < tolerance)
            {
                return 0;
            }
            else
            {
                comparision_result = (lhs_max_score < rhs_max_score);
            }
        }
        else
        {
            comparision_result = (lhs_min_score < rhs_min_score);
        }
    }
    else
    {
        comparision_result = (lhs_score < rhs_score);
    }
    return (comparision_result ? 1 : -1);
}

bool kata_score_sort_greater (const IKataPerformer& lhs,
                              const IKataPerformer& rhs)
{
    for(uint8_t i = max_number_of_kata_rounds; i > 0; --i)
    {
        const uint8_t round = i-1u;
        const auto compare_result = compare_scores_for_round(lhs, rhs, round);
        if ((0 < lhs.get_overall_score_of_round(round)) || (0 < rhs.get_overall_score_of_round(round)))
        {
            return (compare_result > 0);
        }
    }
    return false;
}

bool kata_score_is_equal (const IKataPerformer& lhs,
                          const IKataPerformer& rhs)
{
    return (!kata_score_sort_greater(lhs, rhs) && !kata_score_sort_greater(rhs, lhs));
}

} // namespace
