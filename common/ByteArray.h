#pragma once
#include <type_traits>
#include <cstring>
#include <vector>

namespace detail
{
  template <typename, typename = void>
  struct is_container : std::false_type {};

  template <typename T>
  struct is_container<T,std::void_t<
                        decltype(std::declval<T>().begin()),
                        decltype(std::declval<T>().end()),
                        typename T::value_type>> : std::true_type {};

  template <typename T>
  inline constexpr bool is_container_v = is_container<T>::value;

  template <typename, typename = void>
  struct is_contiguous : std::false_type {};

  template <typename T>
  struct is_contiguous<T,std::void_t<
                        decltype(std::declval<T>().data())>> : std::true_type {};

  template <typename T>
  inline constexpr bool is_contiguous_v = is_contiguous<T>::value;
} // namespace detail

class ByteArray : public std::vector<unsigned char>
{
public:
  template <typename T>
  inline const ByteArray& operator>>(T& v) const
  {
    if constexpr (detail::is_container_v<T>)
    {
      typename T::size_type size{0};
      operator>>(size);
      if constexpr (detail::is_contiguous_v<T>)
      {
        v.resize(size / sizeof(typename T::value_type));
        popMem(v.data(), size);
      }
      else
      {
        v.resize(size / sizeof(typename T::value_type));
        auto iter = v.begin();
        while (iter != v.end())
        {
          operator>>(*iter++);
        }
      }
    }
    else if constexpr (std::is_arithmetic_v<T>)
    {
      std::memcpy(&v, data() + mPos, sizeof v);
      mPos += sizeof v;
    }
    return *this;
  }

  template <typename T>
  inline ByteArray& operator<<(T v)
  {
    if constexpr (std::is_same_v<T, const char*>)
    {
      auto size = std::strlen(v);
      operator<<(size);
      pushMem(v, size);
    }
    else if constexpr (detail::is_container_v<T>)
    {
      operator<<(std::size(v) * sizeof(typename T::value_type));
      if constexpr (detail::is_contiguous_v<T>)
      {
        pushMem(v.data(), sizeof(typename T::value_type) * std::size(v));
      }
      else
      {
        for(const auto& el : v)
        {
          operator<<(el);
        }
      }
    }
    else if constexpr (std::is_arithmetic_v<T>)
    {
      pushMem(&v, sizeof(T));
    }

    return *this;
  }
protected:
  void pushMem(const void* src, size_type count)
  {
    size_type oldSize{this->size()};
    resize(oldSize + count);
    std::memcpy(data() + oldSize, src, count);
  }

  void popMem(void* dst, size_type count) const
  {
    std::memcpy(dst, data() + mPos, count);
    mPos += count;
  }

protected:
  mutable size_type mPos{0};
};