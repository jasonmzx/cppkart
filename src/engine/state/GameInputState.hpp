#ifndef GAMEINPUTSTATE_HPP
#define GAMEINPUTSTATE_HPP


class GameInputState {

    public:
        
        enum Control {
        
        VehicleAccelerate = 0,
        VehicleBrake,
        VehicleDown,
        VehicleUp,
        VehicleTurnLeft,
        VehicleTurnRight,

        _MaxControls

        };

        float levels[_MaxControls] = {}; //* As the enum auto-incr, we know max controls the largest, aka buffer len :-)

    GameInputState() {
        // Initialize levels array to 0 (optional, since it's already the default behavior)
        for (int i = 0; i < _MaxControls; ++i) {
            levels[i] = 0.0f;
        }
    }

};

typedef std::vector<GameInputState::Control> ActiveInput;


#endif