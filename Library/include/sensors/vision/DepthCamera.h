//
//  DepthCamera.h
//  Stonefish
//
//  Created by Patryk Cieslak on 07/05/18.
//  Copyright (c) 2018 Patryk Cieslak. All rights reserved.
//

#ifndef __Stonefish_DepthCamera__
#define __Stonefish_DepthCamera__

#include <functional>
#include "sensors/vision/Camera.h"
#include "graphics/OpenGLDepthCamera.h"

namespace sf
{
    //!
    class DepthCamera : public Camera
    {
    public:
        DepthCamera(std::string uniqueName, uint32_t resX, uint32_t resY, Scalar horizFOVDeg, Scalar minDepth, Scalar maxDepth, Scalar frequency = Scalar(-1));
        virtual ~DepthCamera();
        
        void InternalUpdate(Scalar dt);
        void SetupCamera(const Vector3& eye, const Vector3& dir, const Vector3& up);
        void InstallNewDataHandler(std::function<void(DepthCamera*)> callback);
        void NewDataReady();
        GLfloat* getDataPointer(); //Should be only used in the callback
        glm::vec2 getDepthRange();
        
    private:
        OpenGLDepthCamera* glCamera;
        GLfloat* imageData;
        glm::vec2 depthRange;
        std::function<void(DepthCamera*)> newDataCallback;
    };
}

#endif
