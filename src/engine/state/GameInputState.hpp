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

        Null, //No Input

        _MaxControls

        };

};

typedef std::vector<GameInputState::Control> ActiveInput;


#endif