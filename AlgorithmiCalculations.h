#pragma once
#include <vector>
#include <random>
#include <cmath>

class AlgorithmiCalculations
{
public:
    template<typename T> void FisherYatesShuffle(std::vector<T>& array)
    {
        for (size_t i = array.size() - 1; i > 0; i--)
        {
            std::uniform_int_distribution<size_t> distr(0, i);
            size_t j = distr(rng);
            std::swap(array[i], array[j]);
        }
    }

    template<typename T> int WayBetweenTwoPoint(T _A, T _B)
    {
        result = sqrt(((_A - currentX) * (_A - currentX)) + ((_B - currentY) * (_B - currentY)));
        currentX = _A;
        currentY = _B;
        return result;
    }

    float speedMeter(float S, float t)
    {
        return (t > 0.f) ? (S / t) : 0.f;
    }

    float mergeTwoUnrelatedRanges(float _1from, float _1to, float _2from, float _2to)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> range1(-_1from, _1to);
        std::uniform_real_distribution<float> range2(_2from, _2to);
        std::uniform_int_distribution<int> choose_range(0, 1);
        return choose_range(gen) == 0 ? range1(gen) : range2(gen);
    }

private:
    int result = 0;
    int currentX = 0;
    int currentY = 0;
    std::mt19937 rng{ std::random_device{}() };
};