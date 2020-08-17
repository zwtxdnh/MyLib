test -e build/ || mkdir build
echo "mkdir build/"

test -e bin/ || mkdir bin
echo "mkdir bin/"
cd build/
cmake ..
make
cd ../test
g++ main.cpp ../bin/libmylib.a -lpthread -o test
cp test ../bin/test
rm test
../bin/test