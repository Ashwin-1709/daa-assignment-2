g++ segment-least-squares.cpp dbg.hh -o segment -std=c++17
./segment < $1 > out.txt
python3 vis.py < out.txt