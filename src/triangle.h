/*
* Vulkan Example - Basic indexed triangle rendering
*
* Note:
*	This is a "pedal to the metal" example to show off how to get Vulkan up and displaying something
*	Contrary to the other examples, this one won't make use of helper functions or initializers
*	Except in a few cases (swap chain setup e.g.)
*
* Copyright (C) 2016-2024 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include "vulkanexamplebase.h"

#include <libcpp/libcpp.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fstream>
#include <vector>
#include <exception>
#include <vulkan/vulkan.h>

#define MAX_CONCURRENT_FRAMES 2

EXTERN_C_BEGIN

class foxintangoAPI VulkanExample : public VulkanExampleBase
{
public:
	// Vertex layout used in this example
	struct Vertex {
		float position[3];
		float color[3];
	};

	// Vertex buffer and attributes
	struct {
		VkDeviceMemory memory{ VK_NULL_HANDLE }; // Handle to the device memory for this buffer
		VkBuffer buffer{ VK_NULL_HANDLE };		 // Handle to the Vulkan buffer object that the memory is bound to
	} vertices;

	// Index buffer
	struct {
		VkDeviceMemory memory{ VK_NULL_HANDLE };
		VkBuffer buffer{ VK_NULL_HANDLE };
		uint32_t count{ 0 };
	} indices;

	// Uniform buffer block object
	struct UniformBuffer {
		VkDeviceMemory memory{ VK_NULL_HANDLE };
		VkBuffer buffer{ VK_NULL_HANDLE };
		// The descriptor set stores the resources bound to the binding points in a shader
		// It connects the binding points of the different shaders with the buffers and images used for those bindings
		VkDescriptorSet descriptorSet{ VK_NULL_HANDLE };
		// We keep a pointer to the mapped buffer, so we can easily update it's contents via a memcpy
		uint8_t* mapped{ nullptr };
	};
	// We use one UBO per frame, so we can have a frame overlap and make sure that uniforms aren't updated while still in use
	std::array<UniformBuffer, MAX_CONCURRENT_FRAMES> uniformBuffers;

	// For simplicity we use the same uniform block layout as in the shader:
	//
	//	layout(set = 0, binding = 0) uniform UBO
	//	{
	//		mat4 projectionMatrix;
	//		mat4 modelMatrix;
	//		mat4 viewMatrix;
	//	} ubo;
	//
	// This way we can just memcopy the ubo data to the ubo
	// Note: You should use data types that align with the GPU in order to avoid manual padding (vec4, mat4)
	struct ShaderData {
		glm::mat4 projectionMatrix;
		glm::mat4 modelMatrix;
		glm::mat4 viewMatrix;
	};

	// The pipeline layout is used by a pipeline to access the descriptor sets
	// It defines interface (without binding any actual data) between the shader stages used by the pipeline and the shader resources
	// A pipeline layout can be shared among multiple pipelines as long as their interfaces match
	VkPipelineLayout pipelineLayout{ VK_NULL_HANDLE };

	// Pipelines (often called "pipeline state objects") are used to bake all states that affect a pipeline
	// While in OpenGL every state can be changed at (almost) any time, Vulkan requires to layout the graphics (and compute) pipeline states upfront
	// So for each combination of non-dynamic pipeline states you need a new pipeline (there are a few exceptions to this not discussed here)
	// Even though this adds a new dimension of planning ahead, it's a great opportunity for performance optimizations by the driver
	VkPipeline pipeline{ VK_NULL_HANDLE };

	// The descriptor set layout describes the shader binding layout (without actually referencing descriptor)
	// Like the pipeline layout it's pretty much a blueprint and can be used with different descriptor sets as long as their layout matches
	VkDescriptorSetLayout descriptorSetLayout{ VK_NULL_HANDLE };

	// Synchronization primitives
	// Synchronization is an important concept of Vulkan that OpenGL mostly hid away. Getting this right is crucial to using Vulkan.

	// Semaphores are used to coordinate operations within the graphics queue and ensure correct command ordering
	std::array<VkSemaphore, MAX_CONCURRENT_FRAMES> presentCompleteSemaphores{};
	std::array<VkSemaphore, MAX_CONCURRENT_FRAMES> renderCompleteSemaphores{};

	VkCommandPool commandPool{ VK_NULL_HANDLE };
	std::array<VkCommandBuffer, MAX_CONCURRENT_FRAMES> commandBuffers{};
	std::array<VkFence, MAX_CONCURRENT_FRAMES> waitFences{};

	// To select the correct sync objects, we need to keep track of the current frame
	uint32_t currentFrame{ 0 };

	VulkanExample();

	~VulkanExample();

	uint32_t getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties);

	// Create the per-frame (in flight) Vulkan synchronization primitives used in this example
	void createSynchronizationPrimitives();
	void createCommandBuffers();

	// Prepare vertex and index buffers for an indexed triangle
	// Also uploads them to device local memory using staging and initializes vertex input and attribute binding to match the vertex shader
	void createVertexBuffer();

	// Descriptors are allocated from a pool, that tells the implementation how many and what types of descriptors we are going to use (at maximum)
	void createDescriptorPool();

	// Descriptor set layouts define the interface between our application and the shader
	// Basically connects the different shader stages to descriptors for binding uniform buffers, image samplers, etc.
	// So every shader binding should map to one descriptor set layout binding
	void createDescriptorSetLayout();

	// Shaders access data using descriptor sets that "point" at our uniform buffers
	// The descriptor sets make use of the descriptor set layouts created above 
	void createDescriptorSets();

	// Create the depth (and stencil) buffer attachments used by our framebuffers
	// Note: Override of virtual function in the base class and called from within VulkanExampleBase::prepare
	void setupDepthStencil();

	// Create a frame buffer for each swap chain image
	// Note: Override of virtual function in the base class and called from within VulkanExampleBase::prepare
	void setupFrameBuffer();

	// Render pass setup
	// Render passes are a new concept in Vulkan. They describe the attachments used during rendering and may contain multiple subpasses with attachment dependencies
	// This allows the driver to know up-front what the rendering will look like and is a good opportunity to optimize especially on tile-based renderers (with multiple subpasses)
	// Using sub pass dependencies also adds implicit layout transitions for the attachment used, so we don't need to add explicit image memory barriers to transform them
	// Note: Override of virtual function in the base class and called from within VulkanExampleBase::prepare
	void setupRenderPass();

	// Vulkan loads its shaders from an immediate binary representation called SPIR-V
	// Shaders are compiled offline from e.g. GLSL using the reference glslang compiler
	// This function loads such a shader from a binary file and returns a shader module structure
	VkShaderModule loadSPIRVShader(std::string filename);

	void createPipelines();

	void createUniformBuffers();

	void prepare();

	virtual void render();
};

EXTERN_C_END