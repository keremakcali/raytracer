#include "object.h"

Object::Object(const Material *material) : material(material) {}

const Material &Object::getMaterial() {
    return *material;
}
