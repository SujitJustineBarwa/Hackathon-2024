#ifndef WALL_H
#define WALL_H

#include "cell.h"

class wall
{
   public:
       wall();
       virtual ~wall();

       float area_of_triangle(float x3, float y3);

       // Getters and setters for coordinates
       void setX1(float x);
       float getX1() const;
       void setY1(float y);
       float getY1() const;
       void setX2(float x);
       float getX2() const;
       void setY2(float y);
       float getY2() const;

       // Getters and setters for cells
        void setCell(cell* c1, cell* c2);
        cell* getCell1() const;
        cell* getCell2() const;

   private:
       float x1, y1, x2, y2;
       cell* cell_1;
       cell* cell_2;
};

#endif // WALL_H
