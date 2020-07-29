/*++
Copyright (c) Microsoft Corporation
Licensed under the MIT license.

Module Name:
- Profile.hpp

Abstract:
- A profile acts as a single set of terminal settings. Many tabs or panes could
     exist side-by-side with different profiles simultaneously.

Author(s):
- Mike Griese - March 2019

--*/
#pragma once
#include "ColorScheme.h"
#include "SettingsTypes.h"

// fwdecl unittest classes
namespace TerminalAppLocalTests
{
    class SettingsTests;
    class ProfileTests;
};
namespace TerminalAppUnitTests
{
    class JsonTests;
    class DynamicProfileTests;
};

// GUID used for generating GUIDs at runtime, for profiles that did not have a
// GUID specified manually.
constexpr GUID RUNTIME_GENERATED_PROFILE_NAMESPACE_GUID = { 0xf65ddb7e, 0x706b, 0x4499, { 0x8a, 0x50, 0x40, 0x31, 0x3c, 0xaf, 0x51, 0x0a } };

namespace TerminalApp
{
    class Profile;
}

class TerminalApp::Profile final
{
public:
    Profile();
    explicit Profile(const std::optional<GUID>& guid);

    ~Profile();

    winrt::Microsoft::Terminal::Settings::TerminalSettings CreateTerminalSettings(const std::unordered_map<std::wstring, ColorScheme>& schemes) const;

    Json::Value GenerateStub() const;
    static Profile FromJson(const Json::Value& json);
    bool ShouldBeLayered(const Json::Value& json) const;
    void LayerJson(const Json::Value& json);
    static bool IsDynamicProfileObject(const Json::Value& json);

    bool HasGuid() const noexcept;
    bool HasSource() const noexcept;
    GUID GetGuid() const;
    void SetSource(std::wstring_view sourceNamespace) noexcept;
    std::wstring_view GetName() const noexcept;
    bool HasConnectionType() const noexcept;
    GUID GetConnectionType() const noexcept;

    void SetGuid(GUID guid) noexcept { _guid = guid; }
    void SetFontFace(std::wstring fontFace) noexcept;
    void SetColorScheme(std::optional<std::wstring> schemeName) noexcept;
    const std::optional<std::wstring>& GetSchemeName() const noexcept;
    void SetTabTitle(std::wstring tabTitle) noexcept;
    void SetSuppressApplicationTitle(bool suppressApplicationTitle) noexcept;
    void SetAcrylicOpacity(double opacity) noexcept;
    void SetCommandline(std::wstring cmdline) noexcept;
    void SetStartingDirectory(std::wstring startingDirectory) noexcept;
    void SetName(const std::wstring_view name) noexcept;
    void SetUseAcrylic(bool useAcrylic) noexcept;
    void SetDefaultForeground(til::color defaultForeground) noexcept;
    void SetDefaultBackground(til::color defaultBackground) noexcept;
    void SetSelectionBackground(til::color selectionBackground) noexcept;
    void SetCloseOnExitMode(CloseOnExitMode mode) noexcept;
    void SetConnectionType(GUID connectionType) noexcept;

    bool HasIcon() const noexcept;
    winrt::hstring GetExpandedIconPath() const;
    void SetIconPath(std::wstring_view path);
    void ResetIconPath();

    bool HasBackgroundImage() const noexcept;
    winrt::hstring GetExpandedBackgroundImagePath() const;
    void ResetBackgroundImagePath();

    CloseOnExitMode GetCloseOnExitMode() const noexcept;
    bool GetSuppressApplicationTitle() const noexcept;
    bool IsHidden() const noexcept;

    void GenerateGuidIfNecessary() noexcept;

    static GUID GetGuidOrGenerateForJson(const Json::Value& json) noexcept;

    void SetRetroTerminalEffect(bool value) noexcept;

private:
    static std::wstring EvaluateStartingDirectory(const std::wstring& directory);

    static GUID _GenerateGuidForProfile(const std::wstring& name, const std::optional<std::wstring>& source) noexcept;

    std::optional<GUID> _guid{ std::nullopt };
    std::optional<std::wstring> _source{ std::nullopt };
    std::wstring _name;
    std::optional<GUID> _connectionType;
    bool _hidden;

    // If this is set, then our colors should come from the associated color scheme
    std::optional<std::wstring> _schemeName;

    std::optional<til::color> _defaultForeground;
    std::optional<til::color> _defaultBackground;
    std::optional<til::color> _selectionBackground;
    std::optional<til::color> _cursorColor;
    std::optional<std::wstring> _tabTitle;
    bool _suppressApplicationTitle;
    int32_t _historySize;
    bool _snapOnInput;
    bool _altGrAliasing;
    uint32_t _cursorHeight;
    winrt::Microsoft::Terminal::Settings::CursorStyle _cursorShape;

    std::wstring _commandline;
    std::wstring _fontFace;
    std::optional<std::wstring> _startingDirectory;
    int32_t _fontSize;
    winrt::Windows::UI::Text::FontWeight _fontWeight;
    double _acrylicTransparency;
    bool _useAcrylic;

    std::optional<std::wstring> _backgroundImage;
    std::optional<double> _backgroundImageOpacity;
    std::optional<winrt::Windows::UI::Xaml::Media::Stretch> _backgroundImageStretchMode;
    std::optional<std::tuple<winrt::Windows::UI::Xaml::HorizontalAlignment, winrt::Windows::UI::Xaml::VerticalAlignment>> _backgroundImageAlignment;

    std::optional<::winrt::Microsoft::Terminal::Settings::ScrollbarState> _scrollbarState;
    CloseOnExitMode _closeOnExitMode;
    std::wstring _padding;

    std::optional<std::wstring> _icon;

    winrt::Microsoft::Terminal::Settings::TextAntialiasingMode _antialiasingMode;

    friend class TerminalAppLocalTests::SettingsTests;
    friend class TerminalAppLocalTests::ProfileTests;
    friend class TerminalAppUnitTests::JsonTests;
    friend class TerminalAppUnitTests::DynamicProfileTests;

    std::optional<bool> _retroTerminalEffect;
};
