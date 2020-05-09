//
// Created by Aloysio Galvão Lopes on 2020-05-09.
//

#include <vcl/math/noise/PerlinNoise.h>
#include <iostream>
#include <random>

int main(){
    vcl::PerlinNoise noiseGenerator;
    int tot = 200;

    double freq;

    int max;

    std::cin >> max;

    vcl::NoiseParameters parameters;
    parameters.If = 1;

    parameters.octaves = max;

    for (int i = 0; i < tot; ++i){
        double x = (double)i/((double)tot-1);
        for (int j = 0; j < tot; ++j){
            double y = (double)j/((double)tot-1);
            std::cout << x << " " << y << " " << noiseGenerator.fbmNoise(x, y, parameters) << std::endl;
        }
    }

}
