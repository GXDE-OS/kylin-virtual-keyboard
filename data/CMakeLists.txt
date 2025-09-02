install(FILES org.ukui.virtualkeyboard.gschema.xml DESTINATION ${CMAKE_INSTALL_PREFIX}/share/glib-2.0/schemas/)
install(FILES ukui-virtual-keyboard-symbolic.svg DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/scalable/actions)
install(FILES kylin-virtual-keyboard.desktop DESTINATION /etc/xdg/autostart/)
install(PROGRAMS kylin-virtual-keyboard-xwayland DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)