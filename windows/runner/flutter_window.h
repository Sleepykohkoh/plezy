#ifndef RUNNER_FLUTTER_WINDOW_H_
#define RUNNER_FLUTTER_WINDOW_H_

#include <flutter/dart_project.h>
#include <flutter/flutter_view_controller.h>
#include <flutter/method_channel.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <string>

#include "win32_window.h"

class FlutterWindow : public Win32Window {
 public:
  explicit FlutterWindow(const flutter::DartProject& project);
  virtual ~FlutterWindow();

  // COPYDATASTRUCT::dwData of the WM_COPYDATA that hands a plezy: link from a
  // second launch (main.cpp) to the running window.
  static constexpr ULONG_PTR kDeepLinkCopyData = 0x504C5A59;  // 'PLZY'

 protected:
  bool OnCreate() override;
  void OnDestroy() override;
  LRESULT MessageHandler(HWND window, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept override;

 private:
  flutter::DartProject project_;

  std::unique_ptr<flutter::FlutterViewController> flutter_controller_;

  std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> window_channel_;

  // plezy: links (com.plezy/deep_link). A link waits in pending_deep_link_
  // until Dart reads it with getInitialLink; later links go out as onLink.
  std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> deep_link_channel_;
  std::string pending_deep_link_;
  bool deep_link_live_ = false;

  // Fullscreen state tracking for monitor-aware native fullscreen.
  // Maximize state lives inside `placement_before_fullscreen_.showCmd`.
  bool is_fullscreen_ = false;
  WINDOWPLACEMENT placement_before_fullscreen_{};
  LONG_PTR style_before_fullscreen_ = 0;
  LONG_PTR ex_style_before_fullscreen_ = 0;

  void RegisterWindowChannel();
  void RegisterDeepLinkChannel();
  void ReceiveDeepLink(const std::string& url);
  void SetNativeFullScreen(bool fullscreen);
  void NotifyFullScreenChanged();
};

#endif  // RUNNER_FLUTTER_WINDOW_H_
