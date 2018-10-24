//
// SceneNode.cpp
//

#include "pch.h"
#include "SceneNode.h"


SceneNode::SceneNode()
{
    // By default this node is native.
    native = nullptr;
    nodes.clear();
}


SceneNode::~SceneNode()
{
    // Delete all nodes
    for (unsigned int i = 0; i < nodes.size(); ++i)
        delete nodes[i];

    nodes.clear();
}

void SceneNode::AddNode(SceneNode* node)
{
    // Set this node to be native of other node and add second to the nodes list.
    node->native = this;
    nodes.push_back(node);
}

void SceneNode::SetNative(SceneNode* node)
{
    // Do not applied if already have native.
    if (native != nullptr)
        return;

    // Otherwise add this to the native node.
    node->AddNode(this);
    native = node;
}

void SceneNode::Update(DX::StepTimer const& timer)
{
    // Do not update if nodes are empty.
    if (nodes.empty())
        return;

    // Update all nodes of native node.
    for (auto i = nodes.begin(); i != nodes.end(); ++i)
        (*i)->Update(timer);
}

void SceneNode::Render()
{
    // Do not render if nodes empty.
    if (nodes.empty())
        return;

    // Render all nodes of native.
    for (auto i = nodes.begin(); i != nodes.end(); ++i)
        (*i)->Render();
}