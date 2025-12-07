if (PSP)
	message(STATUS "Building PSP version...")
	set(NEOTUX_USE_MIXER OFF)
	set(NEOTUX_PSP ON)
	set(NEOTUX_DATA_DIR "data")

	get_target_property(compile_defs NeoTux COMPILE_DEFINITIONS)
	message(status "Its ${compile_defs}")
	#string(REPLACE "-pthread" "" NEW_OPTS "${CURRENT_OPTS}")
	set_target_properties(NeoTux PROPERTIES LINK_OPTIONS "")
	
	create_pbp_file(
		TARGET NeoTux
		ICON_PATH ${PROJECT_SOURCE_DIR}/data/images/engine/pspicon0.png
		BACKGROUND_PATH ${PROJECT_SOURCE_DIR}/data/images/engine/psppic1.png
		PREVIEW_PATH NULL
		TITLE NeoTux
		VERSION 01.00
	)
endif()
