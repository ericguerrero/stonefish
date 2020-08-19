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



#ifndef __Stonefish_StereoCamera__
#define __Stonefish_StereoCamera__

#include <functional>
#include "sensors/vision/ColorCamera.h"

namespace sf
{
    
    //! A class representing a stereo camera.
    class StereoCamera
    {
    public:
        //! A constructor.
        /*!
         \param uniqueName a name for the sensor
         \param resolutionX the horizontal resolution [pix]
         \param resolutionY the vertical resolution[pix]
         \param horizFOVDeg the horizontal field of view [deg]
         \param frequency the sampling frequency of the sensor [Hz] (-1 if updated every simulation step)
         \param minDistance the minimum drawing distance [m]
         \param maxDistance the maximum drawing distance [m]
         \param baseline stereo camera baseline [mm]
         */
        StereoCamera(std::string uniqueName, unsigned int resolutionX, unsigned int resolutionY, Scalar horizFOVDeg, Scalar frequency, Scalar baseline, Scalar minDistance, Scalar maxDistance);
    

        //! A destructor.
        ~StereoCamera();


        //! Get stereo camera
        std::pair<ColorCamera*,ColorCamera*> getStereoPair();


        //! A method to get the camera baseline in case of included in an stereo pair
        Scalar getBaseline();
        

    private:
        ColorCamera leftCam;
        ColorCamera rightCam;
        Scalar bl;
    };
}

#endif
