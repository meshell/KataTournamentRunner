#include "tournament_runner/tournament.h"

#include <algorithm>

namespace TournamentRunner
{

Tournament::Tournament (TournamentData data) :
    data_{std::move(data)},
    participants_{}
{

}

std::string Tournament::date_as_string () const
{
    return data_.date_.to_iso_extended_string();
}

void Tournament::add_participant (const Karateka& participant)
{
    auto tmp_participant = participant;
    tmp_participant.set_startnumber(participants_.size());
    participants_.push_back(tmp_participant);
}


void Tournament::remove_participant(size_t start_number)
{
    participants_.erase(
                std::remove_if(
                    std::begin(participants_),
                    std::end(participants_),
                    [start_number](const Karateka& karateka)
                            { return start_number == karateka.get_startnumber(); }),
                std::end(participants_));
    reassign_startnumbers();
}

std::vector<Karateka> Tournament::get_ranked_list_of_participants () const
{
    auto ranked_list = std::vector<Karateka>{};
    ranked_list.assign(std::begin(participants_), std::end(participants_));
    std::stable_sort(std::begin(ranked_list), std::end(ranked_list), &TournamentRunner::kata_score_sort_greater);
    std::reverse(std::begin(ranked_list), std::end(ranked_list));
    return ranked_list;
}

std::vector<Tournament::RankAnnotatedParticipant> Tournament::get_anotated_ranked_list_of_participants() const
{
    auto ranklist = std::vector<Tournament::RankAnnotatedParticipant>{};
    auto rankedlist = get_ranked_list_of_participants();
    if(rankedlist.empty())
    {
        return ranklist;
    }
    size_t rank = 1U;
    ranklist.push_back(std::make_pair(rank, rankedlist.front()));
    rankedlist.erase(std::begin(rankedlist));
    for (const auto& participant : rankedlist)
    {
        auto next_rank = ++rank;
        const auto& last_ranked_participant = std::get<rank_annotated_participant_karateka_idx>(ranklist.back());
        if(kata_score_is_equal(last_ranked_participant, participant))
        {
            next_rank = std::get<rank_annotated_participant_rank_idx>(ranklist.back());
        }
        ranklist.push_back(std::make_pair(next_rank, participant));
    }
    return ranklist;
}


uint8_t Tournament::start_next_kata_round ()
{
    std::for_each(std::begin(participants_),
                  std::end(participants_),
                  [](TournamentRunner::Karateka& participant)
                  {
                      participant.next_round();
                  });
    return ++current_kata_round_;
}


std::vector<size_t> Tournament::get_startlist_for_next_kata_round (size_t no_of_participants_for_round) const
{
    auto start_list = get_list_of_participants_for_next_kata_round(no_of_participants_for_round);
    std::reverse(std::begin(start_list), std::end(start_list));
    return start_list;
}

std::vector<size_t> Tournament::get_list_of_participants_for_next_kata_round (size_t no_of_participants_for_round) const
{
    auto ranked_list = get_ranked_list_of_participants();
    auto next_round_list = std::vector<Karateka>{};
    const auto no_of_participants_in_next_round = static_cast<long int>(std::min(no_of_participants_for_round, ranked_list.size()));
    std::copy_n(std::begin(ranked_list), no_of_participants_in_next_round, std::back_inserter(next_round_list));

    const auto copy_if_predicate = [&next_round_list](Karateka next)
    {
        return kata_score_is_equal(next, next_round_list.back());
    };

    // The participants with same score as the last one of the startlict calculated before
    // can also participate in the next round
    std::copy_if(std::begin(ranked_list) + no_of_participants_in_next_round,
                 std::end(ranked_list),
                 std::back_inserter(next_round_list),
                 copy_if_predicate);

    auto next_round_startnumbers_list = std::vector<size_t>{};
    std::for_each(std::begin(next_round_list), std::end(next_round_list),
                  [&next_round_startnumbers_list](const Karateka& karateka)
                  {
                      next_round_startnumbers_list.emplace_back(karateka.get_startnumber());
                  });

    return next_round_startnumbers_list;
}


void Tournament::reassign_startnumbers ()
{
    auto startnumber = 0u;
    for (auto& participant : participants_)
    {
        participant.set_startnumber(startnumber++);
    }
}

} // namespace TournamentRunner
