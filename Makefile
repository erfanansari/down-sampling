CXX = g++
CXXFLAGS = -std=c++11 -I/opt/homebrew/Cellar/opencv/4.9.0_1/include/opencv4
LDFLAGS = -L/opt/homebrew/Cellar/opencv/4.9.0_1/lib
LDLIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs

TARGET = down-sampling

SRC = down-sampling.cpp

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(TARGET)
