//
// Created by igor on 23/04/2020.
//

#include <string.h>
#include "SingletonException.h"

SingletonException::SingletonException(std::string className) {
    this->className = const_cast<char *>(className.c_str());
}

SingletonException::~SingletonException() noexcept {
    delete className;
}

const char *SingletonException::what() const noexcept {
    return strcat(className, " is a singleton class.");
}
