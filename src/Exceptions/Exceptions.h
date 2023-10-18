//
// Created by edgar on 10/6/2023.
//
#include <stdexcept>
namespace Exceptions
{
class ShaderNotFoundException : std::runtime_error
{
  public:
	explicit ShaderNotFoundException(const char *message) noexcept : std::runtime_error(message) {}
	using std::runtime_error::what;
	virtual const char *what() { return std::runtime_error::what(); }
};

class GlCreateShaderProgramException : std::runtime_error
{
  public:
	explicit GlCreateShaderProgramException(const char *message) noexcept : std::runtime_error(message) {}
	using std::runtime_error::what;
	virtual const char *what() { return std::runtime_error::what(); }
};
} // namespace Exceptions
