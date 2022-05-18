#include "gl.h"

OglComponent::OglComponent()
{
	setOpaque(true);

	context.setRenderer(this);
	context.setContinuousRepainting(false);
	context.attachTo(*this);

	shapes.push_back(std::make_unique<Rectangle>());
	//shapes.push_back(std::make_unique<Rectangle>());

	//shapes.back()
}

OglComponent::~OglComponent()
{
	context.detach();
}

void OglComponent::setFrameRate(const int& fps_init)
{
	stopTimer();
	fps = fps_init;
	startTimerHz(fps);
}

void OglComponent::timerCallback()
{
	context.triggerRepaint();
}

void OglComponent::component_add()
{

}

void OglComponent::component_remove()
{

}

void OglComponent::newOpenGLContextCreated()
{
	shaderProgram.reset(new juce::OpenGLShaderProgram(context)); // Create an instance of OpenGLShaderProgram

	bool compile_has_error = false;

	for (int i = 0; i < shapes.size(); ++i)
	{
		context.extensions.glGenBuffers(1, &shapes.at(i)->vbo); // Generate 1 buffer, using our vbo variable to store its ID.

		context.extensions.glGenBuffers(1, &shapes.at(i)->ibo); // Generate 1 more buffer, this time using our IBO variable.

		context.extensions.glBindBuffer(juce::gl::GL_ARRAY_BUFFER, shapes.at(i)->vbo); // Bind the VBO.

		context.extensions.glBufferData(juce::gl::GL_ARRAY_BUFFER, sizeof(Vertex) * shapes.at(i)->vertices.size(), shapes.at(i)->vertices.data(), juce::gl::GL_STATIC_DRAW);

		context.extensions.glBindBuffer(juce::gl::GL_ELEMENT_ARRAY_BUFFER, shapes.at(i)->ibo); // Bind the IBO.

		context.extensions.glBufferData(juce::gl::GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * shapes.at(i)->get_index_buffer().size(), shapes.at(i)->get_index_buffer().data(), juce::gl::GL_STATIC_DRAW);

		// Compile and link the shader.
		// Each of these methods will return false if something goes wrong so we'll
		// wrap them in an if statement
		if (shaderProgram->addVertexShader(shapes.at(i)->shader_vertex) && shaderProgram->addFragmentShader(shapes.at(i)->shader_fragment) && shaderProgram->link()) 1;

		else compile_has_error = true;
	}

	if (!compile_has_error) shaderProgram->use();
}

void OglComponent::renderOpenGL()
{
	// Clear the screen by filling it with black.
	juce::OpenGLHelpers::clear(juce::Colours::grey);

	shaderProgram->use(); // Tell the renderer to use this shader program

	for (int i = 0; i < shapes.size(); ++i)
	{
		context.extensions.glBindBuffer(juce::gl::GL_ARRAY_BUFFER, shapes.at(i)->vbo);
		context.extensions.glBindBuffer(juce::gl::GL_ELEMENT_ARRAY_BUFFER, shapes.at(i)->ibo);

		context.extensions.glVertexAttribPointer(0, 2, juce::gl::GL_FLOAT, juce::gl::GL_FALSE, sizeof(Vertex), nullptr);
		context.extensions.glEnableVertexAttribArray(0);

		context.extensions.glVertexAttribPointer(1, 4, juce::gl::GL_FLOAT, juce::gl::GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(float) * 2));
		context.extensions.glEnableVertexAttribArray(1);

		juce::gl::glDrawElements(
			shapes.at(i)->get_polygon_type(),       // Tell OpenGL to render triangles.
			static_cast<GLsizei>(shapes.at(i)->get_index_buffer().size()), // How many indices we have.
			juce::gl::GL_UNSIGNED_INT,    // What type our indices are.
			nullptr             // We already gave OpenGL our indices so we don't
								// need to pass that again here, so pass nullptr.
		);

		context.extensions.glDisableVertexAttribArray(0);
		context.extensions.glDisableVertexAttribArray(1);
	}
}

void OglComponent::mouseUp(const juce::MouseEvent&)
{
	rendering = !rendering;

	if (rendering) setFrameRate(60);

	else stopTimer();
}

void OglComponent::openGLContextClosing() {}

void OglComponent::paint(juce::Graphics&) {}