//
//  Robot.cpp
//  Stonefish
//
//  Created by Patryk Cieslak on 5/11/2018.
//  Copyright(c) 2018 Patryk Cieslak. All rights reserved.
//

#include "core/Robot.h"

#include "core/SimulationApp.h"
#include "core/SimulationManager.h"
#include "core/Console.h"
#include "entities/SolidEntity.h"
#include "entities/FeatherstoneEntity.h"
#include "actuators/LinkActuator.h"
#include "actuators/JointActuator.h"
#include "sensors/scalar/LinkSensor.h"
#include "sensors/scalar/JointSensor.h"
#include "sensors/VisionSensor.h"

namespace sf
{

Robot::Robot(std::string uniqueName, bool fixedBase)
{
    name = SimulationApp::getApp()->getSimulationManager()->getNameManager()->AddName(uniqueName);
    dynamics = NULL;
	fixed = fixedBase;
}

Robot::~Robot()
{
	SimulationApp::getApp()->getSimulationManager()->getNameManager()->RemoveName(name);
}

void Robot::getFreeLinkPair(const std::string& parentName, const std::string& childName, unsigned int& parentId, unsigned int& childId)
{
	if(dynamics == NULL)
		cCritical("Robot links not defined!");
		
	if(detachedLinks.size() == 0)
		cCritical("No more free links allocated!");
	
	//Find parent ID
	parentId = UINT32_MAX;
	for(unsigned int i=0; i<dynamics->getNumOfLinks(); ++i)
		if(dynamics->getLink(i).solid->getName() == parentName)
			parentId = i;
	
	if(parentId >= dynamics->getNumOfLinks())
		cCritical("Parent link '%s' not yet joined with robot!", parentName.c_str());
	
	//Find child ID
	childId = UINT32_MAX;
	for(unsigned int i=0; i<detachedLinks.size(); ++i)
		if(detachedLinks[i]->getName() == childName)
			childId = i;
			
	if(childId >= detachedLinks.size())
		cCritical("Child link '%s' doesn't exist!", childName.c_str());
}

SolidEntity* Robot::getLink(const std::string& name)
{
    for(size_t i=0; i<links.size(); ++i)
        if(links[i]->getName() == name) return links[i];
    
    for(size_t i=0; i<detachedLinks.size(); ++i)
        if(detachedLinks[i]->getName() == name) return detachedLinks[i];
    
    return NULL;
}

int Robot::getJoint(const std::string& name)
{
    if(dynamics == NULL)
        cCritical("Robot links not defined!");
    
    for(int i=0; i<dynamics->getNumOfJoints(); ++i)
        if(dynamics->getJointName(i) == name) return i;
    
    return -1;
}

Transform Robot::getTransform() const
{
	if(dynamics != NULL)
		return dynamics->getLinkTransform(0);
	else
		return Transform::getIdentity();
}

void Robot::DefineLinks(SolidEntity* baseLink, std::vector<SolidEntity*> otherLinks)
{
	if(dynamics != NULL)
		cCritical("Robot cannot be redefined!");
	
    links.push_back(baseLink);
    detachedLinks = otherLinks;
	dynamics = new FeatherstoneEntity(name + "_Dynamics", (unsigned short)detachedLinks.size() + 1, baseLink, fixed);
}

void Robot::DefineRevoluteJoint(std::string jointName, std::string parentName, std::string childName, const Transform& origin, const Vector3& axis, std::pair<Scalar,Scalar> positionLimits, Scalar torqueLimit)
{
	unsigned int parentId, childId;
	getFreeLinkPair(parentName, childName, parentId, childId);
	
	//Add link to the dynamic tree
    Transform linkTrans = dynamics->getLinkTransform(parentId) * dynamics->getLink(parentId).solid->getCG2OTransform() * origin;
	dynamics->AddLink(detachedLinks[childId], linkTrans);
    links.push_back(detachedLinks[childId]);
    detachedLinks.erase(detachedLinks.begin()+childId);
	dynamics->AddRevoluteJoint(jointName, parentId, dynamics->getNumOfLinks()-1, linkTrans.getOrigin(), linkTrans.getBasis() * axis);
       
	if(positionLimits.first < positionLimits.second)
		dynamics->AddJointLimit(dynamics->getNumOfJoints()-1, positionLimits.first, positionLimits.second);
	
	//dynamics->AddJointMotor(dynamics->getNumOfJoints()-1, torqueLimit/SimulationApp::getApp()->getSimulationManager()->getStepsPerSecond());
	//dynamics->setJointDamping(dynamics->getNumOfJoints()-1, 0, 0.5);
}

void Robot::DefinePrismaticJoint(std::string jointName, std::string parentName, std::string childName, const Transform& origin, const Vector3& axis, std::pair<Scalar,Scalar> positionLimits, Scalar forceLimit)
{
	unsigned int parentId, childId;
	getFreeLinkPair(parentName, childName, parentId, childId);
	
	//Add link to the dynamic tree
	Transform linkTrans = dynamics->getLinkTransform(parentId) * dynamics->getLink(parentId).solid->getCG2OTransform() * origin;
	dynamics->AddLink(detachedLinks[childId], linkTrans);
    links.push_back(detachedLinks[childId]);
	detachedLinks.erase(detachedLinks.begin()+childId);
    dynamics->AddPrismaticJoint(jointName, parentId, dynamics->getNumOfLinks()-1, linkTrans.getBasis() * axis);
       
	if(positionLimits.first < positionLimits.second)
		dynamics->AddJointLimit(dynamics->getNumOfJoints()-1, positionLimits.first, positionLimits.second);
	
	//dynamics->AddJointMotor(dynamics->getNumOfJoints()-1, forceLimit/SimulationApp::getApp()->getSimulationManager()->getStepsPerSecond());
	//dynamics->setJointDamping(dynamics->getNumOfJoints()-1, 0, 0.5);
}

void Robot::DefineFixedJoint(std::string jointName, std::string parentName, std::string childName, const Transform& origin)
{
	unsigned int parentId, childId;
	getFreeLinkPair(parentName, childName, parentId, childId);
	
	//Add link to the dynamic tree
	Transform linkTrans = dynamics->getLinkTransform(parentId) * dynamics->getLink(parentId).solid->getCG2OTransform() * origin;
	dynamics->AddLink(detachedLinks[childId], linkTrans);
    links.push_back(detachedLinks[childId]);
    detachedLinks.erase(detachedLinks.begin()+childId);
	dynamics->AddFixedJoint(jointName, parentId, dynamics->getNumOfLinks()-1, linkTrans.getOrigin());
}

void Robot::AddLinkSensor(LinkSensor* s, const std::string& monitoredLinkName, const Transform& origin)
{
    SolidEntity* link = getLink(monitoredLinkName);
    if(link != NULL)
    {
        s->AttachToSolid(link, origin);
        sensors.push_back(s);
    }
    else
        cCritical("Link '%s' doesn't exist. Sensor '%s' cannot be attached!", monitoredLinkName.c_str(), s->getName().c_str());
}

void Robot::AddJointSensor(JointSensor* s, const std::string& monitoredJointName)
{
    int jointId = getJoint(monitoredJointName);
    if(jointId > -1)
    {
        s->AttachToJoint(dynamics, jointId);
        sensors.push_back(s);
    }
    else
        cCritical("Joint '%s' doesn't exist. Sensor '%s' cannot be attached!", monitoredJointName.c_str(), s->getName().c_str());
}

void Robot::AddVisionSensor(VisionSensor* s, const std::string& attachmentLinkName, const Transform& origin)
{
    SolidEntity* link = getLink(attachmentLinkName);
    if(link != NULL)
    {
        s->AttachToSolid(link, origin);
        sensors.push_back(s);
    }
    else
        cCritical("Link '%s' doesn't exist. Sensor '%s' cannot be attached!", attachmentLinkName.c_str(), s->getName().c_str());
}

void Robot::AddLinkActuator(LinkActuator* a, const std::string& actuatedLinkName, const Transform& origin)
{
    SolidEntity* link = getLink(actuatedLinkName);
    if(link != NULL)
    {
        a->AttachToSolid(link, origin);
        actuators.push_back(a);
    }
    else
        cCritical("Link '%s' doesn't exist. Actuator '%s' cannot be attached!", actuatedLinkName.c_str(), a->getName().c_str());
}

void Robot::AddJointActuator(JointActuator* a, const std::string& actuatedJointName)
{
    int jointId = getJoint(actuatedJointName);
    if(jointId > -1)
    {
        a->AttachToJoint(dynamics, jointId);
        actuators.push_back(a);
    }
    else
        cCritical("Joint '%s' doesn't exist. Actuator '%s' cannot be attached!", actuatedJointName.c_str(), a->getName().c_str());
}

void Robot::AddToSimulation(SimulationManager* sm, const Transform& origin)
{
    if(detachedLinks.size() > 0)
        cCritical("Detected unconnected links!");
    
    sm->AddFeatherstoneEntity(dynamics, origin);
    for(size_t i=0; i<sensors.size(); ++i)
        sm->AddSensor(sensors[i]);
    for(size_t i=0; i<actuators.size(); ++i)
        sm->AddActuator(actuators[i]);
}

}
