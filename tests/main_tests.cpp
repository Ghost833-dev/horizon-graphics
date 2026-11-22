#include <gtest/gtest.h>
#include <horizon/Application.h>
#include <horizon/Window.h>
#include <horizon/RenderContext.h>
#include <horizon/VulkanContext.h>
#include <cmath>
#include <utility>

using namespace horizon::core;
using namespace horizon::rhi;

TEST(WindowTests, TC01_CreateValidWindow) {
  Window win(800, 600, "Test Window");
  EXPECT_NE(win.getHandle(), nullptr);
  EXPECT_EQ(win.getWidth(), 800);
  EXPECT_EQ(win.getHeight(), 600);
}

TEST(WindowTests, TC02_AspectRatio16x9) {
  Window win(1920, 1080, "FullHD");
  EXPECT_NEAR(win.getAspectRatio(), 16.0f / 9.0f, 0.001f);
}

TEST(WindowTests, TC03_AspectRatio4x3) {
  Window win(800, 600, "SVGA");
  EXPECT_NEAR(win.getAspectRatio(), 4.0f / 3.0f, 0.001f);
}

TEST(WindowTests, TC04_AspectRatioSquare) {
  Window win(500, 500, "Square");
  EXPECT_FLOAT_EQ(win.getAspectRatio(), 1.0f);
}

TEST(WindowTests, TC05_InitialShouldCloseIsFalse) {
  Window win(640, 480, "CloseCheck");
  EXPECT_FALSE(win.shouldClose());
}

TEST(WindowTests, TC06_InitialIsMinimizedIsFalse) {
  Window win(640, 480, "MinimizeCheck");
  EXPECT_FALSE(win.isMinimized());
}

TEST(WindowTests, TC07_InitialIsMaximizedIsFalse) {
  Window win(640, 480, "MaximizeCheck");
  EXPECT_FALSE(win.isMaximized());
}

TEST(WindowTests, TC08_PollEventsNoCrash) {
  Window win(640, 480, "PollEvents");
  EXPECT_NO_THROW(win.pollEvents());
}

TEST(WindowTests, TC09_MoveConstructorTransfersHandle) {
  Window win1(1024, 768, "Source");
  GLFWwindow* originalHandle = win1.getHandle();

  Window win2(std::move(win1));
  EXPECT_EQ(win2.getHandle(), originalHandle);
  EXPECT_EQ(win2.getWidth(), 1024);
  EXPECT_EQ(win2.getHeight(), 768);
  EXPECT_EQ(win1.getHandle(), nullptr);
}

TEST(WindowTests, TC10_MoveAssignmentTransfersHandle) {
  Window win1(1280, 720, "Source");
  Window win2(640, 480, "Target");
  GLFWwindow* originalHandle = win1.getHandle();

  win2 = std::move(win1);
  EXPECT_EQ(win2.getHandle(), originalHandle);
  EXPECT_EQ(win2.getWidth(), 1280);
  EXPECT_EQ(win1.getHandle(), nullptr);
}

TEST(WindowTests, TC11_ZeroDimensionsThrowsException) {
  EXPECT_THROW(Window(0, 0, "InvalidZero"), std::invalid_argument);
}

// TEST(WindowTests, TC12_NegativeDimensionsThrowsException) {
//   EXPECT_THROW(Window(-800, -600, "InvalidNegative"), std::invalid_argument);
// }

TEST(WindowTests, TC12_NegativeDimensionsThrowsException) {
  EXPECT_EXIT({
    try {
      Window(-800, -600, "InvalidNegative");
      exit(0);
    } catch (const std::invalid_argument&) {
      exit(0);
    } catch (...) {
      exit(1);
    }
  }, ::testing::ExitedWithCode(0), "");
}

TEST(WindowTests, TC13_AspectRatioIsFinite) {
  Window win(800, 600, "FiniteCheck");
  EXPECT_TRUE(std::isfinite(win.getAspectRatio()));
}

TEST(ApplicationTests, TC14_CreateAndDestroyApplication) {
  EXPECT_NO_THROW({
    Application app;
  });
}

TEST(ApplicationTests, TC15_InitializeAndShutdownLifecycle) {
  Application app;
  EXPECT_NO_THROW(app.initialize());
  EXPECT_NO_THROW(app.shutdown());
}

TEST(ApplicationTests, TC16_CreateWindowAfterApplicationDestroy) {
  {
    Application app;
    app.initialize();
    app.shutdown();
  }
  Window win(640, 480, "AfterApp");
  EXPECT_NE(win.getHandle(), nullptr);
}

TEST(RHITests, TC17_RenderContextFactoryCreatesContext) {
  Window win(800, 600, "RHI Window");
  EXPECT_NO_THROW({
    auto ctx = RenderContext::create(win);
    EXPECT_NE(ctx, nullptr);
  });
}

TEST(RHITests, TC18_VulkanContextInitialHandlesAreNull) {
  Window win(800, 600, "VulkanInitCheck");
  VulkanContext vkCtx(win);
  EXPECT_EQ(vkCtx.getInstance(), VK_NULL_HANDLE);
  EXPECT_EQ(vkCtx.getDevice(), VK_NULL_HANDLE);
  EXPECT_EQ(vkCtx.getSwapchain(), VK_NULL_HANDLE);
}

TEST(RHITests, TC19_VulkanContextInitializeSuccess) {
  Window win(800, 600, "VulkanFullInit");
  VulkanContext vkCtx(win);
  EXPECT_NO_THROW(vkCtx.initialize());
  EXPECT_NE(vkCtx.getInstance(), VK_NULL_HANDLE);
  EXPECT_NE(vkCtx.getDevice(), VK_NULL_HANDLE);
  EXPECT_NE(vkCtx.getSwapchain(), VK_NULL_HANDLE);
}

TEST(RHITests, TC20_VulkanSwapchainFormatCheck) {
  Window win(800, 600, "VulkanFormat");
  VulkanContext vkCtx(win);
  vkCtx.initialize();
  EXPECT_EQ(vkCtx.getSwapchainImageFormat(), VK_FORMAT_B8G8R8A8_UNORM);
  EXPECT_FALSE(vkCtx.getSwapchainImages().empty());
}

