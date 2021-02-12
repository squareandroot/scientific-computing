# debug:
# 	g++ finite-elements.cpp -o finite-elements.out -Wall -D_GLIBCXX_DEBUG -g && ./finite-elements.out
# release:
# 	g++ finite-elements.cpp -o finite-elements.out -Wall -O3 && ./finite-elements.out

debug:
	g++ finite-elements-heatsink.cpp -o finite-elements-heatsink.out -Wall -D_GLIBCXX_DEBUG -g -lboost_system -lboost_filesystem && ./finite-elements-heatsink.out
release:
	g++ finite-elements-heatsink.cpp -o finite-elements-heatsink.out -Wall -O3 -lboost_system -lboost_filesystem && ./finite-elements-heatsink.out
