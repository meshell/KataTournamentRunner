# based on http://sleepycoders.blogspot.ch/2013/03/sharing-travis-ci-generated-files.html
if [ "$TRAVIS_PULL_REQUEST" == "false" ]; then
  echo -e "Starting to publish doxygen docu on gh-pages\n"
  #copy data we're interested in to other place
  cp -R doc $HOME/doc
  #go to home and setup git
  cd $HOME
  git config --global user.email "travis@travis-ci.org"
  git config --global user.name "Travis"
  #using token clone gh-pages branch
  git clone --quiet --branch=gh-pages https://${GH_TOKEN}@github.com/meshell/KataTournamentRunner.git gh-pages > /dev/null
  cd gh-pages
  cp -Rf $HOME/doc .
  #add, commit and push files
  git add -f .
  git commit -m "Travis build $TRAVIS_BUILD_NUMBER pushed doxygen docu to gh-pages"
  git push -fq origin gh-pages > /dev/null
fi
