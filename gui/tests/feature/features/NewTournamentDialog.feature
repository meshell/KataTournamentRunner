# language: en

Feature: New tournament
  As a Kata tournament operator
  I want to create a new tournament
  In order to start a tournament

Scenario: New tournament
  When I choose the "New Tournament" menu entry
  And I enter the following tournament data in the "New Tournament" dialog:
    | name  |   date     | location | 
    |  SM   | 2014-01-20 |   Kriens |
  And I submit the data

  Then the mainscreen shows the following tournament:
    | name  |   date     | location | 
    |  SM   | 2014-01-20 |   Kriens |

@needsXWindow
Scenario: New tournament using the shortcut
  When I press the "Ctrl" and the "N" key together
  And I enter the following tournament data in the "New Tournament" dialog:
    | name  |   date     | location |
    |  SM   | 2014-01-20 |   Kriens |
  And I submit the data

  Then the mainscreen shows the following tournament:
    | name  |   date     | location |
    |  SM   | 2014-01-20 |   Kriens |
