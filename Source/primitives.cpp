#include "primitives.h"

Shape::Shape(const Polygon& polygon_type_init, const int& vertices_init)
	:polygon_type(polygon_type_init), vertices(std::vector<Vertex>(vertices_init, Vertex())) {}

void Shape::x(const float& _x)
{
	transform.x = _x;
	update_position();
}

void Shape::y(const float& _y)
{
	transform.y = _y;
	update_position();
}

void Shape::xa(const float& _xa)
{
	transform.xa = _xa;
	update_shape();
}

void Shape::ya(const float& _ya)
{
	transform.ya = _ya;
	update_shape();
}

void Shape::w(const float& _w)
{
	transform.w = _w;
	update_shape();
}

void Shape::h(const float& _h)
{
	transform.h = _h;
	update_shape();
}

void Shape::s(const float& _s)
{
	transform.s = _s;
	update_transform();
}

void Shape::r(const float& _r)
{
	transform.r = _r;
	update_transform();
}

void Shape::t(const Transform& _transform)
{
	transform = _transform;
	update_position();
	update_shape();
	update_transform();
}

float Shape::x() { return transform.x; }

float Shape::y() { return transform.y; }

float Shape::xa() { return transform.xa; }

float Shape::ya() { return transform.ya; }

float Shape::w() { return transform.w; }

float Shape::h() { return transform.h; }

float Shape::s() { return transform.s; }

float Shape::r() { return transform.r; }

Transform Shape::t() { return transform; }

void Shape::update_position() {}

void Shape::update_shape() {}

void Shape::update_transform() {}