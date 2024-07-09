#ifndef GAMEINPUT_HPP
#define GAMEINPUT_HPP


#include <SDL2/SDL.h>
#include <unordered_map>
#include <iostream>
#include <vector>

class GameInput {
    public:

        enum Control {
        
        VehicleAccelerate = 0,
        VehicleBrake,
        VehicleDown,
        VehicleUp,

        // Steer Levels
        FullVehicleTurnLeft,
        FullVehicleTurnRight,
        HalfVehicleTurnLeft,
        HalfVehicleTurnRight,
        QuarterVehicleTurnLeft,
        QuarterVehicleTurnRight,

        Idle, //No Input

        _MaxControls

        };
        
        typedef std::vector<Control> ActiveInput;
        
        void keyboardUpdateInput(const Uint8 *state);
        void xboxControllerUpdateInput(const int leftX, const int leftY, const int lt, const int rt);

        void getActiveControls();

        Control currentAcceleration = Idle;
        Control currentTurn = Idle;

        // Debug
        void setDebugRaycastXYZ(float x, float y, float z);
        
        float debugRaycastX;
        float debugRaycastY;
        float debugRaycastZ;

    private:
        ActiveInput activeControlsBuffer;

};

#endif