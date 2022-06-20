target_sources(whirl PRIVATE
  wrl/reader.c
  wrl/value.c
  wrl/list.c)

target_sources(whirl.cli PRIVATE
  wrl/wrl-main.c)
