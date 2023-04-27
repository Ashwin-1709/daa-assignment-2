g++ src/segment-least-squares.cpp src/dbg.hh -o segment -std=c++17
./segment < $1 > out.txt
python3 ./utils/vis.py < out.txt
