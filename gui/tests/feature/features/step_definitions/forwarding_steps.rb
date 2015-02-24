When(/^I save the profile$/) do
  step "I choose the \"Save\" menu entry"
end

When(/^I submit the data$/) do
  step "I press the \"Enter\" key"
end

Given(/^the following Tournament was saved to profile "(.*?)":$/) do |file, table|
  step "the following Tournament is open:", table
  step "I save the profile to \"#{file}\""
end

When(/^I save the profile to "(.*?)"$/) do |file|
    step "I choose the \"Save As\" menu entry"
    step "save the profile to \"#{file}\""
end

When(/^I enter the following tournament data in the "(.*?)" dialog:$/) do |dialog, table|
    step "I enter the \"#{dialog}\" dialog"
    step "I enter the following tournament data:", table
end


When(/^I enter the following participant data in the "(.*?)" dialog:$/) do |dialog, table|
  step "I enter the \"#{dialog}\" dialog"
  step "I enter the following participant data:", table
end
