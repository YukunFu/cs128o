
#include "elevation_dataset.hpp"

#include "grayscale_image.hpp"
#include <cstdlib> 
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include<fstream>

using namespace std;


  GrayscaleImage::GrayscaleImage(const ElevationDataset& dataset){
    height_=dataset.Height();
    width_=dataset.Width();

    for(unsigned int h = 0; h < height_; h++){
      vector<Color> set;
        for(unsigned int w = 0; w<width_; w++){
          int gray = 0;
            if(!(dataset.MinEle()==dataset.MaxEle())){
                gray = (int)round((double)(dataset.DatumAt(h,w) - dataset.MinEle()) / (double)(dataset.MaxEle() - dataset.MinEle()) * kMaxColorValue);
            }    
                set.push_back(Color(gray,gray,gray));
        }
        image_.push_back(set);
    }
  }


  GrayscaleImage::GrayscaleImage(const std::string& filename, size_t width, size_t height){
    
    ElevationDataset dataset(filename,width,height);
    width_=width;
    height_=height;

     for(unsigned int h = 0; h < height_; h++){
      vector<Color> set;
      for(unsigned int w = 0; w<width_; w++){
        int gray = 0;
        if(!(dataset.MinEle()==dataset.MaxEle())){
          gray = (int)round((double)(dataset.DatumAt(h,w) - dataset.MinEle()) / (double)(dataset.MaxEle() - dataset.MinEle()) * kMaxColorValue);
        }    
        set.push_back(Color(gray,gray,gray));
      }
      image_.push_back(set);
    }   
  }

  size_t GrayscaleImage::Width() const{
    return width_;
  }
  
  size_t GrayscaleImage::Height() const{
    return height_;
  }
  unsigned int GrayscaleImage::MaxColorValue() const{
    return kMaxColorValue;
  }

  const Color& GrayscaleImage::ColorAt(int row, int col) const{
    return image_[row][col];
  }
  const std::vector<std::vector<Color> >& GrayscaleImage::GetImage() const{
    return image_;
  }
  void GrayscaleImage::ToPpm( const std::string& name ) const{
    ofstream streams(name);
    streams<<"P3\n";
    streams<< width_ << " " << height_<< '\n' ;
    streams<<"255"<<'\n';
    for(unsigned int h =0; h < height_;h++){
        for(unsigned int w = 0; w < width_ ;w++){
            streams<< ColorAt(h, w).Red()  << " " <<ColorAt(h , w).Green() <<" " << ColorAt(h , w).Blue() << " ";
        }
        streams<<'\n';
    }



    streams.close();
  }
