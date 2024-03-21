#include <glm/gtc/type_ptr.hpp>

#include <Transform.hpp>
#include <SceneNode.hpp>
#include <Mesh.hpp>

// Constructors
SceneNode::SceneNode() {}

SceneNode::SceneNode(Mesh *mesh) {
    this->mesh = mesh;
}

// Destructor
SceneNode::~SceneNode() {
    for(SceneNode* child : children) {
        delete child;
    }
}

// Public methods
void SceneNode::set_parent(SceneNode *parent) {
    this->parent = parent;
    parent->children.push_back(this);
}

void SceneNode::add_child(SceneNode *child) {
    child->set_parent(this);
}

glm::mat4 SceneNode::get_world_transform() {
    if(parent) {
        return parent->get_world_transform() * transform.get_matrix();
    }
    else {
        return transform.get_matrix();
    }
}

void SceneNode::draw() {
    if(mesh) {
        glm::mat4 model = get_world_transform();
        mesh->shader.useShader();
        mesh->shader.setBindMatrix4fv("model", 1, 0, glm::value_ptr(model));
        mesh->draw();
    }
    for(SceneNode* child : children) {
        child->draw();
    }
}