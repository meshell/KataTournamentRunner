# language: en

Feature: Kata tournament rankedlist
  As a Kata tournament executor
  I want to have a rankedlist of the participants after the final round
  In order to determine the winner of the tournament

Scenario: Create the final ranked list.
  Given a tournaments is open
  And the following participants have performed katas:
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

  Then the following table is shown on the finalpage:
  | rank | name       | surname    | overall 1 | min 1 | max 1 | overall 2 | min 2 | max 2 | overall 3 | min 3 | max 3 |
  |  1   | Birrer     | Simon      |    22.7   |  7.5  |  7.6  |   23.1    |  7.6  |  7.8  |   23.6    |  7.8  |  7.9  |
  |  2   | Emmenegger | André      |    23.3   |  7.6  |  7.8  |   23.4    |  7.6  |  7.9  |   23.4    |  7.8  |  8.0  |
  |  3   | Müller     | Roland     |    22.7   |  7.5  |  7.7  |   23.1    |  7.6  |  7.8  |   23.0    |  7.6  |  7.8  |
  |  4   | Pfändler   | Ramon      |    21.9   |  7.3  |  7.4  |   22.5    |  7.4  |  7.5  |   22.7    |  7.4  |  7.6  |
  |  5   | Cehajic    | Edi        |    22.3   |  7.4  |  7.5  |   22.3    |  7.4  |  7.6  |   22.2    |  7.3  |  7.5  |
  |  6   | Graziano   | Markus     |    22.1   |  7.2  |  7.4  |   22.0    |  7.3  |  7.4  |   22.1    |  7.3  |  7.4  |
  |  7   | Klein      | Lorenz     |    21.4   |  7.0  |  7.3  |   21.8    |  7.2  |  7.4  |   21.6    |  7.1  |  7.3  |
  |  8   | Paradiso   | Alessandro |    21.6   |  7.0  |  7.4  |   21.7    |  7.2  |  7.3  |   21.3    |  7.0  |  7.3  |
  |  9   | Grande     | Davide     |    21.5   |  7.0  |  7.3  |   21.7    |  7.1  |  7.4  |           |       |       |
  |  9   | Felder     | Luca       |    21.2   |  6.9  |  7.2  |   21.7    |  7.1  |  7.4  |           |       |       |
  | 11   | Frey       | Dominique  |    21.1   |  6.9  |  7.2  |   21.1    |  6.9  |  7.3  |           |       |       |
