#pragma once
#include <cstdint>
#include <string>

enum class EMessage : uint8_t
{
  Login,
  LoginStatus
};

struct Login
{
  std::string name;
  std::string key;
  std::string path;
};

enum class ELoginStatus : uint8_t
{
  Failed,
  Succeeded
};

struct LoginStatus
{
  ELoginStatus status;
};