#pragma once

#include <JuceHeader.h>

class Transform
{
public:

	Transform() {}

	Transform(const float& x_init, const float& y_init, const float& xa_init, const float& ya_init, const float& w_init, const float& h_init, const float& s_init, const float& r_init) :
		x(x_init), y(y_init), xa(xa_init), ya(ya_init), w(w_init), h(h_init), s(s_init), r(r_init) {}

	float x = 0;
	float y = 0;
	float xa = 0;
	float ya = 0;
	float w = 0;
	float h = 0;
	float s = 1;
	float r = 0;
};

template <class T>
class Vector2
{
public:

	Vector2() {}

	Vector2(const T& _x, const T& _y)
		:x(_x), y(_y) {}

	Vector2(const Vector2<T>& vector)
		:x(vector.x), y(vector.y) {}

	T x{};
	T y{};
};

typedef Vector2<float> v2f;

template <class T>
class Color
{
public:

	Color() {}

	Color(const T& _r, const T& _g, const T& _b, const T& _a)
		:r(_r), g(_g), b(_b), a(_a) {}

	Color(const Color& color)
		:r(color.r), g(color.g), b(color.b), a(color.a) {}

	T r{};
	T g{};
	T b{};
	T a{};
};

typedef Color<float> colorF;

class Vertex
{
public:

	Vertex() {}

	Vertex(const float& x, const float& y, const float& r, const float& g, const float& b, const float& a)
		:position{ x,y }, color{ r,g,b,a } {}

	Vertex(const v2f& _position, const colorF& _color)
		:position{ _position.x,_position.y }, color{ _color.r, _color.g, _color.b, _color.a } {}

	float position[2]{ 0,0 };
	float color[4]{ 1,1,1,1 };
};

typedef std::vector<Vertex> vBuffer;

class Shape
{
public:

	enum class Polygon { points, lines, line_loop, line_strip, triangles, triangle_strip, trialge_fan, quads };

	Shape(const Polygon& polygon_type_init, const int& vertices_init);

	void x(const float& _x);
	void y(const float& _y);
	void xa(const float& _xa);
	void ya(const float& _ya);
	void w(const float& _w);
	void h(const float& _h);
	void s(const float& _s);
	void r(const float& _r);
	void t(const Transform& _transform);

	float x();
	float y();
	float xa();
	float ya();
	float w();
	float h();
	float s();
	float r();
	Transform t();

	std::vector<unsigned int> get_index_buffer()
	{
		if (vertex_index_initialized) return vertex_index;

		for (int i = 0; i < vertices.size(); ++i) vertex_index.push_back(i);

		vertex_index_initialized = true;

		return vertex_index;
	}

	vBuffer vertices;

	std::vector<unsigned int> vertex_index;

	bool vertex_index_initialized = false;

	Polygon polygon_type = Polygon::points;

	int get_polygon_type()
	{
		switch (polygon_type)
		{
		case Polygon::points:return juce::gl::GL_POINTS;
		case Polygon::lines:return juce::gl::GL_LINES;
		case Polygon::line_loop:return juce::gl::GL_LINE_LOOP;
		case Polygon::line_strip:return juce::gl::GL_LINE_STRIP;
		case Polygon::triangles:return juce::gl::GL_TRIANGLES;
		case Polygon::triangle_strip:return juce::gl::GL_TRIANGLE_STRIP;
		case Polygon::trialge_fan:return juce::gl::GL_TRIANGLE_FAN;
		case Polygon::quads:return juce::gl::GL_QUADS;
		}
	}

	GLuint vbo = 0;
	GLuint ibo = 0;

	juce::String shader_vertex =
		R"(
            #version 330 core
            
            // Input attributes.
            in vec4 position;      
            in vec4 sourceColour;
            
            // Output to fragment shader.
            out vec4 fragColour;
            
            void main()
            {
                // Set the position to the attribute we passed in.
                gl_Position = position;
                
                // Set the frag colour to the attribute we passed in.
                // This value will be interpolated for us before the fragment
                // shader so the colours we defined ateach vertex will create a
                // gradient across the shape.
                fragColour = sourceColour;
            }
        )";

	juce::String shader_fragment =
		R"(
            #version 330 core
            
            // The value that was output by the vertex shader.
            // This MUST have the exact same name that we used in the vertex shader.
            in vec4 fragColour;
            
            void main()
            {
                // Set the fragment's colour to the attribute passed in by the
                // vertex shader.
                gl_FragColor = fragColour;
            }
        )";

protected:

	virtual void update_position();
	virtual void update_shape();
	virtual void update_transform();

	Transform transform;
};

class Rectangle : public Shape
{
public:

	Rectangle() 
		:Shape(Polygon::quads, 4)
	{
		x(0.f);
		y(0.f);
		xa(.25f);
		ya(.25f);
		w(0.5f);
		h(0.5f);

		//setColor(colorF(1, 1, 0, 1));
	}

	void setColor(const colorF& color_init)
	{
		for (int i = 0; i < vertices.size(); ++i)
		{
			vertices.at(i).color[0] = color_init.r;
			vertices.at(i).color[1] = color_init.g;
			vertices.at(i).color[2] = color_init.b;
			vertices.at(i).color[3] = color_init.a;
		}
	}

	void update_position() override{}

	void update_shape()override
	{
		vertices.at(0) = Vertex(v2f(transform.x - transform.xa, transform.y - transform.ya), colorF(1, 0, 0, 1)); // should be top left
		vertices.at(1) = Vertex(v2f(transform.x - transform.xa + transform.w, transform.y - transform.ya), colorF(0, 0, 1, 1)); // should be top right
		vertices.at(2) = Vertex(v2f(transform.x - transform.xa + transform.w, transform.y - transform.ya + transform.h), colorF(0, 0, 0, 1));
		vertices.at(3) = Vertex(v2f(transform.x - transform.xa , transform.y - transform.ya + transform.h), colorF(0, 0, 0, 1));
	}

	void update_transform()override
	{

	}

	colorF color;


};