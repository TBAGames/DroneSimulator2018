#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>

#include "scene_node.h"
#include "settings.h"

namespace game {

SceneNode::SceneNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture){

    // Set name of scene node
    name_ = name;

    // Set geometry
    if (geometry->GetType() == PointSet){
        mode_ = GL_POINTS;
    } else if (geometry->GetType() == Mesh){
        mode_ = GL_TRIANGLES;
    } else {
        throw(std::invalid_argument(std::string("Invalid type of geometry")));
    }

    array_buffer_ = geometry->GetArrayBuffer();
    element_array_buffer_ = geometry->GetElementArrayBuffer();
    size_ = geometry->GetSize();

    // Set material (shader program)
    if (material->GetType() != Material){
        throw(std::invalid_argument(std::string("Invalid type of material")));
    }

    material_ = material->GetResource();

    // Set texture
    if (texture){
        texture_ = texture->GetResource();
    } else {
        texture_ = 0;
    }

    // Other attributes
    scale_ = glm::vec3(1.0, 1.0, 1.0);

	children_ = std::vector<SceneNode *>(0);
	parent_ = NULL;
}

SceneNode::SceneNode(const std::string name) {

	// Set name of scene node
	name_ = name;

	// Other attributes
	scale_ = glm::vec3(1.0, 1.0, 1.0);

	children_ = std::vector<SceneNode *>(0);
	parent_ = NULL;
}


SceneNode::~SceneNode(){
}


const std::string SceneNode::GetName(void) const {

    return name_;
}


glm::vec3 SceneNode::GetPosition(void) const {

    return position_;
}

glm::quat SceneNode::GetOrientation(void) const {

    return orientation_;
}


glm::vec3 SceneNode::GetScale(void) const {

    return scale_;
}


void SceneNode::SetPosition(glm::vec3 position){
	
	std::vector<glm::vec3> offsets;

	// Get childrens' local positions
	if (parent_ != NULL) {
		for (int i = 0; i < children_.size(); i++) {
			SceneNode * child = children_.at(i);
			offsets.push_back(child->GetPosition() - position_);
		}
	}

	position_ = position;

	// Update children's positions
	if (parent_ != NULL) {
		for (int i = 0; i < children_.size(); i++) {
			SceneNode * child = children_.at(i);
			child->SetPosition(position_ + offsets.at(i));
		}
	}
}


void SceneNode::SetOrientation(glm::quat orientation){

    orientation_ = orientation;
}


void SceneNode::SetScale(glm::vec3 scale){

    scale_ = scale;
}


void SceneNode::Translate(glm::vec3 trans){
	
    position_ += trans;

	//if (parent_ != NULL) {
		for (int i = 0; i < children_.size(); i++)
		{
			//std::cout << "Children of " << this->GetName() << ": " << std::endl;
			children_.at(i)->Translate(trans);
		}
	//}
}

void SceneNode::Rotate(glm::quat rot){

    orientation_ *= rot;
    orientation_ = glm::normalize(orientation_);

	//if (parent_ != NULL) {
		for (int i = 0; i < children_.size(); i++)
		{
			//SceneNode * child = children_.at(i);
			/*glm::vec3 offset = child->GetPosition() - position_;
			glm::vec3 direction = glm::normalize(-offset);*/
			children_.at(i)->Rotate(rot);
		}
	//}
}


void SceneNode::Scale(glm::vec3 scale){

    scale_ *= scale;
}

glm::vec3 SceneNode::GetForward(void) const {

	glm::vec3 current_forward = orientation_ * forward_;
	return -current_forward; // Return -forward since the camera coordinate system points in the opposite direction
}


glm::vec3 SceneNode::GetSide(void) const {

	glm::vec3 current_side = orientation_ * side_;
	return current_side;
}


glm::vec3 SceneNode::GetUp(void) const {

	glm::vec3 current_forward = orientation_ * forward_;
	glm::vec3 current_side = orientation_ * side_;
	glm::vec3 current_up = glm::cross(current_forward, current_side);
	current_up = glm::normalize(current_up);
	return current_up;
}


void SceneNode::Pitch(float angle) {

	glm::quat rotation = glm::angleAxis(angle, GetSide());
	orientation_ = rotation * orientation_;
	orientation_ = glm::normalize(orientation_);

	for (int i = 0; i<children_.size(); i++) {
		glm::vec3 offset_vec = children_.at(i)->GetPosition() - GetPosition();
		glm::vec3 translation = qrot(rotation, offset_vec);
		children_.at(i)->Translate(translation);
		children_.at(i)->SetOrientation(orientation_);
	}
}


void SceneNode::Yaw(float angle) {

	glm::quat rotation = glm::angleAxis(angle, GetUp());
	orientation_ = rotation * orientation_;
	orientation_ = glm::normalize(orientation_);

	for (int i = 0; i<children_.size(); i++) {
		glm::vec3 offset_vec = children_.at(i)->GetPosition() - GetPosition();
		glm::vec3 translation = qrot(rotation, offset_vec);
		children_.at(i)->Translate(translation);
		children_.at(i)->SetOrientation(orientation_);
	}
}


void SceneNode::Roll(float angle) {

	glm::quat rotation = glm::angleAxis(angle, GetForward());
	orientation_ = rotation * orientation_;
	orientation_ = glm::normalize(orientation_);

	for (int i = 0; i<children_.size(); i++) {
		glm::vec3 offset_vec = children_.at(i)->GetPosition() - GetPosition();
		glm::vec3 translation = qrot(rotation, offset_vec);
		children_.at(i)->Translate(translation);
		children_.at(i)->SetOrientation(orientation_);
	}
}


// Function derived from https://code.google.com/archive/p/kri/wikis/Quaternions.wiki
glm::vec3 SceneNode::qrot(glm::quat q, glm::vec3 v) {

	glm::vec4 qv = glm::vec4(q.x, q.y, q.z, q.w);
	return 2.0f*glm::cross(glm::vec3(qv), glm::cross(glm::vec3(qv), v) + q.w*v);
}

GLenum SceneNode::GetMode(void) const {

    return mode_;
}


GLuint SceneNode::GetArrayBuffer(void) const {

    return array_buffer_;
}


GLuint SceneNode::GetElementArrayBuffer(void) const {

    return element_array_buffer_;
}


GLsizei SceneNode::GetSize(void) const {

    return size_;
}


GLuint SceneNode::GetMaterial(void) const {

    return material_;
}


std::vector<SceneNode *> SceneNode::GetChildren() {
	
	return children_;
}

SceneNode *SceneNode::GetParent() {
	
	return parent_;
}

void SceneNode::AddChild(SceneNode *child) {
	
	SceneNode *prevParent = child->GetParent();

	std::cout << "Has Prev Parent? " << ((prevParent != NULL) ? "Yes" : "No") << std::endl;

	// BROKEN
	// Remove child from any other parent->child relation
	/*if (prevParent != NULL) {
		prevParent->RemoveChild(child);
	}*/

	// Add node to tree
	std::cout << "Here" << std::endl;
	child->SetParent(this);
	children_.push_back(child);
	std::string children = "Parent: " + ((this->GetParent() != NULL) ? this->GetParent()->GetName() : "?") + " -> " + this->GetName() + " -> Children: ";
	for (int i = 0; i < children_.size(); i++)
	{
		children += children_.at(i)->GetName() + ", ";
	}
	std::cout << children << std::endl;
}

void SceneNode::SetParent(SceneNode *parent) {

	parent_ = parent;
}

void SceneNode::RemoveChild(SceneNode * node) {

	for (int i = 0; i < children_.size(); i++) {
		SceneNode *child;
		if ((child = children_.at(i)) == node) {
			children_.erase(children_.begin() + i);
			child->RemoveParent();
			return;
		}
	}
}

SceneNode *SceneNode::GetChild(std::string name) {
	
	SceneNode * child;

	for (int i = 0; i < children_.size(); i++)
	{
		if ((child = children_.at(i))->GetName() == name) {
			return child;
		}
	}
}

void SceneNode::RemoveParent() {
	parent_ = NULL;
}


std::vector<SceneNode *> *SceneNode::BuildNodeSubTree(std::vector<SceneNode *> *buildTree) {

	buildTree->insert(buildTree->end(), children_.begin(), children_.end());

	for (int i = 0; i < children_.size(); i++) {
		children_.at(i)->BuildNodeSubTree(buildTree);
	}

	return buildTree;
}



/*void SceneNode::Draw(Camera *camera){

    // Select proper material (shader program)
    glUseProgram(material_);

    // Set geometry to draw
    glBindBuffer(GL_ARRAY_BUFFER, array_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer_);

    // Set globals for camera
    camera->SetupShader(material_);

    // Set world matrix and other shader input variables
    SetupShader(material_);

    // Draw geometry
    if (mode_ == GL_POINTS){
        glDrawArrays(mode_, 0, size_);
    } else {
        glDrawElements(mode_, size_, GL_UNSIGNED_INT, 0);
    }
}*/

void SceneNode::SetupGeometry()
{
	if (!material_) {
		return;
	}

	// Select proper material (shader program)
	glUseProgram(material_);

	// Set geometry to draw
	glBindBuffer(GL_ARRAY_BUFFER, array_buffer_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer_);
}


void SceneNode::DrawGeometry(bool toonModeOn, float roughness)
{
	// Set world matrix and other shader input variables
	SetupShader(material_, toonModeOn, roughness);

	// Draw geometry
	if (mode_ == GL_POINTS) {
		glDrawArrays(mode_, 0, size_);
	}
	else {
		glDrawElements(mode_, size_, GL_UNSIGNED_INT, 0);
	}
}


void SceneNode::Update(void){

    // Do nothing for this generic type of scene node
}


void SceneNode::SetupShader(GLuint program, bool toonModeOn, float roughness){

    // Set attributes for shaders
    GLint vertex_att = glGetAttribLocation(program, "vertex");
    glVertexAttribPointer(vertex_att, 3, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), 0);
    glEnableVertexAttribArray(vertex_att);

    GLint normal_att = glGetAttribLocation(program, "normal");
    glVertexAttribPointer(normal_att, 3, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), (void *) (3*sizeof(GLfloat)));
    glEnableVertexAttribArray(normal_att);

    GLint color_att = glGetAttribLocation(program, "color");
    glVertexAttribPointer(color_att, 3, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), (void *) (6*sizeof(GLfloat)));
    glEnableVertexAttribArray(color_att);

    GLint tex_att = glGetAttribLocation(program, "uv");
    glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), (void *) (9*sizeof(GLfloat)));
    glEnableVertexAttribArray(tex_att);

    // World transformation
    glm::mat4 scaling = glm::scale(glm::mat4(1.0), scale_);
    glm::mat4 rotation = glm::mat4_cast(orientation_);
    glm::mat4 translation = glm::translate(glm::mat4(1.0), position_);
    glm::mat4 transf = translation * rotation * scaling;

    GLint world_mat = glGetUniformLocation(program, "world_mat");
    glUniformMatrix4fv(world_mat, 1, GL_FALSE, glm::value_ptr(transf));

    // Normal matrix
    glm::mat4 normal_matrix = glm::transpose(glm::inverse(transf));
    GLint normal_mat = glGetUniformLocation(program, "normal_mat");
    glUniformMatrix4fv(normal_mat, 1, GL_FALSE, glm::value_ptr(normal_matrix));

	// Toon setting
	GLint toon_enabled = glGetUniformLocation(program, "toon_enabled");
	glUniform1i(toon_enabled, toonModeOn);

	GLint surface_roughness = glGetUniformLocation(program, "surface_roughness");
	glUniform1f(surface_roughness, roughness);

    // Texture
    if (texture_){
        GLint tex = glGetUniformLocation(program, "texture_map");
        glUniform1i(tex, 0); // Assign the first texture to the map
        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, texture_); // First texture we bind

		if (!toonModeOn) {
			// Define texture interpolation
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
    }

    // Timer
    GLint timer_var = glGetUniformLocation(program, "timer");
    double current_time = glfwGetTime();
    glUniform1f(timer_var, (float) current_time);
}

} // namespace game;
