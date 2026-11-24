#include "raytracing/AdaptiveRounds.h"
#include "../stdafx.h"

namespace raytracer
{
    AdaptiveRounds::AdaptiveRounds(const Size2& resolution)
        : resolution(resolution), firstRoundErrors(x(resolution) * y(resolution))
    {
    }

    void AdaptiveRounds::setFirstRoundError(const ASizeT imageIndex, const Float error) noexcept
    {
        firstRoundErrors[imageIndex] = error;
    }

    ASizeT AdaptiveRounds::roundsOf(const Size2& pixel, const ASizeT maxRounds, const Float threshold) const noexcept
    {
        using Coordinate = Size2::ValueType;
        const auto width = x(resolution);
        const auto px = x(pixel);
        const auto py = y(pixel);
        const auto firstX = px > Zero<Coordinate>() ? px - One<Coordinate>() : px;
        const auto lastX = min(px + One<Coordinate>(), width - One<Coordinate>());
        const auto firstY = py > Zero<Coordinate>() ? py - One<Coordinate>() : py;
        const auto lastY = min(py + One<Coordinate>(), y(resolution) - One<Coordinate>());

        auto sumOfSquares = Zero<Float>();
        auto neighbours = Zero<Float>();
        for (auto ny = firstY; ny <= lastY; ++ny)
        {
            for (auto nx = firstX; nx <= lastX; ++nx)
            {
                if (nx == px && ny == py)
                {
                    continue;
                }
                const auto error = firstRoundErrors[ny * width + nx];
                sumOfSquares += error * error;
                neighbours += One<Float>();
            }
        }
        if (!(sumOfSquares > Zero<Float>()))
        {
            return Zero<ASizeT>();
        }

        // k further rounds divide the standard error by sqrt(k + 1)
        const auto rounds = vectorization::ceil(sumOfSquares / neighbours / sqr(threshold)) - One<Float>();
        return static_cast<ASizeT>(clamp(rounds, Zero<Float>(), static_cast<Float>(maxRounds)));
    }
}
