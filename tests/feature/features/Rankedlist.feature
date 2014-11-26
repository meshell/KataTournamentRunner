# language: en

Feature: Kata tournament rankedlist
  As a Tournament executor
  I want to have a rankedlist of the participants after the final round
  In order to determine the winner of the tournament

Scenario: Create the final ranked list.

  Given the following participants have performed katas:
    | name       | surname    | 1-1 | 1-2 | 1-3 | 1-4 | 1-5 | 2-1 | 2-2 | 2-3 | 2-4 | 2-5 | 3-1 | 3-2 | 3-3 | 3-4 | 3-5 |
    | Müller     | Roland     | 7.7 | 7.5 | 7.5 | 7.7 | 7.5 | 7.6 | 7.7 | 7.6 | 7.8 | 7.8 | 7.7 | 7.6 | 7.7 | 7.6 | 7.8 |
    | Paradiso   | Alessandro | 7.4 | 7.0 | 7.3 | 7.2 | 7.1 | 7.2 | 7.3 | 7.2 | 7.2 | 7.3 | 7.1 | 7.1 | 7.1 | 7.0 | 7.3 |
    | Birrer     | Simon      | 7.5 | 7.6 | 7.6 | 7.5 | 7.6 | 7.7 | 7.8 | 7.7 | 7.7 | 7.6 | 7.9 | 7.8 | 7.8 | 7.9 | 7.9 |
    | Cehajic    | Edi        | 7.5 | 7.4 | 7.5 | 7.4 | 7.4 | 7.4 | 7.5 | 7.4 | 7.6 | 7.4 | 7.4 | 7.3 | 7.4 | 7.5 | 7.4 |
    | Felder     | Luca       | 7.2 | 6.9 | 7.0 | 7.1 | 7.1 | 7.3 | 7.4 | 7.2 | 7.2 | 7.1 |     |     |     |     |     |
    | Klein      | Lorenz     | 7.3 | 7.0 | 7.2 | 7.2 | 7.0 | 7.4 | 7.2 | 7.3 | 7.3 | 7.2 | 7.2 | 7.1 | 7.2 | 7.3 | 7.2 |
    | Graziano   | Markus     | 7.3 | 7.4 | 7.2 | 7.4 | 7.4 | 7.3 | 7.3 | 7.4 | 7.3 | 7.4 | 7.4 | 7.4 | 7.3 | 7.3 | 7.4 |
    | Emmenegger | André      | 7.8 | 7.8 | 7.7 | 7.8 | 7.6 | 7.6 | 7.8 | 7.8 | 7.9 | 7.8 | 7.8 | 8.0 | 7.8 | 7.8 | 7.8 |
    | Pfändler   | Ramon      | 7.3 | 7.3 | 7.3 | 7.4 | 7.3 | 7.5 | 7.5 | 7.5 | 7.4 | 7.5 | 7.6 | 7.4 | 7.6 | 7.5 | 7.6 |
    | Grande     | Davide     | 7.3 | 7.0 | 7.1 | 7.2 | 7.2 | 7.1 | 7.3 | 7.1 | 7.4 | 7.3 |     |     |     |     |     |
    | Frey       | Dominique  | 7.1 | 7.0 | 6.9 | 7.0 | 7.2 | 7.0 | 7.1 | 7.0 | 7.3 | 6.9 |     |     |     |     |     |

  Then the rankedlist is the following:
  | rank | name       | surname    |
  |  1   | Birrer     | Simon      |
  |  2   | Emmenegger | André      |
  |  3   | Müller     | Roland     |
  |  4   | Pfändler   | Ramon      |
  |  5   | Cehajic    | Edi        |
  |  6   | Graziano   | Markus     |
  |  7   | Klein      | Lorenz     |
  |  8   | Paradiso   | Alessandro |
  |  9   | Grande     | Davide     |
  |  9   | Felder     | Luca       |
  | 11   | Frey       | Dominique  |
