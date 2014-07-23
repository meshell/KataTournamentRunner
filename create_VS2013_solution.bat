pushd externals
.\win_externals_include.exe -y
.\win_externals_lib-msvc12.exe -y

popd
mkdir vs_solution
pushd vs_solution 
cmake -G"Visual Studio 12" ..
popd