#ifndef MATHTOOLS_H
#define MATHTOOLS_H


class MathTools
{
public:
    MathTools();

    template <typename T>
    static T map(T value, T minIn, T maxIn, T minOut, T maxOut)
    {
        return (value - minIn) * (maxOut - minOut) / (maxIn - minIn) + minOut;
    }

    template <typename T>
    static T clamp(T value, T minIn, T maxIn)
    {
        if(value < minIn) return minIn;
        if(value > maxIn) return maxIn;
        return value;
    }

    template <typename T>
    static T randomInRange(T minIn, T maxIn)
    {
        return qrand() % ((maxIn + 1) - minIn) + minIn;;
    }

};

#endif // MATHTOOLS_H
