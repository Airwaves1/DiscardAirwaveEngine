#include "aw_vk_context.hpp"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#include <vector>
#include "window/aw_glfw_window.hpp"
#include <unordered_set>
namespace Airwave
{

// 指定需要的layers和extensions
const std::vector<DeviceFeature> requestedLayers = {
    {VK_LAYER_KHRONOS_validation, true}, {"ABC_LAYER", false}, {"XYZ_LAYER", false}};

const std::vector<DeviceFeature> requestedExtensions = {
    {VK_KHR_SURFACE_EXTENSION_NAME, true},
#ifdef AW_PLATFORM_WINDOWS
    {VK_KHR_WIN32_SURFACE_EXTENSION_NAME, true},
#elif AW_PLATFORM_LINUX
    {VK_KHR_XCB_SURFACE_EXTENSION_NAME, true},
#elif AW_PLATFORM_MACOS
    {VK_MVK_MACOS_SURFACE_EXTENSION_NAME, true},
#endif
    {VK_EXT_debug_report_EXTENSION_NAME, true}, // 添加这一行
};

AwVkContext::AwVkContext(AwWindow *window)
{
    createInstance();
    createSurface(window);
    pickPhysicalDevice();
}

AwVkContext::~AwVkContext()
{
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    vkDestroyInstance(m_instance, nullptr);
    // LOG_INFO("Vulkan instance destroyed");
}

static VkBool32 VkDebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
                                uint64_t srcObject, size_t location, int32_t msgCode,
                                const char *layerPrefix, const char *msg, void *userData)
{
    switch (flags)
    {
        case VK_DEBUG_REPORT_ERROR_BIT_EXT:
            LOG_ERROR("Vulkan Error: {0}", msg);
            break;
        case VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT:
            LOG_WARN("Vulkan Warning: {0}", msg);
            break;
        default:
            LOG_DEBUG("Vulkan Debug: {0}", msg);
            break;
    }

    return VK_TRUE;
}

void AwVkContext::createInstance()
{

    // 1. 构建layers=========================================================
    // =======================================================================
    uint32_t availableLayerCount = 0;
    CALL_VK(vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr));
    std::vector<VkLayerProperties> availableLayers(availableLayerCount);
    CALL_VK(vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers.data()));

    uint32_t enableLayerCount = 0;
    const char *enableLayers[32];
    if (m_needValidationLayer)
    {
        if (!checkDeviceFeatures("Instance Layers", false, availableLayerCount,
                                 availableLayers.data(), requestedLayers.size(),
                                 requestedLayers.data(), &enableLayerCount, enableLayers))
        {
            return;
        }
    }
    // 2. 构建extensions ====================================================
    // =======================================================================
    uint32_t availableExtensionCount = 0;
    CALL_VK(vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr));
    std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
    CALL_VK(vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount,
                                                   availableExtensions.data()));

    // glfw extensions
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> glfwExtensionVector(glfwExtensions,
                                                  glfwExtensions + glfwExtensionCount);
    std::unordered_set<std::string> glfwExtensionSet(glfwExtensions,
                                                     glfwExtensions + glfwExtensionCount);

    uint32_t enableExtensionCount = 0;
    const char *enableExtensions[32];
    if (!checkDeviceFeatures("Instance Extensions", true, availableExtensionCount,
                             availableExtensions.data(), requestedExtensions.size(),
                             requestedExtensions.data(), &enableExtensionCount, enableExtensions))
    {
        return;
    }

    // 3. 创建instance =======================================================
    // =======================================================================
    VkApplicationInfo appInfo = {
        .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext              = nullptr,
        .pApplicationName   = "AirwaveEngine",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName        = "AirwaveEngine",
        .engineVersion      = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion         = VK_API_VERSION_1_0,
    };

    VkDebugReportCallbackCreateInfoEXT debugCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
        .pNext = nullptr,
        .flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT |
                 VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT,
        .pfnCallback = VkDebugCallback,
        .pUserData   = nullptr,
    };

    VkInstanceCreateInfo instanceCreateInfo = {
        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext                   = m_needValidationLayer ? &debugCreateInfo : nullptr,
        .flags                   = 0,
        .pApplicationInfo        = &appInfo,
        .enabledLayerCount       = enableLayerCount,
        .ppEnabledLayerNames     = enableLayerCount ? enableLayers : nullptr,
        .enabledExtensionCount   = enableExtensionCount,
        .ppEnabledExtensionNames = enableExtensionCount ? enableExtensions : nullptr,
    };

    CALL_VK(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance));

    LOG_INFO("Vulkan instance created");
}

void AwVkContext::createSurface(AwWindow *window)
{
    if (!window)
    {
        LOG_ERROR("no window to create surface");
        return;
    }

    auto glfwWindow = dynamic_cast<AwGLFWwindow *>(window);
    if (!glfwWindow)
    {
        LOG_ERROR("window is not a GLFW window");
        return;
    }

    auto glfwWindowHandle = static_cast<GLFWwindow *>(glfwWindow->getNativeWindow());

    CALL_VK(glfwCreateWindowSurface(m_instance, glfwWindowHandle, nullptr, &m_surface));

    LOG_INFO("Vulkan surface created");
}

void AwVkContext::pickPhysicalDevice()
{
    uint32_t phyDeviceCount;
    CALL_VK(vkEnumeratePhysicalDevices(m_instance, &phyDeviceCount, nullptr));
    std::vector<VkPhysicalDevice> phyDevices(phyDeviceCount);
    CALL_VK(vkEnumeratePhysicalDevices(m_instance, &phyDeviceCount, phyDevices.data()));

    uint32_t maxScore              = 0;
    int32_t maxScorePhyDeviceIndex = -1;
    LOG_DEBUG("-----------------------------");
    LOG_DEBUG("Physical devices: ");
    for (int i = 0; i < phyDeviceCount; i++)
    {
        // log
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(phyDevices[i], &props);
        printPhysicalDeviceProperties(props);

        uint32_t score = getPhyDeviceScore(props);
        uint32_t formatCount;
        CALL_VK(
            vkGetPhysicalDeviceSurfaceFormatsKHR(phyDevices[i], m_surface, &formatCount, nullptr));
        std::vector<VkSurfaceFormatKHR> formats(formatCount);
        CALL_VK(vkGetPhysicalDeviceSurfaceFormatsKHR(phyDevices[i], m_surface, &formatCount,
                                                     formats.data()));
        for (int j = 0; j < formatCount; j++)
        {
            if (formats[j].format == VK_FORMAT_B8G8R8A8_UNORM &&
                formats[j].colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
            {
                score += 10;
                break;
            }
        }

        // query queue family
        uint32_t queueFamilyCount;
        vkGetPhysicalDeviceQueueFamilyProperties(phyDevices[i], &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilys(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(phyDevices[i], &queueFamilyCount,
                                                 queueFamilys.data());

        LOG_DEBUG("score    --->    : {0}", score);
        LOG_DEBUG("queue family     : {0}", queueFamilyCount);
        if (score < maxScore)
        {
            continue;
        }

        for (int j = 0; j < queueFamilyCount; j++)
        {
            if (queueFamilys[j].queueCount == 0)
            {
                continue;
            }

            // 1. graphic family
            if (queueFamilys[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                m_graphicQueueFamily.queueFamilyIndex = j;
                m_graphicQueueFamily.queueCount       = queueFamilys[j].queueCount;
            }

            if (m_graphicQueueFamily.queueFamilyIndex >= 0 &&
                m_presentQueueFamily.queueFamilyIndex >= 0 &&
                m_graphicQueueFamily.queueFamilyIndex != m_presentQueueFamily.queueFamilyIndex)
            {
                break;
            }

            // 2. present family
            VkBool32 bSupportSurface;
            vkGetPhysicalDeviceSurfaceSupportKHR(phyDevices[i], j, m_surface, &bSupportSurface);
            if (bSupportSurface)
            {
                m_presentQueueFamily.queueFamilyIndex = j;
                m_presentQueueFamily.queueCount       = queueFamilys[j].queueCount;
            }
        }
        if (m_graphicQueueFamily.queueFamilyIndex >= 0 &&
            m_presentQueueFamily.queueFamilyIndex >= 0)
        {
            maxScorePhyDeviceIndex = i;
            maxScore               = score;
        }
    }
    LOG_DEBUG("-----------------------------");

    if (maxScorePhyDeviceIndex < 0)
    {
        LOG_WARN("Maybe can not find a suitable physical device, will 0.");
        maxScorePhyDeviceIndex = 0;
    }

    m_phyDevice = phyDevices[maxScorePhyDeviceIndex];
    vkGetPhysicalDeviceMemoryProperties(m_phyDevice, &m_phyDeviceMemoryProperties);
    LOG_TRACE("{0} : physical device:{1}, score:{2}, graphic queue: {3} : {4}, present queue: {5} "
              ": "
              "{6}",
              __FUNCTION__, maxScorePhyDeviceIndex, maxScore, m_graphicQueueFamily.queueFamilyIndex,
              m_graphicQueueFamily.queueCount, m_presentQueueFamily.queueFamilyIndex,
              m_presentQueueFamily.queueCount);
}

void AwVkContext::printPhysicalDeviceProperties(VkPhysicalDeviceProperties &props)
{
    const char *deviceType = props.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ? "integrat"
                                                                                          "ed gpu"
                             : props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? "discrete"
                                                                                          " gpu"
                             : props.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU  ? "virtual "
                                                                                          "gpu"
                             : props.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU          ? "cpu"
                                                                                        : "others";

    uint32_t driverVersionMajor = VK_VERSION_MAJOR(props.driverVersion);
    uint32_t driverVersionMinor = VK_VERSION_MINOR(props.driverVersion);
    uint32_t driverVersionPatch = VK_VERSION_PATCH(props.driverVersion);

    uint32_t apiVersionMajor = VK_VERSION_MAJOR(props.apiVersion);
    uint32_t apiVersionMinor = VK_VERSION_MINOR(props.apiVersion);
    uint32_t apiVersionPatch = VK_VERSION_PATCH(props.apiVersion);

    LOG_DEBUG("-----------------------------");
    LOG_DEBUG("deviceName       : {0}", props.deviceName);
    LOG_DEBUG("deviceType       : {0}", deviceType);
    LOG_DEBUG("vendorID         : {0}", props.vendorID);
    LOG_DEBUG("deviceID         : {0}", props.deviceID);
    LOG_DEBUG("driverVersion    : {0}.{1}.{2}", driverVersionMajor, driverVersionMinor,
              driverVersionPatch);
    LOG_DEBUG("apiVersion       : {0}.{1}.{2}", apiVersionMajor, apiVersionMinor, apiVersionPatch);
}

uint32_t AwVkContext::getPhyDeviceScore(VkPhysicalDeviceProperties &props)
{
    VkPhysicalDeviceType deviceType = props.deviceType;
    uint32_t score                  = 0;
    switch (deviceType)
    {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            score += 40;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            score += 30;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            score += 20;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            score += 10;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM:
            break;
    }
    return score;
}

} // namespace Airwave
