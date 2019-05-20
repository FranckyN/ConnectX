#ifndef MULTIDIMARRAY_H
#define MULTIDIMARRAY_H

#include <vector>
#include <memory>
#include <algorithm>

/**
 * @class MultiDimArray
 * @file multi_dim_array.h
 * @brief Classical implementation of a multi-dimensional array.
 * The elements are stored in a one-dimensionnal array array_ internally
 * Coordinates are transformed to a single index in order to access elements.
 */
template <class T>
class MultiDimArray
{
public:
  // Representation of the dimension and the sizes for each.
  using DimCoordinates = std::vector<std::size_t>;
  
  explicit MultiDimArray(const DimCoordinates& dimensions)
      : dims_(dimensions) {
    std::size_t num_elements(1);
    std::for_each(dims_.begin(), dims_.end(), 
        [this, &num_elements] (std::size_t val) {
          mult_.emplace_back(num_elements);
          num_elements *= val;
        });
    array_.resize(num_elements);
  }
   
  //Total number of elements in the array 
  std::size_t capacity() const { return array_.size(); }
  
  T& operator[](const DimCoordinates& lookup) {
    return array_[GetOffset(lookup)];
  }

  const T operator[](const DimCoordinates& lookup) const { 
    return array_[GetOffset(lookup)];
  }

  void Fill(const T& val) { 
    std::fill(array_.begin(), array_.end(), val); 
  }
  
  bool Find(const T& val, std::list<DimCoordinates>& occurences) {
    auto it = array_.begin();
    occurences.clear();
    while(array_.end() != (it = std::find(it,array_.end(),val))) {
      occurences.push_back(GetPosition(it - array_.begin()));
      it++;
    }
    return !occurences.empty();
  }
  
  std::size_t GetDimensionSize(std::size_t num_axis) const {
    return dims_[num_axis]; 
  }
    
  const DimCoordinates& GetDimensions() const {
    return dims_; 
  }
  
 private:
  std::size_t GetOffset(const DimCoordinates& position) const {
    std::size_t offset = 0;
    auto mit = mult_.begin();
    std::for_each(position.begin(), position.end(), 
                  [&offset, &mit] (std::size_t val) {
      offset += *mit * val;
      ++mit;
    });
    return offset;
  }
  
  DimCoordinates GetPosition(const std::size_t& offset) const {
    DimCoordinates result(mult_.size());
    auto mit = mult_.rbegin();
    auto xit = result.rbegin();
    std::size_t quotient(offset);
    while(mit != mult_.rend()) {
      *xit = quotient / *mit;
      quotient = quotient - *xit * *mit;
      xit++; ++mit;
    }
    return result;
  }
  
  DimCoordinates dims_;
  DimCoordinates mult_;
  std::vector<T> array_;
};

#endif // MULTIDIMARRAY_H
