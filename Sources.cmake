target_sources(whirl PRIVATE
  wrl/builtin.c
  wrl/reader.c
  wrl/tir.c
  wrl/value.c
  wrl/list.c)

target_sources(whirl.cli PRIVATE
  wrl/wrl-main.c)
