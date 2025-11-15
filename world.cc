/* 

Copyright (C) 2000 Matthew Danish

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include"world.h"
#include"vector.h"
#include"defines.h"
#include<stdlib.h>

extern int FLOOR_BLOCKS_Z,FLOOR_BLOCKS_X,NUM_FOOD,NUM_ORGANISMS;

WorldClass::WorldClass(int argc, char *argv[])
{
  size.X(WORLD_X);
  size.Y(WORLD_Y);
  size.Z(WORLD_Z);
  position.Y(1);
  position.Z(5);
}

WorldClass::~WorldClass() {

}

VectorClass &WorldClass::Size() {
  return size;
}

VectorClass &WorldClass::Pos() {
  return position;
}

AngleClass &WorldClass::Heading() {
  return heading;
}

VectorClass * WorldClass::NewPosition() {
  VectorClass *v;

  // Ensure minimum spawn area even for 1x1 world (use FLOOR_QUAD_SIZE as minimum)
  double spawn_width = Size().X() > 0 ? Size().X() : FLOOR_QUAD_SIZE;
  double spawn_depth = Size().Z() > 0 ? Size().Z() : FLOOR_QUAD_SIZE;
  
  // Generate uniformly distributed random positions across the world
  // Use better random distribution: map [0, RAND_MAX] to [0, 1] more accurately
  double x_pos = (double(rand()) / RAND_MAX) * spawn_width - spawn_width/2.0;
  double z_pos = (double(rand()) / RAND_MAX) * spawn_depth - spawn_depth/2.0;
  
  v = new VectorClass(x_pos, 0, z_pos);
  
  return v;
}

bool WorldClass::ChangePosition(VectorClass &p, VectorClass &d) {
  
  if(p.X() + d.X() > (FLOOR_BLOCKS_X/2)*FLOOR_QUAD_SIZE)
    d.X((FLOOR_BLOCKS_X/2)*FLOOR_QUAD_SIZE - p.X());
  if(p.X() + d.X() < (-FLOOR_BLOCKS_X/2-1)*FLOOR_QUAD_SIZE)
    d.X((-FLOOR_BLOCKS_X/2-1)*FLOOR_QUAD_SIZE - p.X());


  if(p.Z() + d.Z() > FLOOR_BLOCKS_Z/2*FLOOR_QUAD_SIZE)
    d.Z(FLOOR_BLOCKS_Z/2*FLOOR_QUAD_SIZE - p.Z());
  if(p.Z() + d.Z() < (-FLOOR_BLOCKS_Z/2-1)*FLOOR_QUAD_SIZE)
    d.Z((-FLOOR_BLOCKS_Z/2-1)*FLOOR_QUAD_SIZE - p.Z());
  
  p.X(p.X() + d.X());
  p.Y(p.Y() + d.Y());
  p.Z(p.Z() + d.Z());

  return true;
}

