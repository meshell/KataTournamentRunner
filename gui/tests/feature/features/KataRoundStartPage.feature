# language: en

Feature: Kata Round Start Page
  As a user of the Karate Tournament Manager
  I want to have a startpage for each new kata round
  In order to have an overview on the startlist

Scenario: First round start page

  Given a tournaments is open
  And the Tournament has the following participants:
    | name   | surname | date of birth |   dojo      | belt   |
    |  Oyama |  Mas    |  1923-07-27   |   Honbu     | 10 Dan |
    | Arneil |  Steve  |  1934-08-29   |   London    |  9 Dan |
    | Norris |  Chuck  |  1940-03-10   | Chun Kuk Do |  6 Dan |
  And the mainpage is shown
  When I press the "Start" button
  Then a startpage for the first round is shown
  And the following partipants are shown on the startpage:
    | start position | name                        |
    |     1          | Oyama, Mas (Honbu)          |
    |     2          | Arneil, Steve (London)      |
    |     3          | Norris, Chuck (Chun Kuk Do) |

Scenario: Second round start page

  Given a tournaments is open
  And the following participants have performed a kata:
    | start No | name     | surname | score 1 | score 2| score 3 | score 4 | score 5 |
    |     0    | Lee      |  Brian  |   5.0   |   5.2  |   5.3   |   5.2   |    5.2  |
    |     1    | Oyama    |  Mas    |   5.8   |   5.8  |   5.7   |   5.7   |    5.5  |
    |     2    | Arneil   |  Steve  |   5.9   |   5.7  |   5.7   |   5.8   |    5.5  |
    |     3    | Jet      |  Jimi   |   5.0   |   5.2  |   5.2   |   5.2   |    5.2  |
    |     4    | Norris   |  John   |   5.2   |   5.2  |   5.4   |   5.2   |    5.2  |
    |     5    | Norris   |  Chuck  |   5.6   |   5.6  |   5.6   |   5.6   |    5.6  |
    |     6    | Oyama    | Shigeru |   5.8   |   5.8  |   5.8   |   5.8   |    5.8  |
    |     7    | Nakamura | Tadashi |   5.6   |   5.7  |   5.7   |   5.6   |    5.6  |
    |     8    | Lundgren | Hans    |   5.4   |   5.4  |   5.4   |   5.4   |    5.4  |
    |     9    | DaCosta  | Nick    |   5.5   |   5.4  |   5.5   |   5.4   |    5.5  |
  When the startpage for the second round is shown
  Then the following partipants are shown on the startpage:
    | start position | name              |
    |   1            | Norris, John      |
    |   2            | Lundgren, Hans    |
    |   3            | DaCosta, Nick     |
    |   4            | Norris, Chuck     |
    |   5            | Nakamura, Tadashi |
    |   6            | Oyama, Mas        |
    |   7            | Arneil, Steve     |
    |   8            | Oyama, Shigeru    |

