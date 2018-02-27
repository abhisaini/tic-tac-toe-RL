arr=(0.1 0.1 3 100 100 0.1 0.1 3 100 100)
g++ -std=c++0x try_map.cpp -std=c++11 -I/usr/include/python2.7 -lpython2.7
echo ${arr[*]} | ./a.out 

