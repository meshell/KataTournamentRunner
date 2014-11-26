# language: en

Feature: Kata tournament round ranking
  As a Tournament executor
  I want to have the points given for a kata summed up for every performance
  In order to do a ranking

Scenario Outline: Calculate the score for a kata.
    The maximum and minimum scores awarded are deleted. The three remaining scores are added together to calculate the score.

  Given a participant has performed a kata

  When a referee gives score <score 1>
  And a referee gives score <score 2>
  And a referee gives score <score 3>
  And a referee gives score <score 4>
  And a referee gives score <score 5>

  Then the sum of points given should be <sum>
  And the maximum score canceled should be <maximum>
  And the minimum score canceled should be <minimum>

  Examples:
    |score 1 | score 2| score 3 | score 4 | score 5 |  sum  | maximum | minimum |
    |  5.8   |   5.6  |   5.6   |   5.7   |    5.5  | 16.9  |   5.8   |   5.5   |
    |  5.9   |   5.9  |   5.9   |   5.9   |    5.5  | 17.7  |   5.9   |   5.5   |
    |  5.9   |   5.9  |   5.9   |   5.9   |    5.9  | 17.7  |   5.9   |   5.9   |
    |  5.3   |   5.3  |   5.4   |   5.3   |    5.3  | 15.9  |   5.4   |   5.3   |
    |  5.0   |   5.1  |   5.0   |   5.0   |    5.1  | 15.1  |   5.1   |   5.0   |

Scenario: Calculate the rank per round. If scores are equal the larger canceled mimimum score is first taken into account, and
  if they are the same the larger canceled maximum score is taken into account.

  Given the following participants have performed a kata:
    |start No | name   | surname | date of birth |   dojo      | belt   |score 1 | score 2| score 3 | score 4 | score 5 |
    |    0    | Lee    |  Brian  |  1975-05-24   | Iron Dragon |  1 Kyu |  5.0   |   5.2  |   5.3   |   5.2   |    5.2  |
    |    1    | Oyama  |  Mas    |  1923-07-27   |   Honbu     | 10 Dan |  5.8   |   5.8  |   5.7   |   5.7   |    5.5  |
    |    2    | Arneil |  Steve  |  1934-08-29   |   London    |  9 Dan |  5.9   |   5.7  |   5.7   |   5.8   |    5.5  |
    |    3    | Jet    |  Jimi   |  1960-01-20   | Mushin      |  2 Kyu |  5.0   |   5.2  |   5.2   |   5.2   |    5.2  |
    |    4    | Norris |  John   |  1970-05-14   | Chun Kuk Do |  1 Dan |  5.2   |   5.2  |   5.4   |   5.2   |    5.2  |
    |    5    | Norris |  Chuck  |  1940-03-10   | Chun Kuk Do |  6 Dan |  5.6   |   5.6  |   5.6   |   5.6   |    5.6  |

  When the ranks for a round are caclulated
  Then the ranked list should be the following:
    |start No | rank  | name   | surname | date of birth |   dojo      | belt   |  sum   | maximum | minimum |
    |    2    |  1    | Arneil |  Steve  |  1934-08-29   |   London    |  9 Dan |  17.2  |   5.9   |   5.5   |
    |    1    |  2    | Oyama  |  Mas    |  1923-07-27   |   Honbu     | 10 Dan |  17.2  |   5.8   |   5.5   |
    |    5    |  3    | Norris |  Chuck  |  1940-03-10   | Chun Kuk Do |  6 Dan |  16.8  |   5.6   |   5.6   |
    |    4    |  4    | Norris |  John   |  1970-05-14   | Chun Kuk Do |  1 Dan |  15.6  |   5.4   |   5.2   |
    |    0    |  5    | Lee    |  Brian  |  1975-05-24   | Iron Dragon |  1 Kyu |  15.6  |   5.3   |   5.0   |
    |    3    |  6    | Jet    |  Jimi   |  1960-01-20   | Mushin      |  2 Kyu |  15.6  |   5.2   |   5.0   |

Scenario Outline: Calculate the score for a kata with an overall deduction.
    The maximum and minimum scores awarded are deleted. The three remaining scores are added together and the deductions substracted to calculate the score.

  Given a participant has performed a kata

  When a referee gives score <score 1>
  And a referee gives score <score 2>
  And a referee gives score <score 3>
  And a referee gives score <score 4>
  And a referee gives score <score 5>
  And the overall deduction is <deduction>

  Then the sum of points given should be <sum>
  And the maximum score canceled should be <maximum>
  And the minimum score canceled should be <minimum>

  Examples:
    |score 1 | score 2| score 3 | score 4 | score 5 | deduction | sum   | maximum | minimum |
    |  5.8   |   5.6  |   5.6   |   5.7   |    5.5  |    1.0    | 15.9  |   5.8   |   5.5   |
    |  5.9   |   5.9  |   5.9   |   5.9   |    5.5  |    0.5    | 17.2  |   5.9   |   5.5   |
    |  5.9   |   5.9  |   5.9   |   5.9   |    5.9  |    1.5    | 16.2  |   5.9   |   5.9   |
    |  5.3   |   5.3  |   5.4   |   5.3   |    5.3  |    1.0    | 14.9  |   5.4   |   5.3   |
    |  5.0   |   5.1  |   5.0   |   5.0   |    5.1  |    0.0    | 15.1  |   5.1   |   5.0   |


 




