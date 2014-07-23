# language: en
Feature: Persistence data
  As a Tournament Organizer
  I want to safe my tournament data persistence
  In order to continue with an earlier state

Scenario: Persistent tournament data
  Given the following Tournament is open:
    |name         |    date    | location |
    | SM          | 2013-06-20 |  Wohlen  |
  
  When I save the profile to "temp/default.ktm"
  And I restart the application
  And I open the profile "temp/default.ktm"

  Then the following tournament is opened:
    |name         |    date    | location |
    | SM          | 2013-06-20 |  Wohlen  |


Scenario: Persistent tournament participants
  Given the following Tournament is open:
    |name         |    date    | location |
    | SM          | 2013-06-20 |  Wohlen  |

  And the Tournament has the following participants:
    | name   | surname | date of birth |   dojo      | belt   |
    |  Oyama |  Mas    |  1923-07-27   |   Honbu     | 10 Dan |
    | Arneil |  Steve  |  1934-08-29   |   London    |  9 Dan |
    | Norris |  Chuck  |  1940-03-10   | Chun Kuk Do |  6 Dan |

  When I save the profile to "temp/default.ktm"
  And I restart the application
  And I open the profile "temp/default.ktm"

  Then the Tournament should have the following participants:
    | name   | surname | date of birth |   dojo      | belt   |
    |  Oyama |  Mas    |  1923-07-27   |   Honbu     | 10 Dan |
    | Arneil |  Steve  |  1934-08-29   |   London    |  9 Dan |
    | Norris |  Chuck  |  1940-03-10   | Chun Kuk Do |  6 Dan |

Scenario: Persistent tournament participants scores
  Given the following Tournament is open:
    |name         |    date    | location |
    | WM          | 2013-06-20 |  Wohlen  |

  And the Tournament has the following participants:
    | name   | surname | score 1-1 | score 1-2 | score 1-3 | score 1-4 | score 1-5 | score 2-1 | score 2-2 | score 2-3 | score 2-4 | score 2-5 |
    | Lee    |  Brian  |    5.0    |    5.2    |    5.3    |    5.2    |     5.2   |    5.8    |    5.8    |    5.7    |    5.7    |     5.5   |
    | Oyama  |  Mas    |    5.8    |    5.8    |    5.7    |    5.7    |     5.5   |    5.9    |    5.7    |    5.7    |    5.8    |     5.5   |
    | Arneil |  Steve  |    5.9    |    5.7    |    5.7    |    5.8    |     5.5   |    5.0    |    5.2    |    5.2    |    5.2    |     5.2   |
    | Jet    |  Jimi   |    5.0    |    5.2    |    5.2    |    5.2    |     5.2   |    5.2    |    5.2    |    5.4    |    5.2    |     5.2   |
    | Norris |  John   |    5.2    |    5.2    |    5.4    |    5.2    |     5.2   |    5.6    |    5.6    |    5.6    |    5.6    |     5.6   |
    | Norris |  Chuck  |    5.6    |    5.6    |    5.6    |    5.6    |     5.6   |    5.0    |    5.2    |    5.3    |    5.2    |     5.2   |

  When I save the profile to "temp/default.ktm"
  And I restart the application
  And I open the profile "temp/default.ktm"

 Then the Tournament should have the following participants:
  | name   | surname | score 1-1 | score 1-2 | score 1-3 | score 1-4 | score 1-5 | score 2-1 | score 2-2 | score 2-3 | score 2-4 | score 2-5 |
  | Lee    |  Brian  |    5.0    |    5.2    |    5.3    |    5.2    |     5.2   |    5.8    |    5.8    |    5.7    |    5.7    |     5.5   |
  | Oyama  |  Mas    |    5.8    |    5.8    |    5.7    |    5.7    |     5.5   |    5.9    |    5.7    |    5.7    |    5.8    |     5.5   |
  | Arneil |  Steve  |    5.9    |    5.7    |    5.7    |    5.8    |     5.5   |    5.0    |    5.2    |    5.2    |    5.2    |     5.2   |
  | Jet    |  Jimi   |    5.0    |    5.2    |    5.2    |    5.2    |     5.2   |    5.2    |    5.2    |    5.4    |    5.2    |     5.2   |
  | Norris |  John   |    5.2    |    5.2    |    5.4    |    5.2    |     5.2   |    5.6    |    5.6    |    5.6    |    5.6    |     5.6   |
  | Norris |  Chuck  |    5.6    |    5.6    |    5.6    |    5.6    |     5.6   |    5.0    |    5.2    |    5.3    |    5.2    |     5.2   |
