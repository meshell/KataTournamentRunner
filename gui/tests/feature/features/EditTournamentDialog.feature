# language: en

Feature: Edit tournament data
  As a Kata tournament operator
  I want to edit the tournament data
  In order to change an existing tournament

Scenario: Edit tournament
  Given the following Tournament is open:
    |name   |    date    | location |
    | SM    | 2013-06-20 |   Kriens |
  When I press the "Edit Tournament" button
  And I enter the following tournament data in the "Edit Tournament" dialog:
    | name  |   date     | location | 
    |  WM   | 2014-01-20 |   Berlin |
  And I submit the data
  Then the mainscreen shows the following tournament:
    | name  |   date     | location | 
    |  WM   | 2014-01-20 |   Berlin |

