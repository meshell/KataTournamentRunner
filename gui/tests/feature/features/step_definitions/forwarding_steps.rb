When(/^I save the profile$/) do
  step "I choose the \"Save\" menu entry"
end

Given(/^the following Tournament was saved to profile "(.*?)":$/) do |file, table|
  step "the following Tournament is open:", table
  step "I save the profile to \"#{file}\""
end


