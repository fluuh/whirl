target_sources(whirl PRIVATE
  wrl/build.c
  wrl/builtin.c
  wrl/reader.c
  wrl/tir.c
  wrl/value.c
  wrl/list.c
  wrl/util.c)

target_sources(whirl.cli PRIVATE
  wrl/wrl-main.c)
