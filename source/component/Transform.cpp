#include <radix/component/Transform.hpp>

#include <serine/Archiver.hpp>

#include <radix/Entity.hpp>
#include <radix/component/Player.hpp>
#include <radix/component/RigidBody.hpp>

namespace radix {

void Transform::serialize(serine::Archiver &ar) {
  ar("pos", position);
  if (ar.isLoading() || (ar.isSaving() && (scale.x != 0 || scale.y != 0 || scale.z != 0))) {
    ar("scl", scale);
  }
  ar("orient", orientation);
}

void Transform::setPosition(const Vector3f &val) {
  position = val;
  if (entity.hasComponent<RigidBody>()) {
    btRigidBody &rb = *entity.getComponent<RigidBody>().body;
    btTransform t = rb.getWorldTransform();
    t.setOrigin(val);
    rb.setWorldTransform(t);
  } else if (entity.hasComponent<Player>()) {
    entity.getComponent<Player>().controller->warp(val);
  }
}

void Transform::setScale(const Vector3f &val) {
  scale = val;
}

void Transform::setOrientation(const Quaternion &val) {
  orientation = val;
  if (entity.hasComponent<RigidBody>()) {
    btRigidBody &rb = *entity.getComponent<RigidBody>().body;
    btTransform t = rb.getWorldTransform();
    t.setRotation(val);
    rb.setWorldTransform(t);
  }
}

void Transform::applyModelMtx(Matrix4f &m) const {
  m.translate(position);
  m.rotate(orientation);
  m.scale(scale);
}

void Transform::getModelMtx(Matrix4f &m) const {
  m.setIdentity();
  applyModelMtx(m);
}

} /* namespace radix */
