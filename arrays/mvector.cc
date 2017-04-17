#include "mvector.h"
#include <iostream>

namespace mg {

MVector::MVector(int capacity) : size_(0) {
  if (capacity < 1) {
    std::cout << "Cannot make vector of that size" << std::endl;
    exit(EXIT_FAILURE);
  }

  int true_capacity = DetermineCapacity(capacity);

  capacity_ = true_capacity;
  data_ = std::unique_ptr<int[]>(new int[true_capacity]);
}

MVector::~MVector() {}

int MVector::DetermineCapacity(int capacity) const {
  int true_capacity = kMinCapacity;

  while (capacity > true_capacity / kGrowthFactor) {
    true_capacity *= kGrowthFactor;
  }

  return true_capacity;
}

int MVector::GetSize() const { return size_; }
bool MVector::IsEmpty() const { return size_ == 0; }

int MVector::GetCapacity() const { return capacity_; }

void MVector::DebugString() const {
  std::cout << "size: " << size_ << std::endl
            << "capacity: " << capacity_ << std::endl
            << "items: " << std::endl;

  for (int i = 0; i < size_; ++i) {
    printf("%d: %d", i, data_[i]);
    std::cout << std::endl;
  }
}

void MVector::Push(int value) {
  ResizeForSize(size_ + 1);

  data_[size_] = value;
  ++size_;
}

void MVector::ResizeForSize(int candidate_size) {
  if (size_ < candidate_size) {  // grow
    if (size_ == capacity_) {
      IncreaseSize();
    }
  } else if (size_ > candidate_size) {  // shrink
    if (size_ < capacity_ / kShrinkFactor) {
      DecreaseSize();
    }
  }  // nothing needs to happen otherwise
}

void MVector::IncreaseSize() {
  int old_capacity = capacity_;
  int new_capacity = DetermineCapacity(old_capacity);

  if (old_capacity != new_capacity) {
    std::unique_ptr<int[]> new_data(new int[new_capacity]);

    for (int i = 0; i < size_; ++i) {
      new_data[i] = data_[i];
    }

    data_ = std::move(new_data);
    capacity_ = new_capacity;
  }
}

void MVector::DecreaseSize() {
  int old_capacity = capacity_;
  int new_capacity = old_capacity / 2;

  if (new_capacity < kMinCapacity) {
    new_capacity = kMinCapacity;
  }

  if (new_capacity != old_capacity) {
    std::unique_ptr<int[]> new_data(new int[new_capacity]);

    for (int i = 0; i < size_; ++i) {
      new_data[i] = data_[i];
    }

    data_ = std::move(new_data);
    capacity_ = new_capacity;
  }
}

int MVector::GetValueAt(int index) const {
  if (index < 0 || index >= size_) {
    std::cout << "Index out of bounds." << std::endl;
    exit(EXIT_FAILURE);
  }

  return data_[index];
}

int MVector::Pop() {
  if (size_ == 0) {
    std::cout << "Nothing to pop." << std::endl;
    exit(EXIT_FAILURE);
  }

  ResizeForSize(size_ - 1);

  int value = data_[size_ - 1];

  --size_;

  return value;
}

void MVector::Insert(int index, int value) {
  if (index < 0 || index >= size_) {
    std::cout << "Index out of bounds." << std::endl;
    exit(EXIT_FAILURE);
  }

  ResizeForSize(size_ + 1);

  // shift items to the right
  for (int i = size_; i > index; --i) {
    data_[i] = data_[i - 1];
  }

  // insert item
  data_[index] = value;

  ++size_;
}

void MVector::Prepend(int value) { Insert(0, 42); }

void MVector::Delete(int index) {
  if (index < 0 || index >= size_) {
    std::cout << "Index out of bounds." << std::endl;
    exit(EXIT_FAILURE);
  }

  ResizeForSize(size_ - 1);

  for (int i = index; i < size_ - 1; i++) {
    data_[i] = data_[i + 1];
  }

  --size_;
}

void MVector::Remove(int value) {
  for (int i = 0; i < size_; ++i) {
    if (data_[i] == value) {
      Delete(i);
      --i;  // since items will shift, recheck this index
    }
  }
}

int MVector::Find(int value) const {
  int found_index = -1;

  for (int i = 0; i < size_; ++i) {
    if (data_[i] == value) {
      found_index = i;
      break;
    }
  }

  return found_index;
}

}  // namespace mg
