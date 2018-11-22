find_package (Qt5 REQUIRED COMPONENTS
    Core
    DBus
    Gui
)

find_package (KF5 REQUIRED COMPONENTS
    Config
    ConfigWidgets
    CoreAddons
    WindowSystem
)

find_package (KWinEffects REQUIRED)
find_package (Libanimation REQUIRED)

set (effect_SRCS
    plugin.cc
    WobblyWindowsEffect.cc
)

kconfig_add_kcfg_files (effect_SRCS
    WobblyWindowsConfig.kcfgc
)

add_library (kwin4_effect_libanimation_wobbly SHARED ${effect_SRCS})

target_link_libraries (kwin4_effect_libanimation_wobbly
    PUBLIC
        Qt5::Core
        Qt5::Gui
        KF5::ConfigCore
        KF5::ConfigGui
        KF5::CoreAddons
        KF5::WindowSystem
        KWinEffects::KWinEffects
        Libanimation::Libanimation
)

install (
    TARGETS
        kwin4_effect_libanimation_wobbly

    DESTINATION
        ${PLUGIN_INSTALL_DIR}/kwin/effects/plugins/
)
