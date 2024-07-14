#ifndef ECOMPONENT_HPP
#define ECOMPONENT_HPP

class EComponent {
    public:
        virtual ~EComponent() = default;

        virtual void tick() = 0;
};

#endif // ECOMPONENT_HPP