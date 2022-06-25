target_sources(whirl PRIVATE
  whirl/build.c
  whirl/builtin.c
  whirl/reader.c
  whirl/tir.c
  whirl/value.c
  whirl/list.c
  whirl/util.c)

target_sources(whirl.cli PRIVATE
  whirl/wrl-main.c)
