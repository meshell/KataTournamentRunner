# language: en

Feature: Add participant Dialog
  As a Kata tournament operator
  I want to have a dialog to enter participant data
  In order to add participant to a tournament

Scenario: Add a participant
  Given a tournaments is open
  When I press the "Add Participant" button
  And I enter the following participant data in the "Add Participant" dialog:
  | name   | surname | date of birth |   dojo      |  rank    |
  | Lee    |  Brian  |  1975-05-24   | Iron Dragon |  1st Kyu |

  And I submit the data

  Then the participant list contains the following participants:
  | name   | surname | date of birth |   dojo      |   rank   |
  | Lee    |  Brian  |  1975-05-24   | Iron Dragon |  1st Kyu |


