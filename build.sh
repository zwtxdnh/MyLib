mkdir build
echo "mkdir build"
mkdir bin
echo "mkdir bin"
cd ./build
cmake ..
make
cd ../test
g++ main.cpp -g ../bin/libmylib.a -lpthread -o test -std=c++11
cp test ../bin/test
rm test
../bin/test