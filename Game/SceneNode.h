//
// SceneNode.h - @todo: improve functionality.
//

#pragma once

#include <vector>
#include "StepTimer.h"

class SceneNode
{
    public:
        SceneNode();
        virtual ~SceneNode();

        void AddChild(SceneNode* childNode);
        
        virtual void Update(DX::StepTimer const& timer);
        virtual void Render();

    protected:
        std::vector<SceneNode*> childrens;
        SceneNode* parent;

};

