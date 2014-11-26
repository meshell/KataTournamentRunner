# language: en

Feature: Delete Participant
  As a Kata tournament operator
  I want to delete an already entered participant
  In order to unregister participants which can not attend

Scenario: Delete a participant

  Given a tournaments is open
  And the Tournament has the following participants:
    | name   | surname | date of birth |   dojo      | rank   |
    |  Oyama |  Mas    |  1923-07-27   |   Honbu     | 10 Dan |
    | Arneil |  Steve  |  1934-08-29   |   London    |  9 Dan |
    | Norris |  Chuck  |  1940-03-10   | Chun Kuk Do |  6 Dan |
  And the mainpage is shown
  When I select the 2nd row
  And I press the "Delete" key
  Then the participant list contains the following participants:
    | name   | surname | date of birth |   dojo      | rank   |
    |  Oyama |  Mas    |  1923-07-27   |   Honbu     | 10 Dan |
    | Norris |  Chuck  |  1940-03-10   | Chun Kuk Do |  6 Dan |
