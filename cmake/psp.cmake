if (PSP)
	message(STATUS "Building PSP version...")
	set(NEOTUX_USE_MIXER OFF)
	set(NEOTUX_PSP ON)
	set(NEOTUX_DATA_DIR "data")
	list(APPEND extra_convert_cmds "--quality=12" "--max-width=512" "--max-height=512" "--split-sprites")

	get_target_property(compile_defs NeoTux COMPILE_DEFINITIONS)
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
