mkdir gtest
pushd gtest
wget --quiet http://googletest.googlecode.com/files/gtest-1.6.0.zip
unzip -qq gtest-1.6.0.zip
pushd gtest-1.6.0
cmake -DCMAKE_BUILD_TYPE=Release
make
sudo cp libgtest* /usr/lib
sudo mkdir /usr/include/gtest
sudo cp -r include/gtest/* /usr/include/gtest
popd
popd


cmake -DCMAKE_BUILD_TYPE=Release ./
make
make test

echo "Running example..."

./bin/example_c

echo "Running tests..."

./bin/libringbuffer_test

echo "Installing Valgrind..."

sudo apt-get -q --yes install valgrind

echo "Valgrind - libringbuffer tests"


valgrind ./bin/libringbuffer_test
valgrind --tool=helgrind ./bin/libringbuffer_test
valgrind --tool=cachegrind ./bin/libringbuffer_test

echo "Valgrind - example.c"

valgrind ./bin/example_c
valgrind --tool=helgrind ./bin/example_c
valgrind --tool=cachegrind ./bin/example_c

echo "Callgrind - example.c"

valgrind --tool=callgrind --dsymutil=yes ./bin/example_c
for outfile in callgrind.out.*; do
    callgrind_annotate --auto=yes $outfile
done