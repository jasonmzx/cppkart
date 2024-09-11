#ifndef SYSTEM_HPP
#define SYSTEM_HPP

// System Base Class for ECS

class System {
    public:
        virtual ~System() = default;

        virtual void tickSystem() = 0;
};

#endif // SYSTEM_HPP