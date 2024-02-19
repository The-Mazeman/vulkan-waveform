#pragma once
#include "dataTypes.hpp"
#include "defines.hpp"
#include "platformWindows.hpp"
#include "math.hpp"
#include "parserWavFile.hpp"

START_SCOPE(vulkan)

struct Buffer
{
    VkBuffer buffer;
    VkDeviceMemory memory;
    uint64 size;
    uint usage;
    uint ownership;
    uint memoryType;
    uint ownerQueue;
};
struct Image
{
    VkImage image;
    VkDeviceMemory memory;
    VkImageView view;
    uint width;
    uint height;
    uint usage;
    uint ownership;
    uint memoryType;
    uint ownerQueue;
};
struct VulkanDevice
{
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
    VkDevice logicalDevice;

    uint presentQueueFamily;
    uint transferQueueFamily;
    uint computeQueueFamily;
    uint graphicsQueueFamily;

	VkCommandPool graphicsCommandPool;
	VkCommandPool computeCommandPool;
	VkCommandPool transferCommandPool;
	VkCommandPool presentCommandPool;

    VkCommandBuffer graphicsCommandBuffer;
    VkCommandBuffer computeCommandBuffer;
    VkCommandBuffer transferCommandBuffer;
    VkCommandBuffer presentCommandBuffer;

    VkQueue graphicsQueue;
    VkQueue computeQueue;
    VkQueue transferQueue;
    VkQueue presentQueue;
};
struct VulkanSwapchain
{
    VkSurfaceKHR surface;
    VkSurfaceFormatKHR surfaceFormat;
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
    VkExtent2D extent;
    uint imageCount;
	VkSwapchainKHR swapchain;
    VkImage image;
    VkImageView imageView;
};

struct Entity
{
    uint transformIndex;
    uint positionIndex;
    uint colorIndex;
    float depth;
};
struct State
{
    HWND windowHandle;

    VulkanDevice device;
    VulkanSwapchain swapchain;

    VkRenderPass swapchainRenderPass;
    VkRenderPass waveformRenderPass;

    VkFramebuffer swapchainFrameBuffer;
    VkFramebuffer waveformFrameBuffer;

    VkDescriptorSet swapchainPassDescriptorSet;
    VkDescriptorSet geometryPassDescriptorSet;
    VkDescriptorSet waveformPassDescriptorSet;

    VkDescriptorSetLayout geometryPassDescriptorSetLayout;
    VkDescriptorSetLayout waveformPassDescriptorSetLayout;

    VkPipelineLayout geometryPassPipelineLayout;
    VkPipelineLayout waveformPassPipelineLayout;
    VkPipelineLayout swapchainPassPipelineLayout;

	VkPipeline geometryPassPipeline;
	VkPipeline swapchainPassPipeline;
	VkPipeline waveformPassPipeline;
	VkPipeline waveformPassComputePipeline;
	VkPipeline waveformPassMaxComputePipeline;
	VkPipeline waveformMipLevelComputePipeline;
    
    Image renderImage;
    Image waveformImage;

    VkSemaphore imageSemaphore;
	VkSemaphore executionSemaphore;
	VkFence pipelineFence;

    Buffer vertexBuffer;
    uint* activeEntities;
    uint* indexSurface;
    Entity* entities;
    vec2* transforms;
    vec2* positions;
    vec4* colors;
    uint activeEntityCount;
    uint entityCount;
    uint transformCount;
    uint positionCount;
    uint colorCount;

    uint waveformWidth;
    uint startOffset;
    uint mipLevelFrameCount;

    Buffer indexSurfaceBuffer;
    Buffer inputSampleBuffer;
    WavFile wavFile;

};
void create(HWND windowHandle, HINSTANCE windowInstance, void** vulkan);
void createEntity(void* vulkan, uint* entityIndex);
void drawEntity(void* vulkan, uint entityIndex);
void createTransform(void* vulkan, uint* transformIndex, vec2** transform);
void getTransform(void* vulkan, uint entityIndex, vec2** transform);
void bindTransform(void* vulkan, uint entityIndex, uint transformIndex);
void createPosition(void* vulkan, uint* positionIndex, vec2** position);
void getPosition(void* vulkan, uint entityIndex, vec2** position);
void bindPosition(void* vulkan, uint entityIndex, uint positionIndex);
void createColor(void* vulkan, uint* colorIndex, vec4** color);
void bindColor(void* vulkan, uint entityIndex, uint colorIndex);
void setDimension(void* vulkan, uint entityIndex, float x, float y, float width, float height);
void draw(void* vulkan);

void getEntityOnScreen(void* vulkan, uint x, uint y, uint* entityIndex);

void setWaveformWidth(void* vulkan, int width);
void setWaveformStartOffset(void* vulkan, int delta);

END_SCOPE
