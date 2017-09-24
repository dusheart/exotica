#ifndef KINEMATICELEMENT_H
#define KINEMATICELEMENT_H

#include <exotica/Tools.h>
#include <kdl/segment.hpp>
#include <kdl/frames.hpp>
#include <geometric_shapes/shapes.h>
#include <stack>

class KinematicElement
{
public:
    KinematicElement(int id, std::shared_ptr<KinematicElement> parent, KDL::Segment segment) :
        Parent(parent), Segment(segment), Id(id), IsControlled(false),
        ControlId(-1), Shape(nullptr), isRobotLink(false), ClosestRobotLink(nullptr)
    {
    }
    inline void updateClosestRobotLink()
    {
        std::shared_ptr<KinematicElement> element = Parent;
        ClosestRobotLink = nullptr;
        while(element && element->Id>0)
        {
            if(element->isRobotLink)
            {
                ClosestRobotLink = element;
                break;
            }
            element = element->Parent;
        }
        setChildrenClosestRobotLink();
    }

    int Id;
    int ControlId;
    bool IsControlled;
    std::shared_ptr<KinematicElement> Parent;
    std::vector<std::shared_ptr<KinematicElement>> Children;
    std::shared_ptr<KinematicElement> ClosestRobotLink;
    KDL::Segment Segment;
    KDL::Frame Frame;
    std::vector<double> JointLimits;
    shapes::ShapeConstPtr Shape;
    bool isRobotLink;
private:
    inline void setChildrenClosestRobotLink()
    {
        std::stack<std::shared_ptr<KinematicElement>> elements;
        for(auto child : Children) elements.push(child);
        while(!elements.empty())
        {
            auto parent = elements.top();
            elements.pop();
            parent->ClosestRobotLink = ClosestRobotLink;
            for(auto child : parent->Children) elements.push(child);
        }
    }
};

#endif // KINEMATICELEMENT_H
