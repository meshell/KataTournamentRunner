#include "tournament_runner/kataround_scores.h"

#include <algorithm>
#include <numeric>
#include <cmath>

namespace TournamentRunner
{

void KataRoundScores::add_kata_score (float score)
{
    if (current_score_ < number_of_kata_scores_per_round)
    {
        kata_scores_[current_score_++] = score;
    }
    else
    {
        throw std::length_error("max number of scores per round reached");
    }
}

void KataRoundScores::add_deduction (float deduction)
{
    deduction_ = deduction;
}

float KataRoundScores::get_overall_score () const
{
    const auto init_value = 0.0F;
    return std::accumulate(std::begin(kata_scores_), std::end(kata_scores_), init_value)
            - get_maximum_score()
            - get_minimum_score()
            - deduction_;
}

float KataRoundScores::get_maximum_score () const
{
    return (*std::max_element(std::begin(kata_scores_), std::end(kata_scores_)));
}

float KataRoundScores::get_minimum_score () const
{
    const auto first_zero_entry = std::find_if_not(std::begin(kata_scores_),
                                                std::end(kata_scores_),
                                                [](float v){return v > 0.0; });
    return (*std::min_element(std::begin(kata_scores_), first_zero_entry));
}

std::vector<float> KataRoundScores::get_scores () const
{
    std::vector<float> scores;
    scores.assign(std::begin(kata_scores_), std::end(kata_scores_));
    return scores;
}

float KataRoundScores::get_deduction() const
{
    return deduction_;
}

} // namespace
