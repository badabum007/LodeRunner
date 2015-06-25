#ifndef POINT3D_H
#define POINT3D_H

#include <gl/gl.h>
#include <cstdio>
class Point3D
{
    public:
        Point3D();
        virtual ~Point3D();
        Point3D(GLfloat x, GLfloat y, GLfloat z);
        Point3D(const Point3D& other);
        GLfloat getX() const;
        GLfloat getY() const;
        GLfloat getZ() const;

        int getMatrixX() const;
        int getMatrixZ() const;

        void setX(GLfloat x);
        void setY(GLfloat y);
        void setZ(GLfloat z);
        void set_coords(GLfloat x, GLfloat y, GLfloat z);

        Point3D operator- (const Point3D& other) const;
        Point3D operator+ (const Point3D& other) const;
        Point3D operator* (double scalar) const;

        explicit operator const GLfloat*() const;

        friend Point3D operator* (const double& left, const Point3D& right);
        double distance_to(const Point3D& other) const;

        const GLfloat* as_opengl_vector() const;
        GLfloat x, y, z;
    protected:
        GLfloat* values = NULL;
    private:
};

#endif // POINT3D_H
