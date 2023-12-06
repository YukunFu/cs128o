#include "path_image.hpp"

#include <cmath>

void HelpBottomRow(int mid, int up, Path& path, unsigned int& currentrow, size_t& col) {
  if (up < mid) { path.SetLoc(col + 1, currentrow - 1); path.IncEleChange(up); currentrow -= 1;} else { path.SetLoc(col + 1, currentrow); path.IncEleChange(mid);}
}
void HelpTopRow(int mid, int down, Path& path, unsigned int& currentrow, size_t& col) {
  if (down < mid) { path.SetLoc(col + 1, currentrow + 1); path.IncEleChange(down); currentrow += 1; } else { path.SetLoc(col + 1, currentrow); path.IncEleChange(mid); }
}
void HelpMiddleRow(int up ,int mid, int down, Path& path, unsigned int& currentrow, size_t& col) {
  if (mid <= up && mid <= down) { path.SetLoc(col + 1, currentrow); path.IncEleChange(mid);
        } else if (down <= up && down < mid) { path.SetLoc(col + 1, currentrow + 1); path.IncEleChange(down); currentrow += 1;
        } else { path.SetLoc(col + 1, currentrow - 1); path.IncEleChange(up); currentrow -= 1;}
}
PathImage::PathImage(const GrayscaleImage& image, const ElevationDataset& dataset) {
  width_ = image.Width(); height_ = image.Height(); path_image_ = image.GetImage(); 
  for (size_t i = 0; i < height_; ++i) { unsigned int current_row = i; Path p(width_, current_row);
    for (size_t j = 0; j < width_ - 1; ++j) {
      p.SetLoc(j, current_row); int mid = std::abs(dataset.DatumAt(current_row, j + 1) - dataset.DatumAt(current_row, j));
      if (current_row == height_ - 1) {
        int up = std::abs(dataset.DatumAt(current_row - 1, j + 1) - dataset.DatumAt(current_row , j));
        HelpBottomRow(mid, up, p, current_row, j);
      } else if (current_row == 0) {
        int down = std::abs(dataset.DatumAt(current_row + 1, j + 1) - dataset.DatumAt(current_row, j));
        HelpTopRow(mid, down, p, current_row, j);
      } else {
        int up = std::abs(dataset.DatumAt(current_row - 1, j + 1) - dataset.DatumAt(current_row, j)); int down = std::abs(dataset.DatumAt(current_row + 1, j + 1) - dataset.DatumAt(current_row, j));
        HelpMiddleRow(up, mid, down, p, current_row, j);
      }
    }
    paths_.push_back(p);
  }
  Path bestpath = paths_.at(0);
  for (size_t i = 0; i < paths_.size(); ++i) {
    if (paths_.at(i).EleChange() < bestpath.EleChange()) {
      bestpath = paths_.at(i);
    }
    for (size_t j = 0; j < paths_.at(i).GetPath().size(); ++j) {
      path_image_.at(paths_.at(i).GetPath()[j]).at(j) =
          Color(kPathRedValue, kPathGreenValue, kPathBlueValue);
    }
  }
  for (size_t i = 0; i < bestpath.GetPath().size(); ++i) {
    path_image_.at(bestpath.GetPath()[i]).at(i) = Color(kBestPathRedValue, kBestPathGreenValue, kBestPathBlueValue);
  }
}

size_t PathImage::Width() const { return width_; }
size_t PathImage::Height() const { return height_; }
unsigned int PathImage::MaxColorValue() const { return kMaxColorValue; }
const std::vector<Path>& PathImage::Paths() const { return paths_; }
const std::vector<std::vector<Color>>& PathImage::GetPathImage() const {
  return path_image_;
}
void PathImage::ToPpm(const std::string& name) const {
  std::ofstream ofs(name);
  if (!ofs.is_open()) {
    throw std::runtime_error("file is not open");
  }
  ofs << "P3" << std::endl;
  ofs << width_ << " " << height_ << std::endl;
  ofs << kMaxColorValue << std::endl;
  for (unsigned int i = 0; i < height_; ++i) {
    for (unsigned int j = 0; j < width_; ++j) {
      ofs << path_image_.at(i).at(j).Red() << " "
          << path_image_.at(i).at(j).Green() << " "
          << path_image_.at(i).at(j).Blue() << std::endl;
    }
    ofs << std::endl;
  }
}