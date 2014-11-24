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
