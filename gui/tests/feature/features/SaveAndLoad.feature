# language: en

Feature: Save and Load profiles
  As a user of the Karate Tournament Manager
  I want to save a profile, i.e. a tournament and load it later
  In order to preserve previous work

Scenario: Save and load to/from a default profile
  Given the following Tournament is open:
    |name   |    date    | location |
    | SM    | 2013-06-20 |   Kriens |
  When I save the profile
  And I restart the application
  Then the mainscreen shows the following tournament:
    | name  |   date     | location | 
    | SM    | 2013-06-20 |   Kriens | 

@shortcut
Scenario: Save and load to/from a default profile using the shortcut
  Given the following Tournament is open:
    |name   |    date    | location |
    | SM    | 2013-06-20 |   Kriens |
  When I press the "Ctrl" and the "S" key together
  And I restart the application
  Then the mainscreen shows the following tournament:
    | name  |   date     | location |
    | SM    | 2013-06-20 |   Kriens |

Scenario Outline: Save to a profile file
  Given the following Tournament is open:
    |name   |    date    | location |
    | SM    | 2013-06-20 |   Kriens |
  When I save the profile to "<file>"
  Then the file "<file>" exists

Examples:
  |      file           |
  | ~/tmp/myprofile.ktr |

@shortcut
Scenario Outline: Save to a profile file using the shortcut
  Given the following Tournament is open:
    |name   |    date    | location |
    | SM    | 2013-06-20 |   Kriens |
  When I press the "Ctrl" the "Shift" and the "S" key together
  And save the profile to "<file>"
  Then the file "<file>" exists

Examples:
  |      file           |
  | ~/tmp/myprofile.ktr |


# TODO make this test work. It works in real application
@wip
Scenario Outline: Load from a profile file
  Given the following Tournament was saved to profile "<file>":
    |name   |    date    | location |
    | WM    | 2014-06-20 |   London |
  When I restart the application
  And I load the profile "<file>"
  Then the mainscreen shows the following tournament:
    | name  |   date     | location |
    | WM    | 2014-06-20 |   London |

Examples:
  |      file           |
  | ~/tmp/myprofile.ktr |
