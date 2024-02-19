#include "include.hpp"
#include "graphicsVulkan.hpp"

START_SCOPE(vulkan)

VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData)
{
	NOT_USING(messageSeverity);
	NOT_USING(messageType);
	NOT_USING(callbackData);
	NOT_USING(userData);
    int a = 0;
    a = 0;
    a = 0;
    a = 0;
    a = 0;
	return 0;
}
void createInstance(VkInstance* instance)
{
	VkResult result = {};

	uint requestedExtensionCount = 3;
	const char* requiredInstanceExtensions[] =
	{
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};

	uint extensionCount = {};
	result = vkEnumerateInstanceExtensionProperties(0, &extensionCount, 0);

	VkExtensionProperties* extensionProperties = {};
	allocateMemory(sizeof(VkExtensionProperties) * extensionCount, (void**)&extensionProperties);
	result = vkEnumerateInstanceExtensionProperties(0, &extensionCount, extensionProperties);

	for (uint i = 0; i != requestedExtensionCount; ++i)
	{
		uint found = {};
		for (uint j = 0; j != extensionCount; ++j)
		{
			char* availableInstanceExtension = extensionProperties[j].extensionName;
			if (strcmp((char*)requiredInstanceExtensions[i], availableInstanceExtension) == 0)
			{
				found = 1;
				break;
			}
		}
		assert(found == 1);
	}
	freeMemory(extensionProperties);

	uint requestedLayerCount = 1;
	const char* requiredinstanceLayers[] =
	{
		"VK_LAYER_KHRONOS_validation",
	};

	uint availableLayerCount = {};
	VkLayerProperties* layerProperties = {};
	result = vkEnumerateInstanceLayerProperties(&availableLayerCount, 0);

	allocateMemory(sizeof(VkLayerProperties) * availableLayerCount, (void**)&layerProperties);
	result = vkEnumerateInstanceLayerProperties(&availableLayerCount, layerProperties);

	for (uint i = 0; i != requestedLayerCount; ++i)
	{
		uint found = {};
		for (uint j = 0; j != availableLayerCount; ++j)
		{
			char* availableInstanceLayer = layerProperties[j].layerName;
			if (strcmp((char*)requiredinstanceLayers[i], availableInstanceLayer) == 0)
			{
				found = 1;
				break;
			}
		}
		assert(found == 1);
	}
	freeMemory(layerProperties);

	VkApplicationInfo applicationInfo = {};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pNext = 0;
	applicationInfo.pApplicationName = "test";
	applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	applicationInfo.pEngineName = "testEngine";
	applicationInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
	applicationInfo.apiVersion = VK_MAKE_VERSION(1, 3, 0);

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext = 0;
	instanceCreateInfo.flags = 0;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;
	instanceCreateInfo.enabledLayerCount = 1;
	instanceCreateInfo.ppEnabledLayerNames = requiredinstanceLayers;
	instanceCreateInfo.enabledExtensionCount = 3;
	instanceCreateInfo.ppEnabledExtensionNames = requiredInstanceExtensions;

	result = vkCreateInstance(&instanceCreateInfo, 0, instance);
	assert(result == VK_SUCCESS);
}
void createDebugMessageCallback(VkInstance instance, VkDebugUtilsMessengerEXT* debugMessenger)
{
	VkResult result = {};

	PFN_vkCreateDebugUtilsMessengerEXT pfnCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	PFN_vkDestroyDebugUtilsMessengerEXT pfnDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	NOT_USING(pfnDestroyDebugUtilsMessengerEXT);
	int flags =
	{
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
	};
	VkDebugUtilsMessageSeverityFlagBitsEXT severityFlags = (VkDebugUtilsMessageSeverityFlagBitsEXT)flags;

	flags =
	{
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT
	};
	VkDebugUtilsMessageTypeFlagBitsEXT messageTypeFlags = (VkDebugUtilsMessageTypeFlagBitsEXT)flags;

	VkDebugUtilsMessengerCreateInfoEXT debugUtilityMessengerCreateInfo = {};
	debugUtilityMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugUtilityMessengerCreateInfo.pNext = 0;
	debugUtilityMessengerCreateInfo.flags = 0;
	debugUtilityMessengerCreateInfo.messageSeverity = severityFlags;
	debugUtilityMessengerCreateInfo.messageType = messageTypeFlags;
	debugUtilityMessengerCreateInfo.pfnUserCallback = vulkanDebugCallback;
	debugUtilityMessengerCreateInfo.pUserData = 0;

	result = pfnCreateDebugUtilsMessengerEXT(instance, &debugUtilityMessengerCreateInfo, 0, debugMessenger);
	assert(result == VK_SUCCESS);
}
void createSurface(VkInstance instance, HWND windowHandle, HINSTANCE windowInstance, VkSurfaceKHR* surface)
{
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hinstance = windowInstance;
	surfaceCreateInfo.hwnd = windowHandle;

	VkResult result = {};
	result = vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, 0, surface);
	assert(result == VK_SUCCESS);
}
void checkExtensionSupport(VkPhysicalDevice physicalDevice, uint* extensionSupport)
{
	VkResult result = {};

	uint extentionCount = {};
	result = vkEnumerateDeviceExtensionProperties(physicalDevice, 0, &extentionCount, 0);
	assert(result == VK_SUCCESS);

	VkExtensionProperties* extensionProperties = {};
	allocateMemory(sizeof(VkExtensionProperties) * extentionCount, (void**)&extensionProperties);
	result = vkEnumerateDeviceExtensionProperties(physicalDevice, 0, &extentionCount, extensionProperties);

	uint requiredExtensionCount = 1;
	const char* requiredExtensions[] =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	for (uint i = 0; i != requiredExtensionCount; ++i)
	{
		for (uint j = 0; j != extentionCount; ++j)
		{
			char* availableExtension = extensionProperties[j].extensionName;
			if (strcmp((char*)requiredExtensions[i], availableExtension) == 0)
			{
				*extensionSupport = 1;
				break;
			}
		}
	}
	freeMemory(extensionProperties);
}
void checkPresentQueueSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR vulkanSurface, uint queueFamilyCount, uint* presentQueueFamily)
{
	VkResult result = {};
	for (uint i = 0; i != queueFamilyCount; ++i)
	{
		VkBool32 presentSupport = {};
		result = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, vulkanSurface, &presentSupport);
		assert(result == VK_SUCCESS);

		if (presentSupport)
		{
			*presentQueueFamily = i;
		}
	}
}
void getQueueFamily(VkQueueFamilyProperties* queueFamilyProperties, uint queueFamilyCount, uint flag, uint* queueFamily)
{
	for (uint i = 0; i != queueFamilyCount; ++i)
	{
		if (queueFamilyProperties[i].queueFlags & flag)
		{
			*queueFamily = i;
		}
	}
}
void checkSurfaceSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint* surfaceFormatCount, uint* surfacePresentModeCount)
{
	VkResult result = {};
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, surfaceFormatCount, 0);

	result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, surfacePresentModeCount, 0);
	assert(result == VK_SUCCESS);
}
void createPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice* physicalDevice, uint* queueFamilies)
{
	VkResult result = {};

	uint physicalDeviceCount = {};
	result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, 0);
	assert(result == VK_SUCCESS);

	VkPhysicalDevice* physicalDevices = {};
	allocateMemory(sizeof(VkPhysicalDevice) * physicalDeviceCount, (void**)&physicalDevices);
	result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices);
	assert(result == VK_SUCCESS);

	uint graphicsQueueFamily = UINT_MAX;
	uint computeQueueFamily = UINT_MAX;
	uint transferQueueFamily = UINT_MAX;
	uint presentQueueFamily = UINT_MAX;

	for (uint i = 0; i != physicalDeviceCount; ++i)
	{
		uint extensionSupport = {};
		checkExtensionSupport(physicalDevices[i], &extensionSupport);
		if (!extensionSupport)
		{
			continue;
		}

		uint queueFamilyCount = {};
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, 0);

		VkQueueFamilyProperties* queueFamilyProperties = {};
		allocateMemory(sizeof(VkQueueFamilyProperties) * queueFamilyCount, (void**)&queueFamilyProperties);

		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, queueFamilyProperties);
		
		getQueueFamily(queueFamilyProperties, queueFamilyCount, VK_QUEUE_GRAPHICS_BIT, &graphicsQueueFamily);
		if (graphicsQueueFamily == UINT_MAX)
		{
			continue;
		}
		
		getQueueFamily(queueFamilyProperties, queueFamilyCount, VK_QUEUE_COMPUTE_BIT, &computeQueueFamily);
		if (computeQueueFamily == UINT_MAX)
		{
			continue;
		}
		
		getQueueFamily(queueFamilyProperties, queueFamilyCount, VK_QUEUE_TRANSFER_BIT, &transferQueueFamily);
		if (transferQueueFamily == UINT_MAX)
		{
			continue;
		}

		checkPresentQueueSupport(physicalDevices[i], surface, queueFamilyCount, &presentQueueFamily);
		if (presentQueueFamily == UINT_MAX)
		{
			continue;
		}

		uint surfaceFormatCount = {};
		uint surfacePresentModeCount = {};
		checkSurfaceSupport(physicalDevices[i], surface, &surfaceFormatCount, &surfacePresentModeCount);
		if (!surfaceFormatCount || !surfacePresentModeCount)
		{
			continue;
		}
		*physicalDevice = physicalDevices[i];
		queueFamilies[0] = presentQueueFamily;
		queueFamilies[1] = transferQueueFamily;
		queueFamilies[2] = computeQueueFamily;
		queueFamilies[3] = graphicsQueueFamily;
	}
}
void createLogicalDevice(VkPhysicalDevice physicalDevice, uint* queueFamilyIndices, VkDevice* logicalDevice)
{
	uint selectedQueueFamilies[4] = {UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX };
	uint selectedQueueFamilyCount = {};
	for (uint i = 0; i != 4; ++i)
	{
		if (queueFamilyIndices[i] != UINT_MAX)
		{
			uint reject = 0;
			for (uint j = 0; j != i; ++j)
			{
				if (queueFamilyIndices[i] == selectedQueueFamilies[j])
				{
					reject = 1;
				}
			}
			if (!reject)
			{
				selectedQueueFamilies[selectedQueueFamilyCount] = queueFamilyIndices[i];
				++selectedQueueFamilyCount;
			}
		}
	}
	float queuePriority = 1.0f;
	VkDeviceQueueCreateInfo* deviceQueueCreateInfo = {};
	allocateMemory(sizeof(VkDeviceQueueCreateInfo) * selectedQueueFamilyCount, (void**)&deviceQueueCreateInfo);
	for (uint i = 0; i != selectedQueueFamilyCount; ++i)
	{
		deviceQueueCreateInfo[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCreateInfo[i].pNext = 0;
		deviceQueueCreateInfo[i].flags = 0;
		deviceQueueCreateInfo[i].queueFamilyIndex = selectedQueueFamilies[i];
		deviceQueueCreateInfo[i].queueCount = 1;
		deviceQueueCreateInfo[i].pQueuePriorities = &queuePriority;
 	}

	uint requiredExtensionCount = 2;
	const char* requiredExtensions[] =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        //VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
        VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME
	};

	uint requiredLayeredCount = 1;
	const char* requiredLayers[] =
	{
		"VK_LAYER_KHRONOS_validation",
	};
#if 0

    VkPhysicalDeviceBufferDeviceAddressFeatures addressFeature = {};
    addressFeature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
    addressFeature.pNext = 0;
    addressFeature.bufferDeviceAddress = VK_TRUE;

    VkPhysicalDeviceFeatures2 physicalDeviceFeatures = {};
    physicalDeviceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    physicalDeviceFeatures.pNext = &addressFeature;
    physicalDeviceFeatures.features = {};
#endif

	VkDeviceCreateInfo logicalDeviceCreateInfo = {};
	logicalDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	logicalDeviceCreateInfo.pNext = 0;//&physicalDeviceFeatures;
	logicalDeviceCreateInfo.flags = 0;
	logicalDeviceCreateInfo.queueCreateInfoCount = selectedQueueFamilyCount;
	logicalDeviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfo;
	logicalDeviceCreateInfo.enabledLayerCount = requiredLayeredCount;
	logicalDeviceCreateInfo.ppEnabledLayerNames = requiredLayers;
	logicalDeviceCreateInfo.enabledExtensionCount = requiredExtensionCount;
	logicalDeviceCreateInfo.ppEnabledExtensionNames = requiredExtensions;
	logicalDeviceCreateInfo.pEnabledFeatures = 0;

	VkResult result = {};
	result = vkCreateDevice(physicalDevice, &logicalDeviceCreateInfo, 0, logicalDevice);
	assert(result == VK_SUCCESS);
}
void getSurfaceFormat(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, int format, int colorSpace, VkSurfaceFormatKHR* surfaceFormat)
{
	VkResult result = {};
	uint surfaceFormatCount = {};
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, 0);

	VkSurfaceFormatKHR* surfaceFormats = {};
	allocateMemory(sizeof(VkSurfaceFormatKHR) * surfaceFormatCount, (void**)&surfaceFormats);
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, surfaceFormats);

	for (uint i = 0; i != surfaceFormatCount; ++i)
	{
		if (surfaceFormats[i].format == format && surfaceFormats[i].colorSpace == colorSpace)
		{
			*surfaceFormat = surfaceFormats[i];
			break;
		}
	}
}
void createCommandPool(VkDevice logicalDevice, uint queueFamily, VkCommandPool* commandPool)
{
	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.pNext = 0;
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	commandPoolCreateInfo.queueFamilyIndex = queueFamily;

	VkResult result = {};
	result = vkCreateCommandPool(logicalDevice, &commandPoolCreateInfo, 0, commandPool);
	assert(result == VK_SUCCESS);
}
void createCommandBuffer(VkDevice logicalDevice, VkCommandPool commandPool, uint level, VkCommandBuffer* commandBuffer)
{
	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.pNext = 0;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = (VkCommandBufferLevel)level;
	commandBufferAllocateInfo.commandBufferCount = 1;

	VkResult result = {};
	result = vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, commandBuffer);
}
void createBufferObject(VkDevice logicalDevice, uint64 size, uint usageFlags, uint ownership, VkBuffer* bufferObject)
{
	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.pNext = 0;
	bufferCreateInfo.flags = 0;
	bufferCreateInfo.size = size;
	bufferCreateInfo.usage = usageFlags;
	bufferCreateInfo.sharingMode = (VkSharingMode)ownership;
	bufferCreateInfo.queueFamilyIndexCount = 0;
	bufferCreateInfo.pQueueFamilyIndices = 0;

	VkResult result = {};
	result = vkCreateBuffer(logicalDevice, &bufferCreateInfo, 0, bufferObject);
	assert(result == VK_SUCCESS);
}
void createBufferMemory(VkDevice logicalDevice, uint64 size, uint requiredMemoryIndex, VkDeviceMemory* bufferMemory)
{
#if 0
    VkMemoryAllocateFlagsInfoKHR flagsInfo = {};
    flagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO_KHR;
    flagsInfo.pNext = 0;
    flagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT_KHR;
    flagsInfo.deviceMask = 0;
#endif
    

	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.pNext = 0;//&flagsInfo;
	memoryAllocateInfo.allocationSize = size;
	memoryAllocateInfo.memoryTypeIndex = requiredMemoryIndex;

	VkResult result = {};
	result = vkAllocateMemory(logicalDevice, &memoryAllocateInfo, 0, bufferMemory);
	assert(result == VK_SUCCESS);
}
void findRequiredMemory(uint memoryTypeBits, VkPhysicalDeviceMemoryProperties deviceMemoryProperties, VkMemoryPropertyFlags requiredMemoryPropertyFlags, uint* requiredMemoryIndex)
{
	uint memoryTypeCount = deviceMemoryProperties.memoryTypeCount;
	for (uint i = 0; i != memoryTypeCount; ++i)
	{
		if ((memoryTypeBits & i))
		{
			if (deviceMemoryProperties.memoryTypes[i].propertyFlags & requiredMemoryPropertyFlags)
			{
				*requiredMemoryIndex = i;
			}
		}
		memoryTypeBits = memoryTypeBits >> 1;
	}
}
void copyBuffer(VulkanDevice* device, Buffer* source, Buffer* destination)
{
	VkResult result = {};

	VkQueue transferQueue = device->transferQueue;
	VkCommandBuffer commandBuffer = device->transferCommandBuffer;

	vkResetCommandBuffer(commandBuffer, 0);

	VkCommandBufferBeginInfo commandBufferBeginInfo = {};
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.pNext = 0;
	commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	commandBufferBeginInfo.pInheritanceInfo = 0;

	result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
	assert(result == VK_SUCCESS);
    
    VkBuffer sourceBuffer = source->buffer;
	VkBufferMemoryBarrier bufferBarrier = {};
	bufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
	bufferBarrier.pNext = 0;
	bufferBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
	bufferBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
	bufferBarrier.srcQueueFamilyIndex = 0;
	bufferBarrier.dstQueueFamilyIndex = 0;
	bufferBarrier.buffer = sourceBuffer;
	bufferBarrier.offset = 0;
	bufferBarrier.size = source->size;

    uint sourceStage = VK_PIPELINE_STAGE_HOST_BIT;
    uint destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, 0, 1, &bufferBarrier, 0, 0);

    uint64 size = source->size;
	VkBufferCopy bufferCopy = {};
	bufferCopy.srcOffset = 0;
	bufferCopy.dstOffset = 0;
	bufferCopy.size = size;

    VkBuffer destinationBuffer = destination->buffer;
	vkCmdCopyBuffer(commandBuffer, sourceBuffer, destinationBuffer, 1, &bufferCopy);

	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = 0;
	submitInfo.waitSemaphoreCount = 0;
	submitInfo.pWaitSemaphores = 0;
	submitInfo.pWaitDstStageMask = 0;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	submitInfo.signalSemaphoreCount = 0;
	submitInfo.pSignalSemaphores = 0;

	result = vkQueueSubmit(transferQueue, 1, &submitInfo, 0);
	assert(result == VK_SUCCESS);

	vkQueueWaitIdle(transferQueue);
}
void allocateBuffer(VulkanDevice* device, Buffer* buffer)
{
    uint usage = buffer->usage;
    uint ownership = buffer->ownership;
    uint64 size = buffer->size;

    VkDevice logicalDevice = device->logicalDevice;

    VkBuffer bufferObject = {};
    createBufferObject(logicalDevice, size, usage, ownership, &bufferObject);
    buffer->buffer = bufferObject;

    VkMemoryPropertyFlags memoryPropertyFlags = buffer->memoryType;

    VkMemoryRequirements memoryRequirements = {};
	vkGetBufferMemoryRequirements(logicalDevice, bufferObject, &memoryRequirements);

    VkPhysicalDeviceMemoryProperties deviceMemoryProperties = device->deviceMemoryProperties;
	uint requiredMemoryIndex = {};
	findRequiredMemory(memoryRequirements.memoryTypeBits, deviceMemoryProperties, memoryPropertyFlags, &requiredMemoryIndex);

    VkDeviceMemory memory = {};
	createBufferMemory(logicalDevice, size, requiredMemoryIndex, &memory);
    buffer->memory = memory;

	VkResult result = {};
	result = vkBindBufferMemory(logicalDevice, bufferObject, memory, 0);
	assert(result == VK_SUCCESS);
}
void fillDeviceBuffer(VulkanDevice* device, void* data, Buffer* buffer)
{
    uint64 size = buffer->size;

    Buffer stagingBuffer = {};
    stagingBuffer.size = size;
    stagingBuffer.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    stagingBuffer.memoryType = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    allocateBuffer(device, &stagingBuffer);

    VkDeviceMemory memory = stagingBuffer.memory;
    VkDevice logicalDevice = device->logicalDevice;

    void* mappedMemory = {};
	VkResult result = {};
	result = vkMapMemory(logicalDevice, memory, 0, size, 0, &mappedMemory);
	assert(result == VK_SUCCESS);

	memcpy(mappedMemory, data, size);

    VkMappedMemoryRange memoryRange = {};
    memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    memoryRange.pNext = 0;
    memoryRange.memory = memory;
    memoryRange.offset = 0;
    memoryRange.size = size;

    vkFlushMappedMemoryRanges(logicalDevice, 1, &memoryRange);
	vkUnmapMemory(logicalDevice, memory);

    copyBuffer(device, &stagingBuffer, buffer);
}
void createImage(VulkanDevice* device, VkImageCreateInfo* imageCreateInfo, Image* image)
{
	VkResult result = {};
    VkDevice logicalDevice = device->logicalDevice;

    VkImage vkImage = {};
    result = vkCreateImage(logicalDevice, imageCreateInfo, 0, &vkImage);
    assert(result == VK_SUCCESS);
    image->image = vkImage;

    VkMemoryRequirements imageMemoryRequirements = {};
    vkGetImageMemoryRequirements(logicalDevice, vkImage, &imageMemoryRequirements);

    VkPhysicalDeviceMemoryProperties deviceMemoryProperties = device->deviceMemoryProperties;

    uint requiredMemoryIndex = {};
    uint memoryTypeBits = imageMemoryRequirements.memoryTypeBits;
    uint memoryFlags = image->memoryType;
    findRequiredMemory(memoryTypeBits, deviceMemoryProperties, memoryFlags, &requiredMemoryIndex);

	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.pNext = 0;
	memoryAllocateInfo.allocationSize = imageMemoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = requiredMemoryIndex;

    VkDeviceMemory imageMemory = {};
    vkAllocateMemory(logicalDevice, &memoryAllocateInfo, 0, &imageMemory);

    image->memory = imageMemory;
    vkBindImageMemory(logicalDevice, vkImage, imageMemory, 0);
}
void transitionImage(VulkanDevice* device, VkImage image, uint oldLayout, uint newLayout)
{
	VkResult result = {};

    VkImageSubresourceRange subresourceRange = {};
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;

    VkImageMemoryBarrier imageMemoryBarrier = {};
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.pNext = 0;
    imageMemoryBarrier.srcAccessMask = 0;
    imageMemoryBarrier.dstAccessMask = 0;
    imageMemoryBarrier.oldLayout = (VkImageLayout)oldLayout;
    imageMemoryBarrier.newLayout = (VkImageLayout)newLayout;
    imageMemoryBarrier.srcQueueFamilyIndex = 0;
    imageMemoryBarrier.dstQueueFamilyIndex = 0;
    imageMemoryBarrier.image = image;
    imageMemoryBarrier.subresourceRange = subresourceRange;

    VkCommandBufferBeginInfo commandBufferBeginInfo = {};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = 0;
    commandBufferBeginInfo.flags = 0;
    commandBufferBeginInfo.pInheritanceInfo = 0;

    VkCommandBuffer graphicsCommandBuffer = device->graphicsCommandBuffer;
    result = vkBeginCommandBuffer(graphicsCommandBuffer, &commandBufferBeginInfo);
    assert(result == VK_SUCCESS);

    uint stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    vkCmdPipelineBarrier(graphicsCommandBuffer, stage, stage, 0, 0, 0, 0, 0, 1, &imageMemoryBarrier);

    result = vkEndCommandBuffer(graphicsCommandBuffer);
    assert(result == VK_SUCCESS);

    VkSubmitInfo barrierSubmitInfo = {};
    barrierSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    barrierSubmitInfo.pNext = 0;
    barrierSubmitInfo.waitSemaphoreCount = 0;
    barrierSubmitInfo.pWaitSemaphores = 0;
    barrierSubmitInfo.pWaitDstStageMask = 0;
    barrierSubmitInfo.commandBufferCount = 1;
    barrierSubmitInfo.pCommandBuffers = &graphicsCommandBuffer;
    barrierSubmitInfo.signalSemaphoreCount = 0;
    barrierSubmitInfo.pSignalSemaphores = 0;

    VkQueue graphicsQueue = device->graphicsQueue;
    result = vkQueueSubmit(graphicsQueue, 1, &barrierSubmitInfo, 0);
    assert(result == VK_SUCCESS);
    vkQueueWaitIdle(graphicsQueue);
}
void fillDeviceImage(VulkanDevice* device, Image* image, VkBuffer buffer)
{
	VkResult result = {};

    VkCommandBufferBeginInfo commandBufferBeginInfo = {};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = 0;
    commandBufferBeginInfo.flags = 0;
    commandBufferBeginInfo.pInheritanceInfo = 0;

	VkQueue transferQueue = device->transferQueue;
	VkCommandBuffer commandBuffer = device->transferCommandBuffer;

	vkResetCommandBuffer(commandBuffer, 0);

    result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
    assert(result == VK_SUCCESS);

    VkImageSubresourceLayers subresourceLayers = {};
    subresourceLayers.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceLayers.mipLevel = 0;
    subresourceLayers.baseArrayLayer = 0;
    subresourceLayers.layerCount = 1;

    uint width = image->width;
    uint height = image->height;

    VkBufferImageCopy region = {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource = subresourceLayers;
    region.imageOffset = {};
    region.imageExtent = {width, height, 1};

    vkCmdCopyBufferToImage(commandBuffer, buffer, image->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    result = vkEndCommandBuffer(commandBuffer);
    assert(result == VK_SUCCESS);

    VkSubmitInfo barrierSubmitInfo = {};
    barrierSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    barrierSubmitInfo.pNext = 0;
    barrierSubmitInfo.waitSemaphoreCount = 0;
    barrierSubmitInfo.pWaitSemaphores = 0;
    barrierSubmitInfo.pWaitDstStageMask = 0;
    barrierSubmitInfo.commandBufferCount = 1;
    barrierSubmitInfo.pCommandBuffers = &commandBuffer;
    barrierSubmitInfo.signalSemaphoreCount = 0;
    barrierSubmitInfo.pSignalSemaphores = 0;

    result = vkQueueSubmit(transferQueue, 1, &barrierSubmitInfo, 0);
    assert(result == VK_SUCCESS);
    vkQueueWaitIdle(transferQueue);
}
void createDescriptorLayoutBinding(uint binding, uint type, uint count, uint stage, VkDescriptorSetLayoutBinding* descriptorSetBinding)
{
	descriptorSetBinding->binding = binding;
	descriptorSetBinding->descriptorType = (VkDescriptorType)type;
	descriptorSetBinding->descriptorCount = count;
	descriptorSetBinding->stageFlags = stage;
	descriptorSetBinding->pImmutableSamplers = 0;
}
void createDescriptorSetLayout(VkDevice logicalDevice, VkDescriptorSetLayoutBinding* descriptorSetBinding, uint bindingCount, VkDescriptorSetLayout* descriptorSetLayout)
{
	VkResult result = {};

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.pNext = 0;
	descriptorSetLayoutCreateInfo.flags = 0;
	descriptorSetLayoutCreateInfo.bindingCount = bindingCount;
	descriptorSetLayoutCreateInfo.pBindings = descriptorSetBinding;

	result = vkCreateDescriptorSetLayout(logicalDevice, &descriptorSetLayoutCreateInfo, 0, descriptorSetLayout);
	assert(result == VK_SUCCESS);
}
void createDescriptorPool(VkDevice logicalDevice, uint* descriptorTypes, uint* descriptorCounts, uint poolSizeCount, VkDescriptorPool* descriptorPool)
{
	VkDescriptorPoolSize* poolSizes = {};
	allocateMemory(sizeof(VkDescriptorPoolSize) * poolSizeCount, (void**)&poolSizes);
	for (uint i = 0; i != poolSizeCount; ++i)
	{
		poolSizes[i].descriptorCount = descriptorCounts[i];
		poolSizes[i].type = (VkDescriptorType)descriptorTypes[i];
	}

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.pNext = 0;
	descriptorPoolCreateInfo.flags = 0;
	descriptorPoolCreateInfo.maxSets = 1;
	descriptorPoolCreateInfo.poolSizeCount = poolSizeCount;
	descriptorPoolCreateInfo.pPoolSizes = poolSizes;

	VkResult result = {};
	result = vkCreateDescriptorPool(logicalDevice, &descriptorPoolCreateInfo, 0, descriptorPool);
	assert(result == VK_SUCCESS);
}
void createDescriptorSet(VkDevice logicalDevice, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet* descriptorSet)
{
	VkResult result = {};

	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.pNext = 0;
	descriptorSetAllocateInfo.descriptorPool = descriptorPool;
	descriptorSetAllocateInfo.descriptorSetCount = 1;
	descriptorSetAllocateInfo.pSetLayouts = &descriptorSetLayout;

	result = vkAllocateDescriptorSets(logicalDevice, &descriptorSetAllocateInfo, descriptorSet);
	assert(result == VK_SUCCESS);
}
void updateDescriptorSet(VkDevice logicalDevice, VkBuffer buffer, uint bindPoint, VkDescriptorSet descriptorSet, uint type)
{
	VkDescriptorBufferInfo descriptorBufferInfo = {};
	descriptorBufferInfo.buffer = buffer;
	descriptorBufferInfo.offset = 0;
	descriptorBufferInfo.range = VK_WHOLE_SIZE;

	VkWriteDescriptorSet writeDescriptorSet = {};
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.pNext = 0;
	writeDescriptorSet.dstSet = descriptorSet;
	writeDescriptorSet.dstBinding = bindPoint;
	writeDescriptorSet.dstArrayElement = 0;
	writeDescriptorSet.descriptorCount = 1;
	writeDescriptorSet.descriptorType = (VkDescriptorType)type;
	writeDescriptorSet.pImageInfo = 0;
	writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;
	writeDescriptorSet.pTexelBufferView = 0;

	vkUpdateDescriptorSets(logicalDevice, 1, &writeDescriptorSet, 0, 0);
}
void createShaderModule(VkDevice logicalDevice, WCHAR* filePath, VkShaderModule* shaderModule)
{
	void* shaderCode;
	uint64 shaderCodeSize;
	loadFile(filePath, &shaderCode, &shaderCodeSize);

	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.pNext = 0;
	shaderModuleCreateInfo.flags = 0;
	shaderModuleCreateInfo.codeSize = shaderCodeSize;
	shaderModuleCreateInfo.pCode = (uint32*)shaderCode;

	VkResult result = {};
	result = vkCreateShaderModule(logicalDevice, &shaderModuleCreateInfo, 0, shaderModule);
	assert(result == VK_SUCCESS);
}
void createShaderStage(VkShaderModule shaderModule, uint stage, VkPipelineShaderStageCreateInfo* shaderStageCreateInfo)
{
	shaderStageCreateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageCreateInfo->pNext = 0;
	shaderStageCreateInfo->flags = 0;
	shaderStageCreateInfo->stage = (VkShaderStageFlagBits)stage;
	shaderStageCreateInfo->module = shaderModule;
	shaderStageCreateInfo->pName = "main";
	shaderStageCreateInfo->pSpecializationInfo = 0;
}
void createSemaphore(VkDevice logicalDevice, VkSemaphore* semaphore)
{
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.pNext = 0;
	semaphoreCreateInfo.flags = 0;

	VkResult result = {};
	result = vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, 0, semaphore);
}
void createFence(VkDevice logicalDevice, VkFence* fence)
{
	VkResult result = {};

	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.pNext = 0;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	result = vkCreateFence(logicalDevice, &fenceCreateInfo, 0, fence);
	assert(result == VK_SUCCESS);
}
void create(HWND windowHandle, HINSTANCE windowInstance, void** vulkan) 
{
    State* state = {};
    allocateMemory(sizeof(State), (void**)&state);
    state->windowHandle = windowHandle;
    state->activeEntityCount = 0;
    state->entityCount = 0;
    state->transformCount = 0;
    state->positionCount = 0;
    state->colorCount = 0;
    state->waveformWidth = 400;
    state->startOffset = 0;

    VkInstance instance = {};
    createInstance(&instance);
    state->device.instance = instance;

    VkDebugUtilsMessengerEXT debugMessenger = {};
    createDebugMessageCallback(instance, &debugMessenger);
    state->device.debugMessenger = debugMessenger;

    VkSurfaceKHR surface = {};
    createSurface(instance, windowHandle, windowInstance, &surface);
    state->swapchain.surface = surface;

    VkPhysicalDevice physicalDevice = {};
    uint queueFamilies[4] = {};
    createPhysicalDevice(instance, surface, &physicalDevice, queueFamilies);
    state->device.physicalDevice = physicalDevice;
    state->device.presentQueueFamily = queueFamilies[0];
    state->device.transferQueueFamily = queueFamilies[1];
    state->device.computeQueueFamily = queueFamilies[2];
    state->device.graphicsQueueFamily = queueFamilies[3];

    VkPhysicalDeviceMemoryProperties deviceMemoryProperties = {};
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemoryProperties);
    state->device.deviceMemoryProperties = deviceMemoryProperties;
    
    VkDevice logicalDevice = {};
    createLogicalDevice(physicalDevice, queueFamilies, &logicalDevice);
    state->device.logicalDevice = logicalDevice;

    uint graphicsQueueFamily = state->device.graphicsQueueFamily;
    VkQueue graphicsQueue = {};
	vkGetDeviceQueue(logicalDevice, graphicsQueueFamily, 0, &graphicsQueue);
    state->device.graphicsQueue = graphicsQueue;

    uint computeQueueFamily = state->device.computeQueueFamily;
    VkQueue computeQueue = {};
	vkGetDeviceQueue(logicalDevice, computeQueueFamily, 0, &computeQueue);
    state->device.computeQueue = computeQueue;

    uint transferQueueFamily = state->device.transferQueueFamily;
    VkQueue transferQueue = {};
	vkGetDeviceQueue(logicalDevice, transferQueueFamily, 0, &transferQueue);
    state->device.transferQueue = transferQueue;

    uint presentQueueFamily = state->device.presentQueueFamily;
    VkQueue presentQueue = {};
	vkGetDeviceQueue(logicalDevice, presentQueueFamily, 0, &presentQueue);
    state->device.presentQueue = presentQueue;

	VkCommandPool graphicsCommandPool = {};
	createCommandPool(logicalDevice, graphicsQueueFamily, &graphicsCommandPool);
    state->device.graphicsCommandPool = graphicsCommandPool;

	VkCommandPool computeCommandPool = {};
	createCommandPool(logicalDevice, computeQueueFamily, &computeCommandPool);
    state->device.computeCommandPool = computeCommandPool;

	VkCommandPool transferCommandPool = {};
	createCommandPool(logicalDevice, transferQueueFamily, &transferCommandPool);
    state->device.transferCommandPool = transferCommandPool;
    
	VkCommandPool presentCommandPool = {};
	createCommandPool(logicalDevice, presentQueueFamily, &presentCommandPool);
    state->device.presentCommandPool = presentCommandPool;

	VkCommandBuffer graphicsCommandBuffer = {};
	createCommandBuffer(logicalDevice, graphicsCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, &graphicsCommandBuffer);
    state->device.graphicsCommandBuffer = graphicsCommandBuffer;

	VkCommandBuffer computeCommandBuffer = {};
	createCommandBuffer(logicalDevice, computeCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, &computeCommandBuffer);
    state->device.computeCommandBuffer = computeCommandBuffer;

	VkCommandBuffer transferCommandBuffer = {};
	createCommandBuffer(logicalDevice, transferCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, &transferCommandBuffer);
    state->device.transferCommandBuffer = transferCommandBuffer;

	VkCommandBuffer presentCommandBuffer = {};
	createCommandBuffer(logicalDevice, presentCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, &presentCommandBuffer);
    state->device.presentCommandBuffer = presentCommandBuffer;

    VulkanDevice* device = &state->device;
    
    int format = VK_FORMAT_R8G8B8A8_SRGB;
    int colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    VkSurfaceFormatKHR surfaceFormat = {};
    getSurfaceFormat(physicalDevice, surface, format, colorSpace, &surfaceFormat);
    state->swapchain.surfaceFormat = surfaceFormat;

	VkResult result = {};
	VkSurfaceCapabilitiesKHR surfaceCapabilities = {};
	result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);
    state->swapchain.surfaceCapabilities = surfaceCapabilities;

    VkExtent2D swapchainExtent = surfaceCapabilities.minImageExtent;
    state->swapchain.extent = swapchainExtent;

    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

    uint swapchainImageCount = 1;
    state->swapchain.imageCount = swapchainImageCount;

    VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.pNext = 0;
	swapchainCreateInfo.flags = 0;
	swapchainCreateInfo.surface = surface;
	swapchainCreateInfo.minImageCount = swapchainImageCount;
	swapchainCreateInfo.imageFormat = surfaceFormat.format;
	swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapchainCreateInfo.imageExtent = swapchainExtent;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
	swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainCreateInfo.queueFamilyIndexCount = 0;
	swapchainCreateInfo.pQueueFamilyIndices = 0;
	swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.presentMode = swapchainPresentMode;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.oldSwapchain = 0;

	VkSwapchainKHR swapchain = {};
	result = vkCreateSwapchainKHR(logicalDevice, &swapchainCreateInfo, 0, &swapchain);
	assert(result == VK_SUCCESS);
    state->swapchain.swapchain = swapchain;

    START_BLOCK

    VkImage image = {};
	result = vkGetSwapchainImagesKHR(logicalDevice, swapchain, &swapchainImageCount, &image);
    state->swapchain.image = image;

    VkImageViewCreateInfo imageViewCreateInfo = {};
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.pNext = 0;
	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.image = image;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = surfaceFormat.format;
	imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;

    VkImageView imageView = {};
    result = vkCreateImageView(logicalDevice, &imageViewCreateInfo, 0, &imageView);
    assert(result == VK_SUCCESS);
    state->swapchain.imageView = imageView;

    END_BLOCK

    START_BLOCK

    VkImageCreateInfo imageCreateInfo = {};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.pNext = 0;
    imageCreateInfo.flags = 0;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = surfaceFormat.format;
    imageCreateInfo.extent.width = swapchainExtent.width;
    imageCreateInfo.extent.height = swapchainExtent.height;
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.usage = 
    {
        VK_IMAGE_USAGE_TRANSFER_DST_BIT |
        VK_IMAGE_USAGE_SAMPLED_BIT | 
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | 
        VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT
    };
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.queueFamilyIndexCount = 0;
    imageCreateInfo.pQueueFamilyIndices = 0;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    Image renderImage = {};
    renderImage.width = swapchainExtent.width;
    renderImage.height = swapchainExtent.height;
    renderImage.memoryType = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    createImage(device, &imageCreateInfo, &renderImage);
    transitionImage(device, renderImage.image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    VkImageViewCreateInfo imageViewCreateInfo = {};
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.pNext = 0;
	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.image = renderImage.image;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = surfaceFormat.format;
	imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;

    VkImageView imageView = {};
    result = vkCreateImageView(logicalDevice, &imageViewCreateInfo, 0, &imageView);
    assert(result == VK_SUCCESS);
    renderImage.view = imageView;

    state->renderImage = renderImage;

    END_BLOCK

    START_BLOCK

    VkImageCreateInfo imageCreateInfo = {};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.pNext = 0;
    imageCreateInfo.flags = 0;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = surfaceFormat.format;
    imageCreateInfo.extent.width = 400;
    imageCreateInfo.extent.height = 100;
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.usage = 
    {
        VK_IMAGE_USAGE_TRANSFER_DST_BIT |
        VK_IMAGE_USAGE_SAMPLED_BIT | 
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | 
        VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT
    };
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.queueFamilyIndexCount = 0;
    imageCreateInfo.pQueueFamilyIndices = 0;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    Image waveformImage = {};
    waveformImage.width = 400;
    waveformImage.height = 100;
    waveformImage.memoryType = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    createImage(device, &imageCreateInfo, &waveformImage);

    VkImageViewCreateInfo imageViewCreateInfo = {};
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.pNext = 0;
	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.image = waveformImage.image;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = surfaceFormat.format;
	imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;

    VkImageView imageView = {};
    result = vkCreateImageView(logicalDevice, &imageViewCreateInfo, 0, &imageView);
    assert(result == VK_SUCCESS);
    waveformImage.view = imageView;
    state->waveformImage = waveformImage;

    END_BLOCK

    Buffer activeEntityBuffer = {};
    activeEntityBuffer.size = sizeof(uint) * 256;
    activeEntityBuffer.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    activeEntityBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    activeEntityBuffer.memoryType = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    allocateBuffer(device, &activeEntityBuffer);

    uint* activeEntities = {};
    vkMapMemory(logicalDevice, activeEntityBuffer.memory, 0, activeEntityBuffer.size, 0, (void**)&activeEntities);
    state->activeEntities = activeEntities;

    Buffer entityBuffer = {};
    entityBuffer.size = sizeof(Entity) * 256;
    entityBuffer.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    entityBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    entityBuffer.memoryType = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    allocateBuffer(device, &entityBuffer);
    Entity* entities = {};
    vkMapMemory(logicalDevice, entityBuffer.memory, 0, entityBuffer.size, 0, (void**)&entities);
    state->entities = entities;

    Buffer transformBuffer = {};
    transformBuffer.size = sizeof(vec2) * 256;
    transformBuffer.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    transformBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    transformBuffer.memoryType = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    allocateBuffer(device, &transformBuffer);

    vec2* transforms = {};
    vkMapMemory(logicalDevice, transformBuffer.memory, 0, transformBuffer.size, 0, (void**)&transforms);
    state->transforms = transforms;

    Buffer positionBuffer = {};
    positionBuffer.size = sizeof(vec2) * 256;
    positionBuffer.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    positionBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    positionBuffer.memoryType = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    allocateBuffer(device, &positionBuffer);

    vec2* positions = {};
    vkMapMemory(logicalDevice, positionBuffer.memory, 0, positionBuffer.size, 0, (void**)&positions);
    state->positions = positions;

    Buffer colorBuffer = {};
    colorBuffer.size = sizeof(float) * 4 * 256;
    colorBuffer.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    colorBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    colorBuffer.memoryType = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    allocateBuffer(device, &colorBuffer);

    vec4* colors = {};
    vkMapMemory(logicalDevice, colorBuffer.memory, 0, colorBuffer.size, 0, (void**)&colors);
    state->colors = colors;

    Buffer indexSurfaceBuffer = {};
    indexSurfaceBuffer.size = sizeof(uint) * swapchainExtent.width * swapchainExtent.height;
    indexSurfaceBuffer.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    indexSurfaceBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    indexSurfaceBuffer.memoryType = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    allocateBuffer(device, &indexSurfaceBuffer);
    state->indexSurfaceBuffer = indexSurfaceBuffer;

    uint* indexSurface = {};
    vkMapMemory(logicalDevice, indexSurfaceBuffer.memory, 0, indexSurfaceBuffer.size, 0, (void**)&indexSurface);
    state->indexSurface = indexSurface;

    START_BLOCK

    uint stages = 
    {
        VK_SHADER_STAGE_VERTEX_BIT |
        VK_SHADER_STAGE_FRAGMENT_BIT
    };

    VkDescriptorSetLayoutBinding activeEntityBinding = {};
	createDescriptorLayoutBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, stages, &activeEntityBinding);

    VkDescriptorSetLayoutBinding entityBinding = {};
	createDescriptorLayoutBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, stages, &entityBinding);

    VkDescriptorSetLayoutBinding transformBinding = {};
	createDescriptorLayoutBinding(2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, stages, &transformBinding);

    VkDescriptorSetLayoutBinding positionBinding = {};
	createDescriptorLayoutBinding(3, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, stages, &positionBinding);

    VkDescriptorSetLayoutBinding colorBinding = {};
	createDescriptorLayoutBinding(4, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, stages, &colorBinding);
    
    VkDescriptorSetLayoutBinding indexSurfaceBinding = {};
	createDescriptorLayoutBinding(5, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, stages, &indexSurfaceBinding);

    VkDescriptorSetLayoutBinding textureBinding = {};
	createDescriptorLayoutBinding(6, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, stages, &textureBinding);

    VkDescriptorSetLayoutBinding bindings[] =
	{
        activeEntityBinding,
        entityBinding,
        transformBinding,
        positionBinding,
        colorBinding,
        indexSurfaceBinding,
        textureBinding,
	};

	VkDescriptorSetLayout descriptorSetLayout = {};
	createDescriptorSetLayout(logicalDevice, bindings, 7, &descriptorSetLayout);
    state->geometryPassDescriptorSetLayout = descriptorSetLayout;

    uint descriptorTypes[] =
	{
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
	};
    uint descriptorCounts[] =
	{
		1,
		1,
		1,
		1,
		1,
		1,
		1,
	};
    VkDescriptorPool descriptorPool = {};
	createDescriptorPool(logicalDevice, descriptorTypes, descriptorCounts, 7, &descriptorPool);

	VkDescriptorSet descriptorSet = {};
	createDescriptorSet(logicalDevice, descriptorPool, descriptorSetLayout, &descriptorSet);
    state->geometryPassDescriptorSet = descriptorSet; 

    updateDescriptorSet(logicalDevice, activeEntityBuffer.buffer, 0, descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
    updateDescriptorSet(logicalDevice, entityBuffer.buffer, 1, descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
    updateDescriptorSet(logicalDevice, transformBuffer.buffer, 2, descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
    updateDescriptorSet(logicalDevice, positionBuffer.buffer, 3, descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
    updateDescriptorSet(logicalDevice, colorBuffer.buffer, 4, descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
    updateDescriptorSet(logicalDevice, indexSurfaceBuffer.buffer, 5, descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);

    VkSamplerCreateInfo samplerCreateInfo = {};
    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.pNext = 0;
    samplerCreateInfo.flags = 0;
    samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
    samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
    samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerCreateInfo.addressModeV= VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerCreateInfo.mipLodBias = 0.0f;
    samplerCreateInfo.anisotropyEnable = VK_FALSE;
    samplerCreateInfo.maxAnisotropy = 1.0f;
    samplerCreateInfo.compareEnable = VK_FALSE;
    samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerCreateInfo.minLod = 0.0f;
    samplerCreateInfo.maxLod = 0.0f;
    samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

    VkSampler sampler = {};
    vkCreateSampler(logicalDevice, &samplerCreateInfo, 0, &sampler);

    VkImageView imageView = state->waveformImage.view;
    VkDescriptorImageInfo descriptorImageInfo = {};
	descriptorImageInfo.sampler = sampler;
	descriptorImageInfo.imageView = imageView;
	descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkWriteDescriptorSet writeDescriptorSet = {};
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.pNext = 0;
	writeDescriptorSet.dstSet = descriptorSet;
	writeDescriptorSet.dstBinding = 6;
	writeDescriptorSet.dstArrayElement = 0;
	writeDescriptorSet.descriptorCount = 1;
	writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	writeDescriptorSet.pImageInfo = &descriptorImageInfo;
	writeDescriptorSet.pBufferInfo = 0;
	writeDescriptorSet.pTexelBufferView = 0;

    vkUpdateDescriptorSets(logicalDevice, 1, &writeDescriptorSet, 0, 0);

    END_BLOCK

    START_BLOCK

    WCHAR filePath[] = L"C:\\Users\\administrator-1\\Desktop\\untitled1.wav";
    void* wavFileMemory = {};
    uint64 size = {};
    loadFile(filePath, &wavFileMemory, &size);

    WavFile wavFile = {};
    wavFile::parse(wavFileMemory, &wavFile);
    state->wavFile = wavFile;

    uint64 sampleChunkSize = wavFile.frameCount * wavFile.header.blockAlign;

    Buffer inputSampleBuffer = {};
    inputSampleBuffer.size = sampleChunkSize * 2;
    inputSampleBuffer.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    inputSampleBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    inputSampleBuffer.memoryType = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    allocateBuffer(device, &inputSampleBuffer);
    state->inputSampleBuffer = inputSampleBuffer;
    inputSampleBuffer.size = sampleChunkSize;
    fillDeviceBuffer(device, wavFile.sampleChunk, &inputSampleBuffer);
    inputSampleBuffer.size = sampleChunkSize * 2;

    Buffer outputSampleBuffer = {};
    outputSampleBuffer.size = sizeof(float) * 4096;
    outputSampleBuffer.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    outputSampleBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    outputSampleBuffer.memoryType = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    allocateBuffer(device, &outputSampleBuffer);
    uint stages = 
    {
        VK_SHADER_STAGE_VERTEX_BIT |
        VK_SHADER_STAGE_FRAGMENT_BIT |
        VK_SHADER_STAGE_COMPUTE_BIT
    };
    VkDescriptorSetLayoutBinding inputSampleBinding = {};
	createDescriptorLayoutBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, stages, &inputSampleBinding);
    
    VkDescriptorSetLayoutBinding outputSampleBinding = {};
	createDescriptorLayoutBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, stages, &outputSampleBinding);

    VkDescriptorSetLayoutBinding bindings[] =
	{
        inputSampleBinding,
        outputSampleBinding,
	};

	VkDescriptorSetLayout descriptorSetLayout = {};
	createDescriptorSetLayout(logicalDevice, bindings, 2, &descriptorSetLayout);
    state->waveformPassDescriptorSetLayout = descriptorSetLayout;

    uint descriptorTypes[] =
	{
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
	};
    uint descriptorCounts[] =
	{
		1,
		1,
	};
    VkDescriptorPool descriptorPool = {};
	createDescriptorPool(logicalDevice, descriptorTypes, descriptorCounts, 2, &descriptorPool);

	VkDescriptorSet descriptorSet = {};
	createDescriptorSet(logicalDevice, descriptorPool, descriptorSetLayout, &descriptorSet);
    state->waveformPassDescriptorSet = descriptorSet; 

    updateDescriptorSet(logicalDevice, inputSampleBuffer.buffer, 0, descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
    updateDescriptorSet(logicalDevice, outputSampleBuffer.buffer, 1, descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);

    END_BLOCK

    float vertices[] =
	{
	    -1.0f, -1.0f, 
		 1.0f, -1.0f, 
		 1.0f,  1.0f, 
		 1.0f,  1.0f, 
		-1.0f,  1.0f, 
	    -1.0f, -1.0f, 
	};

    Buffer vertexBuffer = {};
    vertexBuffer.size = sizeof(vertices);
    vertexBuffer.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    vertexBuffer.ownership = VK_SHARING_MODE_EXCLUSIVE;
    vertexBuffer.memoryType = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    allocateBuffer(device, &vertexBuffer);
    fillDeviceBuffer(device, vertices, &vertexBuffer); 
    state->vertexBuffer = vertexBuffer;

    VkVertexInputAttributeDescription vertexAttributeDescription = {};
    vertexAttributeDescription.location = 0;
    vertexAttributeDescription.binding = 0;
    vertexAttributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
    vertexAttributeDescription.offset = 0;
    
	VkVertexInputBindingDescription vertexBindingDescription = {};
	vertexBindingDescription.binding = 0;
	vertexBindingDescription.stride = sizeof(float) * 2;
	vertexBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    VkPipelineVertexInputStateCreateInfo vertexInputState = {};
	vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputState.pNext = 0;
	vertexInputState.flags = 0;
	vertexInputState.vertexBindingDescriptionCount = 1;
	vertexInputState.pVertexBindingDescriptions = &vertexBindingDescription;
	vertexInputState.vertexAttributeDescriptionCount = 1;
	vertexInputState.pVertexAttributeDescriptions = &vertexAttributeDescription;

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
	inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyState.pNext = 0;
	inputAssemblyState.flags = 0;
	inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyState.primitiveRestartEnable = 0;

    VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.pNext = 0;
	viewportState.flags = 0;
	viewportState.viewportCount = 1;
	viewportState.pViewports = 0;
	viewportState.scissorCount = 1;
	viewportState.pScissors = 0;

    VkPipelineRasterizationStateCreateInfo rasterizationState = {};
	rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationState.pNext = 0;
	rasterizationState.flags = 0;
	rasterizationState.depthClampEnable = 0;
	rasterizationState.rasterizerDiscardEnable = 0;
	rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizationState.cullMode = VK_CULL_MODE_NONE;
	rasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizationState.depthBiasEnable = 0;
	rasterizationState.depthBiasConstantFactor = 0;
	rasterizationState.depthBiasClamp = 0;
	rasterizationState.depthBiasSlopeFactor = 0;
	rasterizationState.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo multisampleState = {};
	multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleState.pNext = 0;
	multisampleState.flags = 0;
	multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampleState.sampleShadingEnable = 0;
	multisampleState.minSampleShading = 1.0f;
	multisampleState.pSampleMask = 0;
	multisampleState.alphaToCoverageEnable = 0;
	multisampleState.alphaToOneEnable = 0;

    VkPipelineColorBlendStateCreateInfo colorBlendState = {};
	uint colorWriteMask =
	{
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT
	};

	VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {};
	colorBlendAttachmentState.blendEnable = VK_TRUE;
	colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachmentState.colorWriteMask = colorWriteMask;

	colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendState.pNext = 0;
	colorBlendState.flags = 0;
	colorBlendState.logicOpEnable = VK_FALSE;
	colorBlendState.logicOp = VK_LOGIC_OP_CLEAR;
	colorBlendState.attachmentCount = 1;
	colorBlendState.pAttachments = &colorBlendAttachmentState;
	colorBlendState.blendConstants[0] = 0.0f;
	colorBlendState.blendConstants[1] = 0.0f;
	colorBlendState.blendConstants[2] = 0.0f;
	colorBlendState.blendConstants[3] = 0.0f;

	VkDynamicState dynamicStates[2] = {};
	dynamicStates[0] = VK_DYNAMIC_STATE_VIEWPORT;
	dynamicStates[1] = VK_DYNAMIC_STATE_SCISSOR;

    VkPipelineDynamicStateCreateInfo dynamicState = {};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.pNext = 0;
	dynamicState.flags = 0;
	dynamicState.dynamicStateCount = 2;
	dynamicState.pDynamicStates = dynamicStates;

    START_BLOCK

    VkAttachmentReference waveformColorBinding = {};
	waveformColorBinding.attachment = 0;
	waveformColorBinding.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription waveformSubpassInfo = {};
	waveformSubpassInfo.flags = 0;
	waveformSubpassInfo.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	waveformSubpassInfo.inputAttachmentCount = 0;
	waveformSubpassInfo.pInputAttachments = 0;
	waveformSubpassInfo.colorAttachmentCount = 1;
	waveformSubpassInfo.pColorAttachments = &waveformColorBinding;
	waveformSubpassInfo.pResolveAttachments = 0;
	waveformSubpassInfo.pDepthStencilAttachment = 0;
	waveformSubpassInfo.preserveAttachmentCount = 0;
	waveformSubpassInfo.pPreserveAttachments = 0;

    VkAttachmentDescription waveformImageDescription = {};
	waveformImageDescription.flags = 0;
	waveformImageDescription.format = surfaceFormat.format;
	waveformImageDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	waveformImageDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	waveformImageDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	waveformImageDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	waveformImageDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	waveformImageDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	waveformImageDescription.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkSubpassDependency subpassDependency = {};
	subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependency.dstSubpass = 0;
	subpassDependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.srcAccessMask =  VK_ACCESS_MEMORY_READ_BIT;
	subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	subpassDependency.dependencyFlags = 0;

    VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.pNext = 0;
	renderPassCreateInfo.flags = 0;
	renderPassCreateInfo.attachmentCount = 1;
	renderPassCreateInfo.pAttachments = &waveformImageDescription;
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &waveformSubpassInfo;
	renderPassCreateInfo.dependencyCount = 1;
	renderPassCreateInfo.pDependencies = &subpassDependency;

    VkRenderPass renderPass = {};
	result = vkCreateRenderPass(logicalDevice, &renderPassCreateInfo, 0, &renderPass);
	assert(result == VK_SUCCESS);
    state->waveformRenderPass = renderPass;

    Image* waveformImage = &state->waveformImage;
    VkImageView imageViews[] = 
    {
        waveformImage->view,
    };

    VkFramebufferCreateInfo frameBufferCreateInfo = {};
    frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frameBufferCreateInfo.pNext = 0;
    frameBufferCreateInfo.flags = 0;
    frameBufferCreateInfo.renderPass = renderPass;
    frameBufferCreateInfo.attachmentCount = 1;
    frameBufferCreateInfo.pAttachments = imageViews;
    frameBufferCreateInfo.width = waveformImage->width;
    frameBufferCreateInfo.height = waveformImage->height;
    frameBufferCreateInfo.layers = 1;

    VkFramebuffer frameBuffer = {};
    result = vkCreateFramebuffer(logicalDevice, &frameBufferCreateInfo, 0, &frameBuffer);
    state->waveformFrameBuffer = frameBuffer;

    END_BLOCK
    START_BLOCK

    VkAttachmentReference swapchainColorBinding = {};
	swapchainColorBinding.attachment = 0;
	swapchainColorBinding.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference renderColorBinding = {};
	renderColorBinding.attachment = 1;
	renderColorBinding.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference renderInputBinding = {};
	renderInputBinding.attachment = 1;
	renderInputBinding.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkSubpassDescription renderSubpassInfo = {};
	renderSubpassInfo.flags = 0;
	renderSubpassInfo.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	renderSubpassInfo.inputAttachmentCount = 0;
	renderSubpassInfo.pInputAttachments = 0;
	renderSubpassInfo.colorAttachmentCount = 1;
	renderSubpassInfo.pColorAttachments = &renderColorBinding;
	renderSubpassInfo.pResolveAttachments = 0;
	renderSubpassInfo.pDepthStencilAttachment = 0;
	renderSubpassInfo.preserveAttachmentCount = 0;
	renderSubpassInfo.pPreserveAttachments = 0;

    VkSubpassDescription swapchainSubpassInfo = {};
	swapchainSubpassInfo.flags = 0;
	swapchainSubpassInfo.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	swapchainSubpassInfo.inputAttachmentCount = 1;
	swapchainSubpassInfo.pInputAttachments = &renderInputBinding;
	swapchainSubpassInfo.colorAttachmentCount = 1;
	swapchainSubpassInfo.pColorAttachments = &swapchainColorBinding;
	swapchainSubpassInfo.pResolveAttachments = 0;
	swapchainSubpassInfo.pDepthStencilAttachment = 0;
	swapchainSubpassInfo.preserveAttachmentCount = 0;
	swapchainSubpassInfo.pPreserveAttachments = 0;

    VkAttachmentDescription swapchainImageDescription = {};
	swapchainImageDescription.flags = 0;
	swapchainImageDescription.format = surfaceFormat.format;
	swapchainImageDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	swapchainImageDescription.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	swapchainImageDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	swapchainImageDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	swapchainImageDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	swapchainImageDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	swapchainImageDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription renderImageDescription = {};
	renderImageDescription.flags = 0;
	renderImageDescription.format = surfaceFormat.format;
	renderImageDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	renderImageDescription.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	renderImageDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	renderImageDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	renderImageDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	renderImageDescription.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	renderImageDescription.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkSubpassDependency subpassDependencies[3] = {};
	subpassDependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependencies[0].dstSubpass = 0;
	subpassDependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	subpassDependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependencies[0].srcAccessMask =  VK_ACCESS_MEMORY_READ_BIT;
	subpassDependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	subpassDependencies[0].dependencyFlags = 0;

	subpassDependencies[1].srcSubpass = 0;
	subpassDependencies[1].dstSubpass = 1;
	subpassDependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	subpassDependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	subpassDependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	subpassDependencies[1].dependencyFlags = 0;

	subpassDependencies[2].srcSubpass = 0;
	subpassDependencies[2].dstSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependencies[2].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependencies[2].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	subpassDependencies[2].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	subpassDependencies[2].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	subpassDependencies[2].dependencyFlags = 0;

    VkAttachmentDescription attachmentDescriptions[] = 
    {
        swapchainImageDescription,
        renderImageDescription,
    };

    VkSubpassDescription subpassDescriptions[] =
    {
        renderSubpassInfo,
        swapchainSubpassInfo,
    };

    VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.pNext = 0;
	renderPassCreateInfo.flags = 0;
	renderPassCreateInfo.attachmentCount = 2;
	renderPassCreateInfo.pAttachments = attachmentDescriptions;
	renderPassCreateInfo.subpassCount = 2;
	renderPassCreateInfo.pSubpasses = subpassDescriptions;
	renderPassCreateInfo.dependencyCount = 3;
	renderPassCreateInfo.pDependencies = subpassDependencies;

    VkRenderPass renderPass = {};
	result = vkCreateRenderPass(logicalDevice, &renderPassCreateInfo, 0, &renderPass);
	assert(result == VK_SUCCESS);
    state->swapchainRenderPass = renderPass;

    VkImageView imageViews[2] = 
    {
        state->swapchain.imageView,
        state->renderImage.view,
    };

    VkFramebufferCreateInfo frameBufferCreateInfo = {};
    frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frameBufferCreateInfo.pNext = 0;
    frameBufferCreateInfo.flags = 0;
    frameBufferCreateInfo.renderPass = renderPass;
    frameBufferCreateInfo.attachmentCount = 2;
    frameBufferCreateInfo.pAttachments = imageViews;
    frameBufferCreateInfo.width = swapchainExtent.width;
    frameBufferCreateInfo.height = swapchainExtent.height;
    frameBufferCreateInfo.layers = 1;

    VkFramebuffer frameBuffer = {};
    result = vkCreateFramebuffer(logicalDevice, &frameBufferCreateInfo, 0, &frameBuffer);
    state->swapchainFrameBuffer = frameBuffer;
    END_BLOCK

    START_BLOCK


    VkShaderModule vertexShaderModule = {};
	WCHAR* vertexShaderPath = (WCHAR*)L"C:\\Users\\administrator-1\\Desktop\\sample-machine\\source\\waveform.vert.spirv";
	createShaderModule(logicalDevice, vertexShaderPath, &vertexShaderModule);

	VkPipelineShaderStageCreateInfo vertexShaderStage = {};
	createShaderStage(vertexShaderModule, VK_SHADER_STAGE_VERTEX_BIT, &vertexShaderStage);

	VkShaderModule fragmentShaderModule = {};
	WCHAR* fragmentShaderPath = (WCHAR*)L"C:\\Users\\administrator-1\\Desktop\\sample-machine\\source\\waveform.frag.spirv";
	createShaderModule(logicalDevice, fragmentShaderPath, &fragmentShaderModule);

	VkPipelineShaderStageCreateInfo fragmentShaderStage = {};
	createShaderStage(fragmentShaderModule, VK_SHADER_STAGE_FRAGMENT_BIT, &fragmentShaderStage);

	VkPipelineShaderStageCreateInfo shaderStages[] =
	{
		vertexShaderStage,
		fragmentShaderStage
	};

    uint stages = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_COMPUTE_BIT;

    VkPushConstantRange pushConstantRange = {};
    pushConstantRange.stageFlags = stages;
    pushConstantRange.offset = 0;
    pushConstantRange.size = 64;

	VkDescriptorSetLayout descriptorSetLayout = state->waveformPassDescriptorSetLayout;

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.pNext = 0;
	pipelineLayoutCreateInfo.flags = 0;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
	pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;

    VkPipelineLayout pipelineLayout = {};
    result = vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, 0, &pipelineLayout);
    state->waveformPassPipelineLayout = pipelineLayout; 

    VkRenderPass renderPass = state->waveformRenderPass;

    VkPipelineInputAssemblyStateCreateInfo waveformInputAssemblyState = {};
	waveformInputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	waveformInputAssemblyState.pNext = 0;
	waveformInputAssemblyState.flags = 0;
	waveformInputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	waveformInputAssemblyState.primitiveRestartEnable = 0;

    VkPipelineVertexInputStateCreateInfo waveformVertexInputState = {};
	waveformVertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	waveformVertexInputState.pNext = 0;
	waveformVertexInputState.flags = 0;
	waveformVertexInputState.vertexBindingDescriptionCount = 0;
	waveformVertexInputState.pVertexBindingDescriptions = 0;
	waveformVertexInputState.vertexAttributeDescriptionCount = 0;
	waveformVertexInputState.pVertexAttributeDescriptions = 0;

    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;;
	graphicsPipelineCreateInfo.pNext = 0;
	graphicsPipelineCreateInfo.flags = 0;
	graphicsPipelineCreateInfo.stageCount = 2;
	graphicsPipelineCreateInfo.pStages = shaderStages;
	graphicsPipelineCreateInfo.pVertexInputState = &waveformVertexInputState;
	graphicsPipelineCreateInfo.pInputAssemblyState = &waveformInputAssemblyState;
	graphicsPipelineCreateInfo.pTessellationState = 0;
	graphicsPipelineCreateInfo.pViewportState = &viewportState;
	graphicsPipelineCreateInfo.pRasterizationState = &rasterizationState;
	graphicsPipelineCreateInfo.pMultisampleState = &multisampleState;
	graphicsPipelineCreateInfo.pDepthStencilState = 0;
	graphicsPipelineCreateInfo.pColorBlendState = &colorBlendState;
	graphicsPipelineCreateInfo.pDynamicState = &dynamicState;
	graphicsPipelineCreateInfo.layout = pipelineLayout;
	graphicsPipelineCreateInfo.renderPass = renderPass;
	graphicsPipelineCreateInfo.subpass = 0;
	graphicsPipelineCreateInfo.basePipelineHandle = 0;
	graphicsPipelineCreateInfo.basePipelineIndex = -1;

	VkPipeline graphicsPipeline = {};
	result = vkCreateGraphicsPipelines(logicalDevice, 0, 1, &graphicsPipelineCreateInfo, 0, &graphicsPipeline);
	assert(result == VK_SUCCESS);
    state->waveformPassPipeline = graphicsPipeline;

    VkShaderModule computeShaderModule = {};
	WCHAR* computeShaderPath = (WCHAR*)L"C:\\Users\\administrator-1\\Desktop\\sample-machine\\source\\waveform.comp.spirv";
	createShaderModule(logicalDevice, computeShaderPath, &computeShaderModule);

	VkPipelineShaderStageCreateInfo computeShaderStage = {};
	createShaderStage(computeShaderModule, VK_SHADER_STAGE_COMPUTE_BIT, &computeShaderStage);

	VkComputePipelineCreateInfo computePipelineCreateInfo = {};
	computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	computePipelineCreateInfo.pNext = 0;
	computePipelineCreateInfo.flags = 0;
	computePipelineCreateInfo.stage = computeShaderStage;
	computePipelineCreateInfo.layout = pipelineLayout;
	computePipelineCreateInfo.basePipelineHandle = 0;
	computePipelineCreateInfo.basePipelineIndex = 0;

	VkPipeline computePipeline = {};
	result = vkCreateComputePipelines(logicalDevice, 0, 1, &computePipelineCreateInfo, 0, &computePipeline);
    state->waveformPassComputePipeline = computePipeline;

    END_BLOCK

    START_BLOCK

    VkShaderModule computeShaderModule = {};
	WCHAR* computeShaderPath = (WCHAR*)L"C:\\Users\\administrator-1\\Desktop\\sample-machine\\source\\waveformMax.comp.spirv";
	createShaderModule(logicalDevice, computeShaderPath, &computeShaderModule);

	VkPipelineShaderStageCreateInfo computeShaderStage = {};
	createShaderStage(computeShaderModule, VK_SHADER_STAGE_COMPUTE_BIT, &computeShaderStage);

    VkPipelineLayout pipelineLayout = state->waveformPassPipelineLayout;
	VkComputePipelineCreateInfo computePipelineCreateInfo = {};
	computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	computePipelineCreateInfo.pNext = 0;
	computePipelineCreateInfo.flags = 0;
	computePipelineCreateInfo.stage = computeShaderStage;
	computePipelineCreateInfo.layout = pipelineLayout;
	computePipelineCreateInfo.basePipelineHandle = 0;
	computePipelineCreateInfo.basePipelineIndex = 0;

	VkPipeline computePipeline = {};
	result = vkCreateComputePipelines(logicalDevice, 0, 1, &computePipelineCreateInfo, 0, &computePipeline);
    state->waveformPassMaxComputePipeline = computePipeline;
    END_BLOCK

    START_BLOCK

    VkShaderModule computeShaderModule = {};
	WCHAR* computeShaderPath = (WCHAR*)L"C:\\Users\\administrator-1\\Desktop\\sample-machine\\source\\waveformMipLevel.comp.spirv";
	createShaderModule(logicalDevice, computeShaderPath, &computeShaderModule);

	VkPipelineShaderStageCreateInfo computeShaderStage = {};
	createShaderStage(computeShaderModule, VK_SHADER_STAGE_COMPUTE_BIT, &computeShaderStage);

    VkPipelineLayout pipelineLayout = state->waveformPassPipelineLayout;
	VkComputePipelineCreateInfo computePipelineCreateInfo = {};
	computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	computePipelineCreateInfo.pNext = 0;
	computePipelineCreateInfo.flags = 0;
	computePipelineCreateInfo.stage = computeShaderStage;
	computePipelineCreateInfo.layout = pipelineLayout;
	computePipelineCreateInfo.basePipelineHandle = 0;
	computePipelineCreateInfo.basePipelineIndex = 0;

	VkPipeline computePipeline = {};
	result = vkCreateComputePipelines(logicalDevice, 0, 1, &computePipelineCreateInfo, 0, &computePipeline);
    state->waveformMipLevelComputePipeline = computePipeline;
    END_BLOCK

    START_BLOCK
    WavFile wavFile = state->wavFile;
    uint frameCount = (uint)wavFile.frameCount;

    result = vkQueueWaitIdle(computeQueue);
    assert(result == VK_SUCCESS);

    VkCommandBufferBeginInfo commandBufferBeginInfo = {};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = 0;
    commandBufferBeginInfo.flags = 0;
    commandBufferBeginInfo.pInheritanceInfo = 0;

    VkCommandBuffer commandBuffer = state->device.computeCommandBuffer;

    vkResetCommandBuffer(commandBuffer, 0);
    result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
    assert(result == VK_SUCCESS);

    VkPipeline computePipeline = state->waveformMipLevelComputePipeline;
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline);

    VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_COMPUTE;
	VkDescriptorSet descriptorSet = state->waveformPassDescriptorSet;
    VkPipelineLayout pipelineLayout = state->waveformPassPipelineLayout;
    vkCmdBindDescriptorSets(commandBuffer, bindPoint, pipelineLayout, 0, 1, &descriptorSet, 0, 0);

    uint mipLevelCount = (uint)log2((float)frameCount);
    uint inputOffset = 0;
    for(uint i = 0; i != mipLevelCount; ++i)
    {
        struct PushConstant
        {
            uint inputOffset;
            uint frameCount;
        };
        PushConstant pushConstant = {};
        pushConstant.inputOffset = inputOffset;
        pushConstant.frameCount = frameCount;

        uint stages = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT |VK_SHADER_STAGE_COMPUTE_BIT;
        vkCmdPushConstants(commandBuffer, pipelineLayout, stages, 0, sizeof(PushConstant), &pushConstant);

        uint srcStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
        uint dstStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
        vkCmdPipelineBarrier(commandBuffer, srcStage, dstStage, 0, 0, 0, 0, 0, 0, 0);

        uint groupCount = (uint)(frameCount / 4) / 64;
        vkCmdDispatch(commandBuffer, groupCount + 1, 1, 1);

        inputOffset += frameCount;
        frameCount /= 2;
    }

    result = vkEndCommandBuffer(commandBuffer);
    assert(result == VK_SUCCESS);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = 0;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = 0;
    submitInfo.pWaitDstStageMask = 0;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = 0;

    result = vkQueueSubmit(computeQueue, 1, &submitInfo, 0);
    assert(result == VK_SUCCESS);

    END_BLOCK

    START_BLOCK

    VkShaderModule vertexShaderModule = {};
	WCHAR* vertexShaderPath = (WCHAR*)L"C:\\Users\\administrator-1\\Desktop\\sample-machine\\source\\geometry.vert.spirv";
	createShaderModule(logicalDevice, vertexShaderPath, &vertexShaderModule);

	VkPipelineShaderStageCreateInfo vertexShaderStage = {};
	createShaderStage(vertexShaderModule, VK_SHADER_STAGE_VERTEX_BIT, &vertexShaderStage);

	VkShaderModule fragmentShaderModule = {};
	WCHAR* fragmentShaderPath = (WCHAR*)L"C:\\Users\\administrator-1\\Desktop\\sample-machine\\source\\geometry.frag.spirv";
	createShaderModule(logicalDevice, fragmentShaderPath, &fragmentShaderModule);

	VkPipelineShaderStageCreateInfo fragmentShaderStage = {};
	createShaderStage(fragmentShaderModule, VK_SHADER_STAGE_FRAGMENT_BIT, &fragmentShaderStage);

	VkPipelineShaderStageCreateInfo shaderStages[] =
	{
		vertexShaderStage,
		fragmentShaderStage
	};

    uint stages = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

    VkPushConstantRange pushConstantRange = {};
    pushConstantRange.stageFlags = stages;
    pushConstantRange.offset = 0;
    pushConstantRange.size = 64;

	VkDescriptorSetLayout descriptorSetLayout = state->geometryPassDescriptorSetLayout;

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.pNext = 0;
	pipelineLayoutCreateInfo.flags = 0;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
	pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;

    VkPipelineLayout pipelineLayout = {};
    result = vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, 0, &pipelineLayout);
    state->geometryPassPipelineLayout = pipelineLayout; 

    VkRenderPass renderPass = state->swapchainRenderPass;

    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;;
	graphicsPipelineCreateInfo.pNext = 0;
	graphicsPipelineCreateInfo.flags = 0;
	graphicsPipelineCreateInfo.stageCount = 2;
	graphicsPipelineCreateInfo.pStages = shaderStages;
	graphicsPipelineCreateInfo.pVertexInputState = &vertexInputState;
	graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
	graphicsPipelineCreateInfo.pTessellationState = 0;
	graphicsPipelineCreateInfo.pViewportState = &viewportState;
	graphicsPipelineCreateInfo.pRasterizationState = &rasterizationState;
	graphicsPipelineCreateInfo.pMultisampleState = &multisampleState;
	graphicsPipelineCreateInfo.pDepthStencilState = 0;
	graphicsPipelineCreateInfo.pColorBlendState = &colorBlendState;
	graphicsPipelineCreateInfo.pDynamicState = &dynamicState;
	graphicsPipelineCreateInfo.layout = pipelineLayout;
	graphicsPipelineCreateInfo.renderPass = renderPass;
	graphicsPipelineCreateInfo.subpass = 0;
	graphicsPipelineCreateInfo.basePipelineHandle = 0;
	graphicsPipelineCreateInfo.basePipelineIndex = -1;

	VkPipeline graphicsPipeline = {};
	result = vkCreateGraphicsPipelines(logicalDevice, 0, 1, &graphicsPipelineCreateInfo, 0, &graphicsPipeline);
	assert(result == VK_SUCCESS);
    state->geometryPassPipeline = graphicsPipeline;

    END_BLOCK

    START_BLOCK

    VkShaderModule vertexShaderModule = {};
	WCHAR* vertexShaderPath = (WCHAR*)L"C:\\Users\\administrator-1\\Desktop\\sample-machine\\source\\swapchain.vert.spirv";
	createShaderModule(logicalDevice, vertexShaderPath, &vertexShaderModule);

	VkPipelineShaderStageCreateInfo vertexShaderStage = {};
	createShaderStage(vertexShaderModule, VK_SHADER_STAGE_VERTEX_BIT, &vertexShaderStage);

	VkShaderModule fragmentShaderModule = {};
	WCHAR* fragmentShaderPath = (WCHAR*)L"C:\\Users\\administrator-1\\Desktop\\sample-machine\\source\\swapchain.frag.spirv";
	createShaderModule(logicalDevice, fragmentShaderPath, &fragmentShaderModule);

	VkPipelineShaderStageCreateInfo fragmentShaderStage = {};
	createShaderStage(fragmentShaderModule, VK_SHADER_STAGE_FRAGMENT_BIT, &fragmentShaderStage);

    VkPipelineShaderStageCreateInfo shaderStages[] =
	{
		vertexShaderStage,
		fragmentShaderStage
	};
    
	VkDescriptorSetLayoutBinding renderImageBinding = {};
	createDescriptorLayoutBinding(0, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1, VK_SHADER_STAGE_FRAGMENT_BIT, &renderImageBinding);

    VkDescriptorSetLayoutBinding layoutBindings[] =
	{
        renderImageBinding,
    };
	VkDescriptorSetLayout descriptorSetLayout = {};
	createDescriptorSetLayout(logicalDevice, layoutBindings, 1, &descriptorSetLayout);

    uint descriptorTypes[] =
	{
		VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
    };
	uint descriptorCounts[] =
	{
		1,
    };
    VkDescriptorPool descriptorPool = {};
	createDescriptorPool(logicalDevice, descriptorTypes, descriptorCounts, 1, &descriptorPool);

    VkDescriptorSet descriptorSet = {};
	createDescriptorSet(logicalDevice, descriptorPool, descriptorSetLayout, &descriptorSet);
    state->swapchainPassDescriptorSet = descriptorSet;

    VkImageView imageView = state->renderImage.view;

    VkDescriptorImageInfo descriptorImageInfo = {};
	descriptorImageInfo.sampler = 0;
	descriptorImageInfo.imageView = imageView;
	descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkWriteDescriptorSet writeDescriptorSet = {};
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.pNext = 0;
	writeDescriptorSet.dstSet = descriptorSet;
	writeDescriptorSet.dstBinding = 0;
	writeDescriptorSet.dstArrayElement = 0;
	writeDescriptorSet.descriptorCount = 1;
	writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	writeDescriptorSet.pImageInfo = &descriptorImageInfo;
	writeDescriptorSet.pBufferInfo = 0;
	writeDescriptorSet.pTexelBufferView = 0;

    vkUpdateDescriptorSets(logicalDevice, 1, &writeDescriptorSet, 0, 0);

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.pNext = 0;
	pipelineLayoutCreateInfo.flags = 0;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
	pipelineLayoutCreateInfo.pPushConstantRanges = 0;

    VkPipelineLayout pipelineLayout = {};
    result = vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, 0, &pipelineLayout);
    state->swapchainPassPipelineLayout = pipelineLayout;

    VkRenderPass renderPass = state->swapchainRenderPass;

    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphicsPipelineCreateInfo.pNext = 0;
	graphicsPipelineCreateInfo.flags = 0;
	graphicsPipelineCreateInfo.stageCount = 2;
	graphicsPipelineCreateInfo.pStages = shaderStages;
	graphicsPipelineCreateInfo.pVertexInputState = &vertexInputState;
	graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
	graphicsPipelineCreateInfo.pTessellationState = 0;
	graphicsPipelineCreateInfo.pViewportState = &viewportState;
	graphicsPipelineCreateInfo.pRasterizationState = &rasterizationState;
	graphicsPipelineCreateInfo.pMultisampleState = &multisampleState;
	graphicsPipelineCreateInfo.pDepthStencilState = 0;
	graphicsPipelineCreateInfo.pColorBlendState = &colorBlendState;
	graphicsPipelineCreateInfo.pDynamicState = &dynamicState;
	graphicsPipelineCreateInfo.layout = pipelineLayout;
	graphicsPipelineCreateInfo.renderPass = renderPass;
	graphicsPipelineCreateInfo.subpass = 1;
	graphicsPipelineCreateInfo.basePipelineHandle = 0;
	graphicsPipelineCreateInfo.basePipelineIndex = -1;

	VkPipeline graphicsPipeline = {};
	result = vkCreateGraphicsPipelines(logicalDevice, 0, 1, &graphicsPipelineCreateInfo, 0, &graphicsPipeline);
	assert(result == VK_SUCCESS);
    state->swapchainPassPipeline = graphicsPipeline;

    END_BLOCK

    VkSemaphore imageSemaphore = {};
	createSemaphore(logicalDevice, &imageSemaphore);
    state->imageSemaphore = imageSemaphore;

	VkSemaphore executionSemaphore = {};
	createSemaphore(logicalDevice, &executionSemaphore);
    state->executionSemaphore = executionSemaphore;

	VkFence pipelineFence = {};
	createFence(logicalDevice, &pipelineFence);
    state->pipelineFence = pipelineFence;

    *vulkan = state;
}
void resizeImages(State* state)
{
	VkResult result = {};
    VulkanDevice* device = &state->device;
    VkDevice logicalDevice = device->logicalDevice;
    vkDeviceWaitIdle(logicalDevice);

    START_BLOCK

	VkFramebuffer frameBuffer = state->swapchainFrameBuffer;
    vkDestroyFramebuffer(logicalDevice, frameBuffer, 0);

	VkImageView imageView = state->swapchain.imageView;
    vkDestroyImageView(logicalDevice, imageView, 0);

    END_BLOCK

    START_BLOCK

	VkImageView imageView = state->renderImage.view;
    vkDestroyImageView(logicalDevice, imageView, 0);

    VkImage image = state->renderImage.image;
    vkDestroyImage(logicalDevice, image, 0);

    END_BLOCK

    START_BLOCK

	VkSwapchainKHR swapchain = state->swapchain.swapchain;
	vkDestroySwapchainKHR(logicalDevice, swapchain, 0);

    END_BLOCK


    VkPhysicalDevice physicalDevice = state->device.physicalDevice;
    VkSurfaceKHR surface = state->swapchain.surface;

	VkSurfaceCapabilitiesKHR surfaceCapabilities = {};
	result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);
    state->swapchain.surfaceCapabilities = surfaceCapabilities;

    int format = VK_FORMAT_R8G8B8A8_SRGB;
    int colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    VkSurfaceFormatKHR surfaceFormat = {};
    getSurfaceFormat(physicalDevice, surface, format, colorSpace, &surfaceFormat);
    state->swapchain.surfaceFormat = surfaceFormat;

    VkExtent2D swapchainExtent = surfaceCapabilities.minImageExtent;
    state->swapchain.extent = swapchainExtent;

    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

    uint swapchainImageCount = 1;
    state->swapchain.imageCount = swapchainImageCount;

    VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.pNext = 0;
	swapchainCreateInfo.flags = 0;
	swapchainCreateInfo.surface = surface;
	swapchainCreateInfo.minImageCount = swapchainImageCount;
	swapchainCreateInfo.imageFormat = surfaceFormat.format;
	swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapchainCreateInfo.imageExtent = swapchainExtent;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
	swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainCreateInfo.queueFamilyIndexCount = 0;
	swapchainCreateInfo.pQueueFamilyIndices = 0;
	swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.presentMode = swapchainPresentMode;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.oldSwapchain = 0;

	VkSwapchainKHR swapchain = {};
	result = vkCreateSwapchainKHR(logicalDevice, &swapchainCreateInfo, 0, &swapchain);
	assert(result == VK_SUCCESS);
    state->swapchain.swapchain = swapchain;

    START_BLOCK

    VkImage image = {};
	result = vkGetSwapchainImagesKHR(logicalDevice, swapchain, &swapchainImageCount, &image);
    state->swapchain.image = image;

    VkImageViewCreateInfo imageViewCreateInfo = {};
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.pNext = 0;
	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.image = image;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = surfaceFormat.format;
	imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;

    VkImageView imageView = {};
    result = vkCreateImageView(logicalDevice, &imageViewCreateInfo, 0, &imageView);
    assert(result == VK_SUCCESS);
    state->swapchain.imageView = imageView;

    END_BLOCK

    START_BLOCK

    VkImageCreateInfo imageCreateInfo = {};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.pNext = 0;
    imageCreateInfo.flags = 0;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = surfaceFormat.format;
    imageCreateInfo.extent.width = swapchainExtent.width;
    imageCreateInfo.extent.height = swapchainExtent.height;
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.usage = 
    {
        VK_IMAGE_USAGE_TRANSFER_DST_BIT |
        VK_IMAGE_USAGE_SAMPLED_BIT | 
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | 
        VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT
    };
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.queueFamilyIndexCount = 0;
    imageCreateInfo.pQueueFamilyIndices = 0;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    Image renderImage = {};
    renderImage.width = swapchainExtent.width;
    renderImage.height = swapchainExtent.height;
    renderImage.memoryType = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    createImage(device, &imageCreateInfo, &renderImage);
    transitionImage(device, renderImage.image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    VkImageViewCreateInfo imageViewCreateInfo = {};
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.pNext = 0;
	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.image = renderImage.image;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = surfaceFormat.format;
	imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;

    VkImageView imageView = {};
    result = vkCreateImageView(logicalDevice, &imageViewCreateInfo, 0, &imageView);
    assert(result == VK_SUCCESS);
    renderImage.view = imageView;

    VkDescriptorSet descriptorSet = state->swapchainPassDescriptorSet;

    VkDescriptorImageInfo descriptorImageInfo = {};
	descriptorImageInfo.sampler = 0;
	descriptorImageInfo.imageView = imageView;
	descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkWriteDescriptorSet writeDescriptorSet = {};
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.pNext = 0;
	writeDescriptorSet.dstSet = descriptorSet;
	writeDescriptorSet.dstBinding = 0;
	writeDescriptorSet.dstArrayElement = 0;
	writeDescriptorSet.descriptorCount = 1;
	writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	writeDescriptorSet.pImageInfo = &descriptorImageInfo;
	writeDescriptorSet.pBufferInfo = 0;
	writeDescriptorSet.pTexelBufferView = 0;

    vkUpdateDescriptorSets(logicalDevice, 1, &writeDescriptorSet, 0, 0);

    state->renderImage = renderImage;
    END_BLOCK

    VkImageView imageViews[2] = 
    {
        state->swapchain.imageView,
        state->renderImage.view,
    };
    VkRenderPass renderPass = state->swapchainRenderPass;

    VkFramebufferCreateInfo frameBufferCreateInfo = {};
    frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frameBufferCreateInfo.pNext = 0;
    frameBufferCreateInfo.flags = 0;
    frameBufferCreateInfo.renderPass = renderPass;
    frameBufferCreateInfo.attachmentCount = 2;
    frameBufferCreateInfo.pAttachments = imageViews;
    frameBufferCreateInfo.width = swapchainExtent.width;
    frameBufferCreateInfo.height = swapchainExtent.height;
    frameBufferCreateInfo.layers = 1;

    VkFramebuffer frameBuffer = {};
    result = vkCreateFramebuffer(logicalDevice, &frameBufferCreateInfo, 0, &frameBuffer);
    state->swapchainFrameBuffer = frameBuffer;

}
void createWaveform(void* vulkan)
{
    State* state = (State*)vulkan;

    VkResult result = {};

    START_BLOCK

    VkQueue computeQueue = state->device.computeQueue;
    result = vkQueueWaitIdle(computeQueue);
    assert(result == VK_SUCCESS);

    VkCommandBufferBeginInfo commandBufferBeginInfo = {};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = 0;
    commandBufferBeginInfo.flags = 0;
    commandBufferBeginInfo.pInheritanceInfo = 0;

    VkCommandBuffer commandBuffer = state->device.computeCommandBuffer;

    vkResetCommandBuffer(commandBuffer, 0);
    result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
    assert(result == VK_SUCCESS);

    VkPipeline computePipeline = state->waveformPassComputePipeline;
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline);

    VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_COMPUTE;
	VkDescriptorSet descriptorSet = state->waveformPassDescriptorSet;
    VkPipelineLayout pipelineLayout = state->waveformPassPipelineLayout;
    vkCmdBindDescriptorSets(commandBuffer, bindPoint, pipelineLayout, 0, 1, &descriptorSet, 0, 0);

    uint width = state->waveformWidth;
    uint frameCount = (uint)state->wavFile.frameCount;
    float framesPerPixel = (float)frameCount / (float)width;
    int mipLevel = (int)log2(framesPerPixel);
    if(mipLevel < 0)
    {
        mipLevel = 0;
    }

    uint mipIndex = {};
    uint mipLevelFrameCount = {};
    for(uint i = 0; i != (uint)mipLevel; ++i)
    {
        mipLevelFrameCount = frameCount / (uint)pow(2.0, (float)i);
        mipIndex += mipLevelFrameCount;
    }
    state->mipLevelFrameCount = mipLevelFrameCount;
    float mipFramesPerPixel = framesPerPixel / (float)pow(2, mipLevel);

    float mipLevelOffset = 1.0f;
    if(mipLevel == 0)
    {
        mipLevelOffset = 0.0f;
    }
    uint startOffset = state->startOffset;

    struct PushConstant
    {
        uint width;
        uint mipIndex;
        float framesPerPixel;
        float mipLevelOffset;
        uint startOffset;
        uint padding;
    };

    if(width > 400)
    {
        width = 400;
    }

    PushConstant pushConstant = {};
    pushConstant.width = width;
    pushConstant.mipIndex = mipIndex;
    pushConstant.framesPerPixel = mipFramesPerPixel;
    pushConstant.mipLevelOffset = mipLevelOffset;
    pushConstant.startOffset = startOffset;

    uint stages = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT |VK_SHADER_STAGE_COMPUTE_BIT;
    vkCmdPushConstants(commandBuffer, pipelineLayout, stages, 0, sizeof(PushConstant), &pushConstant);

    uint groupCount = (uint)width / 64;
    vkCmdDispatch(commandBuffer, groupCount + 1, 1, 1);

    result = vkEndCommandBuffer(commandBuffer);
    assert(result == VK_SUCCESS);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = 0;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = 0;
    submitInfo.pWaitDstStageMask = 0;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = 0;

    result = vkQueueSubmit(computeQueue, 1, &submitInfo, 0);
    assert(result == VK_SUCCESS);

    END_BLOCK

    VkCommandBuffer commandBuffer = state->device.graphicsCommandBuffer;

    VkQueue graphicsQueue = state->device.graphicsQueue;
    result = vkQueueWaitIdle(graphicsQueue);
    vkResetCommandBuffer(commandBuffer, 0);

    VkCommandBufferBeginInfo commandBufferBeginInfo = {};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = 0;
    commandBufferBeginInfo.flags = 0;
    commandBufferBeginInfo.pInheritanceInfo = 0;

    result = vkQueueWaitIdle(graphicsQueue);
    assert(result == VK_SUCCESS);

    result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
    assert(result == VK_SUCCESS);

    VkClearValue clearColor = {};
    clearColor.color.float32[0] = 0.0f;
    clearColor.color.float32[1] = 0.0f;
    clearColor.color.float32[2] = 0.0f;
    clearColor.color.float32[3] = 1.0f;

    VkRenderPass renderPass = state->waveformRenderPass;
    VkFramebuffer frameBuffer = state->waveformFrameBuffer;

    VkRenderPassBeginInfo renderPassBeginInfo = {};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = 0;
    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.framebuffer = frameBuffer;
    renderPassBeginInfo.renderArea.offset.x = 0;
    renderPassBeginInfo.renderArea.offset.y = 0;
    renderPassBeginInfo.renderArea.extent.width = 400;
    renderPassBeginInfo.renderArea.extent.height = 100;
    renderPassBeginInfo.clearValueCount = 1;
    renderPassBeginInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    START_BLOCK

    VkPipeline graphicsPipeline = state->waveformPassPipeline;
    VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    vkCmdBindPipeline(commandBuffer, bindPoint, graphicsPipeline);

    END_BLOCK

    VkViewport viewport = {};
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = 400.0f;
    viewport.height = 100.0f;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor = {};
    scissor.extent.width = 400;
    scissor.extent.height = 100;
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	VkDescriptorSet descriptorSet = state->waveformPassDescriptorSet;
    VkPipelineLayout pipelineLayout = state->waveformPassPipelineLayout;
    vkCmdBindDescriptorSets(commandBuffer, bindPoint, pipelineLayout, 0, 1, &descriptorSet, 0, 0);

    uint width = state->waveformWidth;
    struct PushConstant
    {
        uint frameCount;
        uint width;
    };
    PushConstant pushConstant = {};
    pushConstant.frameCount = 0;
    pushConstant.width = 400;

    uint stages = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_COMPUTE_BIT;
    vkCmdPushConstants(commandBuffer, pipelineLayout, stages, 0, sizeof(PushConstant), &pushConstant);
    if(width > 400)
    {
        width = 400;
    }
    vkCmdDraw(commandBuffer, width, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer);
    result = vkEndCommandBuffer(commandBuffer);
    assert(result == VK_SUCCESS);

    VkPipelineStageFlags pipelineStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSemaphore executionSemaphore = state->executionSemaphore;

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = 0;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = 0;
    submitInfo.pWaitDstStageMask = 0;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = 0;

    result = vkQueueSubmit(graphicsQueue, 1, &submitInfo, 0);
    assert(result == VK_SUCCESS);

    result = vkQueueWaitIdle(graphicsQueue);

}
void draw(void* vulkan)
{
    State* state = (State*)vulkan;
    createWaveform(state);

    uint activeEntityCount = state->activeEntityCount;
    if(!activeEntityCount)
    {
        return;
    }
    VkResult result = {};
    VkDevice logicalDevice = state->device.logicalDevice;
    VkFence pipelineFence = state->pipelineFence;

    uint imageIndex = {};
    result = vkWaitForFences(logicalDevice, 1, &pipelineFence, VK_TRUE, UINT64_MAX);
    assert(result == VK_SUCCESS);

	VkSwapchainKHR swapchain = state->swapchain.swapchain;
    VkSemaphore imageSemaphore = state->imageSemaphore;
    result = vkAcquireNextImageKHR(logicalDevice, swapchain, UINT64_MAX, imageSemaphore, 0, &imageIndex);
    if(result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		resizeImages(state);
		return;
	}

    result = vkResetFences(logicalDevice, 1, &pipelineFence);
    assert(result == VK_SUCCESS);

    VkCommandBuffer commandBuffer = state->device.graphicsCommandBuffer;
    vkResetCommandBuffer(commandBuffer, 0);

    VkCommandBufferBeginInfo commandBufferBeginInfo = {};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = 0;
    commandBufferBeginInfo.flags = 0;
    commandBufferBeginInfo.pInheritanceInfo = 0;

    result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
    assert(result == VK_SUCCESS);


    VkClearValue clearColor = {};
    clearColor.color.float32[0] = 0.0f;
    clearColor.color.float32[1] = 0.0f;
    clearColor.color.float32[2] = 0.0f;
    clearColor.color.float32[3] = 0.0f;

    VkRenderPass renderPass = state->swapchainRenderPass;
    VkFramebuffer frameBuffer = state->swapchainFrameBuffer;
    VkExtent2D swapchainExtent = state->swapchain.extent;

    VkRenderPassBeginInfo renderPassBeginInfo = {};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = 0;
    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.framebuffer = frameBuffer;
    renderPassBeginInfo.renderArea.offset.x = 0;
    renderPassBeginInfo.renderArea.offset.y = 0;
    renderPassBeginInfo.renderArea.extent.width = swapchainExtent.width;
    renderPassBeginInfo.renderArea.extent.height = swapchainExtent.height;
    renderPassBeginInfo.clearValueCount = 1;
    renderPassBeginInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    
    START_BLOCK

    VkPipeline graphicsPipeline = state->geometryPassPipeline;
    VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    vkCmdBindPipeline(commandBuffer, bindPoint, graphicsPipeline);

    END_BLOCK

    VkViewport viewport = {};
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = (float)swapchainExtent.width;
    viewport.height = (float)swapchainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor = {};
    scissor.extent.width = swapchainExtent.width;
    scissor.extent.height = swapchainExtent.height;
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    VkBuffer vertexBuffer = state->vertexBuffer.buffer;
    VkDeviceSize deviceSize = 0;

    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, &deviceSize);

    START_BLOCK

    VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	VkDescriptorSet descriptorSet = state->geometryPassDescriptorSet;
    VkPipelineLayout pipelineLayout = state->geometryPassPipelineLayout;
    vkCmdBindDescriptorSets(commandBuffer, bindPoint, pipelineLayout, 0, 1, &descriptorSet, 0, 0);

    float extent[16] = 
    {
        (float)swapchainExtent.width,
        (float)swapchainExtent.height,
    };

    uint stages = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT ;
    vkCmdPushConstants(commandBuffer, pipelineLayout, stages, 0, 64, &extent);

    END_BLOCK

    vkCmdDraw(commandBuffer, 6, activeEntityCount, 0, 0);
    //state->activeEntityCount = 0;

    vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);

    START_BLOCK
        
    VkPipeline graphicsPipeline = state->swapchainPassPipeline;
    VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    vkCmdBindPipeline(commandBuffer, bindPoint, graphicsPipeline);

    VkPipelineLayout pipelineLayout = state->swapchainPassPipelineLayout;
	VkDescriptorSet descriptorSet = state->swapchainPassDescriptorSet;
    vkCmdBindDescriptorSets(commandBuffer, bindPoint, pipelineLayout, 0, 1, &descriptorSet, 0, 0);

    END_BLOCK

    vkCmdDraw(commandBuffer, 6, 1, 0, 0);
    
    vkCmdEndRenderPass(commandBuffer);


    result = vkEndCommandBuffer(commandBuffer);
    assert(result == VK_SUCCESS);

    VkPipelineStageFlags pipelineStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSemaphore executionSemaphore = state->executionSemaphore;

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = 0;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &imageSemaphore;
    submitInfo.pWaitDstStageMask = &pipelineStageFlags;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &executionSemaphore;

    VkQueue graphicsQueue = state->device.graphicsQueue;
    result = vkQueueSubmit(graphicsQueue, 1, &submitInfo, pipelineFence);
    assert(result == VK_SUCCESS);

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = 0;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &executionSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = 0;

    VkQueue presentQueue = state->device.presentQueue;
    result = vkQueuePresentKHR(presentQueue, &presentInfo);
    if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
	{
		resizeImages(state);
	}
    START_BLOCK

    Buffer indexSurfaceBuffer = state->indexSurfaceBuffer;
    VkMappedMemoryRange memoryRange = {};
    memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    memoryRange.pNext = 0;
    memoryRange.memory = indexSurfaceBuffer.memory;
    memoryRange.offset = 0;
    memoryRange.size = indexSurfaceBuffer.size;

    vkInvalidateMappedMemoryRanges(logicalDevice, 1, &memoryRange);

    END_BLOCK
}
void setDimension(void* vulkan, uint entityIndex, float x, float y, float width, float height)
{
#if 0
    State* state = (State*)vulkan;
    Entity* entity = &state->entities[entityIndex];

    uint transformIndex = entity->transformIndex;
    vec2* transform = &state->transforms[transformIndex];

    identity(transform);

    float swapchainWidth = (float)state->swapchain.extent.width;
    float swapchainHeight = (float)state->swapchain.extent.height;

    float scaleX = width / swapchainWidth;
    float scaleY = height / swapchainHeight;

    scale(transform, scaleX, scaleY);

    float centerX = x / (swapchainWidth / 2) - 1.0f;
    float centerY = y / (swapchainHeight / 2) - 1.0f;

    float positionX = centerX + scaleX;
    float positionY = centerY + scaleY;

    uint positionIndex = entity->positionIndex;
    vec2* position = &state->positions[positionIndex];

    position->cells[0] = positionX;
    position->cells[1] = positionY;
#endif
}
void createEntity(void* vulkan, uint* entityIndex)
{
    State* state = (State*)vulkan;
    uint entityCount = state->entityCount;
    *entityIndex = entityCount;
    ++state->entityCount;
}
void createTransform(void* vulkan, uint* transformIndex, vec2** transform)
{
    State* state = (State*)vulkan;
    uint transformCount = state->transformCount;
    *transformIndex = transformCount;
    *transform = &state->transforms[transformCount];
    ++state->transformCount;
}
void bindTransform(void* vulkan, uint entityIndex, uint transformIndex)
{
    State* state = (State*)vulkan;
    Entity* entity = &state->entities[entityIndex];
    entity->transformIndex = transformIndex;
}
void getTransform(void* vulkan, uint entityIndex, vec2** transform)
{
    State* state = (State*)vulkan;
    Entity* entity = &state->entities[entityIndex];
    uint transformIndex = entity->transformIndex;
    *transform = &state->transforms[transformIndex];
}
void createPosition(void* vulkan, uint* positionIndex, vec2** position)
{
    State* state = (State*)vulkan;
    uint positionCount = state->positionCount;
    *positionIndex = positionCount;
    *position = &state->positions[positionCount];
    ++state->positionCount;
}
void getPosition(void* vulkan, uint entityIndex, vec2** position)
{
    State* state = (State*)vulkan;
    Entity* entity = &state->entities[entityIndex];
    uint positionIndex = entity->positionIndex;
    *position = &state->positions[positionIndex];
}
void bindPosition(void* vulkan, uint entityIndex, uint positionIndex)
{
    State* state = (State*)vulkan;
    Entity* entity = &state->entities[entityIndex];
    entity->positionIndex = positionIndex;
}
void createColor(void* vulkan, uint* colorIndex, vec4** color)
{
    State* state = (State*)vulkan;
    uint colorCount = state->colorCount;
    *colorIndex = colorCount;
    *color = &state->colors[colorCount];
    ++state->colorCount;
}
void bindColor(void* vulkan, uint entityIndex, uint colorIndex)
{
    State* state = (State*)vulkan;
    Entity* entity = &state->entities[entityIndex];
    entity->colorIndex = colorIndex;
}
void drawEntity(void* vulkan, uint entityIndex)
{
    State* state = (State*)vulkan;
    uint activeEntityCount = state->activeEntityCount;
    state->activeEntities[activeEntityCount] = entityIndex;
    ++state->activeEntityCount;
}
void getEntityOnScreen(void* vulkan, uint x, uint y, uint* entityIndex)
{
    State* state = (State*)vulkan;
    uint* indexSurface = state->indexSurface;
    uint surfaceWidth = state->swapchain.extent.width;
    uint pixelPosition = (y * surfaceWidth) + x;

    *entityIndex = indexSurface[pixelPosition];
}
void setWaveformWidth(void* vulkan, int delta)
{
    State* state = (State*)vulkan;
    int width = (int)state->waveformWidth; 
    width += delta;// * (int)(((float)width / 64.0f) + 4.0f);
    if(width  < 1)
    {
        width = 1;
    }
    state->waveformWidth = (uint)width;
}
void setWaveformStartOffset(void* vulkan, int delta)
{
    State* state = (State*)vulkan;
    int startOffset = (int)state->startOffset; 

    int waveformWidth = (int)state->waveformWidth;
    uint mipFrameCount = state->mipLevelFrameCount;
    if(waveformWidth < 400)
    {
        return;
    }
    startOffset += delta;
    if(startOffset  < 0)
    {
        startOffset = 0;
    }
    if((startOffset * 2) + 400 > waveformWidth)
    {
        startOffset = (waveformWidth - 400) / 2;
    }
    state->startOffset = (uint)startOffset;

}
END_SCOPE
