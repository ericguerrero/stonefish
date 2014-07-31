//
//  Trajectory.h
//  Stonefish
//
//  Created by Patryk Cieslak on 25/05/2014.
//  Copyright (c) 2014 Patryk Cieslak. All rights reserved.
//

#ifndef __Stonefish_Trajectory__
#define __Stonefish_Trajectory__

#include "Sensor.h"
#include "SolidEntity.h"

class Trajectory : public Sensor
{
public:
    Trajectory(std::string uniqueName, SolidEntity* attachment, btTransform relativeFrame = btTransform::getIdentity(), btScalar frequency = btScalar(-1.), unsigned int historyLength = 0);
    
    void InternalUpdate(btScalar dt);
    void Reset();
    void Render();
    
private:
    //parameters
    SolidEntity* solid;
    btTransform relToSolid;
};


#endif
