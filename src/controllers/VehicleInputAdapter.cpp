#include "VehicleInputAdapter.h"

void VehicleInputAdapter::vehicleKeyboardInput(const uint8_t *state) {


  //? 1: Gas , 2: Brakes , 0: No-Action for Gas or brakes

    if (state[82]) { //SDL_SCANCODE_UP
      playerInputQueue.Push(1);
    } else if (state[81]) { // SDL_SCANCODE_DOWN
      playerInputQueue.Push(2);
    } else {
      playerInputQueue.Push(0);
    }
  
  //? 3: Steer Left , 4: Steer Right , 5: No Steering Input

    if (SDL_KB_Input[80]) { // SDL_SCANCODE_LEFT
      playerInputQueue.Push(3);
    } else if (SDL_KB_Input[79]) { // SDL_SCANCODE_RIGHT
      playerInputQueue.Push(4);
    } else {
      playerInputQueue.Push(5);
    }

}