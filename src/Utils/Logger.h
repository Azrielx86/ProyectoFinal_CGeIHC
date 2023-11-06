//
// Created by edgar on 11/6/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_LOGGER_H
#define PROYECTOFINAL_CGEIHC_LOGGER_H

#include <iostream>

namespace Utils
{

class Logger
{
  public:
	explicit Logger(const std::string &className);

  private:
	std::string className;
};

} // namespace Utils

#endif // PROYECTOFINAL_CGEIHC_LOGGER_H
