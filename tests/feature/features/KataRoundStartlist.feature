# language: en

Feature: Kata tournament round startlist
  As a Tournament Executor
  I want to have a startlist for the next round based on the scores of the previous round and the number of participant nominated for the next round
  In order to execute a kata tournament according to the rules

Scenario: Create the startlist for the next round.
  The startlist is ordered with the highest ranked performer starting last.

  Given the following participants have performed a kata:
    | start No | name   | surname | date of birth |   dojo      | belt   |score 1 | score 2| score 3 | score 4 | score 5 |
    |     0    | Lee    |  Brian  |  1975-05-24   | Iron Dragon |  1 Kyu |  5.0   |   5.2  |   5.3   |   5.2   |    5.2  |
    |     1    | Oyama  |  Mas    |  1923-07-27   |   Honbu     | 10 Dan |  5.8   |   5.8  |   5.7   |   5.7   |    5.5  |
    |     2    | Arneil |  Steve  |  1934-08-29   |   London    |  9 Dan |  5.9   |   5.7  |   5.7   |   5.8   |    5.5  |
    |     3    | Jet    |  Jimi   |  1960-01-20   | Mushin      |  2 Kyu |  5.0   |   5.2  |   5.2   |   5.2   |    5.2  |
    |     4    | Norris |  John   |  1970-05-14   | Chun Kuk Do |  1 Dan |  5.2   |   5.2  |   5.4   |   5.2   |    5.2  |
    |     5    | Norris |  Chuck  |  1940-03-10   | Chun Kuk Do |  6 Dan |  5.6   |   5.6  |   5.6   |   5.6   |    5.6  |
  And the best 4 participants are nominated for the next round 
  When the next round starts
  Then the list contains 4 participants
  And the list of participants for the round is the following:
    | start position | rank | start No | name   | surname |
    |         1      |   4  |     4    | Norris |  John   |
    |         2      |   3  |     5    | Norris |  Chuck  |
    |         3      |   2  |     1    | Oyama  |  Mas    |
    |         4      |   1  |     2    | Arneil |  Steve  |

Scenario: Create the startlist for the next round with the fourth and fifth having the same scores.

  Given the following participants have performed a kata:
    | start No | name   | surname | date of birth |   dojo      | belt   |score 1 | score 2| score 3 | score 4 | score 5 |
    |    0     | Lee    |  Brian  |  1975-05-24   | Iron Dragon |  1 Kyu |  5.2   |   5.4  |   5.2   |   5.2   |    5.2  |
    |    1     | Oyama  |  Mas    |  1923-07-27   |   Honbu     | 10 Dan |  5.8   |   5.8  |   5.7   |   5.7   |    5.5  |
    |    2     | Arneil |  Steve  |  1934-08-29   |   London    |  9 Dan |  5.9   |   5.7  |   5.7   |   5.8   |    5.5  |
    |    3     | Jet    |  Jimi   |  1960-01-20   | Mushin      |  2 Kyu |  5.0   |   5.2  |   5.2   |   5.2   |    5.2  |
    |    4     | Norris |  John   |  1970-05-14   | Chun Kuk Do |  1 Dan |  5.2   |   5.2  |   5.4   |   5.2   |    5.2  |
    |    5     | Norris |  Chuck  |  1940-03-10   | Chun Kuk Do |  6 Dan |  5.6   |   5.6  |   5.6   |   5.6   |    5.6  |
  And the best 4 participants are nominated for the next round 
  When the next round starts
  Then the list contains 5 participants
  And the list of participants for the round is the following:
    | start position | rank |  start No | name    | surname |
    |         1      |   4  |     0     |  Lee    |  Brian  |
    |         2      |   4  |     4     |  Norris |  John   |
    |         3      |   3  |     5     |  Norris |  Chuck  |
    |         4      |   2  |     1     |  Oyama  |  Mas    |
    |         5      |   1  |     2     |  Arneil |  Steve  |

Scenario: Create the startlist for the next round with the fourth, fifth and sixth having the same scores.

  Given the following participants have performed a kata:
    | start No | name   | surname | date of birth |   dojo      | belt   |score 1 | score 2| score 3 | score 4 | score 5 |
    |    0     | Lee    |  Brian  |  1975-05-24   | Iron Dragon |  1 Kyu |  5.2   |   5.4  |   5.2   |   5.2   |    5.2  |
    |    1     | Oyama  |  Mas    |  1923-07-27   |   Honbu     | 10 Dan |  5.8   |   5.8  |   5.7   |   5.7   |    5.5  |
    |    2     | Arneil |  Steve  |  1934-08-29   |   London    |  9 Dan |  5.9   |   5.7  |   5.7   |   5.8   |    5.5  |
    |    3     | Jet    |  Jimi   |  1960-01-20   | Mushin      |  2 Kyu |  5.4   |   5.2  |   5.2   |   5.2   |    5.2  |
    |    4     | Norris |  John   |  1970-05-14   | Chun Kuk Do |  1 Dan |  5.2   |   5.2  |   5.4   |   5.2   |    5.2  |
    |    5     | Norris |  Chuck  |  1940-03-10   | Chun Kuk Do |  6 Dan |  5.6   |   5.6  |   5.6   |   5.6   |    5.6  |
  And the best 4 participants are nominated for the next round 
  When the next round starts
  Then the list contains 6 participants
  And the list of participants for the round is the following:
    | start position | rank |  start No | name    | surname |
    |         1      |   4  |      0    |  Lee    |  Brian  |
    |         2      |   4  |      3    |  Jet    |  Jimi   |
    |         3      |   4  |      4    |  Norris |  John   |
    |         4      |   3  |      5    |  Norris |  Chuck  |
    |         5      |   2  |      1    |  Oyama  |  Mas    |
    |         6      |   1  |      2    |  Arneil |  Steve  |

Scenario: Create the startlist for the third round.
  The startlist for the third round is based on the scores of the second round only.

  Given the following participants performed a kata in the 1st round:
    | start No | name      | surname     | date of birth |   dojo      | belt   |score 1 | score 2| score 3 | score 4 | score 5 |
    |     0    | Lee       |  Brian      |  1975-05-24   | Iron Dragon |  1 Kyu |  5.0   |   5.1  |   5.3   |   5.2   |    5.2  |
    |     1    | Oyama     |  Mas        |  1923-07-27   |   Honbu     | 10 Dan |  5.8   |   5.8  |   5.7   |   5.7   |    5.5  |
    |     2    | Arneil    |  Steve      |  1934-08-29   |   London    |  9 Dan |  5.9   |   5.7  |   5.7   |   5.8   |    5.5  |
    |     3    | Jet       |  Jimi       |  1960-01-20   | Mushin      |  2 Kyu |  5.0   |   5.2  |   5.2   |   5.2   |    5.2  |
    |     4    | Norris    |  John       |  1970-05-14   | Chun Kuk Do |  1 Dan |  5.2   |   5.2  |   5.4   |   5.2   |    5.2  |
    |     5    | Norris    |  Chuck      |  1940-03-10   | Chun Kuk Do |  6 Dan |  5.6   |   5.6  |   5.6   |   5.6   |    5.6  |
    |     6    | Lundgren  |  Dolph      |  1957-11-03   | Stockholm   |  3 Dan |  5.0   |   5.0  |   5.0   |   5.0   |    5.0  |
    |     7    | Van Damme | Jean-Claude |  1960-10-18   | CNDK        |  1 Dan |  4.8   |   4.9  |   5.0   |   5.0   |    5.1  |
  And the following participants performed a kata in the 2nd round:
    | start No | name      | surname     | date of birth |   dojo      | belt   |score 1 | score 2| score 3 | score 4 | score 5 |
    |     0    | Lee       |  Brian      |  1975-05-24   | Iron Dragon |  1 Kyu |  5.5   |   5.4  |   5.5   |   5.4   |    5.3  |
    |     3    | Jet       |  Jimi       |  1960-01-20   | Mushin      |  2 Kyu |  5.0   |   5.2  |   5.2   |   5.2   |    5.2  |
    |     4    | Norris    |  John       |  1970-05-14   | Chun Kuk Do |  1 Dan |  5.2   |   5.2  |   5.4   |   5.4   |    5.1  |
    |     5    | Norris    |  Chuck      |  1940-03-10   | Chun Kuk Do |  6 Dan |  5.9   |   5.9  |   5.9   |   5.9   |    5.9  |
    |     1    | Oyama     |  Mas        |  1923-07-27   |   Honbu     | 10 Dan |  5.7   |   5.7  |   5.7   |   5.7   |    5.7  |
    |     2    | Arneil    |  Steve      |  1934-08-29   |   London    |  9 Dan |  5.7   |   5.8  |   5.8   |   5.8   |    5.9  |

  And the best 4 participants are nominated for the next round
  When the next round starts
  Then the list contains 4 participants
  And the list of participants for the round is the following:
    | start position | rank | start No | name   | surname |
    |         1      |   4  |     0    | Lee    |  Brian  |
    |         2      |   3  |     1    | Oyama  |  Mas    |
    |         3      |   2  |     2    | Arneil |  Steve  |
    |         4      |   1  |     5    | Norris |  Chuck  |




