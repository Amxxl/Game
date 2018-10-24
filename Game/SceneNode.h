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

        void AddNode(SceneNode* node);
        void SetNative(SceneNode* node);

        virtual void Update(DX::StepTimer const& timer);
        virtual void Render();

    protected:
        std::vector<SceneNode*> nodes;
        SceneNode* native;
};

