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


#include "sensors/vision/StereoCamera.h"

namespace sf
{

StereoCamera::StereoCamera(std::string uniqueName, unsigned int resolutionX, unsigned int resolutionY, Scalar horizFOVDeg, Scalar frequency, Scalar baseline, Scalar minDistance, Scalar maxDistance)
{
    leftCam = new ColorCamera(uniqueName + "/left", resolutionX, resolutionY, horizFOVDeg, frequency, minDistance, maxDistance);
    rightCam = new ColorCamera(uniqueName + "/right", resolutionX, resolutionY, horizFOVDeg, frequency, minDistance, maxDistance);
    bl = baseline;
}

StereoCamera::~StereoCamera()
{
    leftCam = NULL;
    rightCam = NULL;
    bl = NULL;
}

std::pair<ColorCamera*,ColorCamera*> StereoCamera::getStereoPair()
{
    return std::pair<leftCam,rightCam>;
}

Scalar StereoCamera::getBaseline()
{
    return bl;
}

}