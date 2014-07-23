# language: en
Feature: Setup Tournament
  As a Tournament Organizer
  I want to setup a Tournament
  In order to get started

Scenario: Create a new Tournament with a name and date provided only
  Given no tournament is open
  When I create the following Tournament:
    |name |    date   |
    | SM  | 2013-06-20|

  Then the following tournament is opened:
    |name |   date    |
    | SM  | 2013-06-20|


Scenario: Create a new Tournament with location specified
  Given no tournament is open
  When I create the following Tournament:
    |name         |    date    | location |
    | SM          | 2013-06-20 |  Wohlen  |

  Then the following tournament is opened:
    |name         |    date    | location |
    | SM          | 2013-06-20 |  Wohlen  |
    
Scenario: Register an attendee to the tournament
  Given a Tournament is open

  When I add the following participants to the tournament:
    | name   | surname | date of birth |   dojo      | belt   |
    |  Oyama |  Mas    |  1923-07-27   |   Honbu     | 10 Dan |
    | Arneil |  Steve  |  1934-08-29   |   London    |  9 Dan |
    | Norris |  Chuck  |  1940-03-10   | Chun Kuk Do |  6 Dan |

  Then the number of participants for the tournament should be 3


