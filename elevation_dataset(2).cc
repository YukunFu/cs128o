#include "elevation_dataset.hpp"
using namespace std;
#include <fstream>
#include <vector>
#include <stdexcept>

#include <string>


ElevationDataset::ElevationDataset(const std::string& filename, size_t width, size_t height){
    width_ =width;
    height_=height;
    
    ifstream stream;
    stream.open(filename);
    if(!(stream.is_open())){
      throw runtime_error("");
    }
    if (stream.fail()) {
      throw invalid_argument("");
    }
    int i = 0;
    vector<int> vec;
    while (stream.good()) {
        stream >> i;
        if (stream.fail()) {
          stream.clear();
          stream.ignore(1, '\n');
        } else {
        vec.push_back(i);
        }
    }
    if(vec.size() != width_ * height_) {throw invalid_argument("");}
    vector<int> a;
    max_ele_ = vec[1];
    min_ele_ = vec[1];
    for(unsigned int i = 0; i < vec.size(); i++) {
        a.push_back(vec[i]);
        if(a.size() == width){
            data_.push_back(a);
            a.clear();
        }
        if(vec[i] > max_ele_) max_ele_ = vec[i];
        if(vec[i] < min_ele_) min_ele_ = vec[i];       
    }
    
}


size_t ElevationDataset::Width() const{
    return width_;
}
size_t ElevationDataset::Height() const{
    return height_;
}
int ElevationDataset::MaxEle() const{
    return max_ele_;
}
int ElevationDataset::MinEle() const{
    return min_ele_;
}
int ElevationDataset::DatumAt(size_t row, size_t col) const{
    return data_[row][col];
}
const vector<std::vector<int> >&ElevationDataset:: GetData() const{
    return data_;
}
