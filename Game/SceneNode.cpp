//
// SceneNode.cpp
//

#include "pch.h"
#include "SceneNode.h"


SceneNode::SceneNode()
{
    // Only child nodes can have parents,
    // by default node is not a child node.
    parent = nullptr;
    childrens.clear();
}


SceneNode::~SceneNode()
{
    // Delete all childrens
    for (unsigned int i = 0; i < childrens.size(); ++i)
        delete childrens[i];

    childrens.clear();
}

void SceneNode::AddChild(SceneNode* childNode)
{
    // Set this node to be childNode parent and add it to the childrens list.
    childNode->parent = this;
    childrens.push_back(childNode);
}

void SceneNode::Update(DX::StepTimer const& timer)
{
    // Do not update if childrens empty.
    if (childrens.empty())
        return;

    // Update all children nodes.
    for (auto i = childrens.begin(); i != childrens.end(); ++i)
        (*i)->Update(timer);
}

void SceneNode::Render()
{
    // Do not render if childrens empty.
    if (childrens.empty())
        return;

    // Render all children nodes.
    for (auto i = childrens.begin(); i != childrens.end(); ++i)
        (*i)->Render();
}