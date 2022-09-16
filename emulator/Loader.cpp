#include "Loader.hpp"

constexpr Location gMinLocation { 0, 0 };
constexpr Location gMaxLocation { 1000, 1000 };

constexpr LengthUnit gMinSize = 1;
constexpr LengthUnit gMaxSize = 3;

constexpr LengthUnit gMinSpeed = 0;
constexpr LengthUnit gMaxSpeed = 2;

Loader::Loader(const std::string& fileName)
    : mFile(fileName)
{
}

Processor Loader::load()
{
    auto timeLimit = loadTimeLimit();
    auto road = loadRoad();
    auto car = loadCar();
    auto vehicles = loadVehicles();
    return Processor(std::move(road), std::move(car), std::move(vehicles), timeLimit);
}

TimeUnit Loader::loadTimeLimit()
{
    TimeUnit timeLimit;
    mFile >> timeLimit;

    if (!mFile)
        throw std::runtime_error("Cannot parse time limit");

    return timeLimit;
}

LaneList Loader::loadLanes()
{
    std::size_t count;
    mFile >> count;

    if (!mFile || count > 100)
        throw std::runtime_error("Cannot parse lane count");

    LaneList container;
    container.reserve(count);

    for (std::size_t i = 0; i < count; ++i) {
        Location location;
        Direction direction;
        mFile >> location.x >> location.y >> direction.dx >> direction.dy;

        if (!mFile)
            throw std::runtime_error("Cannot parse lane #" + std::to_string(i + 1));

        container.emplace_back(
            Location {
                std::clamp(location.x, gMinLocation.x, gMaxLocation.x),
                std::clamp(location.y, gMinLocation.y, gMaxLocation.y),
            },
            Direction {
                std::clamp(direction.dx, gMinLocation.x, gMaxLocation.x),
                std::clamp(direction.dy, gMinLocation.y, gMaxLocation.y),
            });
    }

    return container;
}

TrafficSignList Loader::loadTrafficSigns()
{
    std::size_t count;
    mFile >> count;

    if (!mFile || count > 100)
        throw std::runtime_error("Cannot parse traffic sign count");

    TrafficSignList container;
    container.reserve(count);

    for (std::size_t i = 0; i < count; ++i) {
        std::size_t index, type;
        LengthUnit location, value;
        mFile >> index >> type >> location >> value;

        if (!mFile)
            throw std::runtime_error("Cannot parse traffic sign #" + std::to_string(i + 1));

        container.emplace_back(static_cast<TrafficSign::Type>(type), index, location, value);
    }

    return container;
}

Road Loader::loadRoad()
{
    auto lanes = loadLanes();
    auto signs = loadTrafficSigns();
    return Road(std::move(lanes), std::move(signs));
}

Car Loader::loadCar()
{
    Location location;
    Direction direction;
    Circle destination;
    LengthUnit size, visibleRange;
    LengthUnit minSpeed, forwardMaxSpeed, reverseMaxSpeed;
    LengthUnit forwardAccelerationRate, reverseAccelerationRate, decelerationRate;
    AngleUnit rotationRate;

    mFile >> location.x >> location.y >> direction.dx >> direction.dy //
        >> destination.center.x >> destination.center.y >> destination.radius //
        >> size >> visibleRange //
        >> minSpeed >> forwardMaxSpeed >> reverseMaxSpeed //
        >> forwardAccelerationRate >> reverseAccelerationRate >> decelerationRate >> rotationRate;

    if (!mFile)
        throw std::runtime_error("Cannot parse car");

    return Car(
        {
            std::clamp(location.x, gMinLocation.x, gMaxLocation.x),
            std::clamp(location.y, gMinLocation.y, gMaxLocation.y),
        },
        Direction {
            std::clamp(direction.dx, gMinLocation.x, gMaxLocation.x),
            std::clamp(direction.dy, gMinLocation.y, gMaxLocation.y),
        },
        {
            {
                std::clamp(destination.center.x, gMinLocation.x, gMaxLocation.x),
                std::clamp(destination.center.y, gMinLocation.y, gMaxLocation.y),
            },
            std::clamp(destination.radius, gMinSize, gMaxSize),
        },
        {
            std::clamp(size, gMinSize, gMaxSize),
            std::clamp(visibleRange, 5., 100.),
            std::clamp(minSpeed, 0.01, 1.0),
            std::clamp(forwardMaxSpeed, 0.1, gMaxSpeed),
            std::clamp(reverseMaxSpeed, 0.1, gMaxSpeed),
            std::clamp(forwardAccelerationRate, 0.01, 1.0),
            std::clamp(reverseAccelerationRate, 0.01, 1.0),
            std::clamp(decelerationRate, 0.01, 1.0),
            std::clamp(rotationRate, 0.01, 1.0),
        });
}

VehicleList Loader::loadVehicles()
{
    std::size_t count;
    mFile >> count;

    if (!mFile || count > 100)
        throw std::runtime_error("Cannot parse vehicle count");

    VehicleList container;
    container.reserve(count);

    for (std::size_t i = 0; i < count; ++i) {
        std::string id;
        Location location;
        Direction direction;
        LengthUnit size, speed;
        mFile >> id >> location.x >> location.y >> size >> direction.dx >> direction.dy >> speed;

        if (!mFile)
            throw std::runtime_error("Cannot parse vehicle " + id);

        container.emplace_back(std::move(id),
            Location {
                std::clamp(location.x, gMinLocation.x, gMaxLocation.x),
                std::clamp(location.y, gMinLocation.y, gMaxLocation.y),
            },
            std::clamp(size, gMinSize, gMaxSize), direction.normalized(), std::clamp(speed, gMinSpeed, gMaxSpeed));
    }

    return container;
}
