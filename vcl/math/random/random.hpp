//
// Created by igor on 15/04/20.
//

#ifndef PGM_RANDOM_H
#define PGM_RANDOM_H

namespace vcl {

template<class T>
T rand(T min, T max) {
    std::default_random_engine generator;
    std::uniform_real_distribution<T> distribution(min, max);
    return distribution(generator);
}

}


#endif //PGM_RANDOM_H
