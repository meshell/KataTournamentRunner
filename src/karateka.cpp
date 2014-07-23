#include "tournament_runner/karateka.h"

#include <algorithm>
#include <numeric>
#include <cmath>

#include "tournament_runner/tournament.h"

namespace TournamentRunner
{

Karateka& Karateka::with_name (const std::string& name)
{
    name_ = name;
    return *this;
}

Karateka& Karateka::with_name (std::string&& name) noexcept
{
    name_ = std::move(name);
    return *this;
}

Karateka& Karateka::with_surname (const std::string& surname)
{
    surname_ = surname;
    return *this;
}

Karateka& Karateka::with_surname (std::string&& surname) noexcept
{
    surname_ = std::move(surname);
    return *this;
}

Karateka& Karateka::with_birthdate (const Date& date_of_birth)
{
    date_of_birth_ = date_of_birth;
    return *this;
}

Karateka& Karateka::with_birthdate (Date&& date_of_birth) noexcept
{
    date_of_birth_ = std::move(date_of_birth);
    return *this;
}

Karateka& Karateka::with_birthdate (const std::string& date_of_birth)
{
    date_of_birth_ = Date(date_of_birth);
    return *this;
}

Karateka& Karateka::from_dojo (const std::string& dojo)
{
    dojo_ = dojo;
    return *this;
}

Karateka& Karateka::from_dojo (std::string&& dojo) noexcept
{
    dojo_ = std::move(dojo);
    return *this;
}

Karateka& Karateka::with_rank (const std::string& rank)
{
    rank_ = rank;
    return *this;
}

Karateka& Karateka::with_rank (std::string&& rank) noexcept
{
    rank_ = std::move(rank);
    return *this;
}

uint32_t Karateka::add_kata_score (float score)
{
    if (number_of_kata_scores_per_round > kata_scores_[kata_round_].size())
    {
        kata_scores_[kata_round_].push_back(score);
    }
    else
    {
        throw std::length_error("max number of scores per round reached");
    }

    return kata_scores_[kata_round_].size();
}

void Karateka::add_deduction (float deduction)
{
    deductions_[kata_round_] = deduction;
}

uint8_t Karateka::next_round()
{
    if (max_number_of_kata_rounds > kata_round_+1)
    {
        if (0 == kata_scores_[kata_round_].size())
        {
            kata_scores_[kata_round_].assign(number_of_kata_scores_per_round, 0.0F);
        }

        ++kata_round_;
    }
    else
    {
        throw std::length_error{"max number of kata rounds reached"};
    }
    return kata_round_;
}


float Karateka::get_maximum_score_of_round (uint8_t round) const
{
    auto maximum_score = 0.0F;
    if (!kata_scores_[round].empty())
    {
        maximum_score = (*std::max_element(std::begin(kata_scores_[round]), std::end(kata_scores_[round])));
    }
    return maximum_score;
}

float Karateka::get_minimum_score_of_round (uint8_t round) const
{
    auto minimum_score = 0.0F;
    if (round >= 0 && !kata_scores_[round].empty())
    {
        minimum_score = (*std::min_element(std::begin(kata_scores_[round]), std::end(kata_scores_[round])));
    }
    return minimum_score;
}

float Karateka::get_overall_score_of_round (uint8_t round) const
{
    const auto init_value = 0.0F;
    auto overall_score = 0.0F;
    if (!kata_scores_[round].empty())
    {
        overall_score = std::accumulate(std::begin(kata_scores_[round]), std::end(kata_scores_[round]), init_value)
                        - get_maximum_score_of_round (round)
                        - get_minimum_score_of_round(round)
                        - deductions_[round];
    }
    return overall_score;
}

float Karateka::get_maximum_score_of_current_round () const
{
    return get_maximum_score_of_round(kata_round_);
}

float Karateka::get_minimum_score_of_current_round () const
{
    return get_minimum_score_of_round(kata_round_);
}

float Karateka::get_overall_score_of_current_round () const
{
    return get_overall_score_of_round(kata_round_);
}

std::vector<float> Karateka::get_scores (uint8_t round) const
{
    return kata_scores_.at(round);
}

float Karateka::get_deduction(uint8_t round) const
{
    return deductions_.at(round);
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
    for(auto round = max_number_of_kata_rounds-1; round >= 0; --round)
    {
        const auto compare_result = compare_scores_for_round(lhs, rhs, round);
        if ((0 != lhs.get_overall_score_of_round(round)) || (0 != rhs.get_overall_score_of_round(round)))
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
