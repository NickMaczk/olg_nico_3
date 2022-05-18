#pragma once

#include "primitives.h"

class OglComponent :
	public juce::Component,
	public juce::OpenGLRenderer,
	public juce::Timer
{
public:

	OglComponent();
	~OglComponent();

	void component_add();
	void component_remove();

	void setFrameRate(const int& fps_init);
	void timerCallback() override;
	void newOpenGLContextCreated() override;
	void renderOpenGL() override;
	void mouseUp(const juce::MouseEvent& e) override;
	void openGLContextClosing() override;
	void paint(juce::Graphics& g) override;

	int fps = 25;
	bool rendering = false;

	//GLuint vbo;
	//GLuint ibo;
	 
	//juce::String vertexShader;
	//juce::String fragmentShader;
	 
	//std::vector<Vertex> vertexBuffer;
	//std::vector<unsigned int> indexBuffer;

	juce::OpenGLContext context;
	std::unique_ptr<juce::OpenGLShaderProgram> shaderProgram;

	std::vector<std::unique_ptr<Shape>> shapes;
};