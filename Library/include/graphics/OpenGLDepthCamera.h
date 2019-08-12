/*    
    This file is a part of Stonefish.

    Stonefish is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Stonefish is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

//
//  OpenGLDepthCamera.h
//  Stonefish
//
//  Created by Patryk Cieslak on 04/05/18.
//  Copyright (c) 2018-2019 Patryk Cieslak. All rights reserved.
//

#ifndef __Stonefish_OpenGLDepthCamera__
#define __Stonefish_OpenGLDepthCamera__

#include "graphics/OpenGLView.h"

namespace sf
{
    class GLSLShader;
    class Camera;
    class SolidEntity;
    
    //! A class representing a depth camera.
    class OpenGLDepthCamera : public OpenGLView
    {
    public:
        //! A constructor.
        /*!
         \param eyePosition the position of the camera eye in world space [m]
         \param direction a unit vector parallel to the camera optical axis
         \param cameraUp a unit vector pointing to the top edge of the image
         \param originX the x coordinate of the view origin in the program window
         \param originY the y coordinate of the view origin in the program window
         \param width the width of the view
         \param height the height of the view
         \param horizontalFovDeg the horizontal field of view of the camera [deg]
         \param minDepth the distance to the near plane [m]
         \param maxDepth the distance to the far plane [m]
         \param useRanges if true the data generated by the camera is range not Z coordinate
         \param verticalFOVDeg the vertical field of view of the camera [deg] (-1 means square pixels)
         */
        OpenGLDepthCamera(glm::vec3 eyePosition, glm::vec3 direction, glm::vec3 cameraUp,
                          GLint originX, GLint originY, GLint width, GLint height,
                          GLfloat horizontalFOVDeg, GLfloat minDepth, GLfloat maxDepth,
                          bool useRanges = false, GLfloat verticalFOVDeg = -1.f);
        
        //! A destructor.
        ~OpenGLDepthCamera();
        
        //! A method to render the low dynamic range (final) image to the screen.
        /*!
         \param destinationFBO the id of the framebuffer used as the destination for rendering
         */
        void DrawLDR(GLuint destinationFBO);
        
        //! A method returning the eye position.
        glm::vec3 GetEyePosition() const;
        
        //! A method returning a unit vector parallel to the optical axis of the camera.
        glm::vec3 GetLookingDirection() const;
        
        //! A method returning a unit vector pointing to the top edge of the image.
        glm::vec3 GetUpDirection() const;

        //! A method returning the projection matrix.
        glm::mat4 GetProjectionMatrix() const;
        
        //! A method returning the view matrix.
        glm::mat4 GetViewMatrix() const;
        
        //! A method that sets up the camera.
        void SetupCamera();
        
        //! A method used to set up the camera.
        /*!
         \param eye the position of the camera [m]
         \param dir a unit vector parallel to the camera optical axis
         \param up a unit vector pointing to the top edge of the image
         */
        void SetupCamera(glm::vec3 eye, glm::vec3 dir, glm::vec3 up);
        
        //! A method that updates camera world transform.
        void UpdateTransform();

        //! A method that flags the camera as needing update.
        void Update();
        
        //! A method that informs if the camera needs update.
        bool needsUpdate();
        
        //! A method to set a pointer to a camera sensor.
        /*!
         \param cam a pointer to a camera sensor
         \param index the id of the OpenGL depth camera
         */
        void setCamera(Camera* cam, unsigned int index = 0);
        
        //! A method returning the type of the view.
        ViewType getType();
        
        //! A static method to load shaders.
        static void Init();
        
        //! A static method to destroy shaders.
        static void Destroy();
        
    protected:
        void LinearizeDepth();
        void Depth2LinearRanges();
        
        Camera* camera;
        unsigned int idx;
        glm::mat4 cameraTransform;
        glm::vec3 eye;
        glm::vec3 dir;
        glm::vec3 up;
        glm::vec3 tempEye;
        glm::vec3 tempDir;
        glm::vec3 tempUp;
        glm::mat4 projection;
        bool _needsUpdate;
        bool update;
        glm::vec2 range;
        bool usesRanges;
        GLuint renderDepthTex;
        GLuint linearDepthTex;
        GLuint linearDepthFBO;
        static GLSLShader* depth2RangesShader;
        static GLSLShader* depthLinearizeShader;
        static GLSLShader* depthVisualizeShader;
    };
}

#endif
