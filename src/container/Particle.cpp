/*
 * Particle.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "Particle.h"

#include <iostream>

#include "utils/ArrayUtils.h"

Particle::Particle(int type_arg) {
    type = type_arg;
    f = {0., 0., 0.};
    old_f = {0., 0., 0.};
    x = {0, 0, 0};
    v = {0, 0, 0};
}

Particle::Particle(const Particle& other) {
    x = other.x;
    v = other.v;
    f = other.f;
    old_f = other.old_f;
    m = other.m;
    type = other.type;
}

Particle::Particle(std::array<double, 3> x_arg, std::array<double, 3> v_arg,
                   double m_arg, int type_arg)
    : x(x_arg),
      v(v_arg),
      f({0., 0., 0.}),
      old_f({0., 0., 0.}),
      m(m_arg),
      type(type_arg) {}

Particle::~Particle() = default;

const std::array<double, 3>& Particle::getX() const { return x; }
// Getter
const std::array<double, 3>& Particle::getV() const { return v; }
const std::array<double, 3>& Particle::getF() const { return f; }
const std::array<double, 3>& Particle::getOldF() const { return old_f; }
double Particle::getM() const { return m; }
int Particle::getType() const { return type; }

void Particle::addF(std::array<double, 3>& aF) {
    for (int i = 0; i < 3; i++) {
        f[i] += aF[i];
    }
}

void Particle::addX(std::array<double, 3>& aX) {
    for (int i = 0; i < 3; i++) {
        x[i] += aX[i];
    }
}

void Particle::addV(std::array<double, 3>& aV) {
    for (int i = 0; i < 3; i++) {
        v[i] += aV[i];
    }
}

void Particle::nextIteration() {
    old_f = f;
    f = {0, 0, 0};
}

std::string Particle::toString() const {
    std::stringstream stream;
    stream << "Particle: X:" << x << " v: " << v << " f: " << f
           << " old_f: " << old_f << " type: " << type;
    return stream.str();
}

bool Particle::operator==(const Particle& other) const {
    return (x == other.x) and (v == other.v) and (f == other.f) and
           (type == other.type) and (m == other.m) and (old_f == other.old_f);
}

std::ostream& operator<<(std::ostream& stream, Particle& p) {
    stream << p.toString();
    return stream;
}
