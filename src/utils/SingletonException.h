//
// Created by igor on 23/04/2020.
//

#ifndef PGM_SINGLETONEXCEPTION_H
#define PGM_SINGLETONEXCEPTION_H

#include <iostream>

/**
 * Exception thrown when singleton classes are instanced twice.
 */
class SingletonException : public std::exception {
public:
    SingletonException(std::string className);

    ~SingletonException() noexcept;

    const char *what() const noexcept override;

private:
    char *className;
};


#endif //PGM_SINGLETONEXCEPTION_H
