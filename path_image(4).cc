#include "path_image.hpp"
#include "elevation_dataset.hpp"
#include "grayscale_image.hpp"

#include "grayscale_image.hpp"
#include <cstdlib> 
#include <vector>
#include <string>

#include <stdexcept>
#include <cmath>
#include<fstream>
#include <limits>


using namespace std;



PathImage::PathImage(const GrayscaleImage &image, const ElevationDataset &dataset):width_(dataset.Width()),height_(dataset.Height()),path_image_(image.GetImage()){
  for (size_t h = 0; h < height_; h++){
    size_t indx = h;
    Path path(width_, h);
    path.SetLoc(0,h);
    for(size_t w = 0; w < width_ - 1; w++){
      int horizontal_difference = abs(dataset.DatumAt(indx, w + 1) - dataset.DatumAt(indx, w));
      int lower_difference = numeric_limits<int>::max();
      int upper_difference = numeric_limits<int>::max();
      if(indx < height_ -1) {lower_difference = abs(dataset.DatumAt(indx + 1,w + 1) - dataset.DatumAt(indx, w));}
      if(indx > 0) {upper_difference = abs(dataset.DatumAt(indx - 1,w + 1) - dataset.DatumAt(indx, w));}
      path.IncEleChange(min({horizontal_difference,lower_difference,upper_difference}));
      if(min({horizontal_difference,lower_difference,upper_difference})==horizontal_difference){
        path.SetLoc(w + 1,indx);
      }else if(( min({horizontal_difference,lower_difference,upper_difference})==lower_difference)||(( min({horizontal_difference,lower_difference,upper_difference})==lower_difference)&&lower_difference==upper_difference)){
          path.SetLoc(w + 1,indx + 1);
          indx = indx + 1;
      } else{
        path.SetLoc(w + 1,indx - 1);
        indx = indx - 1;}
    }
    paths_.push_back(path);
  }
  const Color kGreen = Color(31,253,13);
  const Color kRed = Color(252,25,63);
  Path p1 = paths_[0];
  for(unsigned int h = 0; h < height_;h++){
    if(paths_[h].EleChange() < p1.EleChange()){
      p1=paths_[h];
    } 
    for(unsigned int w = 0; w < width_; w++){
    path_image_[paths_[h].GetPath()[w]][w]=kRed;
    }
  }
  for(unsigned int k = 0; k < width_;k++){
    path_image_[p1.GetPath()[k]][k]=kGreen;
  }
}


size_t PathImage::Width() const{
  return width_;
}
size_t PathImage::Height() const{
  return height_;
}
unsigned int PathImage::MaxColorValue() const{
  return kMaxColorValue;
}
const std::vector<Path>& PathImage::Paths() const{
  return paths_;
}
const std::vector<std::vector<Color> >& PathImage::GetPathImage() const{
  return path_image_;
}
void PathImage::ToPpm(const std::string& name) const{
  ofstream stream(name);
  stream<<"P3\n";
  stream<<width_<<" "<<height_<<'\n';
  stream<<"255"<<'\n';
  for(unsigned int h =0; h < height_ ; h++){
    for(unsigned int w = 0; w < width_ ;w++){
      stream<<path_image_[h][w].Red()<<' '<<path_image_[h][w].Green()<<' ' <<path_image_[h][w].Blue()<<' ';
    }
    stream<<'\n';
  }  
}