#ifndef TOURNAMENT_RUNNER_KATA_PERFORMER_INTERFACE_H_
#define TOURNAMENT_RUNNER_KATA_PERFORMER_INTERFACE_H_

#include <string>
#include <cstdint>


namespace TournamentRunner
{

/**
 * @brief The IKataPerformer interface
 * @details Defines the interface of a karateka participating at a kata tournament
 */
class IKataPerformer
{
public:
    virtual ~IKataPerformer() = default;

    /**
     * @brief Add a kata score for the current round
     * @param score  The score given
     * @return the number of scores already added for this round
     */
    virtual uint32_t add_kata_score (float score) = 0;

    /**
     * @brief Add a deduction, which is subtracted from the overall score
     * @param deduction Deduction score
     * @return
     */
    virtual void add_deduction (float deduction) = 0;

    /**
     * @brief  Move to next round
     * @return the round number of the current round (after update)
     */
    virtual uint8_t next_round () = 0;

    /**
     * @brief Return the maximum score of the given round
     * @param round The round we want the maximum score from
     * @return the maximum score
     */
    virtual float get_maximum_score_of_round (uint8_t round) const = 0;

    /**
     * @brief Return the minimum score of the given round
     * @param round The round we want the minimum score from
     * @return the minimum score
     */
    virtual float get_minimum_score_of_round (uint8_t round) const = 0;

    /**
     * @brief Return the overall score of the given round
     * @param round The round we want the overall score from
     * @return the overall score
     */
    virtual float get_overall_score_of_round (uint8_t round) const = 0;

    /**
     * @brief Return the maximum score of the current round
     * @return the maximum score of the current round
     */
    virtual float get_maximum_score_of_current_round () const = 0;

    /**
     * @brief Return the minimum score of the current round
     * @return the minimum score of the current round
     */
    virtual float get_minimum_score_of_current_round () const = 0;

    /**
     * @brief Return the overall score of the current round
     * @return the overall score of the current round
     */
    virtual float get_overall_score_of_current_round () const = 0;

};

} //namespace TournamentRunner

#endif // TOURNAMENT_RUNNER_KATA_PERFORMER_INTERFACE_H_
