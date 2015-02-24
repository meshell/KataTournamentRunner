# language: en

Feature: Edit Participant
  As a Kata tournament operator
  I want to edit the data of a participant
  In order to modify incorrect data or enter missing data

@wip
Scenario: Edit a participant with doubleclick

  Given a tournaments is open
  And the Tournament has the following participants:
    | name   | surname | date of birth |   dojo      | rank   |
    |  Oyama |  Mas    |  1923-07-27   |   Honbu     | 10 Dan |
    | Arneil |  Stev   |  1934-08-29   |   London    |  1 Dan |
    | Norris |  Chuck  |  1940-03-10   | Chun Kuk Do |  6 Dan |
  And the mainpage is shown
  When I doubleclick the 2nd row
  And I enter the following participant data in the "Edit Participant" dialog:
    | name   | surname | date of birth |   dojo      | rank   |
    | Arneil |  Steve  |  1934-08-29   |   London    |  9 Dan |
  And I submit the data
  Then the participant list contains the following participants:
    | name   | surname | date of birth |   dojo      | rank   |
    |  Oyama |  Mas    |  1923-07-27   |   Honbu     | 10 Dan |
    | Arneil |  Steve  |  1934-08-29   |   London    |  9 Dan |
    | Norris |  Chuck  |  1940-03-10   | Chun Kuk Do |  6 Dan |
