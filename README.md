[![Build Status](https://travis-ci.org/meshell/KataTournamentRunner.png)](https://travis-ci.org/meshell/KataTournamentRunner)
[![Coverage Status](https://coveralls.io/repos/meshell/KataTournamentRunner/badge.svg?branch=master)](https://coveralls.io/r/meshell/KataTournamentRunner?branch=master)
[![coverity Status](https://scan.coverity.com/projects/2824/badge.svg)](https://scan.coverity.com/projects/2824)

KataTournamentRunner
====================

IFK Switzerland Kata Tournament Runner


# Dependencies
----------------
To build the project you need a recent compiler supporting C++11 features, e.g. gcc version >= 4.7, clang >= 3.5 or Microsoft Visual Studio 2013 Update 4 (MSVC12) and Qt >= 5.2 installed.

Building the "Kata Tournament Runner" requires CMake (http://www.cmake.org/) Version 2.8. See CMake documentation for more information about building using CMake.

Building the Unittests requires google mock (http://code.google.com/p/googlemock/) and google test (http://code.google.com/p/googletest/).

Building the feature tests (BDD style tests using Gerkhin) requires Cucumber-cpp (https://github.com/cucumber/cucumber-cpp) by Paolo Ambrosio and Boost (regex, filesystem, date_time, chrono, thread, system). 

See the Cucumber-cpp documentation on how to run the feature tests with cucumber.


Documentation
---------------
See GitHub page (http://meshell.github.io/KataTournamentRunner/) for more documentation



