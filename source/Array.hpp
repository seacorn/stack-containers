#pragma once

namespace sc {

using index_t = unsigned long int;

template <class T, index_t N>
class Array {
  constexpr void copy_all_data(Array const& other) noexcept {
    for (index_t idx = 0; idx < N; ++idx) {
      data_[idx] = other.data_[idx];
    }
  }

  constexpr void move_all_data(Array&& other) noexcept {
    for (index_t idx = 0; idx < N; ++idx) {
      data_[idx] = std::move(other.data_[idx]);
      other.replace_item_with_new(idx);
    }
  }

  T data_[N];  // use char data_[N * sizeof(T)] and also alignas(T)

 public:
  class Iterator;
  class ConstIterator;

  /// Default constructor
  constexpr Array() noexcept : data_{} {}

  /// Copy constructor
  constexpr Array(Array const& other) noexcept : data_{} {
    // copy_all_data(other);
    for (index_t idx = 0; idx < N; ++idx) {
      replace_item_with_new(
          idx, other.data_[idx]);  // data_[idx] = other.data_[idx];
    }
  }

  /// Copy assignment operator
  constexpr Array& operator=(Array const& other) noexcept {
    copy_all_data(other);
  }

  /// Move constructor
  constexpr Array(Array&& other) noexcept : data_{} { move_all_data(other); }

  /// Move assignment operator
  constexpr Array& operator=(Array&& other) noexcept { move_all_data(other); }

  /// Construct with initial value
  // TODO: How to make it work? Array<5> a(10); (type deduced from ctor args,
  // size from template)
  explicit constexpr Array(T const& value) noexcept : data_{} { fill(value); }

  /// Construct from raw array
  constexpr Array(T const (&data)[N]) noexcept : data_{} {
    for (index_t idx = 0; idx < N; ++idx) {
      data_[idx] = data[idx];
    }
  }

  constexpr Iterator begin() noexcept { return Iterator{&data_[0]}; }

  constexpr Iterator end() noexcept { return Iterator{&data_[N]}; }

  constexpr ConstIterator begin() const noexcept {
    return ConstIterator{&data_[0]};
  }

  constexpr ConstIterator end() const noexcept {
    return ConstIterator{&data_[N]};
  }

  constexpr index_t size() const noexcept { return N; }

  constexpr T& front() noexcept { return data_[0]; }

  constexpr T const& front() const noexcept { return data_[0]; }

  constexpr T& back() noexcept { return data_[N - 1]; }

  constexpr T const& back() const noexcept { return data_[N - 1]; }

  constexpr void swap(Array& other) noexcept {
    T temp;
    for (index_t idx = 0; idx < N; ++idx) {
      temp = data_[idx];
      data_[idx] = other.data_[idx];
      other.data_[idx] = temp;
    }
  }

  constexpr void fill(T const& value) noexcept {
    for (index_t idx = 0; idx < N; ++idx) {
      data_[idx] = value;
    }
  }

  template <class... Args>
  constexpr void replace_item_with_new(index_t index, Args&&... args) noexcept {
    data_[index].~T();
    new (&data_[index]) T(std::forward<Args>(args)...);
  }

  constexpr T& operator[](index_t index) noexcept { return data_[index]; }

  constexpr T const& operator[](index_t index) const noexcept {
    return data_[index];
  }

  constexpr bool operator==(Array const& other) const noexcept {
    for (index_t idx = 0; idx < N; ++idx) {
      if (data_[idx] != other.data_[idx]) {
        return false;
      }
    }
    return true;
  }

  constexpr bool operator!=(Array const& other) const noexcept {
    return !(*this == other);
  }

  class Iterator {
    friend class Array;

    constexpr explicit Iterator(T* ptr) : ptr_(ptr) {}

    T* ptr_;

   public:
    constexpr T& operator*() const noexcept { return *ptr_; }

    constexpr bool operator==(Iterator const& other) const noexcept {
      return ptr_ == other.ptr_;
    }

    constexpr bool operator!=(Iterator const& other) const noexcept {
      return ptr_ != other.ptr_;
    }

    constexpr Iterator& operator++() noexcept {
      ++ptr_;
      return *this;
    }

    constexpr Iterator operator++(int) noexcept { return Iterator{ptr_++}; }

    constexpr operator ConstIterator() const noexcept {
      return ConstIterator{ptr_};
    }
  };

  class ConstIterator {
    friend class Array;
    friend class Iterator;

    constexpr explicit ConstIterator(T const* ptr) : ptr_(ptr) {}

    T const* ptr_;

   public:
    constexpr T const& operator*() const noexcept { return *ptr_; }

    constexpr bool operator==(ConstIterator const& other) const noexcept {
      return ptr_ == other.ptr_;
    }

    constexpr bool operator!=(ConstIterator const& other) const noexcept {
      return ptr_ != other.ptr_;
    }

    constexpr ConstIterator& operator++() noexcept {
      ++ptr_;
      return *this;
    }

    constexpr ConstIterator operator++(int) noexcept {
      return ConstIterator{ptr_++};
    }
  };
};

}  // namespace sc
