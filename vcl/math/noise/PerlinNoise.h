//
// Created by Aloysio Galvão Lopes on 2020-05-09.
//

#ifndef PGM_PERLINNOISE_H
#define PGM_PERLINNOISE_H

#include <vector>

namespace vcl {
    struct NoiseParameters{
        // Initial value of noise amplitude
        double Ia = 1.0;
        // Noise gain of each octave
        double g = 0.5;
        // Initial frequency of each octave
        double If = 1;
        // Frequency gain (lacunarity)
        double l = 2;
        // Number of octaves
        int octaves = 7;
        // Noise sharpness
        double Ss = 0.5;
        // Noise height gain
        double Se = 1;
        // Noise total gain
        double Sh = 1;
    };

    class PerlinNoise {
    public:
        /**
         * Initialize Perlin noise with reference values
         */
        PerlinNoise();

        /**
         * Generates a permutation based on the seed
         * @param seed
         */
        PerlinNoise(unsigned int seed);

        /**
         * Generates noise
         * @param x
         * @param y
         * @param z
         * @return
         */
        double noise(double x, double y, double z);

        /**
         * Generates a 2D noise
         * @param x
         * @param y
         * @return
         */
        double noise(double x, double y);

        /**
         * Generates billow noise
         * @param x
         * @param y
         * @return
         */
        double billow(double x, double y);

        /**
         * Generates ridge noise
         * @param x
         * @param y
         * @return
         */
        double ridge(double x, double y);

        /**
         * Sharpness adjusted noise
         * @param x
         * @param y
         * @return
         */
        double sharpnessAdjustedNoise(double x, double y, double sharpness);

        /**
         * Generates a default noise
         * @param x
         * @param y
         * @param parameters
         * @return
         */
        double fbmNoise(double x, double y, NoiseParameters& parameters);

        /**
         * Generates the noise based on height
         * @param x
         * @param y
         * @param parameters
         * @return
         */
        double heightDependentNoise(double x, double y, NoiseParameters& parameters);

        /**
         * Modulates the noise based on height
         * @param h
         * @param parameters
         * @return
         */
        double heightDependentNoise(double h, NoiseParameters& parameters);

        /**
         * Generates eroded terrain noise
         * @param x
         * @param y
         * @param parameters
         * @return
         */
        double erosionFbmNoise(double x, double y, NoiseParameters* parameters);

    private:
        double fade(double t);

        double lerp(double t, double a, double b);

        double grad(int hash, double x, double y, double z);

        // The permutation vector
        std::vector<int> p;
    };
}

#endif //PGM_PERLINNOISE_H
