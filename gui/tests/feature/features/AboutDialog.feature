# language: en

Feature: About Dialog
  As a user of the Kata Tournament Runner
  I want to have an About dialog
  In order get infos about the version

Scenario: Open the dialog using the menu
  When I choose the "About" menu entry
  Then the "About" dialog is shown

@needsXWindow
Scenario: Open the dialog using the shortcut
  When I press the "Alt" and the "F1" key together
  Then the "About" dialog is shown

