install(FILES org.ukui.virtualkeyboard.gschema.xml DESTINATION ${CMAKE_INSTALL_PREFIX}/share/glib-2.0/schemas/)
install(FILES ukui-virtual-keyboard-symbolic.svg DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/scalable/actions)
install(FILES kylin-virtual-keyboard.desktop DESTINATION /etc/xdg/autostart/)

configure_file(org.ukui.virtualkeyboard.start.sh.in ${CMAKE_CURRENT_BINARY_DIR}/org.ukui.virtualkeyboard.start.sh)
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/org.ukui.virtualkeyboard.start.sh DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)