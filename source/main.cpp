#include <iostream>
#include <reducer/color_reducer.hpp>

using namespace std;

int main() {
    ColorReducer reducer(new _ColorReducer("resource/mapping/500-color-mapping.bmp", "resource/distance/500-color/%d.json"));
    reducer->reduce("sandbox/DSC_0042_1500c.jpg", "sandbox/result002c.png", 30);
    return 0;
}