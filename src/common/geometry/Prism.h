//
// Copyright (C) 2013 OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#ifndef __INET_PRISM_H
#define __INET_PRISM_H

#include "Shape3D.h"
#include "Polygon.h"

namespace inet {

/**
 * This class represents 3 dimensional prism with a polygon base face.
 */
class INET_API Prism : public Shape3D
{
  protected:
    double height;
    Polygon base;
    std::vector<Polygon> faces;
    std::vector<Coord> normalVectorsForFaces;

  protected:
    void genereateFaces();
    Coord calculateOutwardNormalVectorForFace(unsigned int i) const;
    void calculateOutwardNormalVectors();

  public:
    Prism() : height(0) {}
    Prism(double height, const Polygon& base);

    double getHeight() const { return height; }
    void setHeight(double height);

    const Polygon& getBase() const { return base; }
    void setBase(const Polygon& base);

    virtual Coord computeSize() const;
    virtual bool computeIntersection(const LineSegment& lineSegment, Coord& intersection1, Coord& intersection2, Coord& normal1, Coord& normal2) const;
};

} // namespace inet

#endif // ifndef __INET_PRISM_H

