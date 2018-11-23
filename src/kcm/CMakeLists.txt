set (kcm_SRCS
    WobblyWindowsEffectKCM.cc
    plugin.cc
)

kconfig_add_kcfg_files (kcm_SRCS
    ../WobblyWindowsConfig.kcfgc
)

include (PkgConfigGetVar)
pkgconfig_getvar (dbus-1 interfaces_dir INTERFACES_DIR)
qt5_add_dbus_interface (kcm_SRCS ${INTERFACES_DIR}/org.kde.kwin.Effects.xml kwineffects_interface)
qt5_wrap_ui (kcm_SRCS WobblyWindowsEffectKCM.ui)

add_library (kwin_libanimation_wobbly_config MODULE ${kcm_SRCS})

target_link_libraries (kwin_libanimation_wobbly_config
    Qt5::Core
    Qt5::DBus
    Qt5::Gui
    KF5::ConfigCore
    KF5::ConfigGui
    KF5::ConfigWidgets
)

install (
    TARGETS
        kwin_libanimation_wobbly_config

    DESTINATION
        ${PLUGIN_INSTALL_DIR}/kwin/effects/configs
)
