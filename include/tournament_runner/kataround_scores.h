#ifndef TOURNAMENT_RUNNER_KATAROUND_SCORES_H_
#define TOURNAMENT_RUNNER_KATAROUND_SCORES_H_

#include <cstdint>
#include <vector>
#include <array>

#include <boost/serialization/access.hpp>
#include <boost/serialization/array.hpp>

namespace TournamentRunner
{

/**
 * @brief Number of kata scores per round (5)
 */
const uint8_t number_of_kata_scores_per_round = 5;

class KataRoundScores
{
public:
   void add_kata_score (float score);
   void add_deduction (float deduction);
   float get_overall_score () const;
   float get_maximum_score () const;
   float get_minimum_score () const;
   std::vector<float> get_scores () const;
   float get_deduction () const;

private:
   friend class boost::serialization::access;

   /**
    * @brief Serialization function (boost::serialization)
    */
   template<class Archive>
   void serialize (Archive& archive, const unsigned int version);

   std::array<float, number_of_kata_scores_per_round> kata_scores_;
   float deduction_{};
   uint8_t current_score_{};
};

template<class Archive>
void KataRoundScores::serialize (Archive& archive,
                          const unsigned int)
{
    archive & kata_scores_;
    archive & deduction_;
    archive & current_score_;
}

} //namespace TournamentRunner

#endif // TOURNAMENT_RUNNER_KATAROUND_SCORES_H_
